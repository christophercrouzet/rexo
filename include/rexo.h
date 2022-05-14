/*
   This is free and unencumbered software released into the public domain.

   Anyone is free to copy, modify, publish, use, compile, sell, or
   distribute this software, either in source code form or as a compiled
   binary, for any purpose, commercial or non-commercial, and by any
   means.

   In jurisdictions that recognize copyright laws, the author or authors
   of this software dedicate any and all copyright interest in the
   software to the public domain. We make this dedication for the benefit
   of the public at large and to the detriment of our heirs and
   successors. We intend this dedication to be an overt act of
   relinquishment in perpetuity of all present and future rights to this
   software under copyright law.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
   OTHER DEALINGS IN THE SOFTWARE.

   For more information, please refer to <https://unlicense.org>
*/

#ifndef REXO_REXO_H
#define REXO_REXO_H

#define RX_MAJOR_VERSION 0
#define RX_MINOR_VERSION 2
#define RX_PATCH_VERSION 2

#define RX_VERSION                                                             \
    ((RX_MAJOR_VERSION << 20) | (RX_MINOR_VERSION << 10) | (RX_PATCH_VERSION))

/*
   The environment macro represents whether the code is to be generated for a
   32-bit or 64-bit target platform. Some CPUs, such as the x86-64 processors,
   allow running code in 32-bit mode if compiled using the -m32 or -mx32
   compiler switches, in which case `RX__ENVIRONMENT` is set to 32.
*/
#if (!(defined(__x86_64__) || defined(_M_X64)) || defined(__ILP32__))          \
    && !(defined(__itanium__) || defined(_M_IA64))                             \
    && !(defined(__powerpc64__) || defined(__ppc64__))                         \
    && !defined(__aarch64__)
    #define RX__ENVIRONMENT 32
#else
    #define RX__ENVIRONMENT 64
#endif

/*
   The common data models, that is ILP32 (most recent 32-bit systems),
   LP64 (Unix-like systems), and LLP64 (Windows), all have the `int` type set to
   32 bits, and the `long long` type to 64 bits.
*/
#if defined(RX_UINT32_TYPE)
    typedef RX_UINT32_TYPE rx_uint32;
#else
    typedef unsigned int rx_uint32;
#endif
typedef char rx__invalid_uint32_type[sizeof(rx_uint32) == 4 ? 1 : -1];

#if defined(RX_UINT64_TYPE)
    typedef RX_UINT64_TYPE rx_uint64;
#else
    #if defined(__GNUC__)
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wlong-long"
    #endif
    /*
       Even though the `long long` type is not part of the C89 and C++98
       standards, assume that it's defined by current compilers.
    */
    typedef unsigned long long rx_uint64;
    #if defined(__GNUC__)
        #pragma GCC diagnostic pop
    #endif
#endif
typedef char rx__invalid_uint64_type[sizeof(rx_uint64) == 8 ? 1 : -1];

/*
   The C standard provides no guarantees about the size of the type `size_t`,
   and some exotic platforms will in fact provide original values, but this
   should cover most of the use cases.
*/
#if defined(RX_SIZE_TYPE)
    typedef RX_SIZE_TYPE rx_size;
#elif RX__ENVIRONMENT == 32
    typedef rx_uint32 rx_size;
#else
    typedef rx_uint64 rx_size;
#endif

#if defined(RX_ENABLE_EXTERNAL_LINKING)
    #define RX__STORAGE extern
#else
    #define RX__STORAGE static
#endif

#if defined(__GNUC__)
    #define RX__MAYBE_UNUSED __attribute__((unused))
#else
    #define RX__MAYBE_UNUSED
#endif

#if defined(_MSC_VER)
    #define RX__DEFINE_PARAMS(TYPE)                                            \
        _Pragma("warning(push)")                                               \
        _Pragma("warning(disable : 4100)")                                     \
        struct rx_context *RX_PARAM_CONTEXT RX__MAYBE_UNUSED,                  \
        TYPE *RX_PARAM_DATA RX__MAYBE_UNUSED                                   \
        _Pragma("warning(pop)")
#else
    #define RX__DEFINE_PARAMS(TYPE)                                            \
    struct rx_context *RX_PARAM_CONTEXT RX__MAYBE_UNUSED,                      \
    TYPE *RX_PARAM_DATA RX__MAYBE_UNUSED
#endif

/*
   Support compilers that checks printf-style functions.
*/
#if defined(__GNUC__)
    #define RX__PRINTF_CHECK(FMT, ARGS)                                        \
        __attribute__((format (printf, (FMT), (ARGS))))
#else
    #define RX__PRINTF_CHECK(FMT, ARGS)
#endif

/* Public Interface                                                O-(''Q)
   -------------------------------------------------------------------------- */

#define RX_PARAM_CONTEXT rx__context
#define RX_PARAM_DATA rx__data

#define RX_DATA RX_PARAM_DATA

enum rx_status {
    RX_SUCCESS = 0,
    RX_ERROR = -1,
    RX_ERROR_ABORTED = -2,
    RX_ERROR_ALLOCATION = -3,
    RX_ERROR_MAX_SIZE_EXCEEDED = -4
};

enum rx_severity { RX_NONFATAL = 0, RX_FATAL = 1 };

enum rx_log_level {
    RX_LOG_LEVEL_NONE = 0,
    RX_LOG_LEVEL_FATAL = 1,
    RX_LOG_LEVEL_ERROR = 3,
    RX_LOG_LEVEL_WARNING = 4,
    RX_LOG_LEVEL_INFO = 5,
    RX_LOG_LEVEL_DEBUG = 6,
    RX_LOG_LEVEL_ALL = RX_LOG_LEVEL_DEBUG
};

struct rx_context;

typedef enum rx_status (*rx_set_up_fn)(RX__DEFINE_PARAMS(void));
typedef void (*rx_tear_down_fn)(RX__DEFINE_PARAMS(void));
typedef void (*rx_run_fn)(RX__DEFINE_PARAMS(void));

struct rx_fixture_config {
    rx_set_up_fn set_up;
    rx_tear_down_fn tear_down;
};

struct rx_fixture {
    rx_size size;
    struct rx_fixture_config config;
};

struct rx_test_case_config {
    int skip;
    struct rx_fixture fixture;
};

struct rx_test_case {
    const char *suite_name;
    const char *name;
    rx_run_fn run;
    struct rx_test_case_config config;
};

struct rx_failure {
    const char *file;
    int line;
    enum rx_severity severity;
    const char *msg;
    const char *diagnostic_msg;
};

struct rx_summary {
    const struct rx_test_case *test_case;
    int skipped;
    const char *error;
    rx_size assessed_count;
    rx_size failure_count;
    struct rx_failure *failures;
    rx_uint64 elapsed;
};

struct rx_summary_group {
    rx_size count;
    const struct rx_summary *array;
};

#if defined(__cplusplus)
extern "C" {
#endif

RX__STORAGE void
rx_abort(struct rx_context *context);

RX__STORAGE enum rx_status
rx_handle_test_result(struct rx_context *context,
                      int result,
                      const char *file,
                      int line,
                      enum rx_severity severity,
                      const char *failure_msg,
                      const char *diagnostic_msg);

RX__STORAGE enum rx_status
rx_summary_initialize(struct rx_summary *summary,
                      const struct rx_test_case *test_case);

RX__STORAGE void
rx_summary_terminate(struct rx_summary *summary);

RX__STORAGE void
rx_summary_print(const struct rx_summary *summary);

RX__STORAGE void
rx_sort_summaries_by_test_suite(struct rx_summary *summaries,
                                rx_size summary_count);

RX__STORAGE void
rx_group_summaries_by_test_suite(rx_size *summary_group_count,
                                 struct rx_summary_group *summary_groups,
                                 rx_size summary_count,
                                 const struct rx_summary *summaries);

RX__STORAGE enum rx_status
rx_test_case_run(struct rx_summary *summary,
                 const struct rx_test_case *test_case);

RX__STORAGE void
rx_enumerate_test_cases(rx_size *test_case_count,
                        struct rx_test_case *test_cases);

RX__STORAGE enum rx_status
rx_run(rx_size test_case_count, const struct rx_test_case *test_cases);

RX__STORAGE enum rx_status
rx_main(rx_size test_case_count,
        const struct rx_test_case *test_cases,
        int argc,
        const char * const *argv);

#if defined(__cplusplus)
}
#endif

/* Helpers                                                         O-(''Q)
   -------------------------------------------------------------------------- */

#define RX__LANG_C 0
#define RX__LANG_CPP 1

#if defined(__cplusplus)
    #define RX__LANG RX__LANG_CPP
    #define RX__LANG_VERSION __cplusplus
#else
    #define RX__LANG RX__LANG_C
    #if defined(__STDC_VERSION__)
        #define RX__LANG_VERSION __STDC_VERSION__
    #else
        #define RX__LANG_VERSION 0L
    #endif
#endif

#if defined(RX_DISABLE_NPRINTF)                                                \
    || (!defined(RX_ENABLE_NPRINTF)                                            \
        && !defined(_MSC_VER)                                                  \
        && ((RX__LANG == RX__LANG_C && RX__LANG_VERSION < 199901L)             \
            || (RX__LANG == RX__LANG_CPP && RX__LANG_VERSION < 201103L)))
    #define RX__HAS_NPRINTF 0
#else
    #define RX__HAS_NPRINTF 1
#endif

#if defined(RX_DISABLE_VARIADIC_MACROS)                                        \
    || (!defined(RX_ENABLE_VARIADIC_MACROS)                                    \
        && !defined(_MSC_VER)                                                  \
        && ((RX__LANG == RX__LANG_C && RX__LANG_VERSION < 199901L)             \
            || (RX__LANG == RX__LANG_CPP && RX__LANG_VERSION < 201103L)))
    #define RX__HAS_VARIADIC_MACROS 0
#else
    #define RX__HAS_VARIADIC_MACROS 1
#endif

#define RX__FALSE ((int)0)
#define RX__TRUE ((int)1)

/* Automatic Registration Framework                                O-(''Q)
   -------------------------------------------------------------------------- */

#define RX_SET_UP(ID)                                                          \
    static enum rx_status                                                      \
    ID(RX__DEFINE_PARAMS(void))

#define RX_TEAR_DOWN(ID)                                                       \
    static void                                                                \
    ID(RX__DEFINE_PARAMS(void))

#if RX__HAS_VARIADIC_MACROS
    #define RX_FIXTURE(...)                                                    \
        RX__EXPAND(                                                            \
            RX__CONCAT(                                                        \
                RX__FIXTURE_DISPATCH_,                                         \
                RX__HAS_AT_LEAST_3_ARGS(__VA_ARGS__)                           \
            )(__VA_ARGS__))                                                    \
        RX__REQUIRE_SEMICOLON

    #define RX__FIXTURE_DISPATCH_0(ID, TYPE)                                   \
        RX__FIXTURE_0(ID, sizeof(TYPE))

    #define RX__FIXTURE_DISPATCH_1(ID, TYPE, ...)                              \
        RX__FIXTURE_1(ID,                                                      \
                      sizeof(TYPE),                                            \
                      RX__COUNT_ARGS(__VA_ARGS__),                             \
                      (__VA_ARGS__))
#else
    #define RX_FIXTURE(ID, TYPE)                                               \
        RX__FIXTURE_0(ID, sizeof(TYPE))                                        \
        RX__REQUIRE_SEMICOLON

    #define RX_FIXTURE_1(ID, TYPE, _0)                                         \
        RX__FIXTURE_1(ID, sizeof(TYPE), 1, (_0))                               \
        RX__REQUIRE_SEMICOLON

    #define RX_FIXTURE_2(ID, TYPE, _0, _1)                                     \
        RX__FIXTURE_1(ID, sizeof(TYPE), 2, (_0, _1))                           \
        RX__REQUIRE_SEMICOLON
#endif

#if RX__HAS_VARIADIC_MACROS
    #define RX_VOID_FIXTURE(...)                                               \
        RX__EXPAND(                                                            \
            RX__CONCAT(                                                        \
                RX__VOID_FIXTURE_DISPATCH_,                                    \
                RX__HAS_AT_LEAST_2_ARGS(__VA_ARGS__)                           \
            )(__VA_ARGS__))                                                    \
        RX__REQUIRE_SEMICOLON

    #define RX__VOID_FIXTURE_DISPATCH_0(ID)                                    \
        RX__FIXTURE_0(ID, 0)

    #define RX__VOID_FIXTURE_DISPATCH_1(ID, ...)                               \
        RX__FIXTURE_1(ID,                                                      \
                      0,                                                       \
                      RX__COUNT_ARGS(__VA_ARGS__),                             \
                      (__VA_ARGS__))
#else
    #define RX_VOID_FIXTURE(ID)                                                \
        RX__FIXTURE_0(ID, 0)                                                   \
        RX__REQUIRE_SEMICOLON

    #define RX_VOID_FIXTURE_1(ID, _0)                                          \
        RX__FIXTURE_1(ID, 0, 1, (_0))                                          \
        RX__REQUIRE_SEMICOLON

    #define RX_VOID_FIXTURE_2(ID, _0, _1)                                      \
        RX__FIXTURE_1(ID, 0, 2, (_0, _1))                                      \
        RX__REQUIRE_SEMICOLON
#endif

#if RX__HAS_VARIADIC_MACROS
    #define RX_TEST_SUITE(...)                                                 \
        RX__EXPAND(                                                            \
            RX__CONCAT(                                                        \
                RX__TEST_SUITE_DISPATCH_,                                      \
                RX__HAS_AT_LEAST_2_ARGS(__VA_ARGS__)                           \
            )(__VA_ARGS__))                                                    \
        RX__REQUIRE_SEMICOLON

    #define RX__TEST_SUITE_DISPATCH_0(ID)                                      \
        RX__TEST_SUITE_0(ID)

    #define RX__TEST_SUITE_DISPATCH_1(ID, ...)                                 \
        RX__TEST_SUITE_1(ID,                                                   \
                         RX__COUNT_ARGS(__VA_ARGS__),                          \
                         (__VA_ARGS__))
#else
    #define RX_TEST_SUITE(ID)                                                  \
        RX__TEST_SUITE_0(ID)                                                   \
        RX__REQUIRE_SEMICOLON

    #define RX_TEST_SUITE_1(ID, _0)                                            \
        RX__TEST_SUITE_1(ID, 1, (_0))                                          \
        RX__REQUIRE_SEMICOLON

    #define RX_TEST_SUITE_2(ID, _0, _1)                                        \
        RX__TEST_SUITE_1(ID, 2, (_0, _1))                                      \
        RX__REQUIRE_SEMICOLON
#endif

#if RX__HAS_VARIADIC_MACROS
    #define RX_TEST_CASE(...)                                                  \
        RX__EXPAND(                                                            \
            RX__CONCAT(                                                        \
                RX__TEST_CASE_DISPATCH_,                                       \
                RX__HAS_AT_LEAST_3_ARGS(__VA_ARGS__)                           \
            )(__VA_ARGS__))

    #define RX__TEST_CASE_DISPATCH_0(SUIE_ID, ID)                             \
        RX__TEST_CASE_0(SUIE_ID, ID)

    #define RX__TEST_CASE_DISPATCH_1(SUIE_ID, ID, ...)                        \
        RX__TEST_CASE_1(SUIE_ID,                                              \
                        ID,                                                    \
                        RX__COUNT_ARGS(__VA_ARGS__),                           \
                        (__VA_ARGS__))
#else
    #define RX_TEST_CASE(SUIE_ID, ID)                                         \
        RX__TEST_CASE_0(SUIE_ID, ID)

    #define RX_TEST_CASE_1(SUIE_ID, ID, _0)                                   \
        RX__TEST_CASE_1(SUIE_ID, ID, 1, (_0))

    #define RX_TEST_CASE_2(SUIE_ID, ID, _0, _1)                               \
        RX__TEST_CASE_1(SUIE_ID, ID, 2, (_0, _1))
#endif

/* Generic Assertions                                              O-(''Q)
   -------------------------------------------------------------------------- */

#if RX__HAS_VARIADIC_MACROS
    #define RX__DEFINE_TEST(SEVERITY, EXPECTED, CONDITION, ...)                \
        rx__assess_value(RX_PARAM_CONTEXT,                                     \
                         !!(CONDITION),                                        \
                         EXPECTED,                                             \
                         #CONDITION,                                           \
                         __FILE__,                                             \
                         __LINE__,                                             \
                         SEVERITY,                                             \
                         __VA_ARGS__)
#else
    #define RX__DEFINE_TEST(SEVERITY, EXPECTED, CONDITION, MSG,                \
                            _0, _1, _2, _3, _4, _5, _6, _7)                    \
        rx__assess_value(RX_PARAM_CONTEXT,                                     \
                         !!(CONDITION),                                        \
                         EXPECTED,                                             \
                         #CONDITION,                                           \
                         __FILE__,                                             \
                         __LINE__,                                             \
                         SEVERITY,                                             \
                         MSG,                                                  \
                         _0, _1, _2, _3, _4, _5, _6, _7)
#endif

#define RX_REQUIRE(CONDITION)                                                  \
    RX_REQUIRE_MSG(CONDITION, NULL)

#if RX__HAS_VARIADIC_MACROS
    #define RX_REQUIRE_MSG(CONDITION, ...)                                     \
        RX__DEFINE_TEST(RX_FATAL, RX__TRUE, CONDITION, __VA_ARGS__)
#else
    #define RX_REQUIRE_MSG(CONDITION, MSG)                                     \
        RX__DEFINE_TEST(RX_FATAL, RX__TRUE, CONDITION, MSG,                    \
                        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#endif

#define RX_REQUIRE_MSG_1(CONDITION, MSG,                                       \
                         _0)                                                   \
    RX__DEFINE_TEST(RX_FATAL, RX__TRUE, CONDITION, MSG,                        \
                    _0, NULL, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_REQUIRE_MSG_2(CONDITION, MSG,                                       \
                         _0, _1)                                               \
    RX__DEFINE_TEST(RX_FATAL, RX__TRUE, CONDITION, MSG,                        \
                    _0, _1, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_REQUIRE_MSG_3(CONDITION, MSG,                                       \
                         _0, _1, _2)                                           \
    RX__DEFINE_TEST(RX_FATAL, RX__TRUE, CONDITION, MSG,                        \
                    _0, _1, _2, NULL, NULL, NULL, NULL, NULL)

#define RX_REQUIRE_MSG_4(CONDITION, MSG,                                       \
                         _0, _1, _2, _3)                                       \
    RX__DEFINE_TEST(RX_FATAL, RX__TRUE, CONDITION, MSG,                        \
                    _0, _1, _2, _3, NULL, NULL, NULL, NULL)

#define RX_REQUIRE_MSG_5(CONDITION, MSG,                                       \
                         _0, _1, _2, _3, _4)                                   \
    RX__DEFINE_TEST(RX_FATAL, RX__TRUE, CONDITION, MSG,                        \
                    _0, _1, _2, _3, _4, NULL, NULL, NULL)

#define RX_REQUIRE_MSG_6(CONDITION, MSG,                                       \
                         _0, _1, _2, _3, _4, _5)                               \
    RX__DEFINE_TEST(RX_FATAL, RX__TRUE, CONDITION, MSG,                        \
                    _0, _1, _2, _3, _4, _5, NULL, NULL)

#define RX_REQUIRE_MSG_7(CONDITION, MSG,                                       \
                         _0, _1, _2, _3, _4, _5, _6)                           \
    RX__DEFINE_TEST(RX_FATAL, RX__TRUE, CONDITION, MSG,                        \
                    _0, _1, _2, _3, _4, _5, _6, NULL)

#define RX_REQUIRE_MSG_8(CONDITION, MSG,                                       \
                         _0, _1, _2, _3, _4, _5, _6, _7)                       \
    RX__DEFINE_TEST(RX_FATAL, RX__TRUE, CONDITION, MSG,                        \
                    _0, _1, _2, _3, _4, _5, _6, _7)

#define RX_CHECK(CONDITION)                                                    \
    RX_CHECK_MSG(CONDITION, NULL)

#if RX__HAS_VARIADIC_MACROS
    #define RX_CHECK_MSG(CONDITION, ...)                                       \
        RX__DEFINE_TEST(RX_NONFATAL, RX__TRUE, CONDITION, __VA_ARGS__)
#else
    #define RX_CHECK_MSG(CONDITION, MSG)                                       \
        RX__DEFINE_TEST(RX_NONFATAL, RX__TRUE, CONDITION, MSG,                 \
                        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#endif

#define RX_CHECK_MSG_1(CONDITION, MSG,                                         \
                       _0)                                                     \
    RX__DEFINE_TEST(RX_NONFATAL, RX__TRUE, CONDITION, MSG,                     \
                    _0, NULL, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_CHECK_MSG_2(CONDITION, MSG,                                         \
                       _0, _1)                                                 \
    RX__DEFINE_TEST(RX_NONFATAL, RX__TRUE, CONDITION, MSG,                     \
                    _0, _1, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_CHECK_MSG_3(CONDITION, MSG,                                         \
                       _0, _1, _2)                                             \
    RX__DEFINE_TEST(RX_NONFATAL, RX__TRUE, CONDITION, MSG,                     \
                    _0, _1, _2, NULL, NULL, NULL, NULL, NULL)

#define RX_CHECK_MSG_4(CONDITION, MSG,                                         \
                       _0, _1, _2, _3)                                         \
    RX__DEFINE_TEST(RX_NONFATAL, RX__TRUE, CONDITION, MSG,                     \
                    _0, _1, _2, _3, NULL, NULL, NULL, NULL)

#define RX_CHECK_MSG_5(CONDITION, MSG,                                         \
                       _0, _1, _2, _3, _4)                                     \
    RX__DEFINE_TEST(RX_NONFATAL, RX__TRUE, CONDITION, MSG,                     \
                    _0, _1, _2, _3, _4, NULL, NULL, NULL)

#define RX_CHECK_MSG_6(CONDITION, MSG,                                         \
                       _0, _1, _2, _3, _4, _5)                                 \
    RX__DEFINE_TEST(RX_NONFATAL, RX__TRUE, CONDITION, MSG,                     \
                    _0, _1, _2, _3, _4, _5, NULL, NULL)

#define RX_CHECK_MSG_7(CONDITION, MSG,                                         \
                       _0, _1, _2, _3, _4, _5, _6)                             \
    RX__DEFINE_TEST(RX_NONFATAL, RX__TRUE, CONDITION, MSG,                     \
                    _0, _1, _2, _3, _4, _5, _6, NULL)

#define RX_CHECK_MSG_8(CONDITION, MSG,                                         \
                       _0, _1, _2, _3, _4, _5, _6, _7)                         \
    RX__DEFINE_TEST(RX_NONFATAL, RX__TRUE, CONDITION, MSG,                     \
                    _0, _1, _2, _3, _4, _5, _6, _7)


/* Boolean Assertions                                              O-(''Q)
   -------------------------------------------------------------------------- */

#if RX__HAS_VARIADIC_MACROS
    #define RX__BOOL_DEFINE_TEST(SEVERITY, EXPECTED, CONDITION, ...)           \
        rx__bool_assess_value(RX_PARAM_CONTEXT,                                \
                              !!(CONDITION),                                   \
                              EXPECTED,                                        \
                              #CONDITION,                                      \
                              __FILE__,                                        \
                              __LINE__,                                        \
                              SEVERITY,                                        \
                              __VA_ARGS__)
#else
    #define RX__BOOL_DEFINE_TEST(SEVERITY, EXPECTED, CONDITION, MSG,           \
                                 _0, _1, _2, _3, _4, _5, _6, _7)               \
        rx__bool_assess_value(RX_PARAM_CONTEXT,                                \
                              !!(CONDITION),                                   \
                              EXPECTED,                                        \
                              #CONDITION,                                      \
                              __FILE__,                                        \
                              __LINE__,                                        \
                              SEVERITY,                                        \
                              MSG,                                             \
                              _0, _1, _2, _3, _4, _5, _6, _7)
#endif

#define RX_BOOL_REQUIRE_TRUE(CONDITION)                                        \
    RX_BOOL_REQUIRE_TRUE_MSG(CONDITION, NULL)

#if RX__HAS_VARIADIC_MACROS
    #define RX_BOOL_REQUIRE_TRUE_MSG(CONDITION, ...)                           \
        RX__BOOL_DEFINE_TEST(RX_FATAL, RX__TRUE, CONDITION, __VA_ARGS__)
#else
    #define RX_BOOL_REQUIRE_TRUE_MSG(CONDITION, MSG)                           \
        RX__BOOL_DEFINE_TEST(RX_FATAL, RX__TRUE, CONDITION, MSG,               \
                             NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#endif

#define RX_BOOL_REQUIRE_TRUE_MSG_1(CONDITION, MSG,                             \
                                   _0)                                         \
    RX__BOOL_DEFINE_TEST(RX_FATAL, RX__TRUE, CONDITION, MSG,                   \
                         _0, NULL, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_BOOL_REQUIRE_TRUE_MSG_2(CONDITION, MSG,                             \
                                   _0, _1)                                     \
    RX__BOOL_DEFINE_TEST(RX_FATAL, RX__TRUE, CONDITION, MSG,                   \
                         _0, _1, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_BOOL_REQUIRE_TRUE_MSG_3(CONDITION, MSG,                             \
                                   _0, _1, _2)                                 \
    RX__BOOL_DEFINE_TEST(RX_FATAL, RX__TRUE, CONDITION, MSG,                   \
                         _0, _1, _2, NULL, NULL, NULL, NULL, NULL)

#define RX_BOOL_REQUIRE_TRUE_MSG_4(CONDITION, MSG,                             \
                                   _0, _1, _2, _3)                             \
    RX__BOOL_DEFINE_TEST(RX_FATAL, RX__TRUE, CONDITION, MSG,                   \
                         _0, _1, _2, _3, NULL, NULL, NULL, NULL)

#define RX_BOOL_REQUIRE_TRUE_MSG_5(CONDITION, MSG,                             \
                                   _0, _1, _2, _3, _4)                         \
    RX__BOOL_DEFINE_TEST(RX_FATAL, RX__TRUE, CONDITION, MSG,                   \
                         _0, _1, _2, _3, _4, NULL, NULL, NULL)

#define RX_BOOL_REQUIRE_TRUE_MSG_6(CONDITION, MSG,                             \
                                   _0, _1, _2, _3, _4, _5)                     \
    RX__BOOL_DEFINE_TEST(RX_FATAL, RX__TRUE, CONDITION, MSG,                   \
                         _0, _1, _2, _3, _4, _5, NULL, NULL)

#define RX_BOOL_REQUIRE_TRUE_MSG_7(CONDITION, MSG,                             \
                                   _0, _1, _2, _3, _4, _5, _6)                 \
    RX__BOOL_DEFINE_TEST(RX_FATAL, RX__TRUE, CONDITION, MSG,                   \
                         _0, _1, _2, _3, _4, _5, _6, NULL)

#define RX_BOOL_REQUIRE_TRUE_MSG_8(CONDITION, MSG,                             \
                                   _0, _1, _2, _3, _4, _5, _6, _7)             \
    RX__BOOL_DEFINE_TEST(RX_FATAL, RX__TRUE, CONDITION, MSG,                   \
                         _0, _1, _2, _3, _4, _5, _6, _7)

#define RX_BOOL_CHECK_TRUE(CONDITION)                                          \
    RX_BOOL_CHECK_TRUE_MSG(CONDITION, NULL)

#if RX__HAS_VARIADIC_MACROS
    #define RX_BOOL_CHECK_TRUE_MSG(CONDITION, ...)                             \
        RX__BOOL_DEFINE_TEST(RX_NONFATAL, RX__TRUE, CONDITION, __VA_ARGS__)
#else
    #define RX_BOOL_CHECK_TRUE_MSG(CONDITION, MSG)                             \
        RX__BOOL_DEFINE_TEST(RX_NONFATAL, RX__TRUE, CONDITION, MSG,            \
                             NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#endif

#define RX_BOOL_CHECK_TRUE_MSG_1(CONDITION, MSG,                               \
                                 _0)                                           \
    RX__BOOL_DEFINE_TEST(RX_NONFATAL, RX__TRUE, CONDITION, MSG,                \
                         _0, NULL, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_BOOL_CHECK_TRUE_MSG_2(CONDITION, MSG,                               \
                                 _0, _1)                                       \
    RX__BOOL_DEFINE_TEST(RX_NONFATAL, RX__TRUE, CONDITION, MSG,                \
                         _0, _1, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_BOOL_CHECK_TRUE_MSG_3(CONDITION, MSG,                               \
                                 _0, _1, _2)                                   \
    RX__BOOL_DEFINE_TEST(RX_NONFATAL, RX__TRUE, CONDITION, MSG,                \
                         _0, _1, _2, NULL, NULL, NULL, NULL, NULL)

#define RX_BOOL_CHECK_TRUE_MSG_4(CONDITION, MSG,                               \
                                 _0, _1, _2, _3)                               \
    RX__BOOL_DEFINE_TEST(RX_NONFATAL, RX__TRUE, CONDITION, MSG,                \
                         _0, _1, _2, _3, NULL, NULL, NULL, NULL)

#define RX_BOOL_CHECK_TRUE_MSG_5(CONDITION, MSG,                               \
                                 _0, _1, _2, _3, _4)                           \
    RX__BOOL_DEFINE_TEST(RX_NONFATAL, RX__TRUE, CONDITION, MSG,                \
                         _0, _1, _2, _3, _4, NULL, NULL, NULL)

#define RX_BOOL_CHECK_TRUE_MSG_6(CONDITION, MSG,                               \
                                 _0, _1, _2, _3, _4, _5)                       \
    RX__BOOL_DEFINE_TEST(RX_NONFATAL, RX__TRUE, CONDITION, MSG,                \
                         _0, _1, _2, _3, _4, _5, NULL, NULL)

#define RX_BOOL_CHECK_TRUE_MSG_7(CONDITION, MSG,                               \
                                 _0, _1, _2, _3, _4, _5, _6)                   \
    RX__BOOL_DEFINE_TEST(RX_NONFATAL, RX__TRUE, CONDITION, MSG,                \
                         _0, _1, _2, _3, _4, _5, _6, NULL)

#define RX_BOOL_CHECK_TRUE_MSG_8(CONDITION, MSG,                               \
                                 _0, _1, _2, _3, _4, _5, _6, _7)               \
    RX__BOOL_DEFINE_TEST(RX_NONFATAL, RX__TRUE, CONDITION, MSG,                \
                         _0, _1, _2, _3, _4, _5, _6, _7)

#define RX_BOOL_REQUIRE_FALSE(CONDITION)                                       \
    RX_BOOL_REQUIRE_FALSE_MSG(CONDITION, NULL)

#if RX__HAS_VARIADIC_MACROS
    #define RX_BOOL_REQUIRE_FALSE_MSG(CONDITION, ...)                          \
        RX__BOOL_DEFINE_TEST(RX_FATAL, RX__FALSE, CONDITION, __VA_ARGS__)
#else
    #define RX_BOOL_REQUIRE_FALSE_MSG(CONDITION, MSG)                          \
        RX__BOOL_DEFINE_TEST(RX_FATAL, RX__FALSE, CONDITION, MSG,              \
                             NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#endif

#define RX_BOOL_REQUIRE_FALSE_MSG_1(CONDITION, MSG,                            \
                                    _0)                                        \
    RX__BOOL_DEFINE_TEST(RX_FATAL, RX__FALSE, CONDITION, MSG,                  \
                         _0, NULL, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_BOOL_REQUIRE_FALSE_MSG_2(CONDITION, MSG,                            \
                                    _0, _1)                                    \
    RX__BOOL_DEFINE_TEST(RX_FATAL, RX__FALSE, CONDITION, MSG,                  \
                         _0, _1, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_BOOL_REQUIRE_FALSE_MSG_3(CONDITION, MSG,                            \
                                    _0, _1, _2)                                \
    RX__BOOL_DEFINE_TEST(RX_FATAL, RX__FALSE, CONDITION, MSG,                  \
                         _0, _1, _2, NULL, NULL, NULL, NULL, NULL)

#define RX_BOOL_REQUIRE_FALSE_MSG_4(CONDITION, MSG,                            \
                                    _0, _1, _2, _3)                            \
    RX__BOOL_DEFINE_TEST(RX_FATAL, RX__FALSE, CONDITION, MSG,                  \
                         _0, _1, _2, _3, NULL, NULL, NULL, NULL)

#define RX_BOOL_REQUIRE_FALSE_MSG_5(CONDITION, MSG,                            \
                                    _0, _1, _2, _3, _4)                        \
    RX__BOOL_DEFINE_TEST(RX_FATAL, RX__FALSE, CONDITION, MSG,                  \
                         _0, _1, _2, _3, _4, NULL, NULL, NULL)

#define RX_BOOL_REQUIRE_FALSE_MSG_6(CONDITION, MSG,                            \
                                    _0, _1, _2, _3, _4, _5)                    \
    RX__BOOL_DEFINE_TEST(RX_FATAL, RX__FALSE, CONDITION, MSG,                  \
                         _0, _1, _2, _3, _4, _5, NULL, NULL)

#define RX_BOOL_REQUIRE_FALSE_MSG_7(CONDITION, MSG,                            \
                                    _0, _1, _2, _3, _4, _5, _6)                \
    RX__BOOL_DEFINE_TEST(RX_FATAL, RX__FALSE, CONDITION, MSG,                  \
                         _0, _1, _2, _3, _4, _5, _6, NULL)

#define RX_BOOL_REQUIRE_FALSE_MSG_8(CONDITION, MSG,                            \
                                    _0, _1, _2, _3, _4, _5, _6, _7)            \
    RX__BOOL_DEFINE_TEST(RX_FATAL, RX__FALSE, CONDITION, MSG,                  \
                         _0, _1, _2, _3, _4, _5, _6, _7)

#define RX_BOOL_CHECK_FALSE(CONDITION)                                         \
    RX_BOOL_CHECK_FALSE_MSG(CONDITION, NULL)

#if RX__HAS_VARIADIC_MACROS
    #define RX_BOOL_CHECK_FALSE_MSG(CONDITION, ...)                            \
        RX__BOOL_DEFINE_TEST(RX_NONFATAL, RX__FALSE, CONDITION, __VA_ARGS__)
#else
    #define RX_BOOL_CHECK_FALSE_MSG(CONDITION, MSG)                            \
        RX__BOOL_DEFINE_TEST(RX_NONFATAL, RX__FALSE, CONDITION, MSG,           \
                             NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#endif

#define RX_BOOL_CHECK_FALSE_MSG_1(CONDITION, MSG,                              \
                                  _0)                                          \
    RX__BOOL_DEFINE_TEST(RX_NONFATAL, RX__FALSE, CONDITION, MSG,               \
                         _0, NULL, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_BOOL_CHECK_FALSE_MSG_2(CONDITION, MSG,                              \
                                  _0, _1)                                      \
    RX__BOOL_DEFINE_TEST(RX_NONFATAL, RX__FALSE, CONDITION, MSG,               \
                         _0, _1, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_BOOL_CHECK_FALSE_MSG_3(CONDITION, MSG,                              \
                                  _0, _1, _2)                                  \
    RX__BOOL_DEFINE_TEST(RX_NONFATAL, RX__FALSE, CONDITION, MSG,               \
                         _0, _1, _2, NULL, NULL, NULL, NULL, NULL)

#define RX_BOOL_CHECK_FALSE_MSG_4(CONDITION, MSG,                              \
                                  _0, _1, _2, _3)                              \
    RX__BOOL_DEFINE_TEST(RX_NONFATAL, RX__FALSE, CONDITION, MSG,               \
                         _0, _1, _2, _3, NULL, NULL, NULL, NULL)

#define RX_BOOL_CHECK_FALSE_MSG_5(CONDITION, MSG,                              \
                                  _0, _1, _2, _3, _4)                          \
    RX__BOOL_DEFINE_TEST(RX_NONFATAL, RX__FALSE, CONDITION, MSG,               \
                         _0, _1, _2, _3, _4, NULL, NULL, NULL)

#define RX_BOOL_CHECK_FALSE_MSG_6(CONDITION, MSG,                              \
                                  _0, _1, _2, _3, _4, _5)                      \
    RX__BOOL_DEFINE_TEST(RX_NONFATAL, RX__FALSE, CONDITION, MSG,               \
                         _0, _1, _2, _3, _4, _5, NULL, NULL)

#define RX_BOOL_CHECK_FALSE_MSG_7(CONDITION, MSG,                              \
                                  _0, _1, _2, _3, _4, _5, _6)                  \
    RX__BOOL_DEFINE_TEST(RX_NONFATAL, RX__FALSE, CONDITION, MSG,               \
                         _0, _1, _2, _3, _4, _5, _6, NULL)

#define RX_BOOL_CHECK_FALSE_MSG_8(CONDITION, MSG,                              \
                                  _0, _1, _2, _3, _4, _5, _6, _7)              \
    RX__BOOL_DEFINE_TEST(RX_NONFATAL, RX__FALSE, CONDITION, MSG,               \
                         _0, _1, _2, _3, _4, _5, _6, _7)

/* Integer Assertions                                              O-(''Q)
   -------------------------------------------------------------------------- */

#if RX__HAS_VARIADIC_MACROS
    #define RX__INT_DEFINE_TEST(SEVERITY, OP, X1, X2, ...)                     \
        rx__int_assess_comparison(RX_PARAM_CONTEXT,                            \
                                  (X1),                                        \
                                  (X2),                                        \
                                  OP,                                          \
                                  #X1,                                         \
                                  #X2,                                         \
                                  __FILE__,                                    \
                                  __LINE__,                                    \
                                  SEVERITY,                                    \
                                  __VA_ARGS__)
#else
    #define RX__INT_DEFINE_TEST(SEVERITY, OP, X1, X2, MSG,                     \
                                _0, _1, _2, _3, _4, _5, _6, _7)                \
        rx__int_assess_comparison(RX_PARAM_CONTEXT,                            \
                                  (X1),                                        \
                                  (X2),                                        \
                                  OP,                                          \
                                  #X1,                                         \
                                  #X2,                                         \
                                  __FILE__,                                    \
                                  __LINE__,                                    \
                                  SEVERITY,                                    \
                                  MSG,                                         \
                                  _0, _1, _2, _3, _4, _5, _6, _7)
#endif

#define RX_INT_REQUIRE_EQUAL(X1, X2)                                           \
    RX_INT_REQUIRE_EQUAL_MSG(X1, X2, NULL)

#if RX__HAS_VARIADIC_MACROS
    #define RX_INT_REQUIRE_EQUAL_MSG(X1, X2, ...)                              \
        RX__INT_DEFINE_TEST(RX_FATAL, RX__OP_EQUAL, X1, X2, __VA_ARGS__)
#else
    #define RX_INT_REQUIRE_EQUAL_MSG(X1, X2, MSG)                              \
        RX__INT_DEFINE_TEST(RX_FATAL, RX__OP_EQUAL, X1, X2, MSG,               \
                            NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#endif

#define RX_INT_REQUIRE_EQUAL_MSG_1(X1, X2, MSG,                                \
                                   _0)                                         \
    RX__INT_DEFINE_TEST(RX_FATAL, RX__OP_EQUAL, X1, X2, MSG,                   \
                        _0, NULL, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_INT_REQUIRE_EQUAL_MSG_2(X1, X2, MSG,                                \
                                   _0, _1)                                     \
    RX__INT_DEFINE_TEST(RX_FATAL, RX__OP_EQUAL, X1, X2, MSG,                   \
                        _0, _1, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_INT_REQUIRE_EQUAL_MSG_3(X1, X2, MSG,                                \
                                   _0, _1, _2)                                 \
    RX__INT_DEFINE_TEST(RX_FATAL, RX__OP_EQUAL, X1, X2, MSG,                   \
                        _0, _1, _2, NULL, NULL, NULL, NULL, NULL)

#define RX_INT_REQUIRE_EQUAL_MSG_4(X1, X2, MSG,                                \
                                   _0, _1, _2, _3)                             \
    RX__INT_DEFINE_TEST(RX_FATAL, RX__OP_EQUAL, X1, X2, MSG,                   \
                        _0, _1, _2, _3, NULL, NULL, NULL, NULL)

#define RX_INT_REQUIRE_EQUAL_MSG_5(X1, X2, MSG,                                \
                                   _0, _1, _2, _3, _4)                         \
    RX__INT_DEFINE_TEST(RX_FATAL, RX__OP_EQUAL, X1, X2, MSG,                   \
                        _0, _1, _2, _3, _4, NULL, NULL, NULL)

#define RX_INT_REQUIRE_EQUAL_MSG_6(X1, X2, MSG,                                \
                                   _0, _1, _2, _3, _4, _5)                     \
    RX__INT_DEFINE_TEST(RX_FATAL, RX__OP_EQUAL, X1, X2, MSG,                   \
                        _0, _1, _2, _3, _4, _5, NULL, NULL)

#define RX_INT_REQUIRE_EQUAL_MSG_7(X1, X2, MSG,                                \
                                   _0, _1, _2, _3, _4, _5, _6)                 \
    RX__INT_DEFINE_TEST(RX_FATAL, RX__OP_EQUAL, X1, X2, MSG,                   \
                        _0, _1, _2, _3, _4, _5, _6, NULL)

#define RX_INT_REQUIRE_EQUAL_MSG_8(X1, X2, MSG,                                \
                                   _0, _1, _2, _3, _4, _5, _6, _7)             \
    RX__INT_DEFINE_TEST(RX_FATAL, RX__OP_EQUAL, X1, X2, MSG,                   \
                        _0, _1, _2, _3, _4, _5, _6, _7)

#define RX_INT_CHECK_EQUAL(X1, X2)                                             \
    RX_INT_CHECK_EQUAL_MSG(X1, X2, NULL)

#if RX__HAS_VARIADIC_MACROS
    #define RX_INT_CHECK_EQUAL_MSG(X1, X2, ...)                                \
        RX__INT_DEFINE_TEST(RX_NONFATAL, RX__OP_EQUAL, X1, X2, __VA_ARGS__)
#else
    #define RX_INT_CHECK_EQUAL_MSG(X1, X2, MSG)                                \
        RX__INT_DEFINE_TEST(RX_NONFATAL, RX__OP_EQUAL, X1, X2, MSG,            \
                            NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#endif

#define RX_INT_CHECK_EQUAL_MSG_1(X1, X2, MSG,                                  \
                                 _0)                                           \
    RX__INT_DEFINE_TEST(RX_NONFATAL, RX__OP_EQUAL, X1, X2, MSG,                \
                        _0, NULL, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_INT_CHECK_EQUAL_MSG_2(X1, X2, MSG,                                  \
                                 _0, _1)                                       \
    RX__INT_DEFINE_TEST(RX_NONFATAL, RX__OP_EQUAL, X1, X2, MSG,                \
                        _0, _1, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_INT_CHECK_EQUAL_MSG_3(X1, X2, MSG,                                  \
                                 _0, _1, _2)                                   \
    RX__INT_DEFINE_TEST(RX_NONFATAL, RX__OP_EQUAL, X1, X2, MSG,                \
                        _0, _1, _2, NULL, NULL, NULL, NULL, NULL)

#define RX_INT_CHECK_EQUAL_MSG_4(X1, X2, MSG,                                  \
                                 _0, _1, _2, _3)                               \
    RX__INT_DEFINE_TEST(RX_NONFATAL, RX__OP_EQUAL, X1, X2, MSG,                \
                        _0, _1, _2, _3, NULL, NULL, NULL, NULL)

#define RX_INT_CHECK_EQUAL_MSG_5(X1, X2, MSG,                                  \
                                 _0, _1, _2, _3, _4)                           \
    RX__INT_DEFINE_TEST(RX_NONFATAL, RX__OP_EQUAL, X1, X2, MSG,                \
                        _0, _1, _2, _3, _4, NULL, NULL, NULL)

#define RX_INT_CHECK_EQUAL_MSG_6(X1, X2, MSG,                                  \
                                 _0, _1, _2, _3, _4, _5)                       \
    RX__INT_DEFINE_TEST(RX_NONFATAL, RX__OP_EQUAL, X1, X2, MSG,                \
                        _0, _1, _2, _3, _4, _5, NULL, NULL)

#define RX_INT_CHECK_EQUAL_MSG_7(X1, X2, MSG,                                  \
                                 _0, _1, _2, _3, _4, _5, _6)                   \
    RX__INT_DEFINE_TEST(RX_NONFATAL, RX__OP_EQUAL, X1, X2, MSG,                \
                        _0, _1, _2, _3, _4, _5, _6, NULL)

#define RX_INT_CHECK_EQUAL_MSG_8(X1, X2, MSG,                                  \
                                 _0, _1, _2, _3, _4, _5, _6, _7)               \
    RX__INT_DEFINE_TEST(RX_NONFATAL, RX__OP_EQUAL, X1, X2, MSG,                \
                        _0, _1, _2, _3, _4, _5, _6, _7)

#define RX_INT_REQUIRE_NOT_EQUAL(X1, X2)                                       \
    RX_INT_REQUIRE_NOT_EQUAL_MSG(X1, X2, NULL)

#if RX__HAS_VARIADIC_MACROS
    #define RX_INT_REQUIRE_NOT_EQUAL_MSG(X1, X2, ...)                          \
        RX__INT_DEFINE_TEST(RX_FATAL, RX__OP_NOT_EQUAL, X1, X2, __VA_ARGS__)
#else
    #define RX_INT_REQUIRE_NOT_EQUAL_MSG(X1, X2, MSG)                          \
        RX__INT_DEFINE_TEST(RX_FATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,           \
                            NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#endif

#define RX_INT_REQUIRE_NOT_EQUAL_MSG_1(X1, X2, MSG,                            \
                                       _0)                                     \
    RX__INT_DEFINE_TEST(RX_FATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,               \
                        _0, NULL, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_INT_REQUIRE_NOT_EQUAL_MSG_2(X1, X2, MSG,                            \
                                       _0, _1)                                 \
    RX__INT_DEFINE_TEST(RX_FATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,               \
                        _0, _1, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_INT_REQUIRE_NOT_EQUAL_MSG_3(X1, X2, MSG,                            \
                                       _0, _1, _2)                             \
    RX__INT_DEFINE_TEST(RX_FATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,               \
                        _0, _1, _2, NULL, NULL, NULL, NULL, NULL)

#define RX_INT_REQUIRE_NOT_EQUAL_MSG_4(X1, X2, MSG,                            \
                                       _0, _1, _2, _3)                         \
    RX__INT_DEFINE_TEST(RX_FATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,               \
                        _0, _1, _2, _3, NULL, NULL, NULL, NULL)

#define RX_INT_REQUIRE_NOT_EQUAL_MSG_5(X1, X2, MSG,                            \
                                       _0, _1, _2, _3, _4)                     \
    RX__INT_DEFINE_TEST(RX_FATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,               \
                        _0, _1, _2, _3, _4, NULL, NULL, NULL)

#define RX_INT_REQUIRE_NOT_EQUAL_MSG_6(X1, X2, MSG,                            \
                                       _0, _1, _2, _3, _4, _5)                 \
    RX__INT_DEFINE_TEST(RX_FATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,               \
                        _0, _1, _2, _3, _4, _5, NULL, NULL)

#define RX_INT_REQUIRE_NOT_EQUAL_MSG_7(X1, X2, MSG,                            \
                                       _0, _1, _2, _3, _4, _5, _6)             \
    RX__INT_DEFINE_TEST(RX_FATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,               \
                        _0, _1, _2, _3, _4, _5, _6, NULL)

#define RX_INT_REQUIRE_NOT_EQUAL_MSG_8(X1, X2, MSG,                            \
                                       _0, _1, _2, _3, _4, _5, _6, _7)         \
    RX__INT_DEFINE_TEST(RX_FATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,               \
                        _0, _1, _2, _3, _4, _5, _6, _7)

#define RX_INT_CHECK_NOT_EQUAL(X1, X2)                                         \
    RX_INT_CHECK_NOT_EQUAL_MSG(X1, X2, NULL)

#if RX__HAS_VARIADIC_MACROS
    #define RX_INT_CHECK_NOT_EQUAL_MSG(X1, X2, ...)                            \
        RX__INT_DEFINE_TEST(RX_NONFATAL, RX__OP_NOT_EQUAL, X1, X2, __VA_ARGS__)
#else
    #define RX_INT_CHECK_NOT_EQUAL_MSG(X1, X2, MSG)                            \
        RX__INT_DEFINE_TEST(RX_NONFATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,        \
                            NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#endif

#define RX_INT_CHECK_NOT_EQUAL_MSG_1(X1, X2, MSG,                              \
                                     _0)                                       \
    RX__INT_DEFINE_TEST(RX_NONFATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,            \
                        _0, NULL, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_INT_CHECK_NOT_EQUAL_MSG_2(X1, X2, MSG,                              \
                                     _0, _1)                                   \
    RX__INT_DEFINE_TEST(RX_NONFATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,            \
                        _0, _1, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_INT_CHECK_NOT_EQUAL_MSG_3(X1, X2, MSG,                              \
                                     _0, _1, _2)                               \
    RX__INT_DEFINE_TEST(RX_NONFATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,            \
                        _0, _1, _2, NULL, NULL, NULL, NULL, NULL)

#define RX_INT_CHECK_NOT_EQUAL_MSG_4(X1, X2, MSG,                              \
                                     _0, _1, _2, _3)                           \
    RX__INT_DEFINE_TEST(RX_NONFATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,            \
                        _0, _1, _2, _3, NULL, NULL, NULL, NULL)

#define RX_INT_CHECK_NOT_EQUAL_MSG_5(X1, X2, MSG,                              \
                                     _0, _1, _2, _3, _4)                       \
    RX__INT_DEFINE_TEST(RX_NONFATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,            \
                        _0, _1, _2, _3, _4, NULL, NULL, NULL)

#define RX_INT_CHECK_NOT_EQUAL_MSG_6(X1, X2, MSG,                              \
                                     _0, _1, _2, _3, _4, _5)                   \
    RX__INT_DEFINE_TEST(RX_NONFATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,            \
                        _0, _1, _2, _3, _4, _5, NULL, NULL)

#define RX_INT_CHECK_NOT_EQUAL_MSG_7(X1, X2, MSG,                              \
                                     _0, _1, _2, _3, _4, _5, _6)               \
    RX__INT_DEFINE_TEST(RX_NONFATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,            \
                        _0, _1, _2, _3, _4, _5, _6, NULL)

#define RX_INT_CHECK_NOT_EQUAL_MSG_8(X1, X2, MSG,                              \
                                     _0, _1, _2, _3, _4, _5, _6, _7)           \
    RX__INT_DEFINE_TEST(RX_NONFATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,            \
                        _0, _1, _2, _3, _4, _5, _6, _7)

#define RX_INT_REQUIRE_GREATER(X1, X2)                                         \
    RX_INT_REQUIRE_GREATER_MSG(X1, X2, NULL)

#if RX__HAS_VARIADIC_MACROS
    #define RX_INT_REQUIRE_GREATER_MSG(X1, X2, ...)                            \
        RX__INT_DEFINE_TEST(RX_FATAL, RX__OP_GREATER, X1, X2, __VA_ARGS__)
#else
    #define RX_INT_REQUIRE_GREATER_MSG(X1, X2, MSG)                            \
        RX__INT_DEFINE_TEST(RX_FATAL, RX__OP_GREATER, X1, X2, MSG,             \
                            NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#endif

#define RX_INT_REQUIRE_GREATER_MSG_1(X1, X2, MSG,                              \
                                     _0)                                       \
    RX__INT_DEFINE_TEST(RX_FATAL, RX__OP_GREATER, X1, X2, MSG,                 \
                        _0, NULL, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_INT_REQUIRE_GREATER_MSG_2(X1, X2, MSG,                              \
                                     _0, _1)                                   \
    RX__INT_DEFINE_TEST(RX_FATAL, RX__OP_GREATER, X1, X2, MSG,                 \
                        _0, _1, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_INT_REQUIRE_GREATER_MSG_3(X1, X2, MSG,                              \
                                     _0, _1, _2)                               \
    RX__INT_DEFINE_TEST(RX_FATAL, RX__OP_GREATER, X1, X2, MSG,                 \
                        _0, _1, _2, NULL, NULL, NULL, NULL, NULL)

#define RX_INT_REQUIRE_GREATER_MSG_4(X1, X2, MSG,                              \
                                     _0, _1, _2, _3)                           \
    RX__INT_DEFINE_TEST(RX_FATAL, RX__OP_GREATER, X1, X2, MSG,                 \
                        _0, _1, _2, _3, NULL, NULL, NULL, NULL)

#define RX_INT_REQUIRE_GREATER_MSG_5(X1, X2, MSG,                              \
                                     _0, _1, _2, _3, _4)                       \
    RX__INT_DEFINE_TEST(RX_FATAL, RX__OP_GREATER, X1, X2, MSG,                 \
                        _0, _1, _2, _3, _4, NULL, NULL, NULL)

#define RX_INT_REQUIRE_GREATER_MSG_6(X1, X2, MSG,                              \
                                     _0, _1, _2, _3, _4, _5)                   \
    RX__INT_DEFINE_TEST(RX_FATAL, RX__OP_GREATER, X1, X2, MSG,                 \
                        _0, _1, _2, _3, _4, _5, NULL, NULL)

#define RX_INT_REQUIRE_GREATER_MSG_7(X1, X2, MSG,                              \
                                     _0, _1, _2, _3, _4, _5, _6)               \
    RX__INT_DEFINE_TEST(RX_FATAL, RX__OP_GREATER, X1, X2, MSG,                 \
                        _0, _1, _2, _3, _4, _5, _6, NULL)

#define RX_INT_REQUIRE_GREATER_MSG_8(X1, X2, MSG,                              \
                                     _0, _1, _2, _3, _4, _5, _6, _7)           \
    RX__INT_DEFINE_TEST(RX_FATAL, RX__OP_GREATER, X1, X2, MSG,                 \
                        _0, _1, _2, _3, _4, _5, _6, _7)

#define RX_INT_CHECK_GREATER(X1, X2)                                           \
    RX_INT_CHECK_GREATER_MSG(X1, X2, NULL)

#if RX__HAS_VARIADIC_MACROS
    #define RX_INT_CHECK_GREATER_MSG(X1, X2, ...)                              \
        RX__INT_DEFINE_TEST(RX_NONFATAL, RX__OP_GREATER, X1, X2, __VA_ARGS__)
#else
    #define RX_INT_CHECK_GREATER_MSG(X1, X2, MSG)                              \
        RX__INT_DEFINE_TEST(RX_NONFATAL, RX__OP_GREATER, X1, X2, MSG,          \
                            NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#endif

#define RX_INT_CHECK_GREATER_MSG_1(X1, X2, MSG,                                \
                                   _0)                                         \
    RX__INT_DEFINE_TEST(RX_NONFATAL, RX__OP_GREATER, X1, X2, MSG,              \
                        _0, NULL, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_INT_CHECK_GREATER_MSG_2(X1, X2, MSG,                                \
                                   _0, _1)                                     \
    RX__INT_DEFINE_TEST(RX_NONFATAL, RX__OP_GREATER, X1, X2, MSG,              \
                        _0, _1, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_INT_CHECK_GREATER_MSG_3(X1, X2, MSG,                                \
                                   _0, _1, _2)                                 \
    RX__INT_DEFINE_TEST(RX_NONFATAL, RX__OP_GREATER, X1, X2, MSG,              \
                        _0, _1, _2, NULL, NULL, NULL, NULL, NULL)

#define RX_INT_CHECK_GREATER_MSG_4(X1, X2, MSG,                                \
                                   _0, _1, _2, _3)                             \
    RX__INT_DEFINE_TEST(RX_NONFATAL, RX__OP_GREATER, X1, X2, MSG,              \
                        _0, _1, _2, _3, NULL, NULL, NULL, NULL)

#define RX_INT_CHECK_GREATER_MSG_5(X1, X2, MSG,                                \
                                   _0, _1, _2, _3, _4)                         \
    RX__INT_DEFINE_TEST(RX_NONFATAL, RX__OP_GREATER, X1, X2, MSG,              \
                        _0, _1, _2, _3, _4, NULL, NULL, NULL)

#define RX_INT_CHECK_GREATER_MSG_6(X1, X2, MSG,                                \
                                   _0, _1, _2, _3, _4, _5)                     \
    RX__INT_DEFINE_TEST(RX_NONFATAL, RX__OP_GREATER, X1, X2, MSG,              \
                        _0, _1, _2, _3, _4, _5, NULL, NULL)

#define RX_INT_CHECK_GREATER_MSG_7(X1, X2, MSG,                                \
                                   _0, _1, _2, _3, _4, _5, _6)                 \
    RX__INT_DEFINE_TEST(RX_NONFATAL, RX__OP_GREATER, X1, X2, MSG,              \
                        _0, _1, _2, _3, _4, _5, _6, NULL)

#define RX_INT_CHECK_GREATER_MSG_8(X1, X2, MSG,                                \
                                   _0, _1, _2, _3, _4, _5, _6, _7)             \
    RX__INT_DEFINE_TEST(RX_NONFATAL, RX__OP_GREATER, X1, X2, MSG,              \
                        _0, _1, _2, _3, _4, _5, _6, _7)

#define RX_INT_REQUIRE_LESSER(X1, X2)                                          \
    RX_INT_REQUIRE_LESSER_MSG(X1, X2, NULL)

#if RX__HAS_VARIADIC_MACROS
    #define RX_INT_REQUIRE_LESSER_MSG(X1, X2, ...)                             \
        RX__INT_DEFINE_TEST(RX_FATAL, RX__OP_LESSER, X1, X2, __VA_ARGS__)
#else
    #define RX_INT_REQUIRE_LESSER_MSG(X1, X2, MSG)                             \
        RX__INT_DEFINE_TEST(RX_FATAL, RX__OP_LESSER, X1, X2, MSG,              \
                            NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#endif

#define RX_INT_REQUIRE_LESSER_MSG_1(X1, X2, MSG,                               \
                                    _0)                                        \
    RX__INT_DEFINE_TEST(RX_FATAL, RX__OP_LESSER, X1, X2, MSG,                  \
                        _0, NULL, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_INT_REQUIRE_LESSER_MSG_2(X1, X2, MSG,                               \
                                    _0, _1)                                    \
    RX__INT_DEFINE_TEST(RX_FATAL, RX__OP_LESSER, X1, X2, MSG,                  \
                        _0, _1, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_INT_REQUIRE_LESSER_MSG_3(X1, X2, MSG,                               \
                                    _0, _1, _2)                                \
    RX__INT_DEFINE_TEST(RX_FATAL, RX__OP_LESSER, X1, X2, MSG,                  \
                        _0, _1, _2, NULL, NULL, NULL, NULL, NULL)

#define RX_INT_REQUIRE_LESSER_MSG_4(X1, X2, MSG,                               \
                                    _0, _1, _2, _3)                            \
    RX__INT_DEFINE_TEST(RX_FATAL, RX__OP_LESSER, X1, X2, MSG,                  \
                        _0, _1, _2, _3, NULL, NULL, NULL, NULL)

#define RX_INT_REQUIRE_LESSER_MSG_5(X1, X2, MSG,                               \
                                    _0, _1, _2, _3, _4)                        \
    RX__INT_DEFINE_TEST(RX_FATAL, RX__OP_LESSER, X1, X2, MSG,                  \
                        _0, _1, _2, _3, _4, NULL, NULL, NULL)

#define RX_INT_REQUIRE_LESSER_MSG_6(X1, X2, MSG,                               \
                                    _0, _1, _2, _3, _4, _5)                    \
    RX__INT_DEFINE_TEST(RX_FATAL, RX__OP_LESSER, X1, X2, MSG,                  \
                        _0, _1, _2, _3, _4, _5, NULL, NULL)

#define RX_INT_REQUIRE_LESSER_MSG_7(X1, X2, MSG,                               \
                                    _0, _1, _2, _3, _4, _5, _6)                \
    RX__INT_DEFINE_TEST(RX_FATAL, RX__OP_LESSER, X1, X2, MSG,                  \
                        _0, _1, _2, _3, _4, _5, _6, NULL)

#define RX_INT_REQUIRE_LESSER_MSG_8(X1, X2, MSG,                               \
                                    _0, _1, _2, _3, _4, _5, _6, _7)            \
    RX__INT_DEFINE_TEST(RX_FATAL, RX__OP_LESSER, X1, X2, MSG,                  \
                        _0, _1, _2, _3, _4, _5, _6, _7)

#define RX_INT_CHECK_LESSER(X1, X2)                                            \
    RX_INT_CHECK_LESSER_MSG(X1, X2, NULL)

#if RX__HAS_VARIADIC_MACROS
    #define RX_INT_CHECK_LESSER_MSG(X1, X2, ...)                               \
        RX__INT_DEFINE_TEST(RX_NONFATAL, RX__OP_LESSER, X1, X2, __VA_ARGS__)
#else
    #define RX_INT_CHECK_LESSER_MSG(X1, X2, MSG)                               \
        RX__INT_DEFINE_TEST(RX_NONFATAL, RX__OP_LESSER, X1, X2, MSG,           \
                            NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#endif

#define RX_INT_CHECK_LESSER_MSG_1(X1, X2, MSG,                                 \
                                  _0)                                          \
    RX__INT_DEFINE_TEST(RX_NONFATAL, RX__OP_LESSER, X1, X2, MSG,               \
                        _0, NULL, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_INT_CHECK_LESSER_MSG_2(X1, X2, MSG,                                 \
                                  _0, _1)                                      \
    RX__INT_DEFINE_TEST(RX_NONFATAL, RX__OP_LESSER, X1, X2, MSG,               \
                        _0, _1, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_INT_CHECK_LESSER_MSG_3(X1, X2, MSG,                                 \
                                  _0, _1, _2)                                  \
    RX__INT_DEFINE_TEST(RX_NONFATAL, RX__OP_LESSER, X1, X2, MSG,               \
                        _0, _1, _2, NULL, NULL, NULL, NULL, NULL)

#define RX_INT_CHECK_LESSER_MSG_4(X1, X2, MSG,                                 \
                                  _0, _1, _2, _3)                              \
    RX__INT_DEFINE_TEST(RX_NONFATAL, RX__OP_LESSER, X1, X2, MSG,               \
                        _0, _1, _2, _3, NULL, NULL, NULL, NULL)

#define RX_INT_CHECK_LESSER_MSG_5(X1, X2, MSG,                                 \
                                  _0, _1, _2, _3, _4)                          \
    RX__INT_DEFINE_TEST(RX_NONFATAL, RX__OP_LESSER, X1, X2, MSG,               \
                        _0, _1, _2, _3, _4, NULL, NULL, NULL)

#define RX_INT_CHECK_LESSER_MSG_6(X1, X2, MSG,                                 \
                                  _0, _1, _2, _3, _4, _5)                      \
    RX__INT_DEFINE_TEST(RX_NONFATAL, RX__OP_LESSER, X1, X2, MSG,               \
                        _0, _1, _2, _3, _4, _5, NULL, NULL)

#define RX_INT_CHECK_LESSER_MSG_7(X1, X2, MSG,                                 \
                                  _0, _1, _2, _3, _4, _5, _6)                  \
    RX__INT_DEFINE_TEST(RX_NONFATAL, RX__OP_LESSER, X1, X2, MSG,               \
                        _0, _1, _2, _3, _4, _5, _6, NULL)

#define RX_INT_CHECK_LESSER_MSG_8(X1, X2, MSG,                                 \
                                  _0, _1, _2, _3, _4, _5, _6, _7)              \
    RX__INT_DEFINE_TEST(RX_NONFATAL, RX__OP_LESSER, X1, X2, MSG,               \
                        _0, _1, _2, _3, _4, _5, _6, _7)

#define RX_INT_REQUIRE_GREATER_OR_EQUAL(X1, X2)                                \
    RX_INT_REQUIRE_GREATER_OR_EQUAL_MSG(X1, X2, NULL)

#if RX__HAS_VARIADIC_MACROS
    #define RX_INT_REQUIRE_GREATER_OR_EQUAL_MSG(X1, X2, ...)                   \
        RX__INT_DEFINE_TEST(RX_FATAL, RX__OP_GREATER_OR_EQUAL, X1, X2,         \
                            __VA_ARGS__)
#else
    #define RX_INT_REQUIRE_GREATER_OR_EQUAL_MSG(X1, X2, MSG)                   \
        RX__INT_DEFINE_TEST(RX_FATAL, RX__OP_GREATER_OR_EQUAL, X1, X2, MSG,    \
                            NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#endif

#define RX_INT_REQUIRE_GREATER_OR_EQUAL_MSG_1(X1, X2, MSG,                     \
                                              _0)                              \
    RX__INT_DEFINE_TEST(RX_FATAL, RX__OP_GREATER_OR_EQUAL, X1, X2, MSG,        \
                        _0, NULL, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_INT_REQUIRE_GREATER_OR_EQUAL_MSG_2(X1, X2, MSG,                     \
                                              _0, _1)                          \
    RX__INT_DEFINE_TEST(RX_FATAL, RX__OP_GREATER_OR_EQUAL, X1, X2, MSG,        \
                        _0, _1, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_INT_REQUIRE_GREATER_OR_EQUAL_MSG_3(X1, X2, MSG,                     \
                                              _0, _1, _2)                      \
    RX__INT_DEFINE_TEST(RX_FATAL, RX__OP_GREATER_OR_EQUAL, X1, X2, MSG,        \
                        _0, _1, _2, NULL, NULL, NULL, NULL, NULL)

#define RX_INT_REQUIRE_GREATER_OR_EQUAL_MSG_4(X1, X2, MSG,                     \
                                              _0, _1, _2, _3)                  \
    RX__INT_DEFINE_TEST(RX_FATAL, RX__OP_GREATER_OR_EQUAL, X1, X2, MSG,        \
                        _0, _1, _2, _3, NULL, NULL, NULL, NULL)

#define RX_INT_REQUIRE_GREATER_OR_EQUAL_MSG_5(X1, X2, MSG,                     \
                                              _0, _1, _2, _3, _4)              \
    RX__INT_DEFINE_TEST(RX_FATAL, RX__OP_GREATER_OR_EQUAL, X1, X2, MSG,        \
                        _0, _1, _2, _3, _4, NULL, NULL, NULL)

#define RX_INT_REQUIRE_GREATER_OR_EQUAL_MSG_6(X1, X2, MSG,                     \
                                              _0, _1, _2, _3, _4, _5)          \
    RX__INT_DEFINE_TEST(RX_FATAL, RX__OP_GREATER_OR_EQUAL, X1, X2, MSG,        \
                        _0, _1, _2, _3, _4, _5, NULL, NULL)

#define RX_INT_REQUIRE_GREATER_OR_EQUAL_MSG_7(X1, X2, MSG,                     \
                                              _0, _1, _2, _3, _4, _5, _6)      \
    RX__INT_DEFINE_TEST(RX_FATAL, RX__OP_GREATER_OR_EQUAL, X1, X2, MSG,        \
                        _0, _1, _2, _3, _4, _5, _6, NULL)

#define RX_INT_REQUIRE_GREATER_OR_EQUAL_MSG_8(X1, X2, MSG,                     \
                                              _0, _1, _2, _3, _4, _5, _6, _7)  \
    RX__INT_DEFINE_TEST(RX_FATAL, RX__OP_GREATER_OR_EQUAL, X1, X2, MSG,        \
                        _0, _1, _2, _3, _4, _5, _6, _7)

#define RX_INT_CHECK_GREATER_OR_EQUAL(X1, X2)                                  \
    RX_INT_CHECK_GREATER_OR_EQUAL_MSG(X1, X2, NULL)

#if RX__HAS_VARIADIC_MACROS
    #define RX_INT_CHECK_GREATER_OR_EQUAL_MSG(X1, X2, ...)                     \
        RX__INT_DEFINE_TEST(RX_NONFATAL, RX__OP_GREATER_OR_EQUAL, X1, X2,      \
                            __VA_ARGS__)
#else
    #define RX_INT_CHECK_GREATER_OR_EQUAL_MSG(X1, X2, MSG)                     \
        RX__INT_DEFINE_TEST(RX_NONFATAL, RX__OP_GREATER_OR_EQUAL, X1, X2, MSG, \
                            NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#endif

#define RX_INT_CHECK_GREATER_OR_EQUAL_MSG_1(X1, X2, MSG,                       \
                                            _0)                                \
    RX__INT_DEFINE_TEST(RX_NONFATAL, RX__OP_GREATER_OR_EQUAL, X1, X2, MSG,     \
                        _0, NULL, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_INT_CHECK_GREATER_OR_EQUAL_MSG_2(X1, X2, MSG,                       \
                                            _0, _1)                            \
    RX__INT_DEFINE_TEST(RX_NONFATAL, RX__OP_GREATER_OR_EQUAL, X1, X2, MSG,     \
                        _0, _1, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_INT_CHECK_GREATER_OR_EQUAL_MSG_3(X1, X2, MSG,                       \
                                            _0, _1, _2)                        \
    RX__INT_DEFINE_TEST(RX_NONFATAL, RX__OP_GREATER_OR_EQUAL, X1, X2, MSG,     \
                        _0, _1, _2, NULL, NULL, NULL, NULL, NULL)

#define RX_INT_CHECK_GREATER_OR_EQUAL_MSG_4(X1, X2, MSG,                       \
                                            _0, _1, _2, _3)                    \
    RX__INT_DEFINE_TEST(RX_NONFATAL, RX__OP_GREATER_OR_EQUAL, X1, X2, MSG,     \
                        _0, _1, _2, _3, NULL, NULL, NULL, NULL)

#define RX_INT_CHECK_GREATER_OR_EQUAL_MSG_5(X1, X2, MSG,                       \
                                            _0, _1, _2, _3, _4)                \
    RX__INT_DEFINE_TEST(RX_NONFATAL, RX__OP_GREATER_OR_EQUAL, X1, X2, MSG,     \
                        _0, _1, _2, _3, _4, NULL, NULL, NULL)

#define RX_INT_CHECK_GREATER_OR_EQUAL_MSG_6(X1, X2, MSG,                       \
                                            _0, _1, _2, _3, _4, _5)            \
    RX__INT_DEFINE_TEST(RX_NONFATAL, RX__OP_GREATER_OR_EQUAL, X1, X2, MSG,     \
                        _0, _1, _2, _3, _4, _5, NULL, NULL)

#define RX_INT_CHECK_GREATER_OR_EQUAL_MSG_7(X1, X2, MSG,                       \
                                            _0, _1, _2, _3, _4, _5, _6)        \
    RX__INT_DEFINE_TEST(RX_NONFATAL, RX__OP_GREATER_OR_EQUAL, X1, X2, MSG,     \
                        _0, _1, _2, _3, _4, _5, _6, NULL)

#define RX_INT_CHECK_GREATER_OR_EQUAL_MSG_8(X1, X2, MSG,                       \
                                            _0, _1, _2, _3, _4, _5, _6, _7)    \
    RX__INT_DEFINE_TEST(RX_NONFATAL, RX__OP_GREATER_OR_EQUAL, X1, X2, MSG,     \
                        _0, _1, _2, _3, _4, _5, _6, _7)

#define RX_INT_REQUIRE_LESSER_OR_EQUAL(X1, X2)                                 \
    RX_INT_REQUIRE_LESSER_OR_EQUAL_MSG(X1, X2, NULL)

#if RX__HAS_VARIADIC_MACROS
    #define RX_INT_REQUIRE_LESSER_OR_EQUAL_MSG(X1, X2, ...)                    \
        RX__INT_DEFINE_TEST(RX_FATAL, RX__OP_LESSER_OR_EQUAL, X1, X2,          \
                            __VA_ARGS__)
#else
    #define RX_INT_REQUIRE_LESSER_OR_EQUAL_MSG(X1, X2, MSG)                    \
        RX__INT_DEFINE_TEST(RX_FATAL, RX__OP_LESSER_OR_EQUAL, X1, X2, MSG,     \
                            NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#endif

#define RX_INT_REQUIRE_LESSER_OR_EQUAL_MSG_1(X1, X2, MSG,                      \
                                             _0)                               \
    RX__INT_DEFINE_TEST(RX_FATAL, RX__OP_LESSER_OR_EQUAL, X1, X2, MSG,         \
                        _0, NULL, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_INT_REQUIRE_LESSER_OR_EQUAL_MSG_2(X1, X2, MSG,                      \
                                             _0, _1)                           \
    RX__INT_DEFINE_TEST(RX_FATAL, RX__OP_LESSER_OR_EQUAL, X1, X2, MSG,         \
                        _0, _1, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_INT_REQUIRE_LESSER_OR_EQUAL_MSG_3(X1, X2, MSG,                      \
                                             _0, _1, _2)                       \
    RX__INT_DEFINE_TEST(RX_FATAL, RX__OP_LESSER_OR_EQUAL, X1, X2, MSG,         \
                        _0, _1, _2, NULL, NULL, NULL, NULL, NULL)

#define RX_INT_REQUIRE_LESSER_OR_EQUAL_MSG_4(X1, X2, MSG,                      \
                                             _0, _1, _2, _3)                   \
    RX__INT_DEFINE_TEST(RX_FATAL, RX__OP_LESSER_OR_EQUAL, X1, X2, MSG,         \
                        _0, _1, _2, _3, NULL, NULL, NULL, NULL)

#define RX_INT_REQUIRE_LESSER_OR_EQUAL_MSG_5(X1, X2, MSG,                      \
                                             _0, _1, _2, _3, _4)               \
    RX__INT_DEFINE_TEST(RX_FATAL, RX__OP_LESSER_OR_EQUAL, X1, X2, MSG,         \
                        _0, _1, _2, _3, _4, NULL, NULL, NULL)

#define RX_INT_REQUIRE_LESSER_OR_EQUAL_MSG_6(X1, X2, MSG,                      \
                                             _0, _1, _2, _3, _4, _5)           \
    RX__INT_DEFINE_TEST(RX_FATAL, RX__OP_LESSER_OR_EQUAL, X1, X2, MSG,         \
                        _0, _1, _2, _3, _4, _5, NULL, NULL)

#define RX_INT_REQUIRE_LESSER_OR_EQUAL_MSG_7(X1, X2, MSG,                      \
                                             _0, _1, _2, _3, _4, _5, _6)       \
    RX__INT_DEFINE_TEST(RX_FATAL, RX__OP_LESSER_OR_EQUAL, X1, X2, MSG,         \
                        _0, _1, _2, _3, _4, _5, _6, NULL)

#define RX_INT_REQUIRE_LESSER_OR_EQUAL_MSG_8(X1, X2, MSG,                      \
                                             _0, _1, _2, _3, _4, _5, _6, _7)   \
    RX__INT_DEFINE_TEST(RX_FATAL, RX__OP_LESSER_OR_EQUAL, X1, X2, MSG,         \
                        _0, _1, _2, _3, _4, _5, _6, _7)

#define RX_INT_CHECK_LESSER_OR_EQUAL(X1, X2)                                   \
    RX_INT_CHECK_LESSER_OR_EQUAL_MSG(X1, X2, NULL)

#if RX__HAS_VARIADIC_MACROS
    #define RX_INT_CHECK_LESSER_OR_EQUAL_MSG(X1, X2, ...)                      \
        RX__INT_DEFINE_TEST(RX_NONFATAL, RX__OP_LESSER_OR_EQUAL, X1, X2,       \
                            __VA_ARGS__)
#else
    #define RX_INT_CHECK_LESSER_OR_EQUAL_MSG(X1, X2, MSG)                      \
        RX__INT_DEFINE_TEST(RX_NONFATAL, RX__OP_LESSER_OR_EQUAL, X1, X2, MSG,  \
                            NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#endif

#define RX_INT_CHECK_LESSER_OR_EQUAL_MSG_1(X1, X2, MSG,                        \
                                           _0)                                 \
    RX__INT_DEFINE_TEST(RX_NONFATAL, RX__OP_LESSER_OR_EQUAL, X1, X2, MSG,      \
                        _0, NULL, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_INT_CHECK_LESSER_OR_EQUAL_MSG_2(X1, X2, MSG,                        \
                                           _0, _1)                             \
    RX__INT_DEFINE_TEST(RX_NONFATAL, RX__OP_LESSER_OR_EQUAL, X1, X2, MSG,      \
                        _0, _1, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_INT_CHECK_LESSER_OR_EQUAL_MSG_3(X1, X2, MSG,                        \
                                           _0, _1, _2)                         \
    RX__INT_DEFINE_TEST(RX_NONFATAL, RX__OP_LESSER_OR_EQUAL, X1, X2, MSG,      \
                        _0, _1, _2, NULL, NULL, NULL, NULL, NULL)

#define RX_INT_CHECK_LESSER_OR_EQUAL_MSG_4(X1, X2, MSG,                        \
                                           _0, _1, _2, _3)                     \
    RX__INT_DEFINE_TEST(RX_NONFATAL, RX__OP_LESSER_OR_EQUAL, X1, X2, MSG,      \
                        _0, _1, _2, _3, NULL, NULL, NULL, NULL)

#define RX_INT_CHECK_LESSER_OR_EQUAL_MSG_5(X1, X2, MSG,                        \
                                           _0, _1, _2, _3, _4)                 \
    RX__INT_DEFINE_TEST(RX_NONFATAL, RX__OP_LESSER_OR_EQUAL, X1, X2, MSG,      \
                        _0, _1, _2, _3, _4, NULL, NULL, NULL)

#define RX_INT_CHECK_LESSER_OR_EQUAL_MSG_6(X1, X2, MSG,                        \
                                           _0, _1, _2, _3, _4, _5)             \
    RX__INT_DEFINE_TEST(RX_NONFATAL, RX__OP_LESSER_OR_EQUAL, X1, X2, MSG,      \
                        _0, _1, _2, _3, _4, _5, NULL, NULL)

#define RX_INT_CHECK_LESSER_OR_EQUAL_MSG_7(X1, X2, MSG,                        \
                                           _0, _1, _2, _3, _4, _5, _6)         \
    RX__INT_DEFINE_TEST(RX_NONFATAL, RX__OP_LESSER_OR_EQUAL, X1, X2, MSG,      \
                        _0, _1, _2, _3, _4, _5, _6, NULL)

#define RX_INT_CHECK_LESSER_OR_EQUAL_MSG_8(X1, X2, MSG,                        \
                                           _0, _1, _2, _3, _4, _5, _6, _7)     \
    RX__INT_DEFINE_TEST(RX_NONFATAL, RX__OP_LESSER_OR_EQUAL, X1, X2, MSG,      \
                        _0, _1, _2, _3, _4, _5, _6, _7)

/* Unsigned Integer Assertions                                     O-(''Q)
   -------------------------------------------------------------------------- */

#if RX__HAS_VARIADIC_MACROS
    #define RX__UINT_DEFINE_TEST(SEVERITY, OP, X1, X2, ...)                    \
        rx__uint_assess_comparison(RX_PARAM_CONTEXT,                           \
                                   (X1),                                       \
                                   (X2),                                       \
                                   OP,                                         \
                                   #X1,                                        \
                                   #X2,                                        \
                                   __FILE__,                                   \
                                   __LINE__,                                   \
                                   SEVERITY,                                   \
                                   __VA_ARGS__)
#else
    #define RX__UINT_DEFINE_TEST(SEVERITY, OP, X1, X2, MSG,                    \
                                 _0, _1, _2, _3, _4, _5, _6, _7)               \
        rx__uint_assess_comparison(RX_PARAM_CONTEXT,                           \
                                   (X1),                                       \
                                   (X2),                                       \
                                   OP,                                         \
                                   #X1,                                        \
                                   #X2,                                        \
                                   __FILE__,                                   \
                                   __LINE__,                                   \
                                   SEVERITY,                                   \
                                   MSG,                                        \
                                   _0, _1, _2, _3, _4, _5, _6, _7)
#endif

#define RX_UINT_REQUIRE_EQUAL(X1, X2)                                          \
    RX_UINT_REQUIRE_EQUAL_MSG(X1, X2, NULL)

#if RX__HAS_VARIADIC_MACROS
    #define RX_UINT_REQUIRE_EQUAL_MSG(X1, X2, ...)                             \
        RX__UINT_DEFINE_TEST(RX_FATAL, RX__OP_EQUAL, X1, X2, __VA_ARGS__)
#else
    #define RX_UINT_REQUIRE_EQUAL_MSG(X1, X2, MSG)                             \
        RX__UINT_DEFINE_TEST(RX_FATAL, RX__OP_EQUAL, X1, X2, MSG,              \
                             NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#endif

#define RX_UINT_REQUIRE_EQUAL_MSG_1(X1, X2, MSG,                               \
                                    _0)                                        \
    RX__UINT_DEFINE_TEST(RX_FATAL, RX__OP_EQUAL, X1, X2, MSG,                  \
                         _0, NULL, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_UINT_REQUIRE_EQUAL_MSG_2(X1, X2, MSG,                               \
                                    _0, _1)                                    \
    RX__UINT_DEFINE_TEST(RX_FATAL, RX__OP_EQUAL, X1, X2, MSG,                  \
                         _0, _1, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_UINT_REQUIRE_EQUAL_MSG_3(X1, X2, MSG,                               \
                                    _0, _1, _2)                                \
    RX__UINT_DEFINE_TEST(RX_FATAL, RX__OP_EQUAL, X1, X2, MSG,                  \
                         _0, _1, _2, NULL, NULL, NULL, NULL, NULL)

#define RX_UINT_REQUIRE_EQUAL_MSG_4(X1, X2, MSG,                               \
                                    _0, _1, _2, _3)                            \
    RX__UINT_DEFINE_TEST(RX_FATAL, RX__OP_EQUAL, X1, X2, MSG,                  \
                         _0, _1, _2, _3, NULL, NULL, NULL, NULL)

#define RX_UINT_REQUIRE_EQUAL_MSG_5(X1, X2, MSG,                               \
                                    _0, _1, _2, _3, _4)                        \
    RX__UINT_DEFINE_TEST(RX_FATAL, RX__OP_EQUAL, X1, X2, MSG,                  \
                         _0, _1, _2, _3, _4, NULL, NULL, NULL)

#define RX_UINT_REQUIRE_EQUAL_MSG_6(X1, X2, MSG,                               \
                                    _0, _1, _2, _3, _4, _5)                    \
    RX__UINT_DEFINE_TEST(RX_FATAL, RX__OP_EQUAL, X1, X2, MSG,                  \
                         _0, _1, _2, _3, _4, _5, NULL, NULL)

#define RX_UINT_REQUIRE_EQUAL_MSG_7(X1, X2, MSG,                               \
                                    _0, _1, _2, _3, _4, _5, _6)                \
    RX__UINT_DEFINE_TEST(RX_FATAL, RX__OP_EQUAL, X1, X2, MSG,                  \
                         _0, _1, _2, _3, _4, _5, _6, NULL)

#define RX_UINT_REQUIRE_EQUAL_MSG_8(X1, X2, MSG,                               \
                                    _0, _1, _2, _3, _4, _5, _6, _7)            \
    RX__UINT_DEFINE_TEST(RX_FATAL, RX__OP_EQUAL, X1, X2, MSG,                  \
                         _0, _1, _2, _3, _4, _5, _6, _7)

#define RX_UINT_CHECK_EQUAL(X1, X2)                                            \
    RX_UINT_CHECK_EQUAL_MSG(X1, X2, NULL)

#if RX__HAS_VARIADIC_MACROS
    #define RX_UINT_CHECK_EQUAL_MSG(X1, X2, ...)                               \
        RX__UINT_DEFINE_TEST(RX_NONFATAL, RX__OP_EQUAL, X1, X2, __VA_ARGS__)
#else
    #define RX_UINT_CHECK_EQUAL_MSG(X1, X2, MSG)                               \
        RX__UINT_DEFINE_TEST(RX_NONFATAL, RX__OP_EQUAL, X1, X2, MSG,           \
                             NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#endif

#define RX_UINT_CHECK_EQUAL_MSG_1(X1, X2, MSG,                                 \
                                  _0)                                          \
    RX__UINT_DEFINE_TEST(RX_NONFATAL, RX__OP_EQUAL, X1, X2, MSG,               \
                         _0, NULL, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_UINT_CHECK_EQUAL_MSG_2(X1, X2, MSG,                                 \
                                  _0, _1)                                      \
    RX__UINT_DEFINE_TEST(RX_NONFATAL, RX__OP_EQUAL, X1, X2, MSG,               \
                         _0, _1, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_UINT_CHECK_EQUAL_MSG_3(X1, X2, MSG,                                 \
                                  _0, _1, _2)                                  \
    RX__UINT_DEFINE_TEST(RX_NONFATAL, RX__OP_EQUAL, X1, X2, MSG,               \
                         _0, _1, _2, NULL, NULL, NULL, NULL, NULL)

#define RX_UINT_CHECK_EQUAL_MSG_4(X1, X2, MSG,                                 \
                                  _0, _1, _2, _3)                              \
    RX__UINT_DEFINE_TEST(RX_NONFATAL, RX__OP_EQUAL, X1, X2, MSG,               \
                         _0, _1, _2, _3, NULL, NULL, NULL, NULL)

#define RX_UINT_CHECK_EQUAL_MSG_5(X1, X2, MSG,                                 \
                                  _0, _1, _2, _3, _4)                          \
    RX__UINT_DEFINE_TEST(RX_NONFATAL, RX__OP_EQUAL, X1, X2, MSG,               \
                          _0, _1, _2, _3, _4, NULL, NULL, NULL)

#define RX_UINT_CHECK_EQUAL_MSG_6(X1, X2, MSG,                                 \
                                  _0, _1, _2, _3, _4, _5)                      \
    RX__UINT_DEFINE_TEST(RX_NONFATAL, RX__OP_EQUAL, X1, X2, MSG,               \
                         _0, _1, _2, _3, _4, _5, NULL, NULL)

#define RX_UINT_CHECK_EQUAL_MSG_7(X1, X2, MSG,                                 \
                                  _0, _1, _2, _3, _4, _5, _6)                  \
    RX__UINT_DEFINE_TEST(RX_NONFATAL, RX__OP_EQUAL, X1, X2, MSG,               \
                         _0, _1, _2, _3, _4, _5, _6, NULL)

#define RX_UINT_CHECK_EQUAL_MSG_8(X1, X2, MSG,                                 \
                                  _0, _1, _2, _3, _4, _5, _6, _7)              \
    RX__UINT_DEFINE_TEST(RX_NONFATAL, RX__OP_EQUAL, X1, X2, MSG,               \
                         _0, _1, _2, _3, _4, _5, _6, _7)

#define RX_UINT_REQUIRE_NOT_EQUAL(X1, X2)                                      \
    RX_UINT_REQUIRE_NOT_EQUAL_MSG(X1, X2, NULL)

#if RX__HAS_VARIADIC_MACROS
    #define RX_UINT_REQUIRE_NOT_EQUAL_MSG(X1, X2, ...)                         \
        RX__UINT_DEFINE_TEST(RX_FATAL, RX__OP_NOT_EQUAL, X1, X2, __VA_ARGS__)
#else
    #define RX_UINT_REQUIRE_NOT_EQUAL_MSG(X1, X2, MSG)                         \
        RX__UINT_DEFINE_TEST(RX_FATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,          \
                             NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#endif

#define RX_UINT_REQUIRE_NOT_EQUAL_MSG_1(X1, X2, MSG,                           \
                                        _0)                                    \
    RX__UINT_DEFINE_TEST(RX_FATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,              \
                         _0, NULL, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_UINT_REQUIRE_NOT_EQUAL_MSG_2(X1, X2, MSG,                           \
                                        _0, _1)                                \
    RX__UINT_DEFINE_TEST(RX_FATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,              \
                         _0, _1, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_UINT_REQUIRE_NOT_EQUAL_MSG_3(X1, X2, MSG,                           \
                                        _0, _1, _2)                            \
    RX__UINT_DEFINE_TEST(RX_FATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,              \
                         _0, _1, _2, NULL, NULL, NULL, NULL, NULL)

#define RX_UINT_REQUIRE_NOT_EQUAL_MSG_4(X1, X2, MSG,                           \
                                        _0, _1, _2, _3)                        \
    RX__UINT_DEFINE_TEST(RX_FATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,              \
                         _0, _1, _2, _3, NULL, NULL, NULL, NULL)

#define RX_UINT_REQUIRE_NOT_EQUAL_MSG_5(X1, X2, MSG,                           \
                                        _0, _1, _2, _3, _4)                    \
    RX__UINT_DEFINE_TEST(RX_FATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,              \
                         _0, _1, _2, _3, _4, NULL, NULL, NULL)

#define RX_UINT_REQUIRE_NOT_EQUAL_MSG_6(X1, X2, MSG,                           \
                                        _0, _1, _2, _3, _4, _5)                \
    RX__UINT_DEFINE_TEST(RX_FATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,              \
                         _0, _1, _2, _3, _4, _5, NULL, NULL)

#define RX_UINT_REQUIRE_NOT_EQUAL_MSG_7(X1, X2, MSG,                           \
                                        _0, _1, _2, _3, _4, _5, _6)            \
    RX__UINT_DEFINE_TEST(RX_FATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,              \
                         _0, _1, _2, _3, _4, _5, _6, NULL)

#define RX_UINT_REQUIRE_NOT_EQUAL_MSG_8(X1, X2, MSG,                           \
                                        _0, _1, _2, _3, _4, _5, _6, _7)        \
    RX__UINT_DEFINE_TEST(RX_FATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,              \
                         _0, _1, _2, _3, _4, _5, _6, _7)

#define RX_UINT_CHECK_NOT_EQUAL(X1, X2)                                        \
    RX_UINT_CHECK_NOT_EQUAL_MSG(X1, X2, NULL)

#if RX__HAS_VARIADIC_MACROS
    #define RX_UINT_CHECK_NOT_EQUAL_MSG(X1, X2, ...)                           \
        RX__UINT_DEFINE_TEST(RX_NONFATAL, RX__OP_NOT_EQUAL, X1, X2, __VA_ARGS__)
#else
    #define RX_UINT_CHECK_NOT_EQUAL_MSG(X1, X2, MSG)                           \
        RX__UINT_DEFINE_TEST(RX_NONFATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,       \
                             NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#endif

#define RX_UINT_CHECK_NOT_EQUAL_MSG_1(X1, X2, MSG,                             \
                                      _0)                                      \
    RX__UINT_DEFINE_TEST(RX_NONFATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,           \
                         _0, NULL, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_UINT_CHECK_NOT_EQUAL_MSG_2(X1, X2, MSG,                             \
                                      _0, _1)                                  \
    RX__UINT_DEFINE_TEST(RX_NONFATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,           \
                         _0, _1, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_UINT_CHECK_NOT_EQUAL_MSG_3(X1, X2, MSG,                             \
                                      _0, _1, _2)                              \
    RX__UINT_DEFINE_TEST(RX_NONFATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,           \
                         _0, _1, _2, NULL, NULL, NULL, NULL, NULL)

#define RX_UINT_CHECK_NOT_EQUAL_MSG_4(X1, X2, MSG,                             \
                                      _0, _1, _2, _3)                          \
    RX__UINT_DEFINE_TEST(RX_NONFATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,           \
                         _0, _1, _2, _3, NULL, NULL, NULL, NULL)

#define RX_UINT_CHECK_NOT_EQUAL_MSG_5(X1, X2, MSG,                             \
                                      _0, _1, _2, _3, _4)                      \
    RX__UINT_DEFINE_TEST(RX_NONFATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,           \
                         _0, _1, _2, _3, _4, NULL, NULL, NULL)

#define RX_UINT_CHECK_NOT_EQUAL_MSG_6(X1, X2, MSG,                             \
                                      _0, _1, _2, _3, _4, _5)                  \
    RX__UINT_DEFINE_TEST(RX_NONFATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,           \
                         _0, _1, _2, _3, _4, _5, NULL, NULL)

#define RX_UINT_CHECK_NOT_EQUAL_MSG_7(X1, X2, MSG,                             \
                                      _0, _1, _2, _3, _4, _5, _6)              \
    RX__UINT_DEFINE_TEST(RX_NONFATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,           \
                         _0, _1, _2, _3, _4, _5, _6, NULL)

#define RX_UINT_CHECK_NOT_EQUAL_MSG_8(X1, X2, MSG,                             \
                                      _0, _1, _2, _3, _4, _5, _6, _7)          \
    RX__UINT_DEFINE_TEST(RX_NONFATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,           \
                         _0, _1, _2, _3, _4, _5, _6, _7)

#define RX_UINT_REQUIRE_GREATER(X1, X2)                                        \
    RX_UINT_REQUIRE_GREATER_MSG(X1, X2, NULL)

#if RX__HAS_VARIADIC_MACROS
    #define RX_UINT_REQUIRE_GREATER_MSG(X1, X2, ...)                           \
        RX__UINT_DEFINE_TEST(RX_FATAL, RX__OP_GREATER, X1, X2, __VA_ARGS__)
#else
    #define RX_UINT_REQUIRE_GREATER_MSG(X1, X2, MSG)                           \
        RX__UINT_DEFINE_TEST(RX_FATAL, RX__OP_GREATER, X1, X2, MSG,            \
                             NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#endif

#define RX_UINT_REQUIRE_GREATER_MSG_1(X1, X2, MSG,                             \
                                      _0)                                      \
    RX__UINT_DEFINE_TEST(RX_FATAL, RX__OP_GREATER, X1, X2, MSG,                \
                         _0, NULL, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_UINT_REQUIRE_GREATER_MSG_2(X1, X2, MSG,                             \
                                      _0, _1)                                  \
    RX__UINT_DEFINE_TEST(RX_FATAL, RX__OP_GREATER, X1, X2, MSG,                \
                         _0, _1, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_UINT_REQUIRE_GREATER_MSG_3(X1, X2, MSG,                             \
                                      _0, _1, _2)                              \
    RX__UINT_DEFINE_TEST(RX_FATAL, RX__OP_GREATER, X1, X2, MSG,                \
                         _0, _1, _2, NULL, NULL, NULL, NULL, NULL)

#define RX_UINT_REQUIRE_GREATER_MSG_4(X1, X2, MSG,                             \
                                      _0, _1, _2, _3)                          \
    RX__UINT_DEFINE_TEST(RX_FATAL, RX__OP_GREATER, X1, X2, MSG,                \
                         _0, _1, _2, _3, NULL, NULL, NULL, NULL)

#define RX_UINT_REQUIRE_GREATER_MSG_5(X1, X2, MSG,                             \
                                      _0, _1, _2, _3, _4)                      \
    RX__UINT_DEFINE_TEST(RX_FATAL, RX__OP_GREATER, X1, X2, MSG,                \
                         _0, _1, _2, _3, _4, NULL, NULL, NULL)

#define RX_UINT_REQUIRE_GREATER_MSG_6(X1, X2, MSG,                             \
                                      _0, _1, _2, _3, _4, _5)                  \
    RX__UINT_DEFINE_TEST(RX_FATAL, RX__OP_GREATER, X1, X2, MSG,                \
                         _0, _1, _2, _3, _4, _5, NULL, NULL)

#define RX_UINT_REQUIRE_GREATER_MSG_7(X1, X2, MSG,                             \
                                      _0, _1, _2, _3, _4, _5, _6)              \
    RX__UINT_DEFINE_TEST(RX_FATAL, RX__OP_GREATER, X1, X2, MSG,                \
                         _0, _1, _2, _3, _4, _5, _6, NULL)

#define RX_UINT_REQUIRE_GREATER_MSG_8(X1, X2, MSG,                             \
                                      _0, _1, _2, _3, _4, _5, _6, _7)          \
    RX__UINT_DEFINE_TEST(RX_FATAL, RX__OP_GREATER, X1, X2, MSG,                \
                         _0, _1, _2, _3, _4, _5, _6, _7)

#define RX_UINT_CHECK_GREATER(X1, X2)                                          \
    RX_UINT_CHECK_GREATER_MSG(X1, X2, NULL)

#if RX__HAS_VARIADIC_MACROS
    #define RX_UINT_CHECK_GREATER_MSG(X1, X2, ...)                             \
        RX__UINT_DEFINE_TEST(RX_NONFATAL, RX__OP_GREATER, X1, X2, __VA_ARGS__)
#else
    #define RX_UINT_CHECK_GREATER_MSG(X1, X2, MSG)                             \
        RX__UINT_DEFINE_TEST(RX_NONFATAL, RX__OP_GREATER, X1, X2, MSG,         \
                             NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#endif

#define RX_UINT_CHECK_GREATER_MSG_1(X1, X2, MSG,                               \
                                    _0)                                        \
    RX__UINT_DEFINE_TEST(RX_NONFATAL, RX__OP_GREATER, X1, X2, MSG,             \
                         _0, NULL, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_UINT_CHECK_GREATER_MSG_2(X1, X2, MSG,                               \
                                    _0, _1)                                    \
    RX__UINT_DEFINE_TEST(RX_NONFATAL, RX__OP_GREATER, X1, X2, MSG,             \
                         _0, _1, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_UINT_CHECK_GREATER_MSG_3(X1, X2, MSG,                               \
                                    _0, _1, _2)                                \
    RX__UINT_DEFINE_TEST(RX_NONFATAL, RX__OP_GREATER, X1, X2, MSG,             \
                         _0, _1, _2, NULL, NULL, NULL, NULL, NULL)

#define RX_UINT_CHECK_GREATER_MSG_4(X1, X2, MSG,                               \
                                    _0, _1, _2, _3)                            \
    RX__UINT_DEFINE_TEST(RX_NONFATAL, RX__OP_GREATER, X1, X2, MSG,             \
                         _0, _1, _2, _3, NULL, NULL, NULL, NULL)

#define RX_UINT_CHECK_GREATER_MSG_5(X1, X2, MSG,                               \
                                    _0, _1, _2, _3, _4)                        \
    RX__UINT_DEFINE_TEST(RX_NONFATAL, RX__OP_GREATER, X1, X2, MSG,             \
                         _0, _1, _2, _3, _4, NULL, NULL, NULL)

#define RX_UINT_CHECK_GREATER_MSG_6(X1, X2, MSG,                               \
                                    _0, _1, _2, _3, _4, _5)                    \
    RX__UINT_DEFINE_TEST(RX_NONFATAL, RX__OP_GREATER, X1, X2, MSG,             \
                         _0, _1, _2, _3, _4, _5, NULL, NULL)

#define RX_UINT_CHECK_GREATER_MSG_7(X1, X2, MSG,                               \
                                    _0, _1, _2, _3, _4, _5, _6)                \
    RX__UINT_DEFINE_TEST(RX_NONFATAL, RX__OP_GREATER, X1, X2, MSG,             \
                         _0, _1, _2, _3, _4, _5, _6, NULL)

#define RX_UINT_CHECK_GREATER_MSG_8(X1, X2, MSG,                               \
                                    _0, _1, _2, _3, _4, _5, _6, _7)            \
    RX__UINT_DEFINE_TEST(RX_NONFATAL, RX__OP_GREATER, X1, X2, MSG,             \
                         _0, _1, _2, _3, _4, _5, _6, _7)

#define RX_UINT_REQUIRE_LESSER(X1, X2)                                         \
    RX_UINT_REQUIRE_LESSER_MSG(X1, X2, NULL)

#if RX__HAS_VARIADIC_MACROS
    #define RX_UINT_REQUIRE_LESSER_MSG(X1, X2, ...)                            \
        RX__UINT_DEFINE_TEST(RX_FATAL, RX__OP_LESSER, X1, X2, __VA_ARGS__)
#else
    #define RX_UINT_REQUIRE_LESSER_MSG(X1, X2, MSG)                            \
        RX__UINT_DEFINE_TEST(RX_FATAL, RX__OP_LESSER, X1, X2, MSG,             \
                             NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#endif

#define RX_UINT_REQUIRE_LESSER_MSG_1(X1, X2, MSG,                              \
                                     _0)                                       \
    RX__UINT_DEFINE_TEST(RX_FATAL, RX__OP_LESSER, X1, X2, MSG,                 \
                         _0, NULL, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_UINT_REQUIRE_LESSER_MSG_2(X1, X2, MSG,                              \
                                     _0, _1)                                   \
    RX__UINT_DEFINE_TEST(RX_FATAL, RX__OP_LESSER, X1, X2, MSG,                 \
                         _0, _1, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_UINT_REQUIRE_LESSER_MSG_3(X1, X2, MSG,                              \
                                     _0, _1, _2)                               \
    RX__UINT_DEFINE_TEST(RX_FATAL, RX__OP_LESSER, X1, X2, MSG,                 \
                         _0, _1, _2, NULL, NULL, NULL, NULL, NULL)

#define RX_UINT_REQUIRE_LESSER_MSG_4(X1, X2, MSG,                              \
                                     _0, _1, _2, _3)                           \
    RX__UINT_DEFINE_TEST(RX_FATAL, RX__OP_LESSER, X1, X2, MSG,                 \
                         _0, _1, _2, _3, NULL, NULL, NULL, NULL)

#define RX_UINT_REQUIRE_LESSER_MSG_5(X1, X2, MSG,                              \
                                     _0, _1, _2, _3, _4)                       \
    RX__UINT_DEFINE_TEST(RX_FATAL, RX__OP_LESSER, X1, X2, MSG,                 \
                         _0, _1, _2, _3, _4, NULL, NULL, NULL)

#define RX_UINT_REQUIRE_LESSER_MSG_6(X1, X2, MSG,                              \
                                     _0, _1, _2, _3, _4, _5)                   \
    RX__UINT_DEFINE_TEST(RX_FATAL, RX__OP_LESSER, X1, X2, MSG,                 \
                         _0, _1, _2, _3, _4, _5, NULL, NULL)

#define RX_UINT_REQUIRE_LESSER_MSG_7(X1, X2, MSG,                              \
                                     _0, _1, _2, _3, _4, _5, _6)               \
    RX__UINT_DEFINE_TEST(RX_FATAL, RX__OP_LESSER, X1, X2, MSG,                 \
                         _0, _1, _2, _3, _4, _5, _6, NULL)

#define RX_UINT_REQUIRE_LESSER_MSG_8(X1, X2, MSG,                              \
                                     _0, _1, _2, _3, _4, _5, _6, _7)           \
    RX__UINT_DEFINE_TEST(RX_FATAL, RX__OP_LESSER, X1, X2, MSG,                 \
                         _0, _1, _2, _3, _4, _5, _6, _7)

#define RX_UINT_CHECK_LESSER(X1, X2)                                           \
    RX_UINT_CHECK_LESSER_MSG(X1, X2, NULL)

#if RX__HAS_VARIADIC_MACROS
    #define RX_UINT_CHECK_LESSER_MSG(X1, X2, ...)                              \
        RX__UINT_DEFINE_TEST(RX_NONFATAL, RX__OP_LESSER, X1, X2, __VA_ARGS__)
#else
    #define RX_UINT_CHECK_LESSER_MSG(X1, X2, MSG)                              \
        RX__UINT_DEFINE_TEST(RX_NONFATAL, RX__OP_LESSER, X1, X2, MSG,          \
                             NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#endif

#define RX_UINT_CHECK_LESSER_MSG_1(X1, X2, MSG,                                \
                                   _0)                                         \
    RX__UINT_DEFINE_TEST(RX_NONFATAL, RX__OP_LESSER, X1, X2, MSG,              \
                         _0, NULL, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_UINT_CHECK_LESSER_MSG_2(X1, X2, MSG,                                \
                                   _0, _1)                                     \
    RX__UINT_DEFINE_TEST(RX_NONFATAL, RX__OP_LESSER, X1, X2, MSG,              \
                         _0, _1, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_UINT_CHECK_LESSER_MSG_3(X1, X2, MSG,                                \
                                   _0, _1, _2)                                 \
    RX__UINT_DEFINE_TEST(RX_NONFATAL, RX__OP_LESSER, X1, X2, MSG,              \
                         _0, _1, _2, NULL, NULL, NULL, NULL, NULL)

#define RX_UINT_CHECK_LESSER_MSG_4(X1, X2, MSG,                                \
                                   _0, _1, _2, _3)                             \
    RX__UINT_DEFINE_TEST(RX_NONFATAL, RX__OP_LESSER, X1, X2, MSG,              \
                         _0, _1, _2, _3, NULL, NULL, NULL, NULL)

#define RX_UINT_CHECK_LESSER_MSG_5(X1, X2, MSG,                                \
                                   _0, _1, _2, _3, _4)                         \
    RX__UINT_DEFINE_TEST(RX_NONFATAL, RX__OP_LESSER, X1, X2, MSG,              \
                         _0, _1, _2, _3, _4, NULL, NULL, NULL)

#define RX_UINT_CHECK_LESSER_MSG_6(X1, X2, MSG,                                \
                                   _0, _1, _2, _3, _4, _5)                     \
    RX__UINT_DEFINE_TEST(RX_NONFATAL, RX__OP_LESSER, X1, X2, MSG,              \
                         _0, _1, _2, _3, _4, _5, NULL, NULL)

#define RX_UINT_CHECK_LESSER_MSG_7(X1, X2, MSG,                                \
                                   _0, _1, _2, _3, _4, _5, _6)                 \
    RX__UINT_DEFINE_TEST(RX_NONFATAL, RX__OP_LESSER, X1, X2, MSG,              \
                         _0, _1, _2, _3, _4, _5, _6, NULL)

#define RX_UINT_CHECK_LESSER_MSG_8(X1, X2, MSG,                                \
                                   _0, _1, _2, _3, _4, _5, _6, _7)             \
    RX__UINT_DEFINE_TEST(RX_NONFATAL, RX__OP_LESSER, X1, X2, MSG,              \
                         _0, _1, _2, _3, _4, _5, _6, _7)

#define RX_UINT_REQUIRE_GREATER_OR_EQUAL(X1, X2)                               \
    RX_UINT_REQUIRE_GREATER_OR_EQUAL_MSG(X1, X2, NULL)

#if RX__HAS_VARIADIC_MACROS
    #define RX_UINT_REQUIRE_GREATER_OR_EQUAL_MSG(X1, X2, ...)                  \
        RX__UINT_DEFINE_TEST(RX_FATAL, RX__OP_GREATER_OR_EQUAL, X1, X2,        \
                             __VA_ARGS__)
#else
    #define RX_UINT_REQUIRE_GREATER_OR_EQUAL_MSG(X1, X2, MSG)                  \
        RX__UINT_DEFINE_TEST(RX_FATAL, RX__OP_GREATER_OR_EQUAL, X1, X2, MSG,   \
                             NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#endif

#define RX_UINT_REQUIRE_GREATER_OR_EQUAL_MSG_1(X1, X2, MSG,                    \
                                               _0)                             \
    RX__UINT_DEFINE_TEST(RX_FATAL, RX__OP_GREATER_OR_EQUAL, X1, X2, MSG,       \
                         _0, NULL, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_UINT_REQUIRE_GREATER_OR_EQUAL_MSG_2(X1, X2, MSG,                    \
                                               _0, _1)                         \
    RX__UINT_DEFINE_TEST(RX_FATAL, RX__OP_GREATER_OR_EQUAL, X1, X2, MSG,       \
                         _0, _1, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_UINT_REQUIRE_GREATER_OR_EQUAL_MSG_3(X1, X2, MSG,                    \
                                               _0, _1, _2)                     \
    RX__UINT_DEFINE_TEST(RX_FATAL, RX__OP_GREATER_OR_EQUAL, X1, X2, MSG,       \
                         _0, _1, _2, NULL, NULL, NULL, NULL, NULL)

#define RX_UINT_REQUIRE_GREATER_OR_EQUAL_MSG_4(X1, X2, MSG,                    \
                                               _0, _1, _2, _3)                 \
    RX__UINT_DEFINE_TEST(RX_FATAL, RX__OP_GREATER_OR_EQUAL, X1, X2, MSG,       \
                         _0, _1, _2, _3, NULL, NULL, NULL, NULL)

#define RX_UINT_REQUIRE_GREATER_OR_EQUAL_MSG_5(X1, X2, MSG,                    \
                                               _0, _1, _2, _3, _4)             \
    RX__UINT_DEFINE_TEST(RX_FATAL, RX__OP_GREATER_OR_EQUAL, X1, X2, MSG,       \
                         _0, _1, _2, _3, _4, NULL, NULL, NULL)

#define RX_UINT_REQUIRE_GREATER_OR_EQUAL_MSG_6(X1, X2, MSG,                    \
                                               _0, _1, _2, _3, _4, _5)         \
    RX__UINT_DEFINE_TEST(RX_FATAL, RX__OP_GREATER_OR_EQUAL, X1, X2, MSG,       \
                         _0, _1, _2, _3, _4, _5, NULL, NULL)

#define RX_UINT_REQUIRE_GREATER_OR_EQUAL_MSG_7(X1, X2, MSG,                    \
                                               _0, _1, _2, _3, _4, _5, _6)     \
    RX__UINT_DEFINE_TEST(RX_FATAL, RX__OP_GREATER_OR_EQUAL, X1, X2, MSG,       \
                         _0, _1, _2, _3, _4, _5, _6, NULL)

#define RX_UINT_REQUIRE_GREATER_OR_EQUAL_MSG_8(X1, X2, MSG,                    \
                                               _0, _1, _2, _3, _4, _5, _6, _7) \
    RX__UINT_DEFINE_TEST(RX_FATAL, RX__OP_GREATER_OR_EQUAL, X1, X2, MSG,       \
                         _0, _1, _2, _3, _4, _5, _6, _7)

#define RX_UINT_CHECK_GREATER_OR_EQUAL(X1, X2)                                 \
    RX_UINT_CHECK_GREATER_OR_EQUAL_MSG(X1, X2, NULL)

#if RX__HAS_VARIADIC_MACROS
    #define RX_UINT_CHECK_GREATER_OR_EQUAL_MSG(X1, X2, ...)                    \
        RX__UINT_DEFINE_TEST(RX_NONFATAL, RX__OP_GREATER_OR_EQUAL, X1, X2,     \
                             __VA_ARGS__)
#else
    #define RX_UINT_CHECK_GREATER_OR_EQUAL_MSG(X1, X2, MSG)                    \
        RX__UINT_DEFINE_TEST(RX_NONFATAL, RX__OP_GREATER_OR_EQUAL, X1, X2, MSG,\
                             NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#endif

#define RX_UINT_CHECK_GREATER_OR_EQUAL_MSG_1(X1, X2, MSG,                      \
                                             _0)                               \
    RX__UINT_DEFINE_TEST(RX_NONFATAL, RX__OP_GREATER_OR_EQUAL, X1, X2, MSG,    \
                         _0, NULL, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_UINT_CHECK_GREATER_OR_EQUAL_MSG_2(X1, X2, MSG,                      \
                                             _0, _1)                           \
    RX__UINT_DEFINE_TEST(RX_NONFATAL, RX__OP_GREATER_OR_EQUAL, X1, X2, MSG,    \
                         _0, _1, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_UINT_CHECK_GREATER_OR_EQUAL_MSG_3(X1, X2, MSG,                      \
                                             _0, _1, _2)                       \
    RX__UINT_DEFINE_TEST(RX_NONFATAL, RX__OP_GREATER_OR_EQUAL, X1, X2, MSG,    \
                         _0, _1, _2, NULL, NULL, NULL, NULL, NULL)

#define RX_UINT_CHECK_GREATER_OR_EQUAL_MSG_4(X1, X2, MSG,                      \
                                             _0, _1, _2, _3)                   \
    RX__UINT_DEFINE_TEST(RX_NONFATAL, RX__OP_GREATER_OR_EQUAL, X1, X2, MSG,    \
                         _0, _1, _2, _3, NULL, NULL, NULL, NULL)

#define RX_UINT_CHECK_GREATER_OR_EQUAL_MSG_5(X1, X2, MSG,                      \
                                             _0, _1, _2, _3, _4)               \
    RX__UINT_DEFINE_TEST(RX_NONFATAL, RX__OP_GREATER_OR_EQUAL, X1, X2, MSG,    \
                         _0, _1, _2, _3, _4, NULL, NULL, NULL)

#define RX_UINT_CHECK_GREATER_OR_EQUAL_MSG_6(X1, X2, MSG,                      \
                                             _0, _1, _2, _3, _4, _5)           \
    RX__UINT_DEFINE_TEST(RX_NONFATAL, RX__OP_GREATER_OR_EQUAL, X1, X2, MSG,    \
                         _0, _1, _2, _3, _4, _5, NULL, NULL)

#define RX_UINT_CHECK_GREATER_OR_EQUAL_MSG_7(X1, X2, MSG,                      \
                                             _0, _1, _2, _3, _4, _5, _6)       \
    RX__UINT_DEFINE_TEST(RX_NONFATAL, RX__OP_GREATER_OR_EQUAL, X1, X2, MSG,    \
                         _0, _1, _2, _3, _4, _5, _6, NULL)

#define RX_UINT_CHECK_GREATER_OR_EQUAL_MSG_8(X1, X2, MSG,                      \
                                             _0, _1, _2, _3, _4, _5, _6, _7)   \
    RX__UINT_DEFINE_TEST(RX_NONFATAL, RX__OP_GREATER_OR_EQUAL, X1, X2, MSG,    \
                         _0, _1, _2, _3, _4, _5, _6, _7)

#define RX_UINT_REQUIRE_LESSER_OR_EQUAL(X1, X2)                                \
    RX_UINT_REQUIRE_LESSER_OR_EQUAL_MSG(X1, X2, NULL)

#if RX__HAS_VARIADIC_MACROS
    #define RX_UINT_REQUIRE_LESSER_OR_EQUAL_MSG(X1, X2, ...)                   \
        RX__UINT_DEFINE_TEST(RX_FATAL, RX__OP_LESSER_OR_EQUAL, X1, X2,         \
                             __VA_ARGS__)
#else
    #define RX_UINT_REQUIRE_LESSER_OR_EQUAL_MSG(X1, X2, MSG)                   \
        RX__UINT_DEFINE_TEST(RX_FATAL, RX__OP_LESSER_OR_EQUAL, X1, X2, MSG,    \
                             NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#endif

#define RX_UINT_REQUIRE_LESSER_OR_EQUAL_MSG_1(X1, X2, MSG,                     \
                                              _0)                              \
    RX__UINT_DEFINE_TEST(RX_FATAL, RX__OP_LESSER_OR_EQUAL, X1, X2, MSG,        \
                         _0, NULL, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_UINT_REQUIRE_LESSER_OR_EQUAL_MSG_2(X1, X2, MSG,                     \
                                              _0, _1)                          \
    RX__UINT_DEFINE_TEST(RX_FATAL, RX__OP_LESSER_OR_EQUAL, X1, X2, MSG,        \
                         _0, _1, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_UINT_REQUIRE_LESSER_OR_EQUAL_MSG_3(X1, X2, MSG,                     \
                                              _0, _1, _2)                      \
    RX__UINT_DEFINE_TEST(RX_FATAL, RX__OP_LESSER_OR_EQUAL, X1, X2, MSG,        \
                         _0, _1, _2, NULL, NULL, NULL, NULL, NULL)

#define RX_UINT_REQUIRE_LESSER_OR_EQUAL_MSG_4(X1, X2, MSG,                     \
                                              _0, _1, _2, _3)                  \
    RX__UINT_DEFINE_TEST(RX_FATAL, RX__OP_LESSER_OR_EQUAL, X1, X2, MSG,        \
                         _0, _1, _2, _3, NULL, NULL, NULL, NULL)

#define RX_UINT_REQUIRE_LESSER_OR_EQUAL_MSG_5(X1, X2, MSG,                     \
                                              _0, _1, _2, _3, _4)              \
    RX__UINT_DEFINE_TEST(RX_FATAL, RX__OP_LESSER_OR_EQUAL, X1, X2, MSG,        \
                         _0, _1, _2, _3, _4, NULL, NULL, NULL)

#define RX_UINT_REQUIRE_LESSER_OR_EQUAL_MSG_6(X1, X2, MSG,                     \
                                              _0, _1, _2, _3, _4, _5)          \
    RX__UINT_DEFINE_TEST(RX_FATAL, RX__OP_LESSER_OR_EQUAL, X1, X2, MSG,        \
                         _0, _1, _2, _3, _4, _5, NULL, NULL)

#define RX_UINT_REQUIRE_LESSER_OR_EQUAL_MSG_7(X1, X2, MSG,                     \
                                              _0, _1, _2, _3, _4, _5, _6)      \
    RX__UINT_DEFINE_TEST(RX_FATAL, RX__OP_LESSER_OR_EQUAL, X1, X2, MSG,        \
                         _0, _1, _2, _3, _4, _5, _6, NULL)

#define RX_UINT_REQUIRE_LESSER_OR_EQUAL_MSG_8(X1, X2, MSG,                     \
                                              _0, _1, _2, _3, _4, _5, _6, _7)  \
    RX__UINT_DEFINE_TEST(RX_FATAL, RX__OP_LESSER_OR_EQUAL, X1, X2, MSG,        \
                         _0, _1, _2, _3, _4, _5, _6, _7)

#define RX_UINT_CHECK_LESSER_OR_EQUAL(X1, X2)                                  \
    RX_UINT_CHECK_LESSER_OR_EQUAL_MSG(X1, X2, NULL)

#if RX__HAS_VARIADIC_MACROS
    #define RX_UINT_CHECK_LESSER_OR_EQUAL_MSG(X1, X2, ...)                     \
        RX__UINT_DEFINE_TEST(RX_NONFATAL, RX__OP_LESSER_OR_EQUAL, X1, X2,      \
                             __VA_ARGS__)
#else
    #define RX_UINT_CHECK_LESSER_OR_EQUAL_MSG(X1, X2, MSG)                     \
        RX__UINT_DEFINE_TEST(RX_NONFATAL, RX__OP_LESSER_OR_EQUAL, X1, X2, MSG, \
                             NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#endif

#define RX_UINT_CHECK_LESSER_OR_EQUAL_MSG_1(X1, X2, MSG,                       \
                                            _0)                                \
    RX__UINT_DEFINE_TEST(RX_NONFATAL, RX__OP_LESSER_OR_EQUAL, X1, X2, MSG,     \
                         _0, NULL, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_UINT_CHECK_LESSER_OR_EQUAL_MSG_2(X1, X2, MSG,                       \
                                            _0, _1)                            \
    RX__UINT_DEFINE_TEST(RX_NONFATAL, RX__OP_LESSER_OR_EQUAL, X1, X2, MSG,     \
                         _0, _1, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_UINT_CHECK_LESSER_OR_EQUAL_MSG_3(X1, X2, MSG,                       \
                                            _0, _1, _2)                        \
    RX__UINT_DEFINE_TEST(RX_NONFATAL, RX__OP_LESSER_OR_EQUAL, X1, X2, MSG,     \
                         _0, _1, _2, NULL, NULL, NULL, NULL, NULL)

#define RX_UINT_CHECK_LESSER_OR_EQUAL_MSG_4(X1, X2, MSG,                       \
                                            _0, _1, _2, _3)                    \
    RX__UINT_DEFINE_TEST(RX_NONFATAL, RX__OP_LESSER_OR_EQUAL, X1, X2, MSG,     \
                         _0, _1, _2, _3, NULL, NULL, NULL, NULL)

#define RX_UINT_CHECK_LESSER_OR_EQUAL_MSG_5(X1, X2, MSG,                       \
                                            _0, _1, _2, _3, _4)                \
    RX__UINT_DEFINE_TEST(RX_NONFATAL, RX__OP_LESSER_OR_EQUAL, X1, X2, MSG,     \
                         _0, _1, _2, _3, _4, NULL, NULL, NULL)

#define RX_UINT_CHECK_LESSER_OR_EQUAL_MSG_6(X1, X2, MSG,                       \
                                            _0, _1, _2, _3, _4, _5)            \
    RX__UINT_DEFINE_TEST(RX_NONFATAL, RX__OP_LESSER_OR_EQUAL, X1, X2, MSG,     \
                         _0, _1, _2, _3, _4, _5, NULL, NULL)

#define RX_UINT_CHECK_LESSER_OR_EQUAL_MSG_7(X1, X2, MSG,                       \
                                            _0, _1, _2, _3, _4, _5, _6)        \
    RX__UINT_DEFINE_TEST(RX_NONFATAL, RX__OP_LESSER_OR_EQUAL, X1, X2, MSG,     \
                         _0, _1, _2, _3, _4, _5, _6, NULL)

#define RX_UINT_CHECK_LESSER_OR_EQUAL_MSG_8(X1, X2, MSG,                       \
                                            _0, _1, _2, _3, _4, _5, _6, _7)    \
    RX__UINT_DEFINE_TEST(RX_NONFATAL, RX__OP_LESSER_OR_EQUAL, X1, X2, MSG,     \
                         _0, _1, _2, _3, _4, _5, _6, _7)

/* Floating-Point Assertions                                       O-(''Q)
   -------------------------------------------------------------------------- */

#if RX__HAS_VARIADIC_MACROS
    #define RX__REAL_DEFINE_TEST(SEVERITY, OP, X1, X2, ...)                    \
        rx__real_assess_comparison(RX_PARAM_CONTEXT,                           \
                                   (X1),                                       \
                                   (X2),                                       \
                                   OP,                                         \
                                   #X1,                                        \
                                   #X2,                                        \
                                   __FILE__,                                   \
                                   __LINE__,                                   \
                                   SEVERITY,                                   \
                                   __VA_ARGS__)
#else
    #define RX__REAL_DEFINE_TEST(SEVERITY, OP, X1, X2, MSG,                    \
                                 _0, _1, _2, _3, _4, _5, _6, _7)               \
        rx__real_assess_comparison(RX_PARAM_CONTEXT,                           \
                                   (X1),                                       \
                                   (X2),                                       \
                                   OP,                                         \
                                   #X1,                                        \
                                   #X2,                                        \
                                   __FILE__,                                   \
                                   __LINE__,                                   \
                                   SEVERITY,                                   \
                                   MSG,                                        \
                                   _0, _1, _2, _3, _4, _5, _6, _7)
#endif

#define RX_REAL_REQUIRE_EQUAL(X1, X2)                                          \
    RX_REAL_REQUIRE_EQUAL_MSG(X1, X2, NULL)

#if RX__HAS_VARIADIC_MACROS
    #define RX_REAL_REQUIRE_EQUAL_MSG(X1, X2, ...)                             \
        RX__REAL_DEFINE_TEST(RX_FATAL, RX__OP_EQUAL, X1, X2, __VA_ARGS__)
#else
    #define RX_REAL_REQUIRE_EQUAL_MSG(X1, X2, MSG)                             \
        RX__REAL_DEFINE_TEST(RX_FATAL, RX__OP_EQUAL, X1, X2, MSG,              \
                             NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#endif

#define RX_REAL_REQUIRE_EQUAL_MSG_1(X1, X2, MSG,                               \
                                    _0)                                        \
    RX__REAL_DEFINE_TEST(RX_FATAL, RX__OP_EQUAL, X1, X2, MSG,                  \
                         _0, NULL, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_REAL_REQUIRE_EQUAL_MSG_2(X1, X2, MSG,                               \
                                    _0, _1)                                    \
    RX__REAL_DEFINE_TEST(RX_FATAL, RX__OP_EQUAL, X1, X2, MSG,                  \
                         _0, _1, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_REAL_REQUIRE_EQUAL_MSG_3(X1, X2, MSG,                               \
                                    _0, _1, _2)                                \
    RX__REAL_DEFINE_TEST(RX_FATAL, RX__OP_EQUAL, X1, X2, MSG,                  \
                         _0, _1, _2, NULL, NULL, NULL, NULL, NULL)

#define RX_REAL_REQUIRE_EQUAL_MSG_4(X1, X2, MSG,                               \
                                    _0, _1, _2, _3)                            \
    RX__REAL_DEFINE_TEST(RX_FATAL, RX__OP_EQUAL, X1, X2, MSG,                  \
                         _0, _1, _2, _3, NULL, NULL, NULL, NULL)

#define RX_REAL_REQUIRE_EQUAL_MSG_5(X1, X2, MSG,                               \
                                    _0, _1, _2, _3, _4)                        \
    RX__REAL_DEFINE_TEST(RX_FATAL, RX__OP_EQUAL, X1, X2, MSG,                  \
                         _0, _1, _2, _3, _4, NULL, NULL, NULL)

#define RX_REAL_REQUIRE_EQUAL_MSG_6(X1, X2, MSG,                               \
                                    _0, _1, _2, _3, _4, _5)                    \
    RX__REAL_DEFINE_TEST(RX_FATAL, RX__OP_EQUAL, X1, X2, MSG,                  \
                         _0, _1, _2, _3, _4, _5, NULL, NULL)

#define RX_REAL_REQUIRE_EQUAL_MSG_7(X1, X2, MSG,                               \
                                    _0, _1, _2, _3, _4, _5, _6)                \
    RX__REAL_DEFINE_TEST(RX_FATAL, RX__OP_EQUAL, X1, X2, MSG,                  \
                         _0, _1, _2, _3, _4, _5, _6, NULL)

#define RX_REAL_REQUIRE_EQUAL_MSG_8(X1, X2, MSG,                               \
                                    _0, _1, _2, _3, _4, _5, _6, _7)            \
    RX__REAL_DEFINE_TEST(RX_FATAL, RX__OP_EQUAL, X1, X2, MSG,                  \
                         _0, _1, _2, _3, _4, _5, _6, _7)

#define RX_REAL_CHECK_EQUAL(X1, X2)                                            \
    RX_REAL_CHECK_EQUAL_MSG(X1, X2, NULL)

#if RX__HAS_VARIADIC_MACROS
    #define RX_REAL_CHECK_EQUAL_MSG(X1, X2, ...)                               \
        RX__REAL_DEFINE_TEST(RX_NONFATAL, RX__OP_EQUAL, X1, X2, __VA_ARGS__)
#else
    #define RX_REAL_CHECK_EQUAL_MSG(X1, X2, MSG)                               \
        RX__REAL_DEFINE_TEST(RX_NONFATAL, RX__OP_EQUAL, X1, X2, MSG,           \
                             NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#endif

#define RX_REAL_CHECK_EQUAL_MSG_1(X1, X2, MSG,                                 \
                                  _0)                                          \
    RX__REAL_DEFINE_TEST(RX_NONFATAL, RX__OP_EQUAL, X1, X2, MSG,               \
                         _0, NULL, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_REAL_CHECK_EQUAL_MSG_2(X1, X2, MSG,                                 \
                                  _0, _1)                                      \
    RX__REAL_DEFINE_TEST(RX_NONFATAL, RX__OP_EQUAL, X1, X2, MSG,               \
                         _0, _1, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_REAL_CHECK_EQUAL_MSG_3(X1, X2, MSG,                                 \
                                  _0, _1, _2)                                  \
    RX__REAL_DEFINE_TEST(RX_NONFATAL, RX__OP_EQUAL, X1, X2, MSG,               \
                         _0, _1, _2, NULL, NULL, NULL, NULL, NULL)

#define RX_REAL_CHECK_EQUAL_MSG_4(X1, X2, MSG,                                 \
                                  _0, _1, _2, _3)                              \
    RX__REAL_DEFINE_TEST(RX_NONFATAL, RX__OP_EQUAL, X1, X2, MSG,               \
                         _0, _1, _2, _3, NULL, NULL, NULL, NULL)

#define RX_REAL_CHECK_EQUAL_MSG_5(X1, X2, MSG,                                 \
                                  _0, _1, _2, _3, _4)                          \
    RX__REAL_DEFINE_TEST(RX_NONFATAL, RX__OP_EQUAL, X1, X2, MSG,               \
                         _0, _1, _2, _3, _4, NULL, NULL, NULL)

#define RX_REAL_CHECK_EQUAL_MSG_6(X1, X2, MSG,                                 \
                                  _0, _1, _2, _3, _4, _5)                      \
    RX__REAL_DEFINE_TEST(RX_NONFATAL, RX__OP_EQUAL, X1, X2, MSG,               \
                         _0, _1, _2, _3, _4, _5, NULL, NULL)

#define RX_REAL_CHECK_EQUAL_MSG_7(X1, X2, MSG,                                 \
                                  _0, _1, _2, _3, _4, _5, _6)                  \
    RX__REAL_DEFINE_TEST(RX_NONFATAL, RX__OP_EQUAL, X1, X2, MSG,               \
                         _0, _1, _2, _3, _4, _5, _6, NULL)

#define RX_REAL_CHECK_EQUAL_MSG_8(X1, X2, MSG,                                 \
                                  _0, _1, _2, _3, _4, _5, _6, _7)              \
    RX__REAL_DEFINE_TEST(RX_NONFATAL, RX__OP_EQUAL, X1, X2, MSG,               \
                         _0, _1, _2, _3, _4, _5, _6, _7)

#define RX_REAL_REQUIRE_NOT_EQUAL(X1, X2)                                      \
    RX_REAL_REQUIRE_NOT_EQUAL_MSG(X1, X2, NULL)

#if RX__HAS_VARIADIC_MACROS
    #define RX_REAL_REQUIRE_NOT_EQUAL_MSG(X1, X2, ...)                         \
        RX__REAL_DEFINE_TEST(RX_FATAL, RX__OP_NOT_EQUAL, X1, X2, __VA_ARGS__)
#else
    #define RX_REAL_REQUIRE_NOT_EQUAL_MSG(X1, X2, MSG)                         \
        RX__REAL_DEFINE_TEST(RX_FATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,          \
                             NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#endif

#define RX_REAL_REQUIRE_NOT_EQUAL_MSG_1(X1, X2, MSG,                           \
                                        _0)                                    \
    RX__REAL_DEFINE_TEST(RX_FATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,              \
                         _0, NULL, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_REAL_REQUIRE_NOT_EQUAL_MSG_2(X1, X2, MSG,                           \
                                        _0, _1)                                \
    RX__REAL_DEFINE_TEST(RX_FATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,              \
                         _0, _1, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_REAL_REQUIRE_NOT_EQUAL_MSG_3(X1, X2, MSG,                           \
                                        _0, _1, _2)                            \
    RX__REAL_DEFINE_TEST(RX_FATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,              \
                         _0, _1, _2, NULL, NULL, NULL, NULL, NULL)

#define RX_REAL_REQUIRE_NOT_EQUAL_MSG_4(X1, X2, MSG,                           \
                                        _0, _1, _2, _3)                        \
    RX__REAL_DEFINE_TEST(RX_FATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,              \
                         _0, _1, _2, _3, NULL, NULL, NULL, NULL)

#define RX_REAL_REQUIRE_NOT_EQUAL_MSG_5(X1, X2, MSG,                           \
                                        _0, _1, _2, _3, _4)                    \
    RX__REAL_DEFINE_TEST(RX_FATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,              \
                         _0, _1, _2, _3, _4, NULL, NULL, NULL)

#define RX_REAL_REQUIRE_NOT_EQUAL_MSG_6(X1, X2, MSG,                           \
                                        _0, _1, _2, _3, _4, _5)                \
    RX__REAL_DEFINE_TEST(RX_FATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,              \
                         _0, _1, _2, _3, _4, _5, NULL, NULL)

#define RX_REAL_REQUIRE_NOT_EQUAL_MSG_7(X1, X2, MSG,                           \
                                        _0, _1, _2, _3, _4, _5, _6)            \
    RX__REAL_DEFINE_TEST(RX_FATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,              \
                         _0, _1, _2, _3, _4, _5, _6, NULL)

#define RX_REAL_REQUIRE_NOT_EQUAL_MSG_8(X1, X2, MSG,                           \
                                        _0, _1, _2, _3, _4, _5, _6, _7)        \
    RX__REAL_DEFINE_TEST(RX_FATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,              \
                         _0, _1, _2, _3, _4, _5, _6, _7)

#define RX_REAL_CHECK_NOT_EQUAL(X1, X2)                                        \
    RX_REAL_CHECK_NOT_EQUAL_MSG(X1, X2, NULL)

#if RX__HAS_VARIADIC_MACROS
    #define RX_REAL_CHECK_NOT_EQUAL_MSG(X1, X2, ...)                           \
        RX__REAL_DEFINE_TEST(RX_NONFATAL, RX__OP_NOT_EQUAL, X1, X2, __VA_ARGS__)
#else
    #define RX_REAL_CHECK_NOT_EQUAL_MSG(X1, X2, MSG)                           \
        RX__REAL_DEFINE_TEST(RX_NONFATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,       \
                             NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#endif

#define RX_REAL_CHECK_NOT_EQUAL_MSG_1(X1, X2, MSG,                             \
                                      _0)                                      \
    RX__REAL_DEFINE_TEST(RX_NONFATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,           \
                         _0, NULL, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_REAL_CHECK_NOT_EQUAL_MSG_2(X1, X2, MSG,                             \
                                      _0, _1)                                  \
    RX__REAL_DEFINE_TEST(RX_NONFATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,           \
                         _0, _1, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_REAL_CHECK_NOT_EQUAL_MSG_3(X1, X2, MSG,                             \
                                      _0, _1, _2)                              \
    RX__REAL_DEFINE_TEST(RX_NONFATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,           \
                         _0, _1, _2, NULL, NULL, NULL, NULL, NULL)

#define RX_REAL_CHECK_NOT_EQUAL_MSG_4(X1, X2, MSG,                             \
                                      _0, _1, _2, _3)                          \
    RX__REAL_DEFINE_TEST(RX_NONFATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,           \
                         _0, _1, _2, _3, NULL, NULL, NULL, NULL)

#define RX_REAL_CHECK_NOT_EQUAL_MSG_5(X1, X2, MSG,                             \
                                      _0, _1, _2, _3, _4)                      \
    RX__REAL_DEFINE_TEST(RX_NONFATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,           \
                         _0, _1, _2, _3, _4, NULL, NULL, NULL)

#define RX_REAL_CHECK_NOT_EQUAL_MSG_6(X1, X2, MSG,                             \
                                      _0, _1, _2, _3, _4, _5)                  \
    RX__REAL_DEFINE_TEST(RX_NONFATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,           \
                         _0, _1, _2, _3, _4, _5, NULL, NULL)

#define RX_REAL_CHECK_NOT_EQUAL_MSG_7(X1, X2, MSG,                             \
                                      _0, _1, _2, _3, _4, _5, _6)              \
    RX__REAL_DEFINE_TEST(RX_NONFATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,           \
                         _0, _1, _2, _3, _4, _5, _6, NULL)

#define RX_REAL_CHECK_NOT_EQUAL_MSG_8(X1, X2, MSG,                             \
                                      _0, _1, _2, _3, _4, _5, _6, _7)          \
    RX__REAL_DEFINE_TEST(RX_NONFATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,           \
                         _0, _1, _2, _3, _4, _5, _6, _7)

#define RX_REAL_REQUIRE_GREATER(X1, X2)                                        \
    RX_REAL_REQUIRE_GREATER_MSG(X1, X2, NULL)

#if RX__HAS_VARIADIC_MACROS
    #define RX_REAL_REQUIRE_GREATER_MSG(X1, X2, ...)                           \
        RX__REAL_DEFINE_TEST(RX_FATAL, RX__OP_GREATER, X1, X2, __VA_ARGS__)
#else
    #define RX_REAL_REQUIRE_GREATER_MSG(X1, X2, MSG)                           \
        RX__REAL_DEFINE_TEST(RX_FATAL, RX__OP_GREATER, X1, X2, MSG,            \
                             NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#endif

#define RX_REAL_REQUIRE_GREATER_MSG_1(X1, X2, MSG,                             \
                                      _0)                                      \
    RX__REAL_DEFINE_TEST(RX_FATAL, RX__OP_GREATER, X1, X2, MSG,                \
                         _0, NULL, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_REAL_REQUIRE_GREATER_MSG_2(X1, X2, MSG,                             \
                                      _0, _1)                                  \
    RX__REAL_DEFINE_TEST(RX_FATAL, RX__OP_GREATER, X1, X2, MSG,                \
                         _0, _1, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_REAL_REQUIRE_GREATER_MSG_3(X1, X2, MSG,                             \
                                      _0, _1, _2)                              \
    RX__REAL_DEFINE_TEST(RX_FATAL, RX__OP_GREATER, X1, X2, MSG,                \
                         _0, _1, _2, NULL, NULL, NULL, NULL, NULL)

#define RX_REAL_REQUIRE_GREATER_MSG_4(X1, X2, MSG,                             \
                                      _0, _1, _2, _3)                          \
    RX__REAL_DEFINE_TEST(RX_FATAL, RX__OP_GREATER, X1, X2, MSG,                \
                         _0, _1, _2, _3, NULL, NULL, NULL, NULL)

#define RX_REAL_REQUIRE_GREATER_MSG_5(X1, X2, MSG,                             \
                                      _0, _1, _2, _3, _4)                      \
    RX__REAL_DEFINE_TEST(RX_FATAL, RX__OP_GREATER, X1, X2, MSG,                \
                         _0, _1, _2, _3, _4, NULL, NULL, NULL)

#define RX_REAL_REQUIRE_GREATER_MSG_6(X1, X2, MSG,                             \
                                      _0, _1, _2, _3, _4, _5)                  \
    RX__REAL_DEFINE_TEST(RX_FATAL, RX__OP_GREATER, X1, X2, MSG,                \
                         _0, _1, _2, _3, _4, _5, NULL, NULL)

#define RX_REAL_REQUIRE_GREATER_MSG_7(X1, X2, MSG,                             \
                                      _0, _1, _2, _3, _4, _5, _6)              \
    RX__REAL_DEFINE_TEST(RX_FATAL, RX__OP_GREATER, X1, X2, MSG,                \
                         _0, _1, _2, _3, _4, _5, _6, NULL)

#define RX_REAL_REQUIRE_GREATER_MSG_8(X1, X2, MSG,                             \
                                      _0, _1, _2, _3, _4, _5, _6, _7)          \
    RX__REAL_DEFINE_TEST(RX_FATAL, RX__OP_GREATER, X1, X2, MSG,                \
                         _0, _1, _2, _3, _4, _5, _6, _7)

#define RX_REAL_CHECK_GREATER(X1, X2)                                          \
    RX_REAL_CHECK_GREATER_MSG(X1, X2, NULL)

#if RX__HAS_VARIADIC_MACROS
    #define RX_REAL_CHECK_GREATER_MSG(X1, X2, ...)                             \
        RX__REAL_DEFINE_TEST(RX_NONFATAL, RX__OP_GREATER, X1, X2, __VA_ARGS__)
#else
    #define RX_REAL_CHECK_GREATER_MSG(X1, X2, MSG)                             \
        RX__REAL_DEFINE_TEST(RX_NONFATAL, RX__OP_GREATER, X1, X2, MSG,         \
                             NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#endif

#define RX_REAL_CHECK_GREATER_MSG_1(X1, X2, MSG,                               \
                                    _0)                                        \
    RX__REAL_DEFINE_TEST(RX_NONFATAL, RX__OP_GREATER, X1, X2, MSG,             \
                         _0, NULL, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_REAL_CHECK_GREATER_MSG_2(X1, X2, MSG,                               \
                                    _0, _1)                                    \
    RX__REAL_DEFINE_TEST(RX_NONFATAL, RX__OP_GREATER, X1, X2, MSG,             \
                         _0, _1, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_REAL_CHECK_GREATER_MSG_3(X1, X2, MSG,                               \
                                    _0, _1, _2)                                \
    RX__REAL_DEFINE_TEST(RX_NONFATAL, RX__OP_GREATER, X1, X2, MSG,             \
                         _0, _1, _2, NULL, NULL, NULL, NULL, NULL)

#define RX_REAL_CHECK_GREATER_MSG_4(X1, X2, MSG,                               \
                                    _0, _1, _2, _3)                            \
    RX__REAL_DEFINE_TEST(RX_NONFATAL, RX__OP_GREATER, X1, X2, MSG,             \
                         _0, _1, _2, _3, NULL, NULL, NULL, NULL)

#define RX_REAL_CHECK_GREATER_MSG_5(X1, X2, MSG,                               \
                                    _0, _1, _2, _3, _4)                        \
    RX__REAL_DEFINE_TEST(RX_NONFATAL, RX__OP_GREATER, X1, X2, MSG,             \
                         _0, _1, _2, _3, _4, NULL, NULL, NULL)

#define RX_REAL_CHECK_GREATER_MSG_6(X1, X2, MSG,                               \
                                    _0, _1, _2, _3, _4, _5)                    \
    RX__REAL_DEFINE_TEST(RX_NONFATAL, RX__OP_GREATER, X1, X2, MSG,             \
                         _0, _1, _2, _3, _4, _5, NULL, NULL)

#define RX_REAL_CHECK_GREATER_MSG_7(X1, X2, MSG,                               \
                                    _0, _1, _2, _3, _4, _5, _6)                \
    RX__REAL_DEFINE_TEST(RX_NONFATAL, RX__OP_GREATER, X1, X2, MSG,             \
                         _0, _1, _2, _3, _4, _5, _6, NULL)

#define RX_REAL_CHECK_GREATER_MSG_8(X1, X2, MSG,                               \
                                    _0, _1, _2, _3, _4, _5, _6, _7)            \
    RX__REAL_DEFINE_TEST(RX_NONFATAL, RX__OP_GREATER, X1, X2, MSG,             \
                         _0, _1, _2, _3, _4, _5, _6, _7)

#define RX_REAL_REQUIRE_LESSER(X1, X2)                                         \
    RX_REAL_REQUIRE_LESSER_MSG(X1, X2, NULL)

#if RX__HAS_VARIADIC_MACROS
    #define RX_REAL_REQUIRE_LESSER_MSG(X1, X2, ...)                            \
        RX__REAL_DEFINE_TEST(RX_FATAL, RX__OP_LESSER, X1, X2, __VA_ARGS__)
#else
    #define RX_REAL_REQUIRE_LESSER_MSG(X1, X2, MSG)                            \
        RX__REAL_DEFINE_TEST(RX_FATAL, RX__OP_LESSER, X1, X2, MSG,             \
                             NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#endif

#define RX_REAL_REQUIRE_LESSER_MSG_1(X1, X2, MSG,                              \
                                     _0)                                       \
    RX__REAL_DEFINE_TEST(RX_FATAL, RX__OP_LESSER, X1, X2, MSG,                 \
                         _0, NULL, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_REAL_REQUIRE_LESSER_MSG_2(X1, X2, MSG,                              \
                                     _0, _1)                                   \
    RX__REAL_DEFINE_TEST(RX_FATAL, RX__OP_LESSER, X1, X2, MSG,                 \
                         _0, _1, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_REAL_REQUIRE_LESSER_MSG_3(X1, X2, MSG,                              \
                                     _0, _1, _2)                               \
    RX__REAL_DEFINE_TEST(RX_FATAL, RX__OP_LESSER, X1, X2, MSG,                 \
                         _0, _1, _2, NULL, NULL, NULL, NULL, NULL)

#define RX_REAL_REQUIRE_LESSER_MSG_4(X1, X2, MSG,                              \
                                     _0, _1, _2, _3)                           \
    RX__REAL_DEFINE_TEST(RX_FATAL, RX__OP_LESSER, X1, X2, MSG,                 \
                         _0, _1, _2, _3, NULL, NULL, NULL, NULL)

#define RX_REAL_REQUIRE_LESSER_MSG_5(X1, X2, MSG,                              \
                                     _0, _1, _2, _3, _4)                       \
    RX__REAL_DEFINE_TEST(RX_FATAL, RX__OP_LESSER, X1, X2, MSG,                 \
                         _0, _1, _2, _3, _4, NULL, NULL, NULL)

#define RX_REAL_REQUIRE_LESSER_MSG_6(X1, X2, MSG,                              \
                                     _0, _1, _2, _3, _4, _5)                   \
    RX__REAL_DEFINE_TEST(RX_FATAL, RX__OP_LESSER, X1, X2, MSG,                 \
                         _0, _1, _2, _3, _4, _5, NULL, NULL)

#define RX_REAL_REQUIRE_LESSER_MSG_7(X1, X2, MSG,                              \
                                     _0, _1, _2, _3, _4, _5, _6)               \
    RX__REAL_DEFINE_TEST(RX_FATAL, RX__OP_LESSER, X1, X2, MSG,                 \
                         _0, _1, _2, _3, _4, _5, _6, NULL)

#define RX_REAL_REQUIRE_LESSER_MSG_8(X1, X2, MSG,                              \
                                     _0, _1, _2, _3, _4, _5, _6, _7)           \
    RX__REAL_DEFINE_TEST(RX_FATAL, RX__OP_LESSER, X1, X2, MSG,                 \
                         _0, _1, _2, _3, _4, _5, _6, _7)

#define RX_REAL_CHECK_LESSER(X1, X2)                                           \
    RX_REAL_CHECK_LESSER_MSG(X1, X2, NULL)

#if RX__HAS_VARIADIC_MACROS
    #define RX_REAL_CHECK_LESSER_MSG(X1, X2, ...)                              \
        RX__REAL_DEFINE_TEST(RX_NONFATAL, RX__OP_LESSER, X1, X2, __VA_ARGS__)
#else
    #define RX_REAL_CHECK_LESSER_MSG(X1, X2, MSG)                              \
        RX__REAL_DEFINE_TEST(RX_NONFATAL, RX__OP_LESSER, X1, X2, MSG,          \
                             NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#endif

#define RX_REAL_CHECK_LESSER_MSG_1(X1, X2, MSG,                                \
                                   _0)                                         \
    RX__REAL_DEFINE_TEST(RX_NONFATAL, RX__OP_LESSER, X1, X2, MSG,              \
                         _0, NULL, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_REAL_CHECK_LESSER_MSG_2(X1, X2, MSG,                                \
                                   _0, _1)                                     \
    RX__REAL_DEFINE_TEST(RX_NONFATAL, RX__OP_LESSER, X1, X2, MSG,              \
                         _0, _1, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_REAL_CHECK_LESSER_MSG_3(X1, X2, MSG,                                \
                                   _0, _1, _2)                                 \
    RX__REAL_DEFINE_TEST(RX_NONFATAL, RX__OP_LESSER, X1, X2, MSG,              \
                         _0, _1, _2, NULL, NULL, NULL, NULL, NULL)

#define RX_REAL_CHECK_LESSER_MSG_4(X1, X2, MSG,                                \
                                   _0, _1, _2, _3)                             \
    RX__REAL_DEFINE_TEST(RX_NONFATAL, RX__OP_LESSER, X1, X2, MSG,              \
                         _0, _1, _2, _3, NULL, NULL, NULL, NULL)

#define RX_REAL_CHECK_LESSER_MSG_5(X1, X2, MSG,                                \
                                   _0, _1, _2, _3, _4)                         \
    RX__REAL_DEFINE_TEST(RX_NONFATAL, RX__OP_LESSER, X1, X2, MSG,              \
                         _0, _1, _2, _3, _4, NULL, NULL, NULL)

#define RX_REAL_CHECK_LESSER_MSG_6(X1, X2, MSG,                                \
                                   _0, _1, _2, _3, _4, _5)                     \
    RX__REAL_DEFINE_TEST(RX_NONFATAL, RX__OP_LESSER, X1, X2, MSG,              \
                         _0, _1, _2, _3, _4, _5, NULL, NULL)

#define RX_REAL_CHECK_LESSER_MSG_7(X1, X2, MSG,                                \
                                   _0, _1, _2, _3, _4, _5, _6)                 \
    RX__REAL_DEFINE_TEST(RX_NONFATAL, RX__OP_LESSER, X1, X2, MSG,              \
                         _0, _1, _2, _3, _4, _5, _6, NULL)

#define RX_REAL_CHECK_LESSER_MSG_8(X1, X2, MSG,                                \
                                   _0, _1, _2, _3, _4, _5, _6, _7)             \
    RX__REAL_DEFINE_TEST(RX_NONFATAL, RX__OP_LESSER, X1, X2, MSG,              \
                         _0, _1, _2, _3, _4, _5, _6, _7)

#define RX_REAL_REQUIRE_GREATER_OR_EQUAL(X1, X2)                               \
    RX_REAL_REQUIRE_GREATER_OR_EQUAL_MSG(X1, X2, NULL)

#if RX__HAS_VARIADIC_MACROS
    #define RX_REAL_REQUIRE_GREATER_OR_EQUAL_MSG(X1, X2, ...)                  \
        RX__REAL_DEFINE_TEST(RX_FATAL, RX__OP_GREATER_OR_EQUAL, X1, X2,        \
                             __VA_ARGS__)
#else
    #define RX_REAL_REQUIRE_GREATER_OR_EQUAL_MSG(X1, X2, MSG)                  \
        RX__REAL_DEFINE_TEST(RX_FATAL, RX__OP_GREATER_OR_EQUAL, X1, X2, MSG,   \
                             NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#endif

#define RX_REAL_REQUIRE_GREATER_OR_EQUAL_MSG_1(X1, X2, MSG,                    \
                                               _0)                             \
    RX__REAL_DEFINE_TEST(RX_FATAL, RX__OP_GREATER_OR_EQUAL, X1, X2, MSG,       \
                         _0, NULL, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_REAL_REQUIRE_GREATER_OR_EQUAL_MSG_2(X1, X2, MSG,                    \
                                               _0, _1)                         \
    RX__REAL_DEFINE_TEST(RX_FATAL, RX__OP_GREATER_OR_EQUAL, X1, X2, MSG,       \
                         _0, _1, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_REAL_REQUIRE_GREATER_OR_EQUAL_MSG_3(X1, X2, MSG,                    \
                                               _0, _1, _2)                     \
    RX__REAL_DEFINE_TEST(RX_FATAL, RX__OP_GREATER_OR_EQUAL, X1, X2, MSG,       \
                         _0, _1, _2, NULL, NULL, NULL, NULL, NULL)

#define RX_REAL_REQUIRE_GREATER_OR_EQUAL_MSG_4(X1, X2, MSG,                    \
                                               _0, _1, _2, _3)                 \
    RX__REAL_DEFINE_TEST(RX_FATAL, RX__OP_GREATER_OR_EQUAL, X1, X2, MSG,       \
                         _0, _1, _2, _3, NULL, NULL, NULL, NULL)

#define RX_REAL_REQUIRE_GREATER_OR_EQUAL_MSG_5(X1, X2, MSG,                    \
                                               _0, _1, _2, _3, _4)             \
    RX__REAL_DEFINE_TEST(RX_FATAL, RX__OP_GREATER_OR_EQUAL, X1, X2, MSG,       \
                         _0, _1, _2, _3, _4, NULL, NULL, NULL)

#define RX_REAL_REQUIRE_GREATER_OR_EQUAL_MSG_6(X1, X2, MSG,                    \
                                               _0, _1, _2, _3, _4, _5)         \
    RX__REAL_DEFINE_TEST(RX_FATAL, RX__OP_GREATER_OR_EQUAL, X1, X2, MSG,       \
                         _0, _1, _2, _3, _4, _5, NULL, NULL)

#define RX_REAL_REQUIRE_GREATER_OR_EQUAL_MSG_7(X1, X2, MSG,                    \
                                               _0, _1, _2, _3, _4, _5, _6)     \
    RX__REAL_DEFINE_TEST(RX_FATAL, RX__OP_GREATER_OR_EQUAL, X1, X2, MSG,       \
                         _0, _1, _2, _3, _4, _5, _6, NULL)

#define RX_REAL_REQUIRE_GREATER_OR_EQUAL_MSG_8(X1, X2, MSG,                    \
                                               _0, _1, _2, _3, _4, _5, _6, _7) \
    RX__REAL_DEFINE_TEST(RX_FATAL, RX__OP_GREATER_OR_EQUAL, X1, X2, MSG,       \
                         _0, _1, _2, _3, _4, _5, _6, _7)

#define RX_REAL_CHECK_GREATER_OR_EQUAL(X1, X2)                                 \
    RX_REAL_CHECK_GREATER_OR_EQUAL_MSG(X1, X2, NULL)

#if RX__HAS_VARIADIC_MACROS
    #define RX_REAL_CHECK_GREATER_OR_EQUAL_MSG(X1, X2, ...)                    \
        RX__REAL_DEFINE_TEST(RX_NONFATAL, RX__OP_GREATER_OR_EQUAL, X1, X2,     \
                             __VA_ARGS__)
#else
    #define RX_REAL_CHECK_GREATER_OR_EQUAL_MSG(X1, X2, MSG)                    \
        RX__REAL_DEFINE_TEST(RX_NONFATAL, RX__OP_GREATER_OR_EQUAL, X1, X2, MSG,\
                             NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#endif

#define RX_REAL_CHECK_GREATER_OR_EQUAL_MSG_1(X1, X2, MSG,                      \
                                             _0)                               \
    RX__REAL_DEFINE_TEST(RX_NONFATAL, RX__OP_GREATER_OR_EQUAL, X1, X2, MSG,    \
                         _0, NULL, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_REAL_CHECK_GREATER_OR_EQUAL_MSG_2(X1, X2, MSG,                      \
                                             _0, _1)                           \
    RX__REAL_DEFINE_TEST(RX_NONFATAL, RX__OP_GREATER_OR_EQUAL, X1, X2, MSG,    \
                         _0, _1, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_REAL_CHECK_GREATER_OR_EQUAL_MSG_3(X1, X2, MSG,                      \
                                             _0, _1, _2)                       \
    RX__REAL_DEFINE_TEST(RX_NONFATAL, RX__OP_GREATER_OR_EQUAL, X1, X2, MSG,    \
                         _0, _1, _2, NULL, NULL, NULL, NULL, NULL)

#define RX_REAL_CHECK_GREATER_OR_EQUAL_MSG_4(X1, X2, MSG,                      \
                                             _0, _1, _2, _3)                   \
    RX__REAL_DEFINE_TEST(RX_NONFATAL, RX__OP_GREATER_OR_EQUAL, X1, X2, MSG,    \
                         _0, _1, _2, _3, NULL, NULL, NULL, NULL)

#define RX_REAL_CHECK_GREATER_OR_EQUAL_MSG_5(X1, X2, MSG,                      \
                                             _0, _1, _2, _3, _4)               \
    RX__REAL_DEFINE_TEST(RX_NONFATAL, RX__OP_GREATER_OR_EQUAL, X1, X2, MSG,    \
                         _0, _1, _2, _3, _4, NULL, NULL, NULL)

#define RX_REAL_CHECK_GREATER_OR_EQUAL_MSG_6(X1, X2, MSG,                      \
                                             _0, _1, _2, _3, _4, _5)           \
    RX__REAL_DEFINE_TEST(RX_NONFATAL, RX__OP_GREATER_OR_EQUAL, X1, X2, MSG,    \
                         _0, _1, _2, _3, _4, _5, NULL, NULL)

#define RX_REAL_CHECK_GREATER_OR_EQUAL_MSG_7(X1, X2, MSG,                      \
                                             _0, _1, _2, _3, _4, _5, _6)       \
    RX__REAL_DEFINE_TEST(RX_NONFATAL, RX__OP_GREATER_OR_EQUAL, X1, X2, MSG,    \
                         _0, _1, _2, _3, _4, _5, _6, NULL)

#define RX_REAL_CHECK_GREATER_OR_EQUAL_MSG_8(X1, X2, MSG,                      \
                                             _0, _1, _2, _3, _4, _5, _6, _7)   \
    RX__REAL_DEFINE_TEST(RX_NONFATAL, RX__OP_GREATER_OR_EQUAL, X1, X2, MSG,    \
                         _0, _1, _2, _3, _4, _5, _6, _7)

#define RX_REAL_REQUIRE_LESSER_OR_EQUAL(X1, X2)                                \
    RX_REAL_REQUIRE_LESSER_OR_EQUAL_MSG(X1, X2, NULL)

#if RX__HAS_VARIADIC_MACROS
    #define RX_REAL_REQUIRE_LESSER_OR_EQUAL_MSG(X1, X2, ...)                   \
        RX__REAL_DEFINE_TEST(RX_FATAL, RX__OP_LESSER_OR_EQUAL, X1, X2,         \
                             __VA_ARGS__)
#else
    #define RX_REAL_REQUIRE_LESSER_OR_EQUAL_MSG(X1, X2, MSG)                   \
        RX__REAL_DEFINE_TEST(RX_FATAL, RX__OP_LESSER_OR_EQUAL, X1, X2, MSG,    \
                             NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#endif

#define RX_REAL_REQUIRE_LESSER_OR_EQUAL_MSG_1(X1, X2, MSG,                     \
                                              _0)                              \
    RX__REAL_DEFINE_TEST(RX_FATAL, RX__OP_LESSER_OR_EQUAL, X1, X2, MSG,        \
                         _0, NULL, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_REAL_REQUIRE_LESSER_OR_EQUAL_MSG_2(X1, X2, MSG,                     \
                                              _0, _1)                          \
    RX__REAL_DEFINE_TEST(RX_FATAL, RX__OP_LESSER_OR_EQUAL, X1, X2, MSG,        \
                         _0, _1, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_REAL_REQUIRE_LESSER_OR_EQUAL_MSG_3(X1, X2, MSG,                     \
                                              _0, _1, _2)                      \
    RX__REAL_DEFINE_TEST(RX_FATAL, RX__OP_LESSER_OR_EQUAL, X1, X2, MSG,        \
                         _0, _1, _2, NULL, NULL, NULL, NULL, NULL)

#define RX_REAL_REQUIRE_LESSER_OR_EQUAL_MSG_4(X1, X2, MSG,                     \
                                              _0, _1, _2, _3)                  \
    RX__REAL_DEFINE_TEST(RX_FATAL, RX__OP_LESSER_OR_EQUAL, X1, X2, MSG,        \
                         _0, _1, _2, _3, NULL, NULL, NULL, NULL)

#define RX_REAL_REQUIRE_LESSER_OR_EQUAL_MSG_5(X1, X2, MSG,                     \
                                              _0, _1, _2, _3, _4)              \
    RX__REAL_DEFINE_TEST(RX_FATAL, RX__OP_LESSER_OR_EQUAL, X1, X2, MSG,        \
                         _0, _1, _2, _3, _4, NULL, NULL, NULL)

#define RX_REAL_REQUIRE_LESSER_OR_EQUAL_MSG_6(X1, X2, MSG,                     \
                                              _0, _1, _2, _3, _4, _5)          \
    RX__REAL_DEFINE_TEST(RX_FATAL, RX__OP_LESSER_OR_EQUAL, X1, X2, MSG,        \
                         _0, _1, _2, _3, _4, _5, NULL, NULL)

#define RX_REAL_REQUIRE_LESSER_OR_EQUAL_MSG_7(X1, X2, MSG,                     \
                                              _0, _1, _2, _3, _4, _5, _6)      \
    RX__REAL_DEFINE_TEST(RX_FATAL, RX__OP_LESSER_OR_EQUAL, X1, X2, MSG,        \
                         _0, _1, _2, _3, _4, _5, _6, NULL)

#define RX_REAL_REQUIRE_LESSER_OR_EQUAL_MSG_8(X1, X2, MSG,                     \
                                              _0, _1, _2, _3, _4, _5, _6, _7)  \
    RX__REAL_DEFINE_TEST(RX_FATAL, RX__OP_LESSER_OR_EQUAL, X1, X2, MSG,        \
                         _0, _1, _2, _3, _4, _5, _6, _7)

#define RX_REAL_CHECK_LESSER_OR_EQUAL(X1, X2)                                  \
    RX_REAL_CHECK_LESSER_OR_EQUAL_MSG(X1, X2, NULL)

#if RX__HAS_VARIADIC_MACROS
    #define RX_REAL_CHECK_LESSER_OR_EQUAL_MSG(X1, X2, ...)                     \
        RX__REAL_DEFINE_TEST(RX_NONFATAL, RX__OP_LESSER_OR_EQUAL, X1, X2,      \
                             __VA_ARGS__)
#else
    #define RX_REAL_CHECK_LESSER_OR_EQUAL_MSG(X1, X2, MSG)                     \
        RX__REAL_DEFINE_TEST(RX_NONFATAL, RX__OP_LESSER_OR_EQUAL, X1, X2, MSG, \
                             NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#endif

#define RX_REAL_CHECK_LESSER_OR_EQUAL_MSG_1(X1, X2, MSG,                       \
                                            _0)                                \
    RX__REAL_DEFINE_TEST(RX_NONFATAL, RX__OP_LESSER_OR_EQUAL, X1, X2, MSG,     \
                         _0, NULL, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_REAL_CHECK_LESSER_OR_EQUAL_MSG_2(X1, X2, MSG,                       \
                                            _0, _1)                            \
    RX__REAL_DEFINE_TEST(RX_NONFATAL, RX__OP_LESSER_OR_EQUAL, X1, X2, MSG,     \
                         _0, _1, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_REAL_CHECK_LESSER_OR_EQUAL_MSG_3(X1, X2, MSG,                       \
                                            _0, _1, _2)                        \
    RX__REAL_DEFINE_TEST(RX_NONFATAL, RX__OP_LESSER_OR_EQUAL, X1, X2, MSG,     \
                         _0, _1, _2, NULL, NULL, NULL, NULL, NULL)

#define RX_REAL_CHECK_LESSER_OR_EQUAL_MSG_4(X1, X2, MSG,                       \
                                            _0, _1, _2, _3)                    \
    RX__REAL_DEFINE_TEST(RX_NONFATAL, RX__OP_LESSER_OR_EQUAL, X1, X2, MSG,     \
                         _0, _1, _2, _3, NULL, NULL, NULL, NULL)

#define RX_REAL_CHECK_LESSER_OR_EQUAL_MSG_5(X1, X2, MSG,                       \
                                            _0, _1, _2, _3, _4)                \
    RX__REAL_DEFINE_TEST(RX_NONFATAL, RX__OP_LESSER_OR_EQUAL, X1, X2, MSG,     \
                         _0, _1, _2, _3, _4, NULL, NULL, NULL)

#define RX_REAL_CHECK_LESSER_OR_EQUAL_MSG_6(X1, X2, MSG,                       \
                                            _0, _1, _2, _3, _4, _5)            \
    RX__REAL_DEFINE_TEST(RX_NONFATAL, RX__OP_LESSER_OR_EQUAL, X1, X2, MSG,     \
                         _0, _1, _2, _3, _4, _5, NULL, NULL)

#define RX_REAL_CHECK_LESSER_OR_EQUAL_MSG_7(X1, X2, MSG,                       \
                                            _0, _1, _2, _3, _4, _5, _6)        \
    RX__REAL_DEFINE_TEST(RX_NONFATAL, RX__OP_LESSER_OR_EQUAL, X1, X2, MSG,     \
                         _0, _1, _2, _3, _4, _5, _6, NULL)

#define RX_REAL_CHECK_LESSER_OR_EQUAL_MSG_8(X1, X2, MSG,                       \
                                            _0, _1, _2, _3, _4, _5, _6, _7)    \
    RX__REAL_DEFINE_TEST(RX_NONFATAL, RX__OP_LESSER_OR_EQUAL, X1, X2, MSG,     \
                         _0, _1, _2, _3, _4, _5, _6, _7)

#if RX__HAS_VARIADIC_MACROS
    #define RX__REAL_DEFINE_FUZZY_TEST(SEVERITY, OP, X1, X2, TOL, ...)         \
        rx__real_assess_fuzzy_comparison(RX_PARAM_CONTEXT,                     \
                                         (X1),                                 \
                                         (X2),                                 \
                                         (TOL),                                \
                                         OP,                                   \
                                         #X1,                                  \
                                         #X2,                                  \
                                         __FILE__,                             \
                                         __LINE__,                             \
                                         SEVERITY,                             \
                                         __VA_ARGS__)
#else
    #define RX__REAL_DEFINE_FUZZY_TEST(SEVERITY, OP, X1, X2, TOL, MSG,         \
                                       _0, _1, _2, _3, _4, _5, _6, _7)         \
        rx__real_assess_fuzzy_comparison(RX_PARAM_CONTEXT,                     \
                                         (X1),                                 \
                                         (X2),                                 \
                                         (TOL),                                \
                                         OP,                                   \
                                         #X1,                                  \
                                         #X2,                                  \
                                         __FILE__,                             \
                                         __LINE__,                             \
                                         SEVERITY,                             \
                                         MSG,                                  \
                                         _0, _1, _2, _3, _4, _5, _6, _7)
#endif

#define RX_REAL_REQUIRE_FUZZY_EQUAL(X1, X2, TOL)                               \
    RX_REAL_REQUIRE_FUZZY_EQUAL_MSG(X1, X2, TOL, NULL)

#if RX__HAS_VARIADIC_MACROS
    #define RX_REAL_REQUIRE_FUZZY_EQUAL_MSG(X1, X2, TOL, ...)                  \
        RX__REAL_DEFINE_FUZZY_TEST(RX_FATAL, RX__OP_EQUAL, X1, X2, TOL,        \
                                   __VA_ARGS__)
#else
    #define RX_REAL_REQUIRE_FUZZY_EQUAL_MSG(X1, X2, TOL, MSG)                  \
        RX__REAL_DEFINE_FUZZY_TEST(                                            \
            RX_FATAL, RX__OP_EQUAL, X1, X2, TOL, MSG,                          \
            NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#endif

#define RX_REAL_REQUIRE_FUZZY_EQUAL_MSG_1(X1, X2, TOL, MSG,                    \
                                          _0)                                  \
    RX__REAL_DEFINE_FUZZY_TEST(RX_FATAL, RX__OP_EQUAL, X1, X2, TOL, MSG,       \
                               _0, NULL, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_REAL_REQUIRE_FUZZY_EQUAL_MSG_2(X1, X2, TOL, MSG,                    \
                                          _0, _1)                              \
    RX__REAL_DEFINE_FUZZY_TEST(RX_FATAL, RX__OP_EQUAL, X1, X2, TOL, MSG,       \
                               _0, _1, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_REAL_REQUIRE_FUZZY_EQUAL_MSG_3(X1, X2, TOL, MSG,                    \
                                          _0, _1, _2)                          \
    RX__REAL_DEFINE_FUZZY_TEST(RX_FATAL, RX__OP_EQUAL, X1, X2, TOL, MSG,       \
                               _0, _1, _2, NULL, NULL, NULL, NULL, NULL)

#define RX_REAL_REQUIRE_FUZZY_EQUAL_MSG_4(X1, X2, TOL, MSG,                    \
                                          _0, _1, _2, _3)                      \
    RX__REAL_DEFINE_FUZZY_TEST(RX_FATAL, RX__OP_EQUAL, X1, X2, TOL, MSG,       \
                               _0, _1, _2, _3, NULL, NULL, NULL, NULL)

#define RX_REAL_REQUIRE_FUZZY_EQUAL_MSG_5(X1, X2, TOL, MSG,                    \
                                          _0, _1, _2, _3, _4)                  \
    RX__REAL_DEFINE_FUZZY_TEST(RX_FATAL, RX__OP_EQUAL, X1, X2, TOL, MSG,       \
                               _0, _1, _2, _3, _4, NULL, NULL, NULL)

#define RX_REAL_REQUIRE_FUZZY_EQUAL_MSG_6(X1, X2, TOL, MSG,                    \
                                          _0, _1, _2, _3, _4, _5)              \
    RX__REAL_DEFINE_FUZZY_TEST(RX_FATAL, RX__OP_EQUAL, X1, X2, TOL, MSG,       \
                               _0, _1, _2, _3, _4, _5, NULL, NULL)

#define RX_REAL_REQUIRE_FUZZY_EQUAL_MSG_7(X1, X2, TOL, MSG,                    \
                                          _0, _1, _2, _3, _4, _5, _6)          \
    RX__REAL_DEFINE_FUZZY_TEST(RX_FATAL, RX__OP_EQUAL, X1, X2, TOL, MSG,       \
                               _0, _1, _2, _3, _4, _5, _6, NULL)

#define RX_REAL_REQUIRE_FUZZY_EQUAL_MSG_8(X1, X2, TOL, MSG,                    \
                                          _0, _1, _2, _3, _4, _5, _6, _7)      \
    RX__REAL_DEFINE_FUZZY_TEST(RX_FATAL, RX__OP_EQUAL, X1, X2, TOL, MSG,       \
                               _0, _1, _2, _3, _4, _5, _6, _7)

#define RX_REAL_CHECK_FUZZY_EQUAL(X1, X2, TOL)                                 \
    RX_REAL_CHECK_FUZZY_EQUAL_MSG(X1, X2, TOL, NULL)

#if RX__HAS_VARIADIC_MACROS
    #define RX_REAL_CHECK_FUZZY_EQUAL_MSG(X1, X2, TOL, ...)                    \
        RX__REAL_DEFINE_FUZZY_TEST(RX_NONFATAL, RX__OP_EQUAL, X1, X2, TOL,     \
                                   __VA_ARGS__)
#else
    #define RX_REAL_CHECK_FUZZY_EQUAL_MSG(X1, X2, TOL, MSG)                    \
        RX__REAL_DEFINE_FUZZY_TEST(                                            \
            RX_NONFATAL, RX__OP_EQUAL, X1, X2, TOL, MSG,                       \
            NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#endif

#define RX_REAL_CHECK_FUZZY_EQUAL_MSG_1(X1, X2, TOL, MSG,                      \
                                        _0)                                    \
    RX__REAL_DEFINE_FUZZY_TEST(RX_NONFATAL, RX__OP_EQUAL, X1, X2, TOL, MSG,    \
                               _0, NULL, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_REAL_CHECK_FUZZY_EQUAL_MSG_2(X1, X2, TOL, MSG,                      \
                                        _0, _1)                                \
    RX__REAL_DEFINE_FUZZY_TEST(RX_NONFATAL, RX__OP_EQUAL, X1, X2, TOL, MSG,    \
                               _0, _1, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_REAL_CHECK_FUZZY_EQUAL_MSG_3(X1, X2, TOL, MSG,                      \
                                        _0, _1, _2)                            \
    RX__REAL_DEFINE_FUZZY_TEST(RX_NONFATAL, RX__OP_EQUAL, X1, X2, TOL, MSG,    \
                               _0, _1, _2, NULL, NULL, NULL, NULL, NULL)

#define RX_REAL_CHECK_FUZZY_EQUAL_MSG_4(X1, X2, TOL, MSG,                      \
                                        _0, _1, _2, _3)                        \
    RX__REAL_DEFINE_FUZZY_TEST(RX_NONFATAL, RX__OP_EQUAL, X1, X2, TOL, MSG,    \
                               _0, _1, _2, _3, NULL, NULL, NULL, NULL)

#define RX_REAL_CHECK_FUZZY_EQUAL_MSG_5(X1, X2, TOL, MSG,                      \
                                        _0, _1, _2, _3, _4)                    \
    RX__REAL_DEFINE_FUZZY_TEST(RX_NONFATAL, RX__OP_EQUAL, X1, X2, TOL, MSG,    \
                               _0, _1, _2, _3, _4, NULL, NULL, NULL)

#define RX_REAL_CHECK_FUZZY_EQUAL_MSG_6(X1, X2, TOL, MSG,                      \
                                        _0, _1, _2, _3, _4, _5)                \
    RX__REAL_DEFINE_FUZZY_TEST(RX_NONFATAL, RX__OP_EQUAL, X1, X2, TOL, MSG,    \
                               _0, _1, _2, _3, _4, _5, NULL, NULL)

#define RX_REAL_CHECK_FUZZY_EQUAL_MSG_7(X1, X2, TOL, MSG,                      \
                                        _0, _1, _2, _3, _4, _5, _6)            \
    RX__REAL_DEFINE_FUZZY_TEST(RX_NONFATAL, RX__OP_EQUAL, X1, X2, TOL, MSG,    \
                               _0, _1, _2, _3, _4, _5, _6, NULL)

#define RX_REAL_CHECK_FUZZY_EQUAL_MSG_8(X1, X2, TOL, MSG,                      \
                                        _0, _1, _2, _3, _4, _5, _6, _7)        \
    RX__REAL_DEFINE_FUZZY_TEST(RX_NONFATAL, RX__OP_EQUAL, X1, X2, TOL, MSG,    \
                               _0, _1, _2, _3, _4, _5, _6, _7)

#define RX_REAL_REQUIRE_FUZZY_NOT_EQUAL(X1, X2, TOL)                           \
    RX_REAL_REQUIRE_FUZZY_NOT_EQUAL_MSG(X1, X2, TOL, NULL)

#if RX__HAS_VARIADIC_MACROS
    #define RX_REAL_REQUIRE_FUZZY_NOT_EQUAL_MSG(X1, X2, TOL, ...)              \
        RX__REAL_DEFINE_FUZZY_TEST(RX_FATAL, RX__OP_NOT_EQUAL, X1, X2, TOL,    \
                                   __VA_ARGS__)
#else
    #define RX_REAL_REQUIRE_FUZZY_NOT_EQUAL_MSG(X1, X2, TOL, MSG)              \
        RX__REAL_DEFINE_FUZZY_TEST(                                            \
            RX_FATAL, RX__OP_NOT_EQUAL, X1, X2, TOL, MSG,                      \
            NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#endif

#define RX_REAL_REQUIRE_FUZZY_NOT_EQUAL_MSG_1(X1, X2, TOL, MSG,                \
                                              _0)                              \
    RX__REAL_DEFINE_FUZZY_TEST(RX_FATAL, RX__OP_NOT_EQUAL, X1, X2, TOL, MSG,   \
                               _0, NULL, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_REAL_REQUIRE_FUZZY_NOT_EQUAL_MSG_2(X1, X2, TOL, MSG,                \
                                              _0, _1)                          \
    RX__REAL_DEFINE_FUZZY_TEST(RX_FATAL, RX__OP_NOT_EQUAL, X1, X2, TOL, MSG,   \
                               _0, _1, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_REAL_REQUIRE_FUZZY_NOT_EQUAL_MSG_3(X1, X2, TOL, MSG,                \
                                              _0, _1, _2)                      \
    RX__REAL_DEFINE_FUZZY_TEST(RX_FATAL, RX__OP_NOT_EQUAL, X1, X2, TOL, MSG,   \
                               _0, _1, _2, NULL, NULL, NULL, NULL, NULL)

#define RX_REAL_REQUIRE_FUZZY_NOT_EQUAL_MSG_4(X1, X2, TOL, MSG,                \
                                              _0, _1, _2, _3)                  \
    RX__REAL_DEFINE_FUZZY_TEST(RX_FATAL, RX__OP_NOT_EQUAL, X1, X2, TOL, MSG,   \
                               _0, _1, _2, _3, NULL, NULL, NULL, NULL)

#define RX_REAL_REQUIRE_FUZZY_NOT_EQUAL_MSG_5(X1, X2, TOL, MSG,                \
                                              _0, _1, _2, _3, _4)              \
    RX__REAL_DEFINE_FUZZY_TEST(RX_FATAL, RX__OP_NOT_EQUAL, X1, X2, TOL, MSG,   \
                               _0, _1, _2, _3, _4, NULL, NULL, NULL)

#define RX_REAL_REQUIRE_FUZZY_NOT_EQUAL_MSG_6(X1, X2, TOL, MSG,                \
                                              _0, _1, _2, _3, _4, _5)          \
    RX__REAL_DEFINE_FUZZY_TEST(RX_FATAL, RX__OP_NOT_EQUAL, X1, X2, TOL, MSG,   \
                               _0, _1, _2, _3, _4, _5, NULL, NULL)

#define RX_REAL_REQUIRE_FUZZY_NOT_EQUAL_MSG_7(X1, X2, TOL, MSG,                \
                                              _0, _1, _2, _3, _4, _5, _6)      \
    RX__REAL_DEFINE_FUZZY_TEST(RX_FATAL, RX__OP_NOT_EQUAL, X1, X2, TOL, MSG,   \
                               _0, _1, _2, _3, _4, _5, _6, NULL)

#define RX_REAL_REQUIRE_FUZZY_NOT_EQUAL_MSG_8(X1, X2, TOL, MSG,                \
                                              _0, _1, _2, _3, _4, _5, _6, _7)  \
    RX__REAL_DEFINE_FUZZY_TEST(RX_FATAL, RX__OP_NOT_EQUAL, X1, X2, TOL, MSG,   \
                               _0, _1, _2, _3, _4, _5, _6, _7)

#define RX_REAL_CHECK_FUZZY_NOT_EQUAL(X1, X2, TOL)                             \
    RX_REAL_CHECK_FUZZY_NOT_EQUAL_MSG(X1, X2, TOL, NULL)

#if RX__HAS_VARIADIC_MACROS
    #define RX_REAL_CHECK_FUZZY_NOT_EQUAL_MSG(X1, X2, TOL, ...)                \
        RX__REAL_DEFINE_FUZZY_TEST(RX_NONFATAL, RX__OP_NOT_EQUAL, X1, X2, TOL, \
                                   __VA_ARGS__)
#else
    #define RX_REAL_CHECK_FUZZY_NOT_EQUAL_MSG(X1, X2, TOL, MSG)                \
        RX__REAL_DEFINE_FUZZY_TEST(                                            \
            RX_NONFATAL, RX__OP_NOT_EQUAL, X1, X2, TOL, MSG,                   \
            NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#endif

#define RX_REAL_CHECK_FUZZY_NOT_EQUAL_MSG_1(X1, X2, TOL, MSG,                  \
                                            _0)                                \
    RX__REAL_DEFINE_FUZZY_TEST(RX_NONFATAL, RX__OP_NOT_EQUAL, X1, X2, TOL, MSG,\
                               _0, NULL, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_REAL_CHECK_FUZZY_NOT_EQUAL_MSG_2(X1, X2, TOL, MSG,                  \
                                            _0, _1)                            \
    RX__REAL_DEFINE_FUZZY_TEST(RX_NONFATAL, RX__OP_NOT_EQUAL, X1, X2, TOL, MSG,\
                               _0, _1, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_REAL_CHECK_FUZZY_NOT_EQUAL_MSG_3(X1, X2, TOL, MSG,                  \
                                            _0, _1, _2)                        \
    RX__REAL_DEFINE_FUZZY_TEST(RX_NONFATAL, RX__OP_NOT_EQUAL, X1, X2, TOL, MSG,\
                               _0, _1, _2, NULL, NULL, NULL, NULL, NULL)

#define RX_REAL_CHECK_FUZZY_NOT_EQUAL_MSG_4(X1, X2, TOL, MSG,                  \
                                            _0, _1, _2, _3)                    \
    RX__REAL_DEFINE_FUZZY_TEST(RX_NONFATAL, RX__OP_NOT_EQUAL, X1, X2, TOL, MSG,\
                               _0, _1, _2, _3, NULL, NULL, NULL, NULL)

#define RX_REAL_CHECK_FUZZY_NOT_EQUAL_MSG_5(X1, X2, TOL, MSG,                  \
                                            _0, _1, _2, _3, _4)                \
    RX__REAL_DEFINE_FUZZY_TEST(RX_NONFATAL, RX__OP_NOT_EQUAL, X1, X2, TOL, MSG,\
                               _0, _1, _2, _3, _4, NULL, NULL, NULL)

#define RX_REAL_CHECK_FUZZY_NOT_EQUAL_MSG_6(X1, X2, TOL, MSG,                  \
                                            _0, _1, _2, _3, _4, _5)            \
    RX__REAL_DEFINE_FUZZY_TEST(RX_NONFATAL, RX__OP_NOT_EQUAL, X1, X2, TOL, MSG,\
                               _0, _1, _2, _3, _4, _5, NULL, NULL)

#define RX_REAL_CHECK_FUZZY_NOT_EQUAL_MSG_7(X1, X2, TOL, MSG,                  \
                                            _0, _1, _2, _3, _4, _5, _6)        \
    RX__REAL_DEFINE_FUZZY_TEST(RX_NONFATAL, RX__OP_NOT_EQUAL, X1, X2, TOL, MSG,\
                               _0, _1, _2, _3, _4, _5, _6, NULL)

#define RX_REAL_CHECK_FUZZY_NOT_EQUAL_MSG_8(X1, X2, TOL, MSG,                  \
                                            _0, _1, _2, _3, _4, _5, _6, _7)    \
    RX__REAL_DEFINE_FUZZY_TEST(RX_NONFATAL, RX__OP_NOT_EQUAL, X1, X2, TOL, MSG,\
                               _0, _1, _2, _3, _4, _5, _6, _7)

/* String Assertions                                               O-(''Q)
   -------------------------------------------------------------------------- */

#if RX__HAS_VARIADIC_MACROS
    #define RX__STR_TEST_DEFINE(SEVERITY, OP, STR_CASE, S1, S2, ...)           \
        rx__str_assess_comparison(RX_PARAM_CONTEXT,                            \
                                  (S1),                                        \
                                  (S2),                                        \
                                  STR_CASE,                                    \
                                  OP,                                          \
                                  #S1,                                         \
                                  #S2,                                         \
                                  __FILE__,                                    \
                                  __LINE__,                                    \
                                  SEVERITY,                                    \
                                  __VA_ARGS__)
#else
    #define RX__STR_TEST_DEFINE(SEVERITY, OP, STR_CASE, S1, S2, MSG,           \
                                _0, _1, _2, _3, _4, _5, _6, _7)                \
        rx__str_assess_comparison(RX_PARAM_CONTEXT,                            \
                                  (S1),                                        \
                                  (S2),                                        \
                                  STR_CASE,                                    \
                                  OP,                                          \
                                  #S1,                                         \
                                  #S2,                                         \
                                  __FILE__,                                    \
                                  __LINE__,                                    \
                                  SEVERITY,                                    \
                                  MSG,                                         \
                                  _0, _1, _2, _3, _4, _5, _6, _7)
#endif

#define RX_STR_REQUIRE_EQUAL(S1, S2)                                           \
    RX_STR_REQUIRE_EQUAL_MSG(S1, S2, NULL)

#if RX__HAS_VARIADIC_MACROS
    #define RX_STR_REQUIRE_EQUAL_MSG(S1, S2, ...)                              \
        RX__STR_TEST_DEFINE(                                                   \
            RX_FATAL, RX__OP_EQUAL, RX__STR_CASE_OBEY, S1, S2, __VA_ARGS__)
#else
    #define RX_STR_REQUIRE_EQUAL_MSG(S1, S2, MSG)                              \
        RX__STR_TEST_DEFINE(                                                   \
            RX_FATAL, RX__OP_EQUAL, RX__STR_CASE_OBEY, S1, S2, MSG,            \
            NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#endif

#define RX_STR_REQUIRE_EQUAL_MSG_1(S1, S2, MSG,                                \
                                   _0)                                         \
        RX__STR_TEST_DEFINE(                                                   \
            RX_FATAL, RX__OP_EQUAL, RX__STR_CASE_OBEY, S1, S2, MSG,            \
            _0, NULL, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_STR_REQUIRE_EQUAL_MSG_2(S1, S2, MSG,                                \
                                   _0, _1)                                     \
        RX__STR_TEST_DEFINE(                                                   \
            RX_FATAL, RX__OP_EQUAL, RX__STR_CASE_OBEY, S1, S2, MSG,            \
            _0, _1, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_STR_REQUIRE_EQUAL_MSG_3(S1, S2, MSG,                                \
                                   _0, _1, _2)                                 \
        RX__STR_TEST_DEFINE(                                                   \
            RX_FATAL, RX__OP_EQUAL, RX__STR_CASE_OBEY, S1, S2, MSG,            \
            _0, _1, _2, NULL, NULL, NULL, NULL, NULL)

#define RX_STR_REQUIRE_EQUAL_MSG_4(S1, S2, MSG,                                \
                                   _0, _1, _2, _3)                             \
        RX__STR_TEST_DEFINE(                                                   \
            RX_FATAL, RX__OP_EQUAL, RX__STR_CASE_OBEY, S1, S2, MSG,            \
            _0, _1, _2, _3, NULL, NULL, NULL, NULL)

#define RX_STR_REQUIRE_EQUAL_MSG_5(S1, S2, MSG,                                \
                                   _0, _1, _2, _3, _4)                         \
        RX__STR_TEST_DEFINE(                                                   \
            RX_FATAL, RX__OP_EQUAL, RX__STR_CASE_OBEY, S1, S2, MSG,            \
            _0, _1, _2, _3, _4, NULL, NULL, NULL)

#define RX_STR_REQUIRE_EQUAL_MSG_6(S1, S2, MSG,                                \
                                   _0, _1, _2, _3, _4, _5)                     \
        RX__STR_TEST_DEFINE(                                                   \
            RX_FATAL, RX__OP_EQUAL, RX__STR_CASE_OBEY, S1, S2, MSG,            \
            _0, _1, _2, _3, _4, _5, NULL, NULL)

#define RX_STR_REQUIRE_EQUAL_MSG_7(S1, S2, MSG,                                \
                                   _0, _1, _2, _3, _4, _5, _6)                 \
        RX__STR_TEST_DEFINE(                                                   \
            RX_FATAL, RX__OP_EQUAL, RX__STR_CASE_OBEY, S1, S2, MSG,            \
            _0, _1, _2, _3, _4, _5, _6, NULL)

#define RX_STR_REQUIRE_EQUAL_MSG_8(S1, S2, MSG,                                \
                                   _0, _1, _2, _3, _4, _5, _6, _7)             \
        RX__STR_TEST_DEFINE(                                                   \
            RX_FATAL, RX__OP_EQUAL, RX__STR_CASE_OBEY, S1, S2, MSG,            \
            _0, _1, _2, _3, _4, _5, _6, _7)

#define RX_STR_CHECK_EQUAL(S1, S2)                                             \
    RX_STR_CHECK_EQUAL_MSG(S1, S2, NULL)

#if RX__HAS_VARIADIC_MACROS
    #define RX_STR_CHECK_EQUAL_MSG(S1, S2, ...)                                \
        RX__STR_TEST_DEFINE(                                                   \
            RX_NONFATAL, RX__OP_EQUAL, RX__STR_CASE_OBEY, S1, S2, __VA_ARGS__)
#else
    #define RX_STR_CHECK_EQUAL_MSG(S1, S2, MSG)                                \
        RX__STR_TEST_DEFINE(                                                   \
            RX_NONFATAL, RX__OP_EQUAL, RX__STR_CASE_OBEY, S1, S2, MSG,         \
            NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#endif

#define RX_STR_CHECK_EQUAL_MSG_1(S1, S2, MSG,                                  \
                                 _0)                                           \
        RX__STR_TEST_DEFINE(                                                   \
            RX_NONFATAL, RX__OP_EQUAL, RX__STR_CASE_OBEY, S1, S2, MSG,         \
            _0, NULL, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_STR_CHECK_EQUAL_MSG_2(S1, S2, MSG,                                  \
                                 _0, _1)                                       \
        RX__STR_TEST_DEFINE(                                                   \
            RX_NONFATAL, RX__OP_EQUAL, RX__STR_CASE_OBEY, S1, S2, MSG,         \
            _0, _1, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_STR_CHECK_EQUAL_MSG_3(S1, S2, MSG,                                  \
                                 _0, _1, _2)                                   \
        RX__STR_TEST_DEFINE(                                                   \
            RX_NONFATAL, RX__OP_EQUAL, RX__STR_CASE_OBEY, S1, S2, MSG,         \
            _0, _1, _2, NULL, NULL, NULL, NULL, NULL)

#define RX_STR_CHECK_EQUAL_MSG_4(S1, S2, MSG,                                  \
                                 _0, _1, _2, _3)                               \
        RX__STR_TEST_DEFINE(                                                   \
            RX_NONFATAL, RX__OP_EQUAL, RX__STR_CASE_OBEY, S1, S2, MSG,         \
            _0, _1, _2, _3, NULL, NULL, NULL, NULL)

#define RX_STR_CHECK_EQUAL_MSG_5(S1, S2, MSG,                                  \
                                 _0, _1, _2, _3, _4)                           \
        RX__STR_TEST_DEFINE(                                                   \
            RX_NONFATAL, RX__OP_EQUAL, RX__STR_CASE_OBEY, S1, S2, MSG,         \
            _0, _1, _2, _3, _4, NULL, NULL, NULL)

#define RX_STR_CHECK_EQUAL_MSG_6(S1, S2, MSG,                                  \
                                 _0, _1, _2, _3, _4, _5)                       \
        RX__STR_TEST_DEFINE(                                                   \
            RX_NONFATAL, RX__OP_EQUAL, RX__STR_CASE_OBEY, S1, S2, MSG,         \
            _0, _1, _2, _3, _4, _5, NULL, NULL)

#define RX_STR_CHECK_EQUAL_MSG_7(S1, S2, MSG,                                  \
                                 _0, _1, _2, _3, _4, _5, _6)                   \
        RX__STR_TEST_DEFINE(                                                   \
            RX_NONFATAL, RX__OP_EQUAL, RX__STR_CASE_OBEY, S1, S2, MSG,         \
            _0, _1, _2, _3, _4, _5, _6, NULL)

#define RX_STR_CHECK_EQUAL_MSG_8(S1, S2, MSG,                                  \
                                 _0, _1, _2, _3, _4, _5, _6, _7)               \
        RX__STR_TEST_DEFINE(                                                   \
            RX_NONFATAL, RX__OP_EQUAL, RX__STR_CASE_OBEY, S1, S2, MSG,         \
            _0, _1, _2, _3, _4, _5, _6, _7)

#define RX_STR_REQUIRE_NOT_EQUAL(S1, S2)                                       \
    RX_STR_REQUIRE_NOT_EQUAL_MSG(S1, S2, NULL)

#if RX__HAS_VARIADIC_MACROS
    #define RX_STR_REQUIRE_NOT_EQUAL_MSG(S1, S2, ...)                          \
        RX__STR_TEST_DEFINE(                                                   \
            RX_FATAL, RX__OP_NOT_EQUAL, RX__STR_CASE_OBEY, S1, S2,             \
            __VA_ARGS__)
#else
    #define RX_STR_REQUIRE_NOT_EQUAL_MSG(S1, S2, MSG)                          \
        RX__STR_TEST_DEFINE(                                                   \
            RX_FATAL, RX__OP_NOT_EQUAL, RX__STR_CASE_OBEY, S1, S2, MSG,        \
            NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#endif

#define RX_STR_REQUIRE_NOT_EQUAL_MSG_1(S1, S2, MSG,                            \
                                       _0)                                     \
        RX__STR_TEST_DEFINE(                                                   \
            RX_FATAL, RX__OP_NOT_EQUAL, RX__STR_CASE_OBEY, S1, S2, MSG,        \
            _0, NULL, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_STR_REQUIRE_NOT_EQUAL_MSG_2(S1, S2, MSG,                            \
                                       _0, _1)                                 \
        RX__STR_TEST_DEFINE(                                                   \
            RX_FATAL, RX__OP_NOT_EQUAL, RX__STR_CASE_OBEY, S1, S2, MSG,        \
            _0, _1, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_STR_REQUIRE_NOT_EQUAL_MSG_3(S1, S2, MSG,                            \
                                       _0, _1, _2)                             \
        RX__STR_TEST_DEFINE(                                                   \
            RX_FATAL, RX__OP_NOT_EQUAL, RX__STR_CASE_OBEY, S1, S2, MSG,        \
            _0, _1, _2, NULL, NULL, NULL, NULL, NULL)

#define RX_STR_REQUIRE_NOT_EQUAL_MSG_4(S1, S2, MSG,                            \
                                       _0, _1, _2, _3)                         \
        RX__STR_TEST_DEFINE(                                                   \
            RX_FATAL, RX__OP_NOT_EQUAL, RX__STR_CASE_OBEY, S1, S2, MSG,        \
            _0, _1, _2, _3, NULL, NULL, NULL, NULL)

#define RX_STR_REQUIRE_NOT_EQUAL_MSG_5(S1, S2, MSG,                            \
                                       _0, _1, _2, _3, _4)                     \
        RX__STR_TEST_DEFINE(                                                   \
            RX_FATAL, RX__OP_NOT_EQUAL, RX__STR_CASE_OBEY, S1, S2, MSG,        \
            _0, _1, _2, _3, _4, NULL, NULL, NULL)

#define RX_STR_REQUIRE_NOT_EQUAL_MSG_6(S1, S2, MSG,                            \
                                       _0, _1, _2, _3, _4, _5)                 \
        RX__STR_TEST_DEFINE(                                                   \
            RX_FATAL, RX__OP_NOT_EQUAL, RX__STR_CASE_OBEY, S1, S2, MSG,        \
            _0, _1, _2, _3, _4, _5, NULL, NULL)

#define RX_STR_REQUIRE_NOT_EQUAL_MSG_7(S1, S2, MSG,                            \
                                       _0, _1, _2, _3, _4, _5, _6)             \
        RX__STR_TEST_DEFINE(                                                   \
            RX_FATAL, RX__OP_NOT_EQUAL, RX__STR_CASE_OBEY, S1, S2, MSG,        \
            _0, _1, _2, _3, _4, _5, _6, NULL)

#define RX_STR_REQUIRE_NOT_EQUAL_MSG_8(S1, S2, MSG,                            \
                                       _0, _1, _2, _3, _4, _5, _6, _7)         \
        RX__STR_TEST_DEFINE(                                                   \
            RX_FATAL, RX__OP_NOT_EQUAL, RX__STR_CASE_OBEY, S1, S2, MSG,        \
            _0, _1, _2, _3, _4, _5, _6, _7)

#define RX_STR_CHECK_NOT_EQUAL(S1, S2)                                         \
    RX_STR_CHECK_NOT_EQUAL_MSG(S1, S2, NULL)

#if RX__HAS_VARIADIC_MACROS
    #define RX_STR_CHECK_NOT_EQUAL_MSG(S1, S2, ...)                            \
        RX__STR_TEST_DEFINE(                                                   \
            RX_NONFATAL, RX__OP_NOT_EQUAL, RX__STR_CASE_OBEY, S1, S2,          \
            __VA_ARGS__)
#else
    #define RX_STR_CHECK_NOT_EQUAL_MSG(S1, S2, MSG)                            \
        RX__STR_TEST_DEFINE(                                                   \
            RX_NONFATAL, RX__OP_NOT_EQUAL, RX__STR_CASE_OBEY, S1, S2, MSG,     \
            NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#endif

#define RX_STR_CHECK_NOT_EQUAL_MSG_1(S1, S2, MSG,                              \
                                     _0)                                       \
        RX__STR_TEST_DEFINE(                                                   \
            RX_NONFATAL, RX__OP_NOT_EQUAL, RX__STR_CASE_OBEY, S1, S2, MSG,     \
            _0, NULL, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_STR_CHECK_NOT_EQUAL_MSG_2(S1, S2, MSG,                              \
                                     _0, _1)                                   \
        RX__STR_TEST_DEFINE(                                                   \
            RX_NONFATAL, RX__OP_NOT_EQUAL, RX__STR_CASE_OBEY, S1, S2, MSG,     \
            _0, _1, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_STR_CHECK_NOT_EQUAL_MSG_3(S1, S2, MSG,                              \
                                     _0, _1, _2)                               \
        RX__STR_TEST_DEFINE(                                                   \
            RX_NONFATAL, RX__OP_NOT_EQUAL, RX__STR_CASE_OBEY, S1, S2, MSG,     \
            _0, _1, _2, NULL, NULL, NULL, NULL, NULL)

#define RX_STR_CHECK_NOT_EQUAL_MSG_4(S1, S2, MSG,                              \
                                     _0, _1, _2, _3)                           \
        RX__STR_TEST_DEFINE(                                                   \
            RX_NONFATAL, RX__OP_NOT_EQUAL, RX__STR_CASE_OBEY, S1, S2, MSG,     \
            _0, _1, _2, _3, NULL, NULL, NULL, NULL)

#define RX_STR_CHECK_NOT_EQUAL_MSG_5(S1, S2, MSG,                              \
                                     _0, _1, _2, _3, _4)                       \
        RX__STR_TEST_DEFINE(                                                   \
            RX_NONFATAL, RX__OP_NOT_EQUAL, RX__STR_CASE_OBEY, S1, S2, MSG,     \
            _0, _1, _2, _3, _4, NULL, NULL, NULL)

#define RX_STR_CHECK_NOT_EQUAL_MSG_6(S1, S2, MSG,                              \
                                     _0, _1, _2, _3, _4, _5)                   \
        RX__STR_TEST_DEFINE(                                                   \
            RX_NONFATAL, RX__OP_NOT_EQUAL, RX__STR_CASE_OBEY, S1, S2, MSG,     \
            _0, _1, _2, _3, _4, _5, NULL, NULL)

#define RX_STR_CHECK_NOT_EQUAL_MSG_7(S1, S2, MSG,                              \
                                     _0, _1, _2, _3, _4, _5, _6)               \
        RX__STR_TEST_DEFINE(                                                   \
            RX_NONFATAL, RX__OP_NOT_EQUAL, RX__STR_CASE_OBEY, S1, S2, MSG,     \
            _0, _1, _2, _3, _4, _5, _6, NULL)

#define RX_STR_CHECK_NOT_EQUAL_MSG_8(S1, S2, MSG,                              \
                                     _0, _1, _2, _3, _4, _5, _6, _7)           \
        RX__STR_TEST_DEFINE(                                                   \
            RX_NONFATAL, RX__OP_NOT_EQUAL, RX__STR_CASE_OBEY, S1, S2, MSG,     \
            _0, _1, _2, _3, _4, _5, _6, _7)

#define RX_STR_REQUIRE_EQUAL_NO_CASE(S1, S2)                                   \
    RX_STR_REQUIRE_EQUAL_NO_CASE_MSG(S1, S2, NULL)

#if RX__HAS_VARIADIC_MACROS
    #define RX_STR_REQUIRE_EQUAL_NO_CASE_MSG(S1, S2, ...)                      \
        RX__STR_TEST_DEFINE(                                                   \
            RX_FATAL, RX__OP_EQUAL, RX__STR_CASE_IGNORE, S1, S2, __VA_ARGS__)
#else
    #define RX_STR_REQUIRE_EQUAL_NO_CASE_MSG(S1, S2, MSG)                      \
        RX__STR_TEST_DEFINE(                                                   \
            RX_FATAL, RX__OP_EQUAL, RX__STR_CASE_IGNORE, S1, S2, MSG,          \
            NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#endif

#define RX_STR_REQUIRE_EQUAL_NO_CASE_MSG_1(S1, S2, MSG,                        \
                                           _0)                                 \
        RX__STR_TEST_DEFINE(                                                   \
            RX_FATAL, RX__OP_EQUAL, RX__STR_CASE_IGNORE, S1, S2, MSG,          \
            _0, NULL, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_STR_REQUIRE_EQUAL_NO_CASE_MSG_2(S1, S2, MSG,                        \
                                           _0, _1)                             \
        RX__STR_TEST_DEFINE(                                                   \
            RX_FATAL, RX__OP_EQUAL, RX__STR_CASE_IGNORE, S1, S2, MSG,          \
            _0, _1, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_STR_REQUIRE_EQUAL_NO_CASE_MSG_3(S1, S2, MSG,                        \
                                           _0, _1, _2)                         \
        RX__STR_TEST_DEFINE(                                                   \
            RX_FATAL, RX__OP_EQUAL, RX__STR_CASE_IGNORE, S1, S2, MSG,          \
            _0, _1, _2, NULL, NULL, NULL, NULL, NULL)

#define RX_STR_REQUIRE_EQUAL_NO_CASE_MSG_4(S1, S2, MSG,                        \
                                           _0, _1, _2, _3)                     \
        RX__STR_TEST_DEFINE(                                                   \
            RX_FATAL, RX__OP_EQUAL, RX__STR_CASE_IGNORE, S1, S2, MSG,          \
            _0, _1, _2, _3, NULL, NULL, NULL, NULL)

#define RX_STR_REQUIRE_EQUAL_NO_CASE_MSG_5(S1, S2, MSG,                        \
                                           _0, _1, _2, _3, _4)                 \
        RX__STR_TEST_DEFINE(                                                   \
            RX_FATAL, RX__OP_EQUAL, RX__STR_CASE_IGNORE, S1, S2, MSG,          \
            _0, _1, _2, _3, _4, NULL, NULL, NULL)

#define RX_STR_REQUIRE_EQUAL_NO_CASE_MSG_6(S1, S2, MSG,                        \
                                           _0, _1, _2, _3, _4, _5)             \
        RX__STR_TEST_DEFINE(                                                   \
            RX_FATAL, RX__OP_EQUAL, RX__STR_CASE_IGNORE, S1, S2, MSG,          \
            _0, _1, _2, _3, _4, _5, NULL, NULL)

#define RX_STR_REQUIRE_EQUAL_NO_CASE_MSG_7(S1, S2, MSG,                        \
                                           _0, _1, _2, _3, _4, _5, _6)         \
        RX__STR_TEST_DEFINE(                                                   \
            RX_FATAL, RX__OP_EQUAL, RX__STR_CASE_IGNORE, S1, S2, MSG,          \
            _0, _1, _2, _3, _4, _5, _6, NULL)

#define RX_STR_REQUIRE_EQUAL_NO_CASE_MSG_8(S1, S2, MSG,                        \
                                           _0, _1, _2, _3, _4, _5, _6, _7)     \
        RX__STR_TEST_DEFINE(                                                   \
            RX_FATAL, RX__OP_EQUAL, RX__STR_CASE_IGNORE, S1, S2, MSG,          \
            _0, _1, _2, _3, _4, _5, _6, _7)

#define RX_STR_CHECK_EQUAL_NO_CASE(S1, S2)                                     \
    RX_STR_CHECK_EQUAL_NO_CASE_MSG(S1, S2, NULL)

#if RX__HAS_VARIADIC_MACROS
    #define RX_STR_CHECK_EQUAL_NO_CASE_MSG(S1, S2, ...)                        \
        RX__STR_TEST_DEFINE(                                                   \
            RX_NONFATAL, RX__OP_EQUAL, RX__STR_CASE_IGNORE, S1, S2, __VA_ARGS__)
#else
    #define RX_STR_CHECK_EQUAL_NO_CASE_MSG(S1, S2, MSG)                        \
        RX__STR_TEST_DEFINE(                                                   \
            RX_NONFATAL, RX__OP_EQUAL, RX__STR_CASE_IGNORE, S1, S2, MSG,       \
            NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#endif

#define RX_STR_CHECK_EQUAL_NO_CASE_MSG_1(S1, S2, MSG,                          \
                                         _0)                                   \
        RX__STR_TEST_DEFINE(                                                   \
            RX_NONFATAL, RX__OP_EQUAL, RX__STR_CASE_IGNORE, S1, S2, MSG,       \
            _0, NULL, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_STR_CHECK_EQUAL_NO_CASE_MSG_2(S1, S2, MSG,                          \
                                         _0, _1)                               \
        RX__STR_TEST_DEFINE(                                                   \
            RX_NONFATAL, RX__OP_EQUAL, RX__STR_CASE_IGNORE, S1, S2, MSG,       \
            _0, _1, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_STR_CHECK_EQUAL_NO_CASE_MSG_3(S1, S2, MSG,                          \
                                         _0, _1, _2)                           \
        RX__STR_TEST_DEFINE(                                                   \
            RX_NONFATAL, RX__OP_EQUAL, RX__STR_CASE_IGNORE, S1, S2, MSG,       \
            _0, _1, _2, NULL, NULL, NULL, NULL, NULL)

#define RX_STR_CHECK_EQUAL_NO_CASE_MSG_4(S1, S2, MSG,                          \
                                         _0, _1, _2, _3)                       \
        RX__STR_TEST_DEFINE(                                                   \
            RX_NONFATAL, RX__OP_EQUAL, RX__STR_CASE_IGNORE, S1, S2, MSG,       \
            _0, _1, _2, _3, NULL, NULL, NULL, NULL)

#define RX_STR_CHECK_EQUAL_NO_CASE_MSG_5(S1, S2, MSG,                          \
                                         _0, _1, _2, _3, _4)                   \
        RX__STR_TEST_DEFINE(                                                   \
            RX_NONFATAL, RX__OP_EQUAL, RX__STR_CASE_IGNORE, S1, S2, MSG,       \
            _0, _1, _2, _3, _4, NULL, NULL, NULL)

#define RX_STR_CHECK_EQUAL_NO_CASE_MSG_6(S1, S2, MSG,                          \
                                         _0, _1, _2, _3, _4, _5)               \
        RX__STR_TEST_DEFINE(                                                   \
            RX_NONFATAL, RX__OP_EQUAL, RX__STR_CASE_IGNORE, S1, S2, MSG,       \
            _0, _1, _2, _3, _4, _5, NULL, NULL)

#define RX_STR_CHECK_EQUAL_NO_CASE_MSG_7(S1, S2, MSG,                          \
                                         _0, _1, _2, _3, _4, _5, _6)           \
        RX__STR_TEST_DEFINE(                                                   \
            RX_NONFATAL, RX__OP_EQUAL, RX__STR_CASE_IGNORE, S1, S2, MSG,       \
            _0, _1, _2, _3, _4, _5, _6, NULL)

#define RX_STR_CHECK_EQUAL_NO_CASE_MSG_8(S1, S2, MSG,                          \
                                         _0, _1, _2, _3, _4, _5, _6, _7)       \
        RX__STR_TEST_DEFINE(                                                   \
            RX_NONFATAL, RX__OP_EQUAL, RX__STR_CASE_IGNORE, S1, S2, MSG,       \
            _0, _1, _2, _3, _4, _5, _6, _7)

#define RX_STR_REQUIRE_NOT_EQUAL_NO_CASE(S1, S2)                               \
    RX_STR_REQUIRE_NOT_EQUAL_NO_CASE_MSG(S1, S2, NULL)

#if RX__HAS_VARIADIC_MACROS
    #define RX_STR_REQUIRE_NOT_EQUAL_NO_CASE_MSG(S1, S2, ...)                  \
        RX__STR_TEST_DEFINE(                                                   \
            RX_FATAL, RX__OP_NOT_EQUAL, RX__STR_CASE_IGNORE, S1, S2,           \
            __VA_ARGS__)
#else
    #define RX_STR_REQUIRE_NOT_EQUAL_NO_CASE_MSG(S1, S2, MSG)                  \
        RX__STR_TEST_DEFINE(                                                   \
            RX_FATAL, RX__OP_NOT_EQUAL, RX__STR_CASE_IGNORE, S1, S2, MSG,      \
            NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#endif

#define RX_STR_REQUIRE_NOT_EQUAL_NO_CASE_MSG_1(S1, S2, MSG,                    \
                                               _0)                             \
        RX__STR_TEST_DEFINE(                                                   \
            RX_FATAL, RX__OP_NOT_EQUAL, RX__STR_CASE_IGNORE, S1, S2, MSG,      \
            _0, NULL, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_STR_REQUIRE_NOT_EQUAL_NO_CASE_MSG_2(S1, S2, MSG,                    \
                                               _0, _1)                         \
        RX__STR_TEST_DEFINE(                                                   \
            RX_FATAL, RX__OP_NOT_EQUAL, RX__STR_CASE_IGNORE, S1, S2, MSG,      \
            _0, _1, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_STR_REQUIRE_NOT_EQUAL_NO_CASE_MSG_3(S1, S2, MSG,                    \
                                               _0, _1, _2)                     \
        RX__STR_TEST_DEFINE(                                                   \
            RX_FATAL, RX__OP_NOT_EQUAL, RX__STR_CASE_IGNORE, S1, S2, MSG,      \
            _0, _1, _2, NULL, NULL, NULL, NULL, NULL)

#define RX_STR_REQUIRE_NOT_EQUAL_NO_CASE_MSG_4(S1, S2, MSG,                    \
                                               _0, _1, _2, _3)                 \
        RX__STR_TEST_DEFINE(                                                   \
            RX_FATAL, RX__OP_NOT_EQUAL, RX__STR_CASE_IGNORE, S1, S2, MSG,      \
            _0, _1, _2, _3, NULL, NULL, NULL, NULL)

#define RX_STR_REQUIRE_NOT_EQUAL_NO_CASE_MSG_5(S1, S2, MSG,                    \
                                               _0, _1, _2, _3, _4)             \
        RX__STR_TEST_DEFINE(                                                   \
            RX_FATAL, RX__OP_NOT_EQUAL, RX__STR_CASE_IGNORE, S1, S2, MSG,      \
            _0, _1, _2, _3, _4, NULL, NULL, NULL)

#define RX_STR_REQUIRE_NOT_EQUAL_NO_CASE_MSG_6(S1, S2, MSG,                    \
                                               _0, _1, _2, _3, _4, _5)         \
        RX__STR_TEST_DEFINE(                                                   \
            RX_FATAL, RX__OP_NOT_EQUAL, RX__STR_CASE_IGNORE, S1, S2, MSG,      \
            _0, _1, _2, _3, _4, _5, NULL, NULL)

#define RX_STR_REQUIRE_NOT_EQUAL_NO_CASE_MSG_7(S1, S2, MSG,                    \
                                               _0, _1, _2, _3, _4, _5, _6)     \
        RX__STR_TEST_DEFINE(                                                   \
            RX_FATAL, RX__OP_NOT_EQUAL, RX__STR_CASE_IGNORE, S1, S2, MSG,      \
            _0, _1, _2, _3, _4, _5, _6, NULL)

#define RX_STR_REQUIRE_NOT_EQUAL_NO_CASE_MSG_8(S1, S2, MSG,                    \
                                               _0, _1, _2, _3, _4, _5, _6, _7) \
        RX__STR_TEST_DEFINE(                                                   \
            RX_FATAL, RX__OP_NOT_EQUAL, RX__STR_CASE_IGNORE, S1, S2, MSG,      \
            _0, _1, _2, _3, _4, _5, _6, _7)

#define RX_STR_CHECK_NOT_EQUAL_NO_CASE(S1, S2)                                 \
    RX_STR_CHECK_NOT_EQUAL_NO_CASE_MSG(S1, S2, NULL)

#if RX__HAS_VARIADIC_MACROS
    #define RX_STR_CHECK_NOT_EQUAL_NO_CASE_MSG(S1, S2, ...)                    \
        RX__STR_TEST_DEFINE(                                                   \
            RX_NONFATAL, RX__OP_NOT_EQUAL, RX__STR_CASE_IGNORE, S1, S2,        \
            __VA_ARGS__)
#else
    #define RX_STR_CHECK_NOT_EQUAL_NO_CASE_MSG(S1, S2, MSG)                    \
        RX__STR_TEST_DEFINE(                                                   \
            RX_NONFATAL, RX__OP_NOT_EQUAL, RX__STR_CASE_IGNORE, S1, S2, MSG,   \
            NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#endif

#define RX_STR_CHECK_NOT_EQUAL_NO_CASE_MSG_1(S1, S2, MSG,                      \
                                             _0)                               \
        RX__STR_TEST_DEFINE(                                                   \
            RX_NONFATAL, RX__OP_NOT_EQUAL, RX__STR_CASE_IGNORE, S1, S2, MSG,   \
            _0, NULL, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_STR_CHECK_NOT_EQUAL_NO_CASE_MSG_2(S1, S2, MSG,                      \
                                             _0, _1)                           \
        RX__STR_TEST_DEFINE(                                                   \
            RX_NONFATAL, RX__OP_NOT_EQUAL, RX__STR_CASE_IGNORE, S1, S2, MSG,   \
            _0, _1, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_STR_CHECK_NOT_EQUAL_NO_CASE_MSG_3(S1, S2, MSG,                      \
                                             _0, _1, _2)                       \
        RX__STR_TEST_DEFINE(                                                   \
            RX_NONFATAL, RX__OP_NOT_EQUAL, RX__STR_CASE_IGNORE, S1, S2, MSG,   \
            _0, _1, _2, NULL, NULL, NULL, NULL, NULL)

#define RX_STR_CHECK_NOT_EQUAL_NO_CASE_MSG_4(S1, S2, MSG,                      \
                                             _0, _1, _2, _3)                   \
        RX__STR_TEST_DEFINE(                                                   \
            RX_NONFATAL, RX__OP_NOT_EQUAL, RX__STR_CASE_IGNORE, S1, S2, MSG,   \
            _0, _1, _2, _3, NULL, NULL, NULL, NULL)

#define RX_STR_CHECK_NOT_EQUAL_NO_CASE_MSG_5(S1, S2, MSG,                      \
                                             _0, _1, _2, _3, _4)               \
        RX__STR_TEST_DEFINE(                                                   \
            RX_NONFATAL, RX__OP_NOT_EQUAL, RX__STR_CASE_IGNORE, S1, S2, MSG,   \
            _0, _1, _2, _3, _4, NULL, NULL, NULL)

#define RX_STR_CHECK_NOT_EQUAL_NO_CASE_MSG_6(S1, S2, MSG,                      \
                                             _0, _1, _2, _3, _4, _5)           \
        RX__STR_TEST_DEFINE(                                                   \
            RX_NONFATAL, RX__OP_NOT_EQUAL, RX__STR_CASE_IGNORE, S1, S2, MSG,   \
            _0, _1, _2, _3, _4, _5, NULL, NULL)

#define RX_STR_CHECK_NOT_EQUAL_NO_CASE_MSG_7(S1, S2, MSG,                      \
                                             _0, _1, _2, _3, _4, _5, _6)       \
        RX__STR_TEST_DEFINE(                                                   \
            RX_NONFATAL, RX__OP_NOT_EQUAL, RX__STR_CASE_IGNORE, S1, S2, MSG,   \
            _0, _1, _2, _3, _4, _5, _6, NULL)

#define RX_STR_CHECK_NOT_EQUAL_NO_CASE_MSG_8(S1, S2, MSG,                      \
                                             _0, _1, _2, _3, _4, _5, _6, _7)   \
        RX__STR_TEST_DEFINE(                                                   \
            RX_NONFATAL, RX__OP_NOT_EQUAL, RX__STR_CASE_IGNORE, S1, S2, MSG,   \
            _0, _1, _2, _3, _4, _5, _6, _7)

/* Pointer Assertions                                              O-(''Q)
   -------------------------------------------------------------------------- */

#if RX__HAS_VARIADIC_MACROS
    #define RX__PTR_DEFINE_TEST(SEVERITY, OP, X1, X2, ...)                     \
        rx__ptr_assess_comparison(RX_PARAM_CONTEXT,                            \
                                  (X1),                                        \
                                  (X2),                                        \
                                  OP,                                          \
                                  #X1,                                         \
                                  #X2,                                         \
                                  __FILE__,                                    \
                                  __LINE__,                                    \
                                  SEVERITY,                                    \
                                  __VA_ARGS__)
#else
    #define RX__PTR_DEFINE_TEST(SEVERITY, OP, X1, X2, MSG,                     \
                                _0, _1, _2, _3, _4, _5, _6, _7)                \
        rx__ptr_assess_comparison(RX_PARAM_CONTEXT,                            \
                                  (X1),                                        \
                                  (X2),                                        \
                                  OP,                                          \
                                  #X1,                                         \
                                  #X2,                                         \
                                  __FILE__,                                    \
                                  __LINE__,                                    \
                                  SEVERITY,                                    \
                                  MSG,                                         \
                                  _0, _1, _2, _3, _4, _5, _6, _7)
#endif

#define RX_PTR_REQUIRE_EQUAL(X1, X2)                                           \
    RX_PTR_REQUIRE_EQUAL_MSG(X1, X2, NULL)

#if RX__HAS_VARIADIC_MACROS
    #define RX_PTR_REQUIRE_EQUAL_MSG(X1, X2, ...)                              \
        RX__PTR_DEFINE_TEST(RX_FATAL, RX__OP_EQUAL, X1, X2, __VA_ARGS__)
#else
    #define RX_PTR_REQUIRE_EQUAL_MSG(X1, X2, MSG)                              \
        RX__PTR_DEFINE_TEST(RX_FATAL, RX__OP_EQUAL, X1, X2, MSG,               \
                            NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#endif

#define RX_PTR_REQUIRE_EQUAL_MSG_1(X1, X2, MSG,                                \
                                    _0)                                        \
    RX__PTR_DEFINE_TEST(RX_FATAL, RX__OP_EQUAL, X1, X2, MSG,                   \
                        _0, NULL, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_PTR_REQUIRE_EQUAL_MSG_2(X1, X2, MSG,                                \
                                   _0, _1)                                     \
    RX__PTR_DEFINE_TEST(RX_FATAL, RX__OP_EQUAL, X1, X2, MSG,                   \
                        _0, _1, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_PTR_REQUIRE_EQUAL_MSG_3(X1, X2, MSG,                                \
                                   _0, _1, _2)                                 \
    RX__PTR_DEFINE_TEST(RX_FATAL, RX__OP_EQUAL, X1, X2, MSG,                   \
                        _0, _1, _2, NULL, NULL, NULL, NULL, NULL)

#define RX_PTR_REQUIRE_EQUAL_MSG_4(X1, X2, MSG,                                \
                                   _0, _1, _2, _3)                             \
    RX__PTR_DEFINE_TEST(RX_FATAL, RX__OP_EQUAL, X1, X2, MSG,                   \
                        _0, _1, _2, _3, NULL, NULL, NULL, NULL)

#define RX_PTR_REQUIRE_EQUAL_MSG_5(X1, X2, MSG,                                \
                                   _0, _1, _2, _3, _4)                         \
    RX__PTR_DEFINE_TEST(RX_FATAL, RX__OP_EQUAL, X1, X2, MSG,                   \
                        _0, _1, _2, _3, _4, NULL, NULL, NULL)

#define RX_PTR_REQUIRE_EQUAL_MSG_6(X1, X2, MSG,                                \
                                   _0, _1, _2, _3, _4, _5)                     \
    RX__PTR_DEFINE_TEST(RX_FATAL, RX__OP_EQUAL, X1, X2, MSG,                   \
                        _0, _1, _2, _3, _4, _5, NULL, NULL)

#define RX_PTR_REQUIRE_EQUAL_MSG_7(X1, X2, MSG,                                \
                                   _0, _1, _2, _3, _4, _5, _6)                 \
    RX__PTR_DEFINE_TEST(RX_FATAL, RX__OP_EQUAL, X1, X2, MSG,                   \
                        _0, _1, _2, _3, _4, _5, _6, NULL)

#define RX_PTR_REQUIRE_EQUAL_MSG_8(X1, X2, MSG,                                \
                                   _0, _1, _2, _3, _4, _5, _6, _7)             \
    RX__PTR_DEFINE_TEST(RX_FATAL, RX__OP_EQUAL, X1, X2, MSG,                   \
                        _0, _1, _2, _3, _4, _5, _6, _7)

#define RX_PTR_CHECK_EQUAL(X1, X2)                                             \
    RX_PTR_CHECK_EQUAL_MSG(X1, X2, NULL)

#if RX__HAS_VARIADIC_MACROS
    #define RX_PTR_CHECK_EQUAL_MSG(X1, X2, ...)                                \
        RX__PTR_DEFINE_TEST(RX_NONFATAL, RX__OP_EQUAL, X1, X2, __VA_ARGS__)
#else
    #define RX_PTR_CHECK_EQUAL_MSG(X1, X2, MSG)                                \
        RX__PTR_DEFINE_TEST(RX_NONFATAL, RX__OP_EQUAL, X1, X2, MSG,            \
                            NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#endif

#define RX_PTR_CHECK_EQUAL_MSG_1(X1, X2, MSG,                                  \
                                 _0)                                           \
    RX__PTR_DEFINE_TEST(RX_NONFATAL, RX__OP_EQUAL, X1, X2, MSG,                \
                        _0, NULL, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_PTR_CHECK_EQUAL_MSG_2(X1, X2, MSG,                                  \
                                 _0, _1)                                       \
    RX__PTR_DEFINE_TEST(RX_NONFATAL, RX__OP_EQUAL, X1, X2, MSG,                \
                        _0, _1, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_PTR_CHECK_EQUAL_MSG_3(X1, X2, MSG,                                  \
                                 _0, _1, _2)                                   \
    RX__PTR_DEFINE_TEST(RX_NONFATAL, RX__OP_EQUAL, X1, X2, MSG,                \
                        _0, _1, _2, NULL, NULL, NULL, NULL, NULL)

#define RX_PTR_CHECK_EQUAL_MSG_4(X1, X2, MSG,                                  \
                                 _0, _1, _2, _3)                               \
    RX__PTR_DEFINE_TEST(RX_NONFATAL, RX__OP_EQUAL, X1, X2, MSG,                \
                        _0, _1, _2, _3, NULL, NULL, NULL, NULL)

#define RX_PTR_CHECK_EQUAL_MSG_5(X1, X2, MSG,                                  \
                                 _0, _1, _2, _3, _4)                           \
    RX__PTR_DEFINE_TEST(RX_NONFATAL, RX__OP_EQUAL, X1, X2, MSG,                \
                        _0, _1, _2, _3, _4, NULL, NULL, NULL)

#define RX_PTR_CHECK_EQUAL_MSG_6(X1, X2, MSG,                                  \
                                 _0, _1, _2, _3, _4, _5)                       \
    RX__PTR_DEFINE_TEST(RX_NONFATAL, RX__OP_EQUAL, X1, X2, MSG,                \
                        _0, _1, _2, _3, _4, _5, NULL, NULL)

#define RX_PTR_CHECK_EQUAL_MSG_7(X1, X2, MSG,                                  \
                                 _0, _1, _2, _3, _4, _5, _6)                   \
    RX__PTR_DEFINE_TEST(RX_NONFATAL, RX__OP_EQUAL, X1, X2, MSG,                \
                        _0, _1, _2, _3, _4, _5, _6, NULL)

#define RX_PTR_CHECK_EQUAL_MSG_8(X1, X2, MSG,                                  \
                                 _0, _1, _2, _3, _4, _5, _6, _7)               \
    RX__PTR_DEFINE_TEST(RX_NONFATAL, RX__OP_EQUAL, X1, X2, MSG,                \
                        _0, _1, _2, _3, _4, _5, _6, _7)

#define RX_PTR_REQUIRE_NOT_EQUAL(X1, X2)                                       \
    RX_PTR_REQUIRE_NOT_EQUAL_MSG(X1, X2, NULL)

#if RX__HAS_VARIADIC_MACROS
    #define RX_PTR_REQUIRE_NOT_EQUAL_MSG(X1, X2, ...)                          \
        RX__PTR_DEFINE_TEST(RX_FATAL, RX__OP_NOT_EQUAL, X1, X2, __VA_ARGS__)
#else
    #define RX_PTR_REQUIRE_NOT_EQUAL_MSG(X1, X2, MSG)                          \
        RX__PTR_DEFINE_TEST(RX_FATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,           \
                            NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#endif

#define RX_PTR_REQUIRE_NOT_EQUAL_MSG_1(X1, X2, MSG,                            \
                                       _0)                                     \
    RX__PTR_DEFINE_TEST(RX_FATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,               \
                        _0, NULL, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_PTR_REQUIRE_NOT_EQUAL_MSG_2(X1, X2, MSG,                            \
                                       _0, _1)                                 \
    RX__PTR_DEFINE_TEST(RX_FATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,               \
                        _0, _1, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_PTR_REQUIRE_NOT_EQUAL_MSG_3(X1, X2, MSG,                            \
                                       _0, _1, _2)                             \
    RX__PTR_DEFINE_TEST(RX_FATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,               \
                        _0, _1, _2, NULL, NULL, NULL, NULL, NULL)

#define RX_PTR_REQUIRE_NOT_EQUAL_MSG_4(X1, X2, MSG,                            \
                                       _0, _1, _2, _3)                         \
    RX__PTR_DEFINE_TEST(RX_FATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,               \
                        _0, _1, _2, _3, NULL, NULL, NULL, NULL)

#define RX_PTR_REQUIRE_NOT_EQUAL_MSG_5(X1, X2, MSG,                            \
                                       _0, _1, _2, _3, _4)                     \
    RX__PTR_DEFINE_TEST(RX_FATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,               \
                        _0, _1, _2, _3, _4, NULL, NULL, NULL)

#define RX_PTR_REQUIRE_NOT_EQUAL_MSG_6(X1, X2, MSG,                            \
                                       _0, _1, _2, _3, _4, _5)                 \
    RX__PTR_DEFINE_TEST(RX_FATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,               \
                        _0, _1, _2, _3, _4, _5, NULL, NULL)

#define RX_PTR_REQUIRE_NOT_EQUAL_MSG_7(X1, X2, MSG,                            \
                                       _0, _1, _2, _3, _4, _5, _6)             \
    RX__PTR_DEFINE_TEST(RX_FATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,               \
                        _0, _1, _2, _3, _4, _5, _6, NULL)

#define RX_PTR_REQUIRE_NOT_EQUAL_MSG_8(X1, X2, MSG,                            \
                                       _0, _1, _2, _3, _4, _5, _6, _7)         \
    RX__PTR_DEFINE_TEST(RX_FATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,               \
                        _0, _1, _2, _3, _4, _5, _6, _7)

#define RX_PTR_CHECK_NOT_EQUAL(X1, X2)                                         \
    RX_PTR_CHECK_NOT_EQUAL_MSG(X1, X2, NULL)

#if RX__HAS_VARIADIC_MACROS
    #define RX_PTR_CHECK_NOT_EQUAL_MSG(X1, X2, ...)                            \
        RX__PTR_DEFINE_TEST(RX_NONFATAL, RX__OP_NOT_EQUAL, X1, X2, __VA_ARGS__)
#else
    #define RX_PTR_CHECK_NOT_EQUAL_MSG(X1, X2, MSG)                            \
        RX__PTR_DEFINE_TEST(RX_NONFATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,        \
                            NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#endif

#define RX_PTR_CHECK_NOT_EQUAL_MSG_1(X1, X2, MSG,                              \
                                     _0)                                       \
    RX__PTR_DEFINE_TEST(RX_NONFATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,            \
                        _0, NULL, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_PTR_CHECK_NOT_EQUAL_MSG_2(X1, X2, MSG,                              \
                                     _0, _1)                                   \
    RX__PTR_DEFINE_TEST(RX_NONFATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,            \
                        _0, _1, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_PTR_CHECK_NOT_EQUAL_MSG_3(X1, X2, MSG,                              \
                                     _0, _1, _2)                               \
    RX__PTR_DEFINE_TEST(RX_NONFATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,            \
                        _0, _1, _2, NULL, NULL, NULL, NULL, NULL)

#define RX_PTR_CHECK_NOT_EQUAL_MSG_4(X1, X2, MSG,                              \
                                     _0, _1, _2, _3)                           \
    RX__PTR_DEFINE_TEST(RX_NONFATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,            \
                        _0, _1, _2, _3, NULL, NULL, NULL, NULL)

#define RX_PTR_CHECK_NOT_EQUAL_MSG_5(X1, X2, MSG,                              \
                                     _0, _1, _2, _3, _4)                       \
    RX__PTR_DEFINE_TEST(RX_NONFATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,            \
                        _0, _1, _2, _3, _4, NULL, NULL, NULL)

#define RX_PTR_CHECK_NOT_EQUAL_MSG_6(X1, X2, MSG,                              \
                                     _0, _1, _2, _3, _4, _5)                   \
    RX__PTR_DEFINE_TEST(RX_NONFATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,            \
                        _0, _1, _2, _3, _4, _5, NULL, NULL)

#define RX_PTR_CHECK_NOT_EQUAL_MSG_7(X1, X2, MSG,                              \
                                     _0, _1, _2, _3, _4, _5, _6)               \
    RX__PTR_DEFINE_TEST(RX_NONFATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,            \
                        _0, _1, _2, _3, _4, _5, _6, NULL)

#define RX_PTR_CHECK_NOT_EQUAL_MSG_8(X1, X2, MSG,                              \
                                     _0, _1, _2, _3, _4, _5, _6, _7)           \
    RX__PTR_DEFINE_TEST(RX_NONFATAL, RX__OP_NOT_EQUAL, X1, X2, MSG,            \
                        _0, _1, _2, _3, _4, _5, _6, _7)

#if RX__HAS_VARIADIC_MACROS
    #define RX__PTR_DEFINE_ALIGNMENT_TEST(SEVERITY, X, ALIGNMENT, ...)         \
        rx__ptr_assess_alignment(RX_PARAM_CONTEXT,                             \
                                 (X),                                          \
                                 (ALIGNMENT),                                  \
                                 #X,                                           \
                                 __FILE__,                                     \
                                 __LINE__,                                     \
                                 SEVERITY,                                     \
                                 __VA_ARGS__)
#else
    #define RX__PTR_DEFINE_ALIGNMENT_TEST(SEVERITY, X, ALIGNMENT, MSG,         \
                                          _0, _1, _2, _3, _4, _5, _6, _7)      \
        rx__ptr_assess_alignment(RX_PARAM_CONTEXT,                             \
                                 (X),                                          \
                                 (ALIGNMENT),                                  \
                                 #X,                                           \
                                 __FILE__,                                     \
                                 __LINE__,                                     \
                                 SEVERITY,                                     \
                                 MSG,                                          \
                                 _0, _1, _2, _3, _4, _5, _6, _7)
#endif

#define RX_PTR_REQUIRE_ALIGNED(X, ALIGNMENT)                                   \
    RX_PTR_REQUIRE_ALIGNED_MSG(X, ALIGNMENT, NULL)

#if RX__HAS_VARIADIC_MACROS
    #define RX_PTR_REQUIRE_ALIGNED_MSG(X, ALIGNMENT, ...)                      \
        RX__PTR_DEFINE_ALIGNMENT_TEST(RX_FATAL, X, ALIGNMENT, __VA_ARGS__)
#else
    #define RX_PTR_REQUIRE_ALIGNED_MSG(X, ALIGNMENT, MSG)                      \
        RX__PTR_DEFINE_ALIGNMENT_TEST(                                         \
            RX_FATAL, X, ALIGNMENT, MSG,                                       \
            NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#endif

#define RX_PTR_REQUIRE_ALIGNED_MSG_1(X, ALIGNMENT, MSG,                        \
                                     _0)                                       \
    RX__PTR_DEFINE_ALIGNMENT_TEST(RX_FATAL, X, ALIGNMENT, MSG,                 \
                                  _0, NULL, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_PTR_REQUIRE_ALIGNED_MSG_2(X, ALIGNMENT, MSG,                        \
                                     _0, _1)                                   \
    RX__PTR_DEFINE_ALIGNMENT_TEST(RX_FATAL, X, ALIGNMENT, MSG,                 \
                                  _0, _1, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_PTR_REQUIRE_ALIGNED_MSG_3(X, ALIGNMENT, MSG,                        \
                                     _0, _1, _2)                               \
    RX__PTR_DEFINE_ALIGNMENT_TEST(RX_FATAL, X, ALIGNMENT, MSG,                 \
                                  _0, _1, _2, NULL, NULL, NULL, NULL, NULL)

#define RX_PTR_REQUIRE_ALIGNED_MSG_4(X, ALIGNMENT, MSG,                        \
                                     _0, _1, _2, _3)                           \
    RX__PTR_DEFINE_ALIGNMENT_TEST(RX_FATAL, X, ALIGNMENT, MSG,                 \
                                  _0, _1, _2, _3, NULL, NULL, NULL, NULL)

#define RX_PTR_REQUIRE_ALIGNED_MSG_5(X, ALIGNMENT, MSG,                        \
                                     _0, _1, _2, _3, _4)                       \
    RX__PTR_DEFINE_ALIGNMENT_TEST(RX_FATAL, X, ALIGNMENT, MSG,                 \
                                  _0, _1, _2, _3, _4, NULL, NULL, NULL)

#define RX_PTR_REQUIRE_ALIGNED_MSG_6(X, ALIGNMENT, MSG,                        \
                                     _0, _1, _2, _3, _4, _5)                   \
    RX__PTR_DEFINE_ALIGNMENT_TEST(RX_FATAL, X, ALIGNMENT, MSG,                 \
                                  _0, _1, _2, _3, _4, _5, NULL, NULL)

#define RX_PTR_REQUIRE_ALIGNED_MSG_7(X, ALIGNMENT, MSG,                        \
                                     _0, _1, _2, _3, _4, _5, _6)               \
    RX__PTR_DEFINE_ALIGNMENT_TEST(RX_FATAL, X, ALIGNMENT, MSG,                 \
                                  _0, _1, _2, _3, _4, _5, _6, NULL)

#define RX_PTR_REQUIRE_ALIGNED_MSG_8(X, ALIGNMENT, MSG,                        \
                                     _0, _1, _2, _3, _4, _5, _6, _7)           \
    RX__PTR_DEFINE_ALIGNMENT_TEST(RX_FATAL, X, ALIGNMENT, MSG,                 \
                                  _0, _1, _2, _3, _4, _5, _6, _7)

#define RX_PTR_CHECK_ALIGNED(X, ALIGNMENT)                                     \
    RX_PTR_CHECK_ALIGNED_MSG(X, ALIGNMENT, NULL)

#if RX__HAS_VARIADIC_MACROS
    #define RX_PTR_CHECK_ALIGNED_MSG(X, ALIGNMENT, ...)                        \
        RX__PTR_DEFINE_ALIGNMENT_TEST(RX_NONFATAL, X, ALIGNMENT, __VA_ARGS__)
#else
    #define RX_PTR_CHECK_ALIGNED_MSG(X, ALIGNMENT, MSG)                        \
        RX__PTR_DEFINE_ALIGNMENT_TEST(                                         \
            RX_NONFATAL, X, ALIGNMENT, MSG,                                    \
            NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#endif

#define RX_PTR_CHECK_ALIGNED_MSG_1(X, ALIGNMENT, MSG,                          \
                                   _0)                                         \
    RX__PTR_DEFINE_ALIGNMENT_TEST(RX_NONFATAL, X, ALIGNMENT, MSG,              \
                                  _0, NULL, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_PTR_CHECK_ALIGNED_MSG_2(X, ALIGNMENT, MSG,                          \
                                   _0, _1)                                     \
    RX__PTR_DEFINE_ALIGNMENT_TEST(RX_NONFATAL, X, ALIGNMENT, MSG,              \
                                  _0, _1, NULL, NULL, NULL, NULL, NULL, NULL)

#define RX_PTR_CHECK_ALIGNED_MSG_3(X, ALIGNMENT, MSG,                          \
                                   _0, _1, _2)                                 \
    RX__PTR_DEFINE_ALIGNMENT_TEST(RX_NONFATAL, X, ALIGNMENT, MSG,              \
                                  _0, _1, _2, NULL, NULL, NULL, NULL, NULL)

#define RX_PTR_CHECK_ALIGNED_MSG_4(X, ALIGNMENT, MSG,                          \
                                   _0, _1, _2, _3)                             \
    RX__PTR_DEFINE_ALIGNMENT_TEST(RX_NONFATAL, X, ALIGNMENT, MSG,              \
                                  _0, _1, _2, _3, NULL, NULL, NULL, NULL)

#define RX_PTR_CHECK_ALIGNED_MSG_5(X, ALIGNMENT, MSG,                          \
                                   _0, _1, _2, _3, _4)                         \
    RX__PTR_DEFINE_ALIGNMENT_TEST(RX_NONFATAL, X, ALIGNMENT, MSG,              \
                                  _0, _1, _2, _3, _4, NULL, NULL, NULL)

#define RX_PTR_CHECK_ALIGNED_MSG_6(X, ALIGNMENT, MSG,                          \
                                   _0, _1, _2, _3, _4, _5)                     \
    RX__PTR_DEFINE_ALIGNMENT_TEST(RX_NONFATAL, X, ALIGNMENT, MSG,              \
                                  _0, _1, _2, _3, _4, _5, NULL, NULL)

#define RX_PTR_CHECK_ALIGNED_MSG_7(X, ALIGNMENT, MSG,                          \
                                   _0, _1, _2, _3, _4, _5, _6)                 \
    RX__PTR_DEFINE_ALIGNMENT_TEST(RX_NONFATAL, X, ALIGNMENT, MSG,              \
                                  _0, _1, _2, _3, _4, _5, _6, NULL)

#define RX_PTR_CHECK_ALIGNED_MSG_8(X, ALIGNMENT, MSG,                          \
                                   _0, _1, _2, _3, _4, _5, _6, _7)             \
    RX__PTR_DEFINE_ALIGNMENT_TEST(RX_NONFATAL, X, ALIGNMENT, MSG,              \
                                  _0, _1, _2, _3, _4, _5, _6, _7)

/* Implementation: Helpers                                         O-(''Q)
   -------------------------------------------------------------------------- */

#if defined(RX_ENABLE_DEBUGGING)                                               \
    || (!defined(RX_DISABLE_DEBUGGING) && (defined(DEBUG) || !defined(NDEBUG)))
    #define RX__DEBUGGING 1
#else
    #define RX__DEBUGGING 0
#endif

#if defined(_WIN32)
    #define RX__PLATFORM_WINDOWS
#elif defined(__unix__) || defined(__APPLE__)
    #define RX__PLATFORM_UNIX
    #if defined(__APPLE__)
        #define RX__PLATFORM_DARWIN
    #elif defined(__linux__)
        #define RX__PLATFORM_LINUX
    #endif
#endif

#include <ctype.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#ifndef RX_ASSERT
    #include <assert.h>
    #define RX_ASSERT assert
#endif

#ifndef RX_MALLOC
    #include <stdlib.h>
    #define RX_MALLOC malloc
#endif

#ifndef RX_REALLOC
    #include <stdlib.h>
    #define RX_REALLOC realloc
#endif

#ifndef RX_FREE
    #include <stdlib.h>
    #define RX_FREE free
#endif

#if defined(RX__PLATFORM_WINDOWS)
    #include <io.h>
    #define RX__ISATTY _isatty
    #define RX__FILENO _fileno
#else
    #include <unistd.h>
    #define RX__ISATTY isatty
    #define RX__FILENO fileno
#endif

typedef char rx__invalid_size_type[sizeof(rx_size) == sizeof(size_t) ? 1 : -1];

#define RX__UNUSED(X) (void)(X)

#define RX__REQUIRE_SEMICOLON void rx__dummy(void)

#define RX__STRINGIFY(X) #X

#define RX__EXPAND(X) X

#define RX__CONCAT_(A, B) A##B
#define RX__CONCAT(A, B) RX__CONCAT_(A, B)

#define RX__STRUCT_SET_MEMBER(X) (*obj) X;

#define RX__STRUCT_UPDATE_0()

#define RX__STRUCT_UPDATE_1(_0)                                                \
    RX__STRUCT_SET_MEMBER(_0)

#define RX__STRUCT_UPDATE_2(_0, _1)                                            \
    RX__STRUCT_SET_MEMBER(_0)                                                  \
    RX__STRUCT_UPDATE_1(_1)

#define RX__STRUCT_UPDATE_3(_0, _1, _2)                                        \
    RX__STRUCT_SET_MEMBER(_0)                                                  \
    RX__STRUCT_UPDATE_2(_1, _2)

#define RX__STRUCT_UPDATE_4(_0, _1, _2, _3)                                    \
    RX__STRUCT_SET_MEMBER(_0)                                                  \
    RX__STRUCT_UPDATE_3(_1, _2, _3)

#define RX__STRUCT_UPDATE_5(_0, _1, _2, _3, _4)                                \
    RX__STRUCT_SET_MEMBER(_0)                                                  \
    RX__STRUCT_UPDATE_4(_1, _2, _3, _4)

#define RX__STRUCT_UPDATE_6(_0, _1, _2, _3, _4, _5)                            \
    RX__STRUCT_SET_MEMBER(_0)                                                  \
    RX__STRUCT_UPDATE_5(_1, _2, _3, _4, _5)

#define RX__STRUCT_UPDATE_7(_0, _1, _2, _3, _4, _5, _6)                        \
    RX__STRUCT_SET_MEMBER(_0)                                                  \
    RX__STRUCT_UPDATE_6(_1, _2, _3, _4, _5, _6)

#define RX__STRUCT_UPDATE_8(_0, _1, _2, _3, _4, _5, _6, _7)                    \
    RX__STRUCT_SET_MEMBER(_0)                                                  \
    RX__STRUCT_UPDATE_7(_1, _2, _3, _4, _5, _6, _7)

#define RX__STRUCT_DEFINE_UPDATE_FN(ID, TYPE, ARG_COUNT, ARGS)                 \
    static void                                                                \
    ID(TYPE *obj)                                                              \
    {                                                                          \
        RX__UNUSED(obj);                                                       \
        RX__EXPAND(RX__CONCAT(RX__STRUCT_UPDATE_, ARG_COUNT) ARGS)             \
    }

#if RX__HAS_VARIADIC_MACROS
    #define RX__ARG(                                                           \
        _0, _1, _2, _3, _4, _5, _6, _7,                                        \
        _8, ...) _8

    #define RX__HAS_AT_LEAST_2_ARGS(...)                                       \
        RX__EXPAND(RX__ARG(                                                    \
            __VA_ARGS__,                                                       \
            1, 1, 1, 1, 1, 1, 1, 0,))

    #define RX__HAS_AT_LEAST_3_ARGS(...)                                       \
        RX__EXPAND(RX__ARG(                                                    \
            __VA_ARGS__,                                                       \
            1, 1, 1, 1, 1, 1, 0, 0,))

    #define RX__COUNT_ARGS(...)                                                \
        RX__EXPAND(RX__ARG(                                                    \
            __VA_ARGS__,                                                       \
             8,  7,  6,  5,  4,  3,  2,  1,                                    \
             0,))
#endif

#define RX__FIXTURE_DESC_GET_ID(ID)                                            \
    rx__fixture_desc_##ID
#define RX__FIXTURE_GET_UPDATE_FN_ID(ID)                                       \
    rx__fixture_update_fn_##ID

#define RX__TEST_SUITE_DESC_GET_ID(ID)                                         \
    rx__test_suite_desc_##ID
#define RX__TEST_SUITE_DESC_PTR_GET_ID(ID)                                     \
    rx__test_suite_desc_ptr_##ID

#define RX__TEST_CASE_DESC_GET_ID(SUITE_ID, ID)                                \
    rx__test_case_desc_##SUITE_ID##_##ID
#define RX__TEST_CASE_DESC_PTR_GET_ID(SUITE_ID, ID)                            \
    rx__test_case_desc_ptr_##SUITE_ID##_##ID

#define RX__TEST_CASE_CONFIG_DESC_GET_ID(ID)                                   \
    rx__test_case_config_desc_##ID
#define RX__TEST_CASE_CONFIG_BLUEPRINT_GET_UPDATE_FN_ID(ID)                    \
    rx__test_case_config_blueprint_update_fn_##ID

typedef intmax_t rx__int;
typedef uintmax_t rx__uint;
typedef long double rx__real;

struct rx_context {
    jmp_buf env;
    struct rx_summary *summary;
};

/* Implementation: Logger                                          O-(''Q)
   -------------------------------------------------------------------------- */

#if !defined(RX_DISABLE_LOG_STYLING)                                           \
    && defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 1
    #define RX__LOG_STYLING 1
#else
    #define RX__LOG_STYLING 0
#endif

#if defined(RX_SET_LOGGING_LEVEL_ALL)
    #define RX__LOGGING_LEVEL RX_LOG_LEVEL_ALL
#elif defined(RX_SET_LOGGING_LEVEL_DEBUG)
    #define RX__LOGGING_LEVEL RX_LOG_LEVEL_DEBUG
#elif defined(RX_SET_LOGGING_LEVEL_INFO)
    #define RX__LOGGING_LEVEL RX_LOG_LEVEL_INFO
#elif defined(RX_SET_LOGGING_LEVEL_WARNING)
    #define RX__LOGGING_LEVEL RX_LOG_LEVEL_WARNING
#elif defined(RX_SET_LOGGING_LEVEL_ERROR)
    #define RX__LOGGING_LEVEL RX_LOG_LEVEL_ERROR
#elif defined(RX_SET_LOGGING_LEVEL_FATAL)
    #define RX__LOGGING_LEVEL RX_LOG_LEVEL_FATAL
#elif defined(RX_SET_LOGGING_LEVEL_NONDE)
    #define RX__LOGGING_LEVEL RX_LOG_LEVEL_NONE
#elif RX__DEBUGGING
    #define RX__LOGGING_LEVEL RX_LOG_LEVEL_DEBUG
#else
    #define RX__LOGGING_LEVEL RX_LOG_LEVEL_WARNING
#endif

#if defined(RX_DISABLE_LOGGING)
    #define RX__LOGGING 0
#else
    #define RX__LOGGING 1
#endif

#ifndef RX_LOG
    #define RX_LOG rx__log
#endif

#if defined(_MSC_VER)
    #define RX__LOG(LEVEL, ARGS)                                               \
        do {                                                                   \
            _Pragma("warning(push)")                                           \
            _Pragma("warning(disable : 4127)")                                 \
            if (RX__LOGGING && (LEVEL) <= RX__LOGGING_LEVEL) {                 \
                RX_LOG ARGS;                                                   \
            }                                                                  \
            _Pragma("warning(pop)")                                            \
        } while (0)
#elif defined(__GNUC__)
    #define RX__LOG(LEVEL, ARGS)                                               \
        do {                                                                   \
            _Pragma("GCC diagnostic push")                                     \
            _Pragma("GCC diagnostic ignored \"-Wtautological-compare\"")       \
            if (RX__LOGGING && (LEVEL) <= RX__LOGGING_LEVEL) {                 \
                RX_LOG ARGS;                                                   \
            }                                                                  \
            _Pragma("GCC diagnostic pop")                                      \
        } while (0)
#else
    #define RX__LOG(LEVEL, ARGS)                                               \
        do {                                                                   \
            if (RX__LOGGING && (LEVEL) <= RX__LOGGING_LEVEL) {                 \
                RX_LOG ARGS;                                                   \
            }                                                                  \
        } while (0)
#endif

#define RX__LOG_DEBUG(MSG)                                                     \
        RX__LOG(RX_LOG_LEVEL_DEBUG,                                            \
                (RX_LOG_LEVEL_DEBUG, __FILE__, __LINE__, MSG))
#define RX__LOG_DEBUG_1(FMT, _0)                                               \
        RX__LOG(RX_LOG_LEVEL_DEBUG,                                            \
                (RX_LOG_LEVEL_DEBUG, __FILE__, __LINE__, FMT, _0))
#define RX__LOG_DEBUG_2(FMT, _0, _1)                                           \
        RX__LOG(RX_LOG_LEVEL_DEBUG,                                            \
                (RX_LOG_LEVEL_DEBUG, __FILE__, __LINE__, FMT, _0, _1))

#define RX__LOG_INFO(MSG)                                                      \
        RX__LOG(RX_LOG_LEVEL_INFO,                                             \
                (RX_LOG_LEVEL_INFO, __FILE__, __LINE__, MSG))
#define RX__LOG_INFO_1(FMT, _0)                                                \
        RX__LOG(RX_LOG_LEVEL_INFO,                                             \
                (RX_LOG_LEVEL_INFO, __FILE__, __LINE__, FMT, _0))
#define RX__LOG_INFO_2(FMT, _0, _1)                                            \
        RX__LOG(RX_LOG_LEVEL_INFO,                                             \
                (RX_LOG_LEVEL_INFO, __FILE__, __LINE__, FMT, _0, _1))

#define RX__LOG_WARNING(MSG)                                                   \
        RX__LOG(RX_LOG_LEVEL_WARNING,                                          \
                (RX_LOG_LEVEL_WARNING, __FILE__, __LINE__, MSG))
#define RX__LOG_WARNING_1(FMT, _0)                                             \
        RX__LOG(RX_LOG_LEVEL_WARNING,                                          \
                (RX_LOG_LEVEL_WARNING, __FILE__, __LINE__, FMT, _0))
#define RX__LOG_WARNING_2(FMT, _0, _1)                                         \
        RX__LOG(RX_LOG_LEVEL_WARNING,                                          \
                (RX_LOG_LEVEL_WARNING, __FILE__, __LINE__, FMT, _0, _1))

#define RX__LOG_ERROR(MSG)                                                     \
        RX__LOG(RX_LOG_LEVEL_ERROR,                                            \
                (RX_LOG_LEVEL_ERROR, __FILE__, __LINE__, MSG))
#define RX__LOG_ERROR_1(FMT, _0)                                               \
        RX__LOG(RX_LOG_LEVEL_ERROR,                                            \
                (RX_LOG_LEVEL_ERROR, __FILE__, __LINE__, FMT, _0))
#define RX__LOG_ERROR_2(FMT, _0, _1)                                           \
        RX__LOG(RX_LOG_LEVEL_ERROR,                                            \
                (RX_LOG_LEVEL_ERROR, __FILE__, __LINE__, FMT, _0, _1))

#define RX__LOG_FATAL(MSG)                                                     \
        RX__LOG(RX_LOG_LEVEL_FATAL,                                            \
                (RX_LOG_LEVEL_FATAL, __FILE__, __LINE__, MSG))
#define RX__LOG_FATAL_1(FMT, _0)                                               \
        RX__LOG(RX_LOG_LEVEL_FATAL,                                            \
                (RX_LOG_LEVEL_FATAL, __FILE__, __LINE__, FMT, _0))
#define RX__LOG_FATAL_2(FMT, _0, _1)                                           \
        RX__LOG(RX_LOG_LEVEL_FATAL,                                            \
                (RX_LOG_LEVEL_FATAL, __FILE__, __LINE__, FMT, _0, _1))

#if RX__LOG_STYLING
enum rx__log_style {
    RX__LOG_STYLE_RESET = 0,
    RX__LOG_STYLE_BLACK = 1,
    RX__LOG_STYLE_RED = 2,
    RX__LOG_STYLE_GREEN = 3,
    RX__LOG_STYLE_YELLOW = 4,
    RX__LOG_STYLE_BLUE = 5,
    RX__LOG_STYLE_MAGENTA = 6,
    RX__LOG_STYLE_CYAN = 7,
    RX__LOG_STYLE_BRIGHT_BLACK = 8,
    RX__LOG_STYLE_BRIGHT_RED = 9,
    RX__LOG_STYLE_BRIGHT_GREEN = 10,
    RX__LOG_STYLE_BRIGHT_YELLOW = 11,
    RX__LOG_STYLE_BRIGHT_BLUE = 12,
    RX__LOG_STYLE_BRIGHT_MAGENTA = 13,
    RX__LOG_STYLE_BRIGHT_CYAN = 14
};
#endif

static void
rx__log_level_get_name(const char **name, enum rx_log_level level)
{
    RX_ASSERT(name != NULL);

    switch (level) {
        case RX_LOG_LEVEL_FATAL:
            *name = "fatal";
            return;
        case RX_LOG_LEVEL_ERROR:
            *name = "error";
            return;
        case RX_LOG_LEVEL_WARNING:
            *name = "warning";
            return;
        case RX_LOG_LEVEL_INFO:
            *name = "info";
            return;
        case RX_LOG_LEVEL_DEBUG:
            *name = "debug";
            return;
        default:
            RX_ASSERT(0);
    }
}

#if RX__LOG_STYLING
static void
rx__log_level_get_style(enum rx__log_style *style, enum rx_log_level level)
{
    RX_ASSERT(style != NULL);

    switch (level) {
        case RX_LOG_LEVEL_FATAL:
            *style = RX__LOG_STYLE_BRIGHT_MAGENTA;
            return;
        case RX_LOG_LEVEL_ERROR:
            *style = RX__LOG_STYLE_BRIGHT_RED;
            return;
        case RX_LOG_LEVEL_WARNING:
            *style = RX__LOG_STYLE_BRIGHT_YELLOW;
            return;
        case RX_LOG_LEVEL_INFO:
            *style = RX__LOG_STYLE_BRIGHT_GREEN;
            return;
        case RX_LOG_LEVEL_DEBUG:
            *style = RX__LOG_STYLE_BRIGHT_CYAN;
            return;
        default:
            RX_ASSERT(0);
    };
}

static void
rx__log_style_get_ansi_code(const char **code, enum rx__log_style style)
{
    RX_ASSERT(code != NULL);

    switch (style) {
        case RX__LOG_STYLE_RESET:
            *code = "\x1b[0m";
            return;
        case RX__LOG_STYLE_BLACK:
            *code = "\x1b[30m";
            return;
        case RX__LOG_STYLE_RED:
            *code = "\x1b[31m";
            return;
        case RX__LOG_STYLE_GREEN:
            *code = "\x1b[32m";
            return;
        case RX__LOG_STYLE_YELLOW:
            *code = "\x1b[33m";
            return;
        case RX__LOG_STYLE_BLUE:
            *code = "\x1b[34m";
            return;
        case RX__LOG_STYLE_MAGENTA:
            *code = "\x1b[35m";
            return;
        case RX__LOG_STYLE_CYAN:
            *code = "\x1b[36m";
            return;
        case RX__LOG_STYLE_BRIGHT_BLACK:
            *code = "\x1b[1;30m";
            return;
        case RX__LOG_STYLE_BRIGHT_RED:
            *code = "\x1b[1;31m";
            return;
        case RX__LOG_STYLE_BRIGHT_GREEN:
            *code = "\x1b[1;32m";
            return;
        case RX__LOG_STYLE_BRIGHT_YELLOW:
            *code = "\x1b[1;33m";
            return;
        case RX__LOG_STYLE_BRIGHT_BLUE:
            *code = "\x1b[1;34m";
            return;
        case RX__LOG_STYLE_BRIGHT_MAGENTA:
            *code = "\x1b[1;35m";
            return;
        case RX__LOG_STYLE_BRIGHT_CYAN:
            *code = "\x1b[1;36m";
            return;
        default:
            RX_ASSERT(0);
    }
}
#endif /* RX__LOG_STYLING */

RX__PRINTF_CHECK(4, 5)
static void
rx__log(enum rx_log_level level,
        const char *file,
        int line,
        const char *fmt,
        ...)
{
    const char *level_name;
    const char *level_style_begin;
    const char *level_style_end;
    va_list args;

    RX_ASSERT(file != NULL);
    RX_ASSERT(fmt != NULL);

    rx__log_level_get_name(&level_name, level);

#if RX__LOG_STYLING
    if (RX__ISATTY(RX__FILENO(stderr))) {
        enum rx__log_style level_style;

        rx__log_level_get_style(&level_style, level);
        rx__log_style_get_ansi_code(&level_style_begin, level_style);
        rx__log_style_get_ansi_code(&level_style_end, RX__LOG_STYLE_RESET);
    } else {
        level_style_begin = level_style_end = "";
    }
#else
    level_style_begin = level_style_end = "";
#endif

    va_start(args, fmt);
    fprintf(stderr,
            "%s:%d: %s%s%s: ",
            file,
            line,
            level_style_begin,
            level_name,
            level_style_end);
    vfprintf(stderr, fmt, args);
    va_end(args);
}

/* Implementation: Timer                                           O-(''Q)
   -------------------------------------------------------------------------- */

#define RX__TICKS_PER_SECOND 1000000000ul
#define RX__TICKS_PER_MICROSECOND 1000ul

#if defined(RX__PLATFORM_WINDOWS)
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#elif defined(RX__PLATFORM_DARWIN)
    #include <mach/mach_time.h>
    #include <sys/resource.h>
#elif defined(RX__PLATFORM_UNIX)
    #include <sys/resource.h>
    #if defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 199309L
        #include <time.h>
        #define RX__USE_CLOCK_GETTIME
        #if defined(CLOCK_MONOTONIC_RAW)
            #define RX__CLOCK_ID CLOCK_MONOTONIC_RAW
        #elif defined(CLOCK_MONOTONIC)
            #define RX__CLOCK_ID CLOCK_MONOTONIC
        #else
            #define RX__CLOCK_ID CLOCK_REALTIME
        #endif
    #else
        #include <sys/time.h>
    #endif
#else
    typedef char rx__unsupported_platform[-1];
#endif

static enum rx_status
rx__get_real_time(uint64_t *time)
{
    RX_ASSERT(time != NULL);

#if defined(RX__PLATFORM_WINDOWS)
    {
        static double time_to_nano;
        LARGE_INTEGER counter;

        if (time_to_nano == 0.0) {
            LARGE_INTEGER frequency;

            if (!QueryPerformanceFrequency(&frequency)) {
                RX__LOG_DEBUG("failed to retrieve the timer's frequency\n");
                return RX_ERROR;
            }

            time_to_nano = (double)RX__TICKS_PER_SECOND / frequency.QuadPart;
        }

        if (!QueryPerformanceCounter(&counter)) {
            RX__LOG_DEBUG("failed to retrieve the current time\n");
            return RX_ERROR;
        }

        *time = (uint64_t)(counter.QuadPart * time_to_nano);
        return RX_SUCCESS;
    }
#elif defined(RX__PLATFORM_DARWIN)
    /*
       Since Darwin 5.2, `clock_gettime()` can return high resolution times
       with the `CLOCK_UPTIME_RAW` clock but it internally only calls
       `mach_absolute_time()` with the overhead of converting the result into
       the `timespec` format.
    */
    {
        static uint64_t time_to_nano;

        if (time_to_nano == 0) {
            mach_timebase_info_data_t info;

            if (mach_timebase_info(&info) != KERN_SUCCESS) {
                RX__LOG_DEBUG("failed to retrieve the current time\n");
                return RX_ERROR;
            }

            time_to_nano = info.numer / info.denom;
        }

        *time = mach_absolute_time() * time_to_nano;
        return RX_SUCCESS;
    }
#elif defined(RX__PLATFORM_UNIX)
    #if defined(RX__USE_CLOCK_GETTIME)
    {
        struct timespec t;

        if (clock_gettime(RX__CLOCK_ID, &t) != 0) {
            RX__LOG_DEBUG("failed to retrieve the current time\n");
            return RX_ERROR;
        }

        *time = (uint64_t)t.tv_sec * RX__TICKS_PER_SECOND + (uint64_t)t.tv_nsec;
        return RX_SUCCESS;
    }
    #else
    {
        struct timeval t;

        if (gettimeofday(&t, NULL) != 0) {
            RX__LOG_DEBUG("failed to retrieve the current time\n");
            return RX_ERROR;
        }

        *time = (uint64_t)t.tv_sec * RX__TICKS_PER_SECOND
                + (uint64_t)t.tv_usec * RX__TICKS_PER_MICROSECOND;
        return RX_SUCCESS;
    }
    #endif
#else
    RX__LOG_DEBUG("platform not supported\n");
    return RX_ERROR;
#endif
}

/* Implementation: Test Failure Array                              O-(''Q)
   -------------------------------------------------------------------------- */

/*
   Simple implementation for dynamic arrays that can grow and stretch at
   runtime. The object returned to the user is a standard pointer to a C array
   but the implementation also allocates a header to keep track of the size
   and the capacity.

   The memory layout is better represented by the diagram below.

   block     user pointer
    /        /
   +--------+--------+
   | header | buffer |
   +--------+--------+

   The block points to the whole memory being allocated while the buffer
   represents the actual array exposed to the user.
*/

#define RX__DYN_ARRAY_GET_BLOCK(BUF)                                           \
    ((void *)&((struct rx__dyn_array_header *)(BUF))[-1])
#define RX__DYN_ARRAY_GET_HEADER(BLOCK)                                        \
    ((struct rx__dyn_array_header *)(BLOCK))
#define RX__DYN_ARRAY_GET_BUFFER(BLOCK)                                        \
    ((void *)&((struct rx__dyn_array_header *)(BLOCK))[1])
#define RX__DYN_ARRAY_GET_CONST_BLOCK(BUF)                                     \
    ((const void *)&((const struct rx__dyn_array_header *)(BUF))[-1])
#define RX__DYN_ARRAY_GET_CONST_HEADER(BLOCK)                                  \
    ((const struct rx__dyn_array_header *)(BLOCK))

struct rx__dyn_array_header {
    size_t size;
    size_t capacity;
};

static const size_t rx__test_failure_array_max_capacity
    = (((size_t)-1 - sizeof(struct rx__dyn_array_header))
       / sizeof(struct rx_failure));

static void
rx__dyn_array_get_new_capacity(size_t *capacity,
                               size_t current,
                               size_t requested,
                               size_t max)
{
    *capacity = current + current / 2 + 1;
    if (*capacity < current) {
        *capacity = max;
        return;
    }

    if (*capacity < requested) {
        *capacity = requested;
    }
}

static enum rx_status
rx__dyn_array_ensure_has_enough_capacity(void **block,
                                         size_t current_capacity,
                                         size_t requested_capacity,
                                         size_t max_capacity,
                                         size_t element_size)
{
    void *buf;
    size_t new_capacity;

    RX_ASSERT(block != NULL);
    RX_ASSERT(element_size > 0);

    if (requested_capacity > max_capacity) {
        RX__LOG_DEBUG("the requested capacity is too large\n");
        return RX_ERROR_MAX_SIZE_EXCEEDED;
    }

    if (*block != NULL && current_capacity >= requested_capacity) {
        return RX_SUCCESS;
    }

    rx__dyn_array_get_new_capacity(
        &new_capacity, current_capacity, requested_capacity, max_capacity);
    RX_ASSERT(new_capacity >= requested_capacity);
    RX_ASSERT(new_capacity <= max_capacity);

    buf = RX_REALLOC(
        *block,
        sizeof(struct rx__dyn_array_header) + element_size * new_capacity);
    if (buf == NULL) {
        RX__LOG_DEBUG("failed to reallocate the block\n");
        return RX_ERROR_ALLOCATION;
    }

    RX__DYN_ARRAY_GET_HEADER(buf)->capacity = new_capacity;
    *block = buf;
    return RX_SUCCESS;
}

static enum rx_status
rx__test_failure_array_create(struct rx_failure **array, size_t size)
{
    void *block;
    size_t capacity;

    RX_ASSERT(array != NULL);

    rx__dyn_array_get_new_capacity(
        &capacity, 0, size, rx__test_failure_array_max_capacity);
    RX_ASSERT(capacity >= size);
    RX_ASSERT(capacity <= rx__test_failure_array_max_capacity);

    block = RX_MALLOC(sizeof(struct rx__dyn_array_header)
                      + sizeof(struct rx_failure) * capacity);
    if (block == NULL) {
        RX__LOG_DEBUG_1("failed to reserve a large enough capacity for "
                        "the test failure array (requested capacity: %lu)\n",
                        (unsigned long)size);
        return RX_ERROR_ALLOCATION;
    }

    RX__DYN_ARRAY_GET_HEADER(block)->size = size;
    RX__DYN_ARRAY_GET_HEADER(block)->capacity = capacity;
    *array = (struct rx_failure *)RX__DYN_ARRAY_GET_BUFFER(block);
    return RX_SUCCESS;
}

static void
rx__test_failure_array_destroy(struct rx_failure *array)
{
    RX_FREE(RX__DYN_ARRAY_GET_BLOCK(array));
}

static void
rx__test_failure_array_get_size(size_t *size, const struct rx_failure *array)
{
    RX_ASSERT(array != NULL);

    *size = RX__DYN_ARRAY_GET_CONST_HEADER(RX__DYN_ARRAY_GET_CONST_BLOCK(array))
        ->size;
}

static enum rx_status
rx__test_failure_array_extend_back(struct rx_failure **slice,
                                   struct rx_failure **array,
                                   size_t size)
{
    enum rx_status status;
    void *block;
    size_t pos;

    RX_ASSERT(array != NULL);
    RX_ASSERT(*array != NULL);

    block = RX__DYN_ARRAY_GET_BLOCK(*array);
    status = rx__dyn_array_ensure_has_enough_capacity(
        &block,
        RX__DYN_ARRAY_GET_HEADER(block)->capacity,
        RX__DYN_ARRAY_GET_HEADER(block)->size + size,
        rx__test_failure_array_max_capacity,
        sizeof(struct rx_failure));
    if (status != RX_SUCCESS) {
        RX__LOG_DEBUG_1(
            "failed to reserve a large enough capacity for "
            "the test failure array (requested capacity: %lu)\n",
            (unsigned long)RX__DYN_ARRAY_GET_HEADER(block)->size + size);
        return status;
    }

    RX_ASSERT(block != NULL);

    *array = (struct rx_failure *)RX__DYN_ARRAY_GET_BUFFER(block);

    pos = RX__DYN_ARRAY_GET_HEADER(block)->size;
    memmove(&(*array)[pos + size],
            &(*array)[pos],
            sizeof(struct rx_failure)
                * (RX__DYN_ARRAY_GET_HEADER(block)->size - pos));

    if (slice != NULL) {
        *slice = &(*array)[pos];
    }

    RX__DYN_ARRAY_GET_HEADER(block)->size += size;
    return RX_SUCCESS;
}

/* Implementation: Memory Sections                                 O-(''Q)
   -------------------------------------------------------------------------- */

/*
   Compiler-specific code that allows grouping objects into specific
   data sections, thus enabling automatic discovery by iterating over
   anything registered in these sections.
*/

#if !defined(RX_DISABLE_TEST_DISCOVERY)                                        \
    && (defined(_MSC_VER) || defined(__GNUC__) || defined(__MINGW64__))
    #define RX__TEST_DISCOVERY 1
#else
    #define RX__TEST_DISCOVERY 0
#endif

#if !RX__TEST_DISCOVERY
    #define RX__TEST_SUITE_REGISTER(NAME) RX__REQUIRE_SEMICOLON
#elif defined(_MSC_VER)
    __pragma(section("rxsuite$a", read))
    __pragma(section("rxsuite$b", read))
    __pragma(section("rxsuite$c", read))

    __declspec(allocate("rxsuite$a"))
    const struct rx__test_suite_desc * const rx__test_suite_section_begin
        = NULL;

    __declspec(allocate("rxsuite$c"))
    const struct rx__test_suite_desc * const rx__test_suite_section_end
        = NULL;

    #define RX__TEST_SUITE_REGISTER(NAME)                                      \
        __declspec(allocate("rxsuite$b"))                                      \
        const struct rx__test_suite_desc * const                               \
        RX__TEST_SUITE_DESC_PTR_GET_ID(NAME)                                   \
            = &RX__TEST_SUITE_DESC_GET_ID(NAME)

    #define RX__TEST_SUITE_SECTION_BEGIN (&rx__test_suite_section_begin + 1)
    #define RX__TEST_SUITE_SECTION_END (&rx__test_suite_section_end)
#elif defined(__GNUC__) || defined(__MINGW64__)
    #if defined(RX__PLATFORM_DARWIN)
        extern const struct rx__test_suite_desc * const __start_rxsuite        \
            __asm("section$start$__DATA$rxsuite");
        extern const struct rx__test_suite_desc * const __stop_rxsuite         \
            __asm("section$end$__DATA$rxsuite");

        #define RX__TEST_SUITE_SECTION                                         \
            __attribute__((used,section("__DATA,rxsuite")))
    #else
        extern const struct rx__test_suite_desc * const __start_rxsuite;
        extern const struct rx__test_suite_desc * const __stop_rxsuite;

        #define RX__TEST_SUITE_SECTION                                         \
            __attribute__((used,section("rxsuite")))
    #endif

    RX__TEST_SUITE_SECTION
    static const struct rx__test_suite_desc * const rx__dummy_suite = NULL;

    #define RX__TEST_SUITE_REGISTER(NAME)                                      \
        RX__TEST_SUITE_SECTION                                                 \
        const struct rx__test_suite_desc * const                               \
        RX__TEST_SUITE_DESC_PTR_GET_ID(NAME)                                   \
            = &RX__TEST_SUITE_DESC_GET_ID(NAME)

    #define RX__TEST_SUITE_SECTION_BEGIN (&__start_rxsuite)
    #define RX__TEST_SUITE_SECTION_END (&__stop_rxsuite)
#endif

#if !RX__TEST_DISCOVERY
    #define RX__TEST_CASE_REGISTER(SUITE_NAME, NAME) RX__REQUIRE_SEMICOLON
#elif defined(_MSC_VER)
    __pragma(section("rxcase$a", read))
    __pragma(section("rxcase$b", read))
    __pragma(section("rxcase$c", read))

    __declspec(allocate("rxcase$a"))
    const struct rx__test_case_desc * const rx__test_case_section_begin
        = NULL;

    __declspec(allocate("rxcase$c"))
    const struct rx__test_case_desc * const rx__test_case_section_end
        = NULL;

    #define RX__TEST_CASE_REGISTER(SUITE_NAME, NAME)                           \
        __declspec(allocate("rxcase$b"))                                       \
        const struct rx__test_case_desc * const                                \
        RX__TEST_CASE_DESC_PTR_GET_ID(SUITE_NAME, NAME)                        \
            = &RX__TEST_CASE_DESC_GET_ID(SUITE_NAME, NAME)

    #define RX__TEST_CASE_SECTION_BEGIN (&rx__test_case_section_begin + 1)
    #define RX__TEST_CASE_SECTION_END (&rx__test_case_section_end)
#elif defined(__GNUC__) || defined(__MINGW64__)
    #if defined(RX__PLATFORM_DARWIN)
        extern const struct rx__test_case_desc * const __start_rxcase          \
            __asm("section$start$__DATA$rxcase");
        extern const struct rx__test_case_desc * const __stop_rxcase           \
            __asm("section$end$__DATA$rxcase");

        #define RX__TEST_CASE_SECTION                                          \
            __attribute__((used,section("__DATA,rxcase")))
    #else
        extern const struct rx__test_case_desc * const __start_rxcase;
        extern const struct rx__test_case_desc * const __stop_rxcase;

        #define RX__TEST_CASE_SECTION                                          \
            __attribute__((used,section("rxcase")))
    #endif

    RX__TEST_CASE_SECTION
    static const struct rx__test_case_desc * const rx__dummy_case = NULL;

    #define RX__TEST_CASE_REGISTER(SUITE_NAME, NAME)                           \
        RX__TEST_CASE_SECTION                                                  \
        const struct rx__test_case_desc * const                                \
        RX__TEST_CASE_DESC_PTR_GET_ID(SUITE_NAME, NAME)                        \
            = &RX__TEST_CASE_DESC_GET_ID(SUITE_NAME, NAME)

    #define RX__TEST_CASE_SECTION_BEGIN (&__start_rxcase)
    #define RX__TEST_CASE_SECTION_END (&__stop_rxcase)
#endif

/* Implementation: Fixture                                         O-(''Q)
   -------------------------------------------------------------------------- */

typedef void (*rx__fixture_config_update_fn)(
    struct rx_fixture_config *);

struct rx__fixture_desc {
    rx_size size;
    const rx__fixture_config_update_fn update;
};

#define RX__FIXTURE_(ID, SIZE, UPDATE_FN)                                      \
    static const struct rx__fixture_desc                                       \
    RX__FIXTURE_DESC_GET_ID(ID)                                                \
        = {SIZE, UPDATE_FN};                                                   \
                                                                               \
    static const struct rx__fixture_desc                                       \
    *ID = &RX__FIXTURE_DESC_GET_ID(ID);

#define RX__FIXTURE_0(ID, SIZE)                                                \
    RX__FIXTURE_(ID, SIZE, NULL)

#define RX__FIXTURE_1(ID, SIZE, ARG_COUNT, ARGS)                               \
    RX__STRUCT_DEFINE_UPDATE_FN(                                               \
        RX__FIXTURE_GET_UPDATE_FN_ID(ID),                                      \
        struct rx_fixture_config,                                              \
        ARG_COUNT,                                                             \
        ARGS)                                                                  \
                                                                               \
    RX__FIXTURE_(ID, SIZE, &RX__FIXTURE_GET_UPDATE_FN_ID(ID))

/* Implementation: Test Case Config                                O-(''Q)
   -------------------------------------------------------------------------- */

struct rx__test_case_config_blueprint {
    int skip;
    const struct rx__fixture_desc *fixture;
};

typedef void (*rx__test_case_config_blueprint_update_fn)(
    struct rx__test_case_config_blueprint *);

struct rx__test_case_config_desc {
    const rx__test_case_config_blueprint_update_fn update;
};

#define RX__TEST_CASE_CONFIG(ID, ARG_COUNT, ARGS)                              \
    RX__STRUCT_DEFINE_UPDATE_FN(                                               \
        RX__TEST_CASE_CONFIG_BLUEPRINT_GET_UPDATE_FN_ID(ID),                   \
        struct rx__test_case_config_blueprint,                                 \
        ARG_COUNT,                                                             \
        ARGS)                                                                  \
                                                                               \
    static const struct rx__test_case_config_desc                              \
    RX__TEST_CASE_CONFIG_DESC_GET_ID(ID)                                       \
        = {RX__TEST_CASE_CONFIG_BLUEPRINT_GET_UPDATE_FN_ID(ID)};

/* Implementation: Test Suite                                      O-(''Q)
   -------------------------------------------------------------------------- */

struct rx__test_suite_desc {
    const char *name;
    const struct rx__test_case_config_desc *config_desc;
};

#define RX__TEST_SUITE_(ID, CONFIG_DESC)                                       \
    RX__MAYBE_UNUSED static const struct rx__test_suite_desc                   \
    RX__TEST_SUITE_DESC_GET_ID(ID)                                             \
        = {#ID, CONFIG_DESC};                                                  \
                                                                               \
    RX__TEST_SUITE_REGISTER(ID)

#define RX__TEST_SUITE_0(ID)                                                   \
    RX__TEST_SUITE_(ID, NULL);

#define RX__TEST_SUITE_1(ID, ARG_COUNT, ARGS)                                  \
    RX__TEST_CASE_CONFIG(ID, ARG_COUNT, ARGS)                                  \
    RX__TEST_SUITE_(ID, &RX__TEST_CASE_CONFIG_DESC_GET_ID(ID));

/* Implementation: Test Case                                       O-(''Q)
   -------------------------------------------------------------------------- */

struct rx__test_case_desc {
    const char *suite_name;
    const char *name;
    rx_run_fn run;
    const struct rx__test_case_config_desc *config_desc;
};

#define RX__TEST_CASE_(SUITE_ID, ID, CONFIG_DESC)                              \
    static void                                                                \
    SUITE_ID##_##ID(RX__DEFINE_PARAMS(void));                                  \
                                                                               \
    RX__MAYBE_UNUSED static const struct rx__test_case_desc                    \
    RX__TEST_CASE_DESC_GET_ID(SUITE_ID, ID)                                    \
        = {#SUITE_ID,                                                          \
           #ID,                                                                \
           SUITE_ID##_##ID,                                                    \
           CONFIG_DESC};                                                       \
                                                                               \
    RX__TEST_CASE_REGISTER(SUITE_ID, ID);                                      \
                                                                               \
    static void                                                                \
    SUITE_ID##_##ID(RX__DEFINE_PARAMS(void))

#define RX__TEST_CASE_0(SUITE_ID, ID)                                          \
    RX__TEST_CASE_(SUITE_ID,                                                   \
                   ID,                                                         \
                   NULL)

#define RX__TEST_CASE_1(SUITE_ID, ID, ARG_COUNT, ARGS)                         \
    RX__TEST_CASE_CONFIG(SUITE_ID##_##ID, ARG_COUNT, ARGS)                     \
    RX__TEST_CASE_(SUITE_ID,                                                   \
                   ID,                                                         \
                   &RX__TEST_CASE_CONFIG_DESC_GET_ID(SUITE_ID##_##ID))

/* Implementation: Operators                                       O-(''Q)
   -------------------------------------------------------------------------- */

enum rx__op {
    RX__OP_EQUAL = 0,
    RX__OP_NOT_EQUAL = 1,
    RX__OP_GREATER = 2,
    RX__OP_LESSER = 3,
    RX__OP_GREATER_OR_EQUAL = 4,
    RX__OP_LESSER_OR_EQUAL = 5
};

static void
rx__op_get_symbol(const char **symbol, enum rx__op op)
{
    RX_ASSERT(symbol != NULL);

    switch (op) {
        case RX__OP_EQUAL:
            *symbol = "==";
            return;
        case RX__OP_NOT_EQUAL:
            *symbol = "!=";
            return;
        case RX__OP_GREATER:
            *symbol = ">";
            return;
        case RX__OP_LESSER:
            *symbol = "<";
            return;
        case RX__OP_GREATER_OR_EQUAL:
            *symbol = ">=";
            return;
        case RX__OP_LESSER_OR_EQUAL:
            *symbol = "<=";
            return;
        default:
            RX_ASSERT(0);
    }
}

static void
rx__op_get_name(const char **name, enum rx__op op)
{
    RX_ASSERT(name != NULL);

    switch (op) {
        case RX__OP_EQUAL:
            *name = "equal to";
            return;
        case RX__OP_NOT_EQUAL:
            *name = "not equal to";
            return;
        case RX__OP_GREATER:
            *name = "greater than";
            return;
        case RX__OP_LESSER:
            *name = "less than";
            return;
        case RX__OP_GREATER_OR_EQUAL:
            *name = "greater than or equal to";
            return;
        case RX__OP_LESSER_OR_EQUAL:
            *name = "less than or equal to";
            return;
        default:
            RX_ASSERT(0);
    }
}

/* Implementation: String                                          O-(''Q)
   -------------------------------------------------------------------------- */

#define RX__STR_LENGTH_ID rx__length

#define RX__STR_CREATE_VA_LIST(STATUS, S, FMT)                                 \
    do {                                                                       \
        va_list args;                                                          \
        size_t RX__STR_LENGTH_ID;                                              \
                                                                               \
        (S) = NULL;                                                            \
                                                                               \
        va_start(args, FMT);                                                   \
        (STATUS)                                                               \
            = rx__str_initialize_va_list(&RX__STR_LENGTH_ID, S, FMT, args);    \
        va_end(args);                                                          \
                                                                               \
        if ((STATUS) == RX_SUCCESS) {                                          \
            (S) = (char *)RX_MALLOC(sizeof *(S) * RX__STR_LENGTH_ID);          \
            if ((S) == NULL) {                                                 \
                RX__LOG_DEBUG_1(                                               \
                    "failed to allocate the string (%lu bytes)\n",             \
                    (unsigned long)sizeof *(S) * RX__STR_LENGTH_ID);           \
                (STATUS) = RX_ERROR_ALLOCATION;                                \
            } else {                                                           \
                va_start(args, FMT);                                           \
                (STATUS) = rx__str_initialize_va_list(                         \
                    &RX__STR_LENGTH_ID, S, FMT, args);                         \
                va_end(args);                                                  \
                if ((STATUS) != RX_SUCCESS) {                                  \
                    RX_FREE(S);                                                \
                }                                                              \
            }                                                                  \
        }                                                                      \
    } while (0)

#define RX__STR_CREATE_(STATUS, S, ARGS)                                       \
    do {                                                                       \
        size_t RX__STR_LENGTH_ID;                                              \
                                                                               \
        (S) = NULL;                                                            \
        (STATUS) = rx__str_initialize ARGS;                                    \
        if ((STATUS) == RX_SUCCESS) {                                          \
            (S) = (char *)RX_MALLOC(sizeof *(S) * RX__STR_LENGTH_ID);          \
            if ((S) == NULL) {                                                 \
                RX__LOG_DEBUG_1(                                               \
                    "failed to allocate the string (%lu bytes)\n",             \
                    (unsigned long)sizeof *(S) * RX__STR_LENGTH_ID);           \
                (STATUS) = RX_ERROR_ALLOCATION;                                \
            } else {                                                           \
                (STATUS) = rx__str_initialize ARGS;                            \
                if ((STATUS) != RX_SUCCESS) {                                  \
                    RX_FREE(S);                                                \
                }                                                              \
            }                                                                  \
        }                                                                      \
    } while (0)

#define RX__STR_CREATE(STATUS, S, MSG)                                         \
    RX__STR_CREATE_(STATUS, S, (&RX__STR_LENGTH_ID, S, MSG))

#define RX__STR_CREATE_1(STATUS, S, FMT, _0)                                   \
    RX__STR_CREATE_(STATUS, S, (&RX__STR_LENGTH_ID, S, FMT, _0))

#define RX__STR_CREATE_2(STATUS, S, FMT, _0, _1)                               \
    RX__STR_CREATE_(STATUS, S, (&RX__STR_LENGTH_ID, S, FMT, _0, _1))

#define RX__STR_CREATE_3(STATUS, S, FMT, _0, _1, _2)                           \
    RX__STR_CREATE_(STATUS, S, (&RX__STR_LENGTH_ID, S, FMT, _0, _1, _2))

#define RX__STR_CREATE_4(STATUS, S, FMT, _0, _1, _2, _3)                       \
    RX__STR_CREATE_(STATUS, S, (&RX__STR_LENGTH_ID, S, FMT, _0, _1, _2, _3))

enum rx__str_case { RX__STR_CASE_OBEY = 0, RX__STR_CASE_IGNORE = 1 };

static void
rx__str_case_get_type(const char **type, enum rx__str_case str_case)
{
    RX_ASSERT(type != NULL);

    switch (str_case) {
        case RX__STR_CASE_OBEY:
            *type = "obey";
            return;
        case RX__STR_CASE_IGNORE:
            *type = "ignore";
            return;
        default:
            RX_ASSERT(0);
    }
}

RX__PRINTF_CHECK(3, 0)
static enum rx_status
rx__str_initialize_va_list(size_t *count,
                           char *s,
                           const char *fmt,
                           va_list args)
{
    int size;

    RX_ASSERT(count != NULL);

    if (s == NULL) {
#if defined(RX__PLATFORM_WINDOWS)
        size = _vscprintf(fmt, args);
#elif RX__HAS_NPRINTF
        size = vsnprintf(NULL, 0, fmt, args);
#else
        {
            FILE *file;

            file = fopen("/dev/null", "w");
            if (file == NULL) {
                RX__LOG_DEBUG("could not open `/dev/null`\n");
                return RX_ERROR;
            }

            size = vfprintf(file, fmt, args);
            fclose(file);
        }
#endif

        if (size < 0) {
            RX__LOG_DEBUG("invalid string formatting\n");
            return RX_ERROR;
        }

        *count = (size_t)size + 1;
        return RX_SUCCESS;
    }

#if defined(_MSC_VER)
    #pragma warning(push)
    #pragma warning(disable : 4996)
#endif
    size = vsprintf(s, fmt, args);
#if defined(_MSC_VER)
    #pragma warning(pop)
#endif
    if (size < 0) {
        RX__LOG_DEBUG("unexpected string formatting error\n");
        return RX_ERROR;
    }

    *count = (size_t)size + 1;
    return RX_SUCCESS;
}

RX__PRINTF_CHECK(3, 4)
static enum rx_status
rx__str_initialize(size_t *count, char *s, const char *fmt, ...)
{
    enum rx_status out;
    va_list args;

    RX_ASSERT(count != NULL);

    va_start(args, fmt);
    out = rx__str_initialize_va_list(count, s, fmt, args);
    va_end(args);

    return out;
}

static enum rx_status
rx__str_copy(char **s, const char *original)
{
    size_t size;

    size = strlen(original) + 1;

    *s = (char *)RX_MALLOC(sizeof **s * size);
    if (*s == NULL) {
        RX__LOG_DEBUG_1("failed to allocate the string (%lu bytes)\n",
                        (unsigned long)sizeof **s * size);
        return RX_ERROR_ALLOCATION;
    }

    memcpy(*s, original, size);
    return RX_SUCCESS;
}

/* Implementation: Helpers                                         O-(''Q)
   -------------------------------------------------------------------------- */

RX__MAYBE_UNUSED static int
rx__compare_test_cases(const void *a, const void *b)
{
    int out;
    const struct rx_test_case *aa;
    const struct rx_test_case *bb;

    aa = (const struct rx_test_case *)a;
    bb = (const struct rx_test_case *)b;

    out = strcmp(aa->suite_name, bb->suite_name);
    if (out != 0) {
        return out;
    }

    return strcmp(aa->name, bb->name);
}

static int
rx__compare_summaries_by_test_suite(const void *a, const void *b)
{
    const struct rx_summary *aa;
    const struct rx_summary *bb;

    aa = (const struct rx_summary *)a;
    bb = (const struct rx_summary *)b;
    return strcmp(aa->test_case->suite_name, bb->test_case->suite_name);
}

static void
rx__real_are_equal_fuzzy(int *result, rx__real a, rx__real b, rx__real tol)
{
    rx__real diff;
    rx__real abs_a;
    rx__real abs_b;

    diff = a > b ? a - b : b - a;
    if (diff <= tol) {
        *result = 1;
        return;
    }

    abs_a = a < 0 ? -a : a;
    abs_b = b < 0 ? -b : b;
    *result = diff <= (abs_a > abs_b ? abs_a : abs_b) * tol;
}

static void
rx__str_are_equal(int *result, const char *a, const char *b)
{
    while (*a != '\0') {
        if (*a != *b) {
            *result = 0;
            return;
        }

        ++a;
        ++b;
    }

    *result = *a == *b;
}

static void
rx__str_are_equal_no_case(int *result, const char *a, const char *b)
{
    while (*a != '\0') {
        if (tolower(*a) != tolower(*b)) {
            *result = 0;
            return;
        }

        ++a;
        ++b;
    }

    *result = tolower(*a) == tolower(*b);
}

RX__MAYBE_UNUSED static enum rx_status
rx__run_test_cases(size_t test_case_count,
                   const struct rx_test_case *test_cases)
{
    size_t i;
    enum rx_status status;
    struct rx_summary *summaries;

    if (test_case_count == 0) {
        RX__LOG_INFO("nothing to run\n");
        return RX_SUCCESS;
    }

    RX_ASSERT(test_cases != NULL);

    summaries = (struct rx_summary *)RX_MALLOC(sizeof *summaries
                                               * test_case_count);
    if (summaries == NULL) {
        RX__LOG_DEBUG("failed to allocate the summaries\n");
        return RX_ERROR_ALLOCATION;
    }

    status = RX_SUCCESS;

    for (i = 0; i < test_case_count;) {
        const struct rx_test_case *test_case;
        struct rx_summary *summary;

        test_case = &test_cases[i];
        summary = &summaries[i];

        RX_ASSERT(test_case->suite_name != NULL);
        RX_ASSERT(test_case->name != NULL);

        status = rx_summary_initialize(summary, test_case);
        if (status != RX_SUCCESS) {
            RX__LOG_ERROR_2("failed to initialize the summary "
                            "(suite: \"%s\", case: \"%s\")\n",
                            test_case->suite_name,
                            test_case->name);
            goto summaries_cleanup;
        }

        ++i;

        status = rx_test_case_run(summary, test_case);
        if (status != RX_SUCCESS) {
            RX__LOG_ERROR_2("failed to run a test case "
                            "(suite: \"%s\", case: \"%s\")\n",
                            test_case->suite_name,
                            test_case->name);
            goto summaries_cleanup;
        }

        rx_summary_print(summary);
    }

    if (status == RX_SUCCESS) {
        size_t j;

        for (j = 0; j < test_case_count; ++j) {
            size_t k;
            struct rx_summary *summary;

            summary = &summaries[j];

            for (k = 0; k < summary->failure_count; ++k) {
                const struct rx_failure *failure;

                failure = &summary->failures[k];
                if (failure->severity == RX_FATAL) {
                    status = RX_ERROR_ABORTED;
                    goto summaries_cleanup;
                }
            }
        }
    }

summaries_cleanup:
    while (i-- > 0) {
        rx_summary_terminate(&summaries[i]);
    }

    RX_FREE(summaries);

    return status;
}

RX__MAYBE_UNUSED static enum rx_status
rx__run_registered_test_cases(void)
{
    enum rx_status out;
    rx_size test_case_count;
    struct rx_test_case *test_cases;

    rx_enumerate_test_cases(&test_case_count, NULL);
    if (test_case_count == 0) {
        return rx__run_test_cases(0, NULL);
    }

    test_cases = (struct rx_test_case *)RX_MALLOC(sizeof *test_cases
                                                  * test_case_count);
    if (test_cases == NULL) {
        RX__LOG_ERROR("failed to allocate the test cases\n");
        return RX_ERROR_ALLOCATION;
    }

    rx_enumerate_test_cases(&test_case_count, test_cases);
    out = rx__run_test_cases(test_case_count, test_cases);
    RX_FREE(test_cases);
    return out;
}

/* Implementation: Test Assessments                                O-(''Q)
   -------------------------------------------------------------------------- */

RX__PRINTF_CHECK(8, 0)
RX__MAYBE_UNUSED static void
rx__assess_value(struct rx_context *context,
                 int x,
                 int expected,
                 const char *expr,
                 const char *file,
                 int line,
                 enum rx_severity severity,
                 const char *failure_fmt,
                 ...)
{
    int result;
    char *failure_msg;
    char *diagnostic_msg;

    RX_ASSERT(context != NULL);
    RX_ASSERT(file != NULL);

    result = ((x && expected) || (!x && !expected));

    if (result) {
        failure_msg = NULL;
        diagnostic_msg = NULL;
    } else {
        enum rx_status status;

        if (failure_fmt == NULL) {
            RX__STR_CREATE_1(status,
                             failure_msg,
                             "`%s` is expected to evaluate to true",
                             expr);
        } else {
            RX__STR_CREATE_VA_LIST(status, failure_msg, failure_fmt);
        }

        if (status != RX_SUCCESS) {
            RX__LOG_DEBUG_2("failed to create the failure message for "
                            "the test located at %s:%d\n",
                            file,
                            line);
            failure_msg = NULL;
        }

        RX__STR_CREATE_1(status, diagnostic_msg, "%d", x);
        if (status != RX_SUCCESS) {
            RX__LOG_DEBUG_2("failed to create the diagnostic message for "
                            "the test located at %s:%d\n",
                            file,
                            line);
            diagnostic_msg = NULL;
        }
    }

    if (rx_handle_test_result(
            context, result, file, line, severity, failure_msg, diagnostic_msg)
        != RX_SUCCESS) {
        RX__LOG_DEBUG_2("failed to handle the test result for the test "
                        "located at %s:%d\n",
                        file,
                        line);
    }

    RX_FREE(failure_msg);
    RX_FREE(diagnostic_msg);

    if (!result && severity == RX_FATAL) {
        rx_abort(context);
    }
}

RX__PRINTF_CHECK(8, 0)
RX__MAYBE_UNUSED static void
rx__bool_assess_value(struct rx_context *context,
                      int x,
                      int expected,
                      const char *expr,
                      const char *file,
                      int line,
                      enum rx_severity severity,
                      const char *failure_fmt,
                      ...)
{
    int result;
    char *failure_msg;
    char *diagnostic_msg;

    RX_ASSERT(context != NULL);
    RX_ASSERT(expr != NULL);
    RX_ASSERT(file != NULL);

    result = ((x && expected) || (!x && !expected));

    if (result) {
        failure_msg = NULL;
        diagnostic_msg = NULL;
    } else {
        enum rx_status status;

        if (failure_fmt == NULL) {
            RX__STR_CREATE_2(status,
                             failure_msg,
                             "`%s` is expected to be %s",
                             expr,
                             expected ? "true" : "false");
        } else {
            RX__STR_CREATE_VA_LIST(status, failure_msg, failure_fmt);
        }

        if (status != RX_SUCCESS) {
            RX__LOG_DEBUG_2("failed to create the failure message for "
                            "the boolean test located at %s:%d\n",
                            file,
                            line);
            failure_msg = NULL;
        }

        RX__STR_CREATE_2(status, diagnostic_msg, "%d == %d", x, expected);
        if (status != RX_SUCCESS) {
            RX__LOG_DEBUG_2("failed to create the diagnostic message for "
                            "the boolean test located at %s:%d\n",
                            file,
                            line);
            diagnostic_msg = NULL;
        }
    }

    if (rx_handle_test_result(
            context, result, file, line, severity, failure_msg, diagnostic_msg)
        != RX_SUCCESS) {
        RX__LOG_DEBUG_2("failed to handle the test result for the boolean test "
                        "located at %s:%d\n",
                        file,
                        line);
    }

    RX_FREE(failure_msg);
    RX_FREE(diagnostic_msg);

    if (!result && severity == RX_FATAL) {
        rx_abort(context);
    }
}

RX__PRINTF_CHECK(10, 0)
RX__MAYBE_UNUSED static void
rx__int_assess_comparison(struct rx_context *context,
                          rx__int x1,
                          rx__int x2,
                          enum rx__op op,
                          const char *expr1,
                          const char *expr2,
                          const char *file,
                          int line,
                          enum rx_severity severity,
                          const char *failure_fmt,
                          ...)
{
    int result;
    char *failure_msg;
    char *diagnostic_msg;

    RX_ASSERT(context != NULL);
    RX_ASSERT(expr1 != NULL);
    RX_ASSERT(expr2 != NULL);
    RX_ASSERT(file != NULL);

    switch (op) {
        case RX__OP_EQUAL:
            result = x1 == x2;
            break;
        case RX__OP_NOT_EQUAL:
            result = x1 != x2;
            break;
        case RX__OP_GREATER:
            result = x1 > x2;
            break;
        case RX__OP_LESSER:
            result = x1 < x2;
            break;
        case RX__OP_GREATER_OR_EQUAL:
            result = x1 >= x2;
            break;
        case RX__OP_LESSER_OR_EQUAL:
            result = x1 <= x2;
            break;
        default:
            RX_ASSERT(0);
            result = 0;
            break;
    }

    if (result) {
        failure_msg = NULL;
        diagnostic_msg = NULL;
    } else {
        enum rx_status status;
        const char *op_symbol;

        if (failure_fmt == NULL) {
            const char *op_name;

            rx__op_get_name(&op_name, op);
            RX__STR_CREATE_3(status,
                             failure_msg,
                             "`%s` is expected to be %s `%s`",
                             expr1,
                             op_name,
                             expr2);
        } else {
            RX__STR_CREATE_VA_LIST(status, failure_msg, failure_fmt);
        }

        if (status != RX_SUCCESS) {
            RX__LOG_DEBUG_2("failed to create the failure message for "
                            "the integer comparison test located at %s:%d\n",
                            file,
                            line);
            failure_msg = NULL;
        }

        rx__op_get_symbol(&op_symbol, op);
        RX__STR_CREATE_3(status,
                         diagnostic_msg,
                         "%ld %s %ld",
                         (long)x1,
                         op_symbol,
                         (long)x2);
        if (status != RX_SUCCESS) {
            RX__LOG_DEBUG_2("failed to create the diagnostic message for "
                            "the integer comparison test located at %s:%d\n",
                            file,
                            line);
            diagnostic_msg = NULL;
        }
    }

    if (rx_handle_test_result(
            context, result, file, line, severity, failure_msg, diagnostic_msg)
        != RX_SUCCESS) {
        RX__LOG_DEBUG_2("failed to handle the test result for the integer "
                        "comparison test located at %s:%d\n",
                        file,
                        line);
    }

    RX_FREE(failure_msg);
    RX_FREE(diagnostic_msg);

    if (!result && severity == RX_FATAL) {
        rx_abort(context);
    }
}

RX__PRINTF_CHECK(10, 0)
RX__MAYBE_UNUSED static void
rx__uint_assess_comparison(struct rx_context *context,
                           rx__uint x1,
                           rx__uint x2,
                           enum rx__op op,
                           const char *expr1,
                           const char *expr2,
                           const char *file,
                           int line,
                           enum rx_severity severity,
                           const char *failure_fmt,
                           ...)
{
    int result;
    char *failure_msg;
    char *diagnostic_msg;

    RX_ASSERT(context != NULL);
    RX_ASSERT(expr1 != NULL);
    RX_ASSERT(expr2 != NULL);
    RX_ASSERT(file != NULL);

    switch (op) {
        case RX__OP_EQUAL:
            result = x1 == x2;
            break;
        case RX__OP_NOT_EQUAL:
            result = x1 != x2;
            break;
        case RX__OP_GREATER:
            result = x1 > x2;
            break;
        case RX__OP_LESSER:
            result = x1 < x2;
            break;
        case RX__OP_GREATER_OR_EQUAL:
            result = x1 >= x2;
            break;
        case RX__OP_LESSER_OR_EQUAL:
            result = x1 <= x2;
            break;
        default:
            RX_ASSERT(0);
            result = 0;
            break;
    }

    if (result) {
        failure_msg = NULL;
        diagnostic_msg = NULL;
    } else {
        enum rx_status status;
        const char *op_symbol;

        if (failure_fmt == NULL) {
            const char *op_name;

            rx__op_get_name(&op_name, op);
            RX__STR_CREATE_3(status,
                             failure_msg,
                             "`%s` is expected to be %s `%s`",
                             expr1,
                             op_name,
                             expr2);
        } else {
            RX__STR_CREATE_VA_LIST(status, failure_msg, failure_fmt);
        }

        if (status != RX_SUCCESS) {
            RX__LOG_DEBUG_2("failed to create the failure message for "
                            "the unsigned integer comparison test located at "
                            "%s:%d\n",
                            file,
                            line);
            failure_msg = NULL;
        }

        rx__op_get_symbol(&op_symbol, op);
        RX__STR_CREATE_3(status,
                         diagnostic_msg,
                         "%lu %s %lu",
                         (unsigned long)x1,
                         op_symbol,
                         (unsigned long)x2);
        if (status != RX_SUCCESS) {
            RX__LOG_DEBUG_2("failed to create the diagnostic message for "
                            "the unsigned integer comparison test located at "
                            "%s:%d\n",
                            file,
                            line);
            diagnostic_msg = NULL;
        }
    }

    if (rx_handle_test_result(
            context, result, file, line, severity, failure_msg, diagnostic_msg)
        != RX_SUCCESS) {
        RX__LOG_DEBUG_2("failed to handle the test result for the unsigned "
                        "integer comparison test located at %s:%d\n",
                        file,
                        line);
    }

    RX_FREE(failure_msg);
    RX_FREE(diagnostic_msg);

    if (!result && severity == RX_FATAL) {
        rx_abort(context);
    }
}

RX__PRINTF_CHECK(10, 0)
RX__MAYBE_UNUSED static void
rx__real_assess_comparison(struct rx_context *context,
                           rx__real x1,
                           rx__real x2,
                           enum rx__op op,
                           const char *expr1,
                           const char *expr2,
                           const char *file,
                           int line,
                           enum rx_severity severity,
                           const char *failure_fmt,
                           ...)
{
    int result;
    char *failure_msg;
    char *diagnostic_msg;

    RX_ASSERT(context != NULL);
    RX_ASSERT(expr1 != NULL);
    RX_ASSERT(expr2 != NULL);
    RX_ASSERT(file != NULL);

    switch (op) {
#if defined(__GNUC__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wfloat-equal"
#endif
        case RX__OP_EQUAL:
            result = x1 == x2;
            break;
        case RX__OP_NOT_EQUAL:
            result = x1 != x2;
            break;
#if defined(__GNUC__)
    #pragma GCC diagnostic pop
#endif
        case RX__OP_GREATER:
            result = x1 > x2;
            break;
        case RX__OP_LESSER:
            result = x1 < x2;
            break;
        case RX__OP_GREATER_OR_EQUAL:
            result = x1 >= x2;
            break;
        case RX__OP_LESSER_OR_EQUAL:
            result = x1 <= x2;
            break;
        default:
            RX_ASSERT(0);
            result = 0;
            break;
    }

    if (result) {
        failure_msg = NULL;
        diagnostic_msg = NULL;
    } else {
        enum rx_status status;
        const char *op_symbol;

        if (failure_fmt == NULL) {
            const char *op_name;

            rx__op_get_name(&op_name, op);
            RX__STR_CREATE_3(status,
                             failure_msg,
                             "`%s` is expected to be %s `%s`",
                             expr1,
                             op_name,
                             expr2);
        } else {
            RX__STR_CREATE_VA_LIST(status, failure_msg, failure_fmt);
        }

        if (status != RX_SUCCESS) {
            RX__LOG_DEBUG_2("failed to create the failure message for "
                            "the real comparison test located at %s:%d\n",
                            file,
                            line);
            failure_msg = NULL;
        }

        rx__op_get_symbol(&op_symbol, op);
        RX__STR_CREATE_3(
            status, diagnostic_msg, "%Lf %s %Lf", x1, op_symbol, x2);
        if (status != RX_SUCCESS) {
            RX__LOG_DEBUG_2("failed to create the diagnostic message for "
                            "the real comparison test located at %s:%d\n",
                            file,
                            line);
            diagnostic_msg = NULL;
        }
    }

    if (rx_handle_test_result(
            context, result, file, line, severity, failure_msg, diagnostic_msg)
        != RX_SUCCESS) {
        RX__LOG_DEBUG_2("failed to handle the test result for the real "
                        "comparison test located at %s:%d\n",
                        file,
                        line);
    }

    RX_FREE(failure_msg);
    RX_FREE(diagnostic_msg);

    if (!result && severity == RX_FATAL) {
        rx_abort(context);
    }
}

RX__PRINTF_CHECK(11, 0)
RX__MAYBE_UNUSED static void
rx__real_assess_fuzzy_comparison(struct rx_context *context,
                                 rx__real x1,
                                 rx__real x2,
                                 rx__real tol,
                                 enum rx__op op,
                                 const char *expr1,
                                 const char *expr2,
                                 const char *file,
                                 int line,
                                 enum rx_severity severity,
                                 const char *failure_fmt,
                                 ...)
{
    int result;
    char *failure_msg;
    char *diagnostic_msg;

    RX_ASSERT(context != NULL);
    RX_ASSERT(expr1 != NULL);
    RX_ASSERT(expr2 != NULL);
    RX_ASSERT(file != NULL);

    switch (op) {
        case RX__OP_EQUAL:
            rx__real_are_equal_fuzzy(&result, x1, x2, tol);
            break;
        case RX__OP_NOT_EQUAL:
            rx__real_are_equal_fuzzy(&result, x1, x2, tol);
            result = !result;
            break;
        default:
            RX_ASSERT(0);
            result = 0;
            break;
    }

    if (result) {
        failure_msg = NULL;
        diagnostic_msg = NULL;
    } else {
        enum rx_status status;
        const char *op_symbol;

        if (failure_fmt == NULL) {
            const char *op_name;

            rx__op_get_name(&op_name, op);
            RX__STR_CREATE_4(status,
                             failure_msg,
                             "`%s` is expected to be almost %s `%s` "
                             "(tolerance: %Lf)",
                             expr1,
                             op_name,
                             expr2,
                             tol);
        } else {
            RX__STR_CREATE_VA_LIST(status, failure_msg, failure_fmt);
        }

        if (status != RX_SUCCESS) {
            RX__LOG_DEBUG_2("failed to create the failure message for "
                            "the real almost equal test located at %s:%d\n",
                            file,
                            line);
            failure_msg = NULL;
        }

        rx__op_get_symbol(&op_symbol, op);
        RX__STR_CREATE_3(
            status, diagnostic_msg, "%Lf %s %Lf", x1, op_symbol, x2);
        if (status != RX_SUCCESS) {
            RX__LOG_DEBUG_2("failed to create the diagnostic message for "
                            "the real almost equal test located at %s:%d\n",
                            file,
                            line);
            diagnostic_msg = NULL;
        }
    }

    if (rx_handle_test_result(
            context, result, file, line, severity, failure_msg, diagnostic_msg)
        != RX_SUCCESS) {
        RX__LOG_DEBUG_2("failed to handle the test result for the real "
                        "almost equal test located at %s:%d\n",
                        file,
                        line);
    }

    RX_FREE(failure_msg);
    RX_FREE(diagnostic_msg);

    if (!result && severity == RX_FATAL) {
        rx_abort(context);
    }
}

RX__PRINTF_CHECK(11, 0)
RX__MAYBE_UNUSED static void
rx__str_assess_comparison(struct rx_context *context,
                          const char *s1,
                          const char *s2,
                          enum rx__str_case str_case,
                          enum rx__op op,
                          const char *expr1,
                          const char *expr2,
                          const char *file,
                          int line,
                          enum rx_severity severity,
                          const char *failure_fmt,
                          ...)
{
    int result;
    char *failure_msg;
    char *diagnostic_msg;

    RX_ASSERT(context != NULL);
    RX_ASSERT(s1 != NULL);
    RX_ASSERT(s2 != NULL);
    RX_ASSERT(expr1 != NULL);
    RX_ASSERT(expr2 != NULL);
    RX_ASSERT(file != NULL);

    switch (op) {
        case RX__OP_EQUAL:
            str_case == RX__STR_CASE_OBEY
                ? rx__str_are_equal(&result, s1, s2)
                : rx__str_are_equal_no_case(&result, s1, s2);
            break;
        case RX__OP_NOT_EQUAL:
            str_case == RX__STR_CASE_OBEY
                ? rx__str_are_equal(&result, s1, s2)
                : rx__str_are_equal_no_case(&result, s1, s2);
            result = !result;
            break;
        default:
            RX_ASSERT(0);
            result = 0;
            break;
    }

    if (result) {
        failure_msg = NULL;
        diagnostic_msg = NULL;
    } else {
        enum rx_status status;
        const char *op_symbol;

        if (failure_fmt == NULL) {
            const char *str_case_type;
            const char *op_name;

            rx__str_case_get_type(&str_case_type, str_case);
            rx__op_get_name(&op_name, op);
            RX__STR_CREATE_4(status,
                             failure_msg,
                             "`%s` is expected to be %s `%s` (case: %s)",
                             expr1,
                             op_name,
                             expr2,
                             str_case_type);
        } else {
            RX__STR_CREATE_VA_LIST(status, failure_msg, failure_fmt);
        }

        if (status != RX_SUCCESS) {
            RX__LOG_DEBUG_2("failed to create the failure message for "
                            "the string comparison test located at %s:%d\n",
                            file,
                            line);
            failure_msg = NULL;
        }

        rx__op_get_symbol(&op_symbol, op);
        RX__STR_CREATE_3(
            status, diagnostic_msg, "\"%s\" %s \"%s\"", s1, op_symbol, s2);
        if (status != RX_SUCCESS) {
            RX__LOG_DEBUG_2("failed to create the diagnostic message for "
                            "the string comparison test located at %s:%d\n",
                            file,
                            line);
            diagnostic_msg = NULL;
        }
    }

    if (rx_handle_test_result(
            context, result, file, line, severity, failure_msg, diagnostic_msg)
        != RX_SUCCESS) {
        RX__LOG_DEBUG_2("failed to handle the test result for the string "
                        "comparison test located at %s:%d\n",
                        file,
                        line);
    }

    RX_FREE(failure_msg);
    RX_FREE(diagnostic_msg);

    if (!result && severity == RX_FATAL) {
        rx_abort(context);
    }
}

RX__PRINTF_CHECK(10, 0)
RX__MAYBE_UNUSED static void
rx__ptr_assess_comparison(struct rx_context *context,
                          const void *x1,
                          const void *x2,
                          enum rx__op op,
                          const char *expr1,
                          const char *expr2,
                          const char *file,
                          int line,
                          enum rx_severity severity,
                          const char *failure_fmt,
                          ...)
{
    int result;
    char *failure_msg;
    char *diagnostic_msg;

    RX_ASSERT(context != NULL);
    RX_ASSERT(expr1 != NULL);
    RX_ASSERT(expr2 != NULL);
    RX_ASSERT(file != NULL);

    switch (op) {
        case RX__OP_EQUAL:
            result = x1 == x2;
            break;
        case RX__OP_NOT_EQUAL:
            result = x1 != x2;
            break;
        default:
            RX_ASSERT(0);
            result = 0;
            break;
    }

    if (result) {
        failure_msg = NULL;
        diagnostic_msg = NULL;
    } else {
        enum rx_status status;
        const char *op_symbol;

        if (failure_fmt == NULL) {
            const char *op_name;

            rx__op_get_name(&op_name, op);
            RX__STR_CREATE_3(status,
                             failure_msg,
                             "`%s` is expected to be %s `%s`",
                             expr1,
                             op_name,
                             expr2);
        } else {
            RX__STR_CREATE_VA_LIST(status, failure_msg, failure_fmt);
        }

        if (status != RX_SUCCESS) {
            RX__LOG_DEBUG_2("failed to create the failure message for "
                            "the pointer comparison test located at "
                            "%s:%d\n",
                            file,
                            line);
            failure_msg = NULL;
        }

        rx__op_get_symbol(&op_symbol, op);
        RX__STR_CREATE_3(status,
                         diagnostic_msg,
                         "0x%08lx %s 0x%08lx",
                         (uintptr_t)x1,
                         op_symbol,
                         (uintptr_t)x2);
        if (status != RX_SUCCESS) {
            RX__LOG_DEBUG_2("failed to create the diagnostic message for "
                            "the pointer comparison test located at "
                            "%s:%d\n",
                            file,
                            line);
            diagnostic_msg = NULL;
        }
    }

    if (rx_handle_test_result(
            context, result, file, line, severity, failure_msg, diagnostic_msg)
        != RX_SUCCESS) {
        RX__LOG_DEBUG_2("failed to handle the test result for the pointer "
                        "comparison test located at %s:%d\n",
                        file,
                        line);
    }

    RX_FREE(failure_msg);
    RX_FREE(diagnostic_msg);

    if (!result && severity == RX_FATAL) {
        rx_abort(context);
    }
}

RX__PRINTF_CHECK(8, 0)
RX__MAYBE_UNUSED static void
rx__ptr_assess_alignment(struct rx_context *context,
                         const void *x,
                         size_t alignment,
                         const char *expr,
                         const char *file,
                         int line,
                         enum rx_severity severity,
                         const char *failure_fmt,
                         ...)
{
    int result;
    char *failure_msg;
    char *diagnostic_msg;

    RX_ASSERT(context != NULL);
    RX_ASSERT(expr != NULL);
    RX_ASSERT(file != NULL);

    result = (uintptr_t)x  % alignment == 0;

    if (result) {
        failure_msg = NULL;
        diagnostic_msg = NULL;
    } else {
        enum rx_status status;

        if (failure_fmt == NULL) {
            RX__STR_CREATE_2(status,
                             failure_msg,
                             "`%s` is expected to have an %lu-byte alignment",
                             expr,
                             (unsigned long)alignment);
        } else {
            RX__STR_CREATE_VA_LIST(status, failure_msg, failure_fmt);
        }

        if (status != RX_SUCCESS) {
            RX__LOG_DEBUG_2("failed to create the failure message for "
                            "the pointer alignment test located at "
                            "%s:%d\n",
                            file,
                            line);
            failure_msg = NULL;
        }

        RX__STR_CREATE_2(status,
                         diagnostic_msg,
                         "0x%08lx %% %lu != 0",
                         (uintptr_t)x,
                         (unsigned long)alignment);
        if (status != RX_SUCCESS) {
            RX__LOG_DEBUG_2("failed to create the diagnostic message for "
                            "the pointer alignment test located at "
                            "%s:%d\n",
                            file,
                            line);
            diagnostic_msg = NULL;
        }
    }

    if (rx_handle_test_result(
            context, result, file, line, severity, failure_msg, diagnostic_msg)
        != RX_SUCCESS) {
        RX__LOG_DEBUG_2("failed to handle the test result for the pointer "
                        "alignment test located at %s:%d\n",
                        file,
                        line);
    }

    RX_FREE(failure_msg);
    RX_FREE(diagnostic_msg);

    if (!result && severity == RX_FATAL) {
        rx_abort(context);
    }
}

/* Implementation: Public API                                      O-(''Q)
   -------------------------------------------------------------------------- */

RX__MAYBE_UNUSED RX__STORAGE void
rx_abort(struct rx_context *context)
{
    longjmp(context->env, 1);
}

RX__MAYBE_UNUSED RX__STORAGE enum rx_status
rx_handle_test_result(struct rx_context *context,
                      int result,
                      const char *file,
                      int line,
                      enum rx_severity severity,
                      const char *failure_msg,
                      const char *diagnostic_msg)
{
    enum rx_status status;
    struct rx_summary *summary;
    struct rx_failure *failure;
    size_t failure_count;

    RX_ASSERT(context != NULL);
    RX_ASSERT(context->summary != NULL);
    RX_ASSERT(context->summary->failures != NULL);
    RX_ASSERT(file != NULL);

    summary = context->summary;

    ++summary->assessed_count;

    if (result) {
        return RX_SUCCESS;
    }

    status
        = rx__test_failure_array_extend_back(&failure, &summary->failures, 1);
    if (status != RX_SUCCESS) {
        summary->error = "failed to extend the test failure array\0";
        RX__LOG_ERROR_2("failed to extend the test failure array for the test "
                        "located at %s:%d\n",
                        file,
                        line);
        return status;
    }

    rx__test_failure_array_get_size(&failure_count, summary->failures);
    summary->failure_count = failure_count;

    {
        char *buf;

        status = rx__str_copy(&buf, file);
        if (status != RX_SUCCESS) {
            RX__LOG_ERROR_2("failed to allocate the file name for the test "
                            "located at %s:%d\n",
                            file,
                            line);
            failure->file = NULL;
        } else {
            failure->file = buf;
        }
    }

    failure->line = line;
    failure->severity = severity;

    if (failure_msg == NULL) {
        failure->msg = NULL;
    } else {
        char *buf;

        status = rx__str_copy(&buf, failure_msg);
        if (status != RX_SUCCESS) {
            RX__LOG_ERROR_2("failed to allocate the failure message for "
                            "the test located at %s:%d\n",
                            file,
                            line);
            failure->msg = NULL;
        } else {
            failure->msg = buf;
        }
    }

    if (diagnostic_msg == NULL) {
        failure->diagnostic_msg = NULL;
    } else {
        char *buf;

        status = rx__str_copy(&buf, diagnostic_msg);
        if (status != RX_SUCCESS) {
            RX__LOG_ERROR_2("failed to allocate the diagnostic message for "
                            "the test located at %s:%d\n",
                            file,
                            line);
            failure->diagnostic_msg = NULL;
        } else {
            failure->diagnostic_msg = buf;
        }
    }

    return RX_SUCCESS;
}

RX__MAYBE_UNUSED RX__STORAGE enum rx_status
rx_summary_initialize(struct rx_summary *summary,
                      const struct rx_test_case *test_case)
{
    enum rx_status status;

    RX_ASSERT(summary != NULL);
    RX_ASSERT(test_case != NULL);

    memset(summary, 0, sizeof *summary);

    status = rx__test_failure_array_create(&summary->failures, 0);
    if (status != RX_SUCCESS) {
        RX__LOG_ERROR_2("failed to create the test failure array "
                        "(suite: \"%s\", case: \"%s\")\n",
                        test_case->suite_name,
                        test_case->name);
        return status;
    }

    summary->test_case = test_case;
    return RX_SUCCESS;
}

RX__MAYBE_UNUSED RX__STORAGE void
rx_summary_terminate(struct rx_summary *summary)
{
    size_t i;

    RX_ASSERT(summary != NULL);
    RX_ASSERT(summary->failures != NULL);

    for (i = 0; i < summary->failure_count; ++i) {
        const struct rx_failure *failure;

        failure = &summary->failures[i];

        RX_FREE((void *)(uintptr_t)failure->file);
        RX_FREE((void *)(uintptr_t)failure->msg);
        RX_FREE((void *)(uintptr_t)failure->diagnostic_msg);
    }

    rx__test_failure_array_destroy(summary->failures);
}

RX__MAYBE_UNUSED RX__STORAGE void
rx_summary_print(const struct rx_summary *summary)
{
    size_t i;
    int passed;
    const char *style_begin;
    const char *style_end;

    RX_ASSERT(summary != NULL);
    RX_ASSERT(summary->test_case != NULL);
    RX_ASSERT(summary->test_case->suite_name != NULL);
    RX_ASSERT(summary->test_case->name != NULL);
    RX_ASSERT(summary->failures != NULL);

    passed = summary->failure_count == 0;

#if RX__LOG_STYLING
    if (RX__ISATTY(RX__FILENO(stderr))) {
        rx__log_style_get_ansi_code(
            &style_begin,
            passed ? RX__LOG_STYLE_BRIGHT_GREEN : RX__LOG_STYLE_BRIGHT_RED);
        rx__log_style_get_ansi_code(&style_end, RX__LOG_STYLE_RESET);
    } else {
        style_begin = style_end = "";
    }
#else
    style_begin = style_end = "";
#endif

    fprintf(stderr,
            "[%s%s%s] \"%s\" / \"%s\" (%f ms)\n",
            style_begin,
            passed ? "PASSED" : "FAILED",
            style_end,
            summary->test_case->suite_name,
            summary->test_case->name,
            (double)summary->elapsed * (1000.0 / RX__TICKS_PER_SECOND));

    for (i = 0; i < summary->failure_count; ++i) {
        const struct rx_failure *failure;
        const char *failure_msg;

        failure = &summary->failures[i];
        failure_msg = failure->msg == NULL ? "" : failure->msg;

        if (failure->diagnostic_msg != NULL) {
            fprintf(stderr,
                    "%s:%d: %s test failure: %s\n%s\n",
                    failure->file,
                    failure->line,
                    failure->severity == RX_FATAL ? "fatal" : "nonfatal",
                    failure_msg,
                    failure->diagnostic_msg);
        } else {
            fprintf(stderr,
                    "%s:%d: %s test failure: %s\n",
                    failure->file,
                    failure->line,
                    failure->severity == RX_FATAL ? "fatal" : "nonfatal",
                    failure_msg);
        }
    }
}

RX__MAYBE_UNUSED RX__STORAGE void
rx_sort_summaries_by_test_suite(struct rx_summary *summaries,
                                rx_size summary_count)
{
    qsort(summaries,
          summary_count,
          sizeof *summaries,
          rx__compare_summaries_by_test_suite);
}

RX__MAYBE_UNUSED RX__STORAGE void
rx_group_summaries_by_test_suite(rx_size *summary_group_count,
                                 struct rx_summary_group *summary_groups,
                                 rx_size summary_count,
                                 const struct rx_summary *summaries)
{
    size_t i;
    struct rx_summary_group *it;

    RX_ASSERT(summary_group_count != NULL);

    if (summary_count == 0) {
        *summary_group_count = 0;
        return;
    }

    RX_ASSERT(summaries != NULL);

    if (summary_groups == NULL) {
        *summary_group_count = 1;
        for (i = 0; i < summary_count - 1; ++i) {
            *summary_group_count += (rx_size)(
                rx__compare_summaries_by_test_suite(&summaries[i],
                                                    &summaries[i + 1])
                != 0);
        }

        return;
    }

    it = summary_groups;
    it->count = 0;
    it->array = summaries;
    for (i = 0; i < summary_count - 1; ++i) {
        ++it->count;

        if (rx__compare_summaries_by_test_suite(&summaries[i],
                                                &summaries[i + 1])
            != 0) {
            ++it;
            it->count = 0;
            it->array = &summaries[i + 1];
        }
    }

    ++it->count;
}

RX__MAYBE_UNUSED RX__STORAGE enum rx_status
rx_test_case_run(struct rx_summary *summary,
                 const struct rx_test_case *test_case)
{
    enum rx_status status;
    struct rx_context context;
    void *data;
    uint64_t time_begin;
    uint64_t time_end;

    RX_ASSERT(summary != NULL);
    RX_ASSERT(test_case != NULL);
    RX_ASSERT(test_case->suite_name != NULL);
    RX_ASSERT(test_case->name != NULL);
    RX_ASSERT(test_case->run != NULL);

    if (test_case->config.skip) {
        summary->skipped = 1;
        return RX_SUCCESS;
    }

    status = RX_SUCCESS;
    context.summary = summary;

    if (test_case->config.fixture.size > 0) {
        data = RX_MALLOC(test_case->config.fixture.size);
        if (data == NULL) {
            summary->error = "failed to allocate the data\0";
            RX__LOG_ERROR_2("failed to allocate the data"
                            "(suite: \"%s\", case: \"%s\")\n",
                            test_case->suite_name,
                            test_case->name);
            return RX_ERROR_ALLOCATION;
        }
    } else {
        data = NULL;
    }

    if (test_case->config.fixture.config.set_up != NULL) {
        status = test_case->config.fixture.config.set_up(&context, data);
        if (status != RX_SUCCESS) {
            summary->error = "failed to set-up the fixture\0";
            RX__LOG_ERROR_2("failed to set-up the fixture "
                            "(suite: \"%s\", case: \"%s\")\n",
                            test_case->suite_name,
                            test_case->name);
            goto data_cleanup;
        }
    }

    if (rx__get_real_time(&time_begin) != RX_SUCCESS) {
        time_begin = (uint64_t)-1;
    }

    if (setjmp(context.env) == 0) {
        test_case->run(&context, data);
    }

    if (time_begin == (uint64_t)-1
        || rx__get_real_time(&time_end) != RX_SUCCESS) {
        RX__LOG_ERROR_2("failed to measure the time elapsed "
                        "(suite: \"%s\", case: \"%s\")\n",
                        test_case->suite_name,
                        test_case->name);
        summary->elapsed = 0;
    } else {
        RX_ASSERT(time_end >= time_begin);
        summary->elapsed = (rx_uint64)(time_end - time_begin);
    }

    if (test_case->config.fixture.config.tear_down != NULL) {
        test_case->config.fixture.config.tear_down(&context, data);
    }

data_cleanup:
    RX_FREE(data);
    return status;
}

RX__MAYBE_UNUSED RX__STORAGE void
rx_enumerate_test_cases(rx_size *test_case_count,
                        struct rx_test_case *test_cases)
{
    size_t i;
    const struct rx__test_case_desc * const *c_it;

    RX_ASSERT(test_case_count != NULL);

#if !RX__TEST_DISCOVERY
    RX__UNUSED(test_cases);
    RX__UNUSED(i);
    RX__UNUSED(c_it);

    *test_case_count = 0;
    return;
#else
    if (test_cases == NULL) {
        *test_case_count = 0;
        for (c_it = RX__TEST_CASE_SECTION_BEGIN;
             c_it != RX__TEST_CASE_SECTION_END;
             ++c_it) {
            *test_case_count += (rx_size)(*c_it != NULL);
        }

        return;
    }

    i = 0;
    for (c_it = RX__TEST_CASE_SECTION_BEGIN;
         c_it != RX__TEST_CASE_SECTION_END;
         ++c_it) {
        const struct rx__test_suite_desc * const *s_it;
        struct rx__test_case_config_blueprint config_blueprint;
        struct rx_test_case *test_case;

        if (*c_it == NULL) {
            continue;
        }

        /* Find the corresponding test suite description, if any. */
        for (s_it = RX__TEST_SUITE_SECTION_BEGIN;
             s_it != RX__TEST_SUITE_SECTION_END;
             ++s_it) {
            if (*s_it == NULL) {
                continue;
            }

            if (strcmp((*s_it)->name, (*c_it)->suite_name) == 0) {
                break;
            }
        }

        memset(&config_blueprint, 0, sizeof config_blueprint);

        if (s_it != RX__TEST_SUITE_SECTION_END
            && (*s_it)->config_desc != NULL) {
            /* Inherit the config from the test suite's description. */
            (*s_it)->config_desc->update(&config_blueprint);
        }

        if ((*c_it)->config_desc != NULL) {
            /* Inherit the config from the test case's description. */
            (*c_it)->config_desc->update(&config_blueprint);
        }

        test_case = &test_cases[i];

        test_case->suite_name = (*c_it)->suite_name;
        test_case->name = (*c_it)->name;
        test_case->run = (*c_it)->run;

        test_case->config.skip = config_blueprint.skip;

        memset(&test_case->config.fixture, 0, sizeof test_case->config.fixture);

        if (config_blueprint.fixture != NULL) {
            test_case->config.fixture.size = config_blueprint.fixture->size;

            if (config_blueprint.fixture->update != NULL) {
                config_blueprint.fixture->update(
                    &test_case->config.fixture.config);
            }
        }

        ++i;
    }

    RX_ASSERT(i == *test_case_count);

    /* Objects that are defined in a custom memory section can only be retrieved
       in an undefined order, so these need to be manually sorted afterwards
       in a sensible way. */
    qsort(test_cases,
          *test_case_count,
          sizeof *test_cases,
          rx__compare_test_cases);
#endif
}

RX__MAYBE_UNUSED RX__STORAGE enum rx_status
rx_run(rx_size test_case_count, const struct rx_test_case *test_cases)
{
    if (test_cases != NULL) {
        return rx__run_test_cases(test_case_count, test_cases);
    }

    /* If no test cases are explicitly passed, fallback to discovering the
       ones defined through the automatic registration framework. */
    return rx__run_registered_test_cases();
}

RX__MAYBE_UNUSED RX__STORAGE enum rx_status
rx_main(rx_size test_case_count,
        const struct rx_test_case *test_cases,
        int argc,
        const char * const *argv)
{
    RX__UNUSED(argc);
    RX__UNUSED(argv);

    return rx_run(test_case_count, test_cases);
}

#endif /* REXO_REXO_H */
