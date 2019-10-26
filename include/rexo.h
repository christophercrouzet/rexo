/*
   The MIT License (MIT)

   Copyright (c) 2018-2019 Christopher Crouzet

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.
*/

#ifndef REXO_REXO_H
#define REXO_REXO_H

#include <ctype.h>
#include <math.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define RX_MAJOR_VERSION 0
#define RX_MINOR_VERSION 1
#define RX_PATCH_VERSION 0
#define RX_VERSION                                                             \
    ((RX_MAJOR_VERSION << 20) | (RX_MINOR_VERSION << 10) | (RX_PATCH_VERSION))

#ifdef RX_STATIC
    #define RX__SCOPE static
#else
    #define RX__SCOPE extern
#endif

#ifdef __GNUC__
    #define RX__MAYBE_UNUSED __attribute__((unused))
#else
    #define RX__MAYBE_UNUSED
#endif

struct rx_context;

#define RX__PARAM_CONTEXT rx__context
#define RX__PARAM_DATA rx__data

#define RX_DATA RX__PARAM_DATA

#define RX__DEFINE_PARAMS(type)                                                \
    struct rx_context *RX__PARAM_CONTEXT RX__MAYBE_UNUSED,                     \
    type *RX__PARAM_DATA RX__MAYBE_UNUSED

#define RX_SET_UP(id, type)                                                    \
    static enum rx_status RX__SET_UP_GET_WRAPPER_ID(id)(                       \
        RX__DEFINE_PARAMS(type));                                              \
    static enum rx_status                                                      \
    id(RX__DEFINE_PARAMS(void))                                                \
    {                                                                          \
        return RX__SET_UP_GET_WRAPPER_ID(id)(                                  \
            RX__PARAM_CONTEXT, (type *)RX__PARAM_DATA);                        \
    }                                                                          \
    static enum rx_status RX__SET_UP_GET_WRAPPER_ID(id)(                       \
        RX__DEFINE_PARAMS(type))

#define RX_TEAR_DOWN(id, type)                                                 \
    static void RX__TEAR_DOWN_GET_WRAPPER_ID(id)(                              \
        RX__DEFINE_PARAMS(type));                                              \
    static void                                                                \
    id(RX__DEFINE_PARAMS(void))                                                \
    {                                                                          \
        RX__TEAR_DOWN_GET_WRAPPER_ID(id)(                                      \
            RX__PARAM_CONTEXT, (type *)RX__PARAM_DATA);                        \
    }                                                                          \
    static void RX__TEAR_DOWN_GET_WRAPPER_ID(id)(                              \
        RX__DEFINE_PARAMS(type))

#define RX_FIXTURE(...)                                                        \
    RX__EXPAND(                                                                \
        RX__CONCAT(                                                            \
            RX__FIXTURE_,                                                      \
            RX__FIXTURE_HAS_VARIADIC_ARGS(__VA_ARGS__)                         \
        )(__VA_ARGS__))                                                        \
    RX__REQUIRE_SEMICOLON

#define RX_FIXTURE_VOID(...)                                                   \
    RX__EXPAND(                                                                \
        RX__CONCAT(                                                            \
            RX__FIXTURE_VOID_,                                                 \
            RX__FIXTURE_VOID_HAS_VARIADIC_ARGS(__VA_ARGS__)                    \
        )(__VA_ARGS__))                                                        \
    RX__REQUIRE_SEMICOLON

#define RX_TEST_SUITE(...)                                                     \
    RX__EXPAND(                                                                \
        RX__CONCAT(                                                            \
            RX__TEST_SUITE_,                                                   \
            RX__TEST_SUITE_HAS_VARIADIC_ARGS(__VA_ARGS__)                      \
        )(__VA_ARGS__))                                                        \
    RX__REQUIRE_SEMICOLON

#define RX_TEST_CASE(...)                                                      \
    RX__EXPAND(                                                                \
        RX__CONCAT(                                                            \
            RX__TEST_CASE_,                                                    \
            RX__TEST_CASE_HAS_VARIADIC_ARGS(__VA_ARGS__)                       \
        )(__VA_ARGS__))                                                        \

#define RX_TEST_CASE_FIXTURE(...)                                              \
    RX__EXPAND(                                                                \
        RX__CONCAT(                                                            \
            RX__TEST_CASE_FIXTURE_,                                            \
            RX__TEST_CASE_FIXTURE_HAS_VARIADIC_ARGS(__VA_ARGS__)               \
        )(__VA_ARGS__))

#define RX__FALSE ((int)0)
#define RX__TRUE ((int)1)

#define RX__DEFINE_TEST(condition, expected, msg, severity)                    \
    rx__assess_test(RX__PARAM_CONTEXT,                                         \
                    !!(condition),                                             \
                    expected,                                                  \
                    msg,                                                       \
                    __FILE__,                                                  \
                    __LINE__,                                                  \
                    severity)

#define RX__DEFINE_BOOL_TEST(condition, expected, severity)                    \
    rx__assess_bool_test(RX__PARAM_CONTEXT,                                    \
                         !!(condition),                                        \
                         expected,                                             \
                         #condition,                                           \
                         __FILE__,                                             \
                         __LINE__,                                             \
                         severity)

#define RX__DEFINE_INT_TEST(x1, x2, op, severity)                              \
    rx__assess_int_comparison_test(RX__PARAM_CONTEXT,                          \
                                   (x1),                                       \
                                   (x2),                                       \
                                   op,                                         \
                                   #x1,                                        \
                                   #x2,                                        \
                                   __FILE__,                                   \
                                   __LINE__,                                   \
                                   severity)

#define RX__DEFINE_UINT_TEST(x1, x2, op, severity)                             \
    rx__assess_uint_comparison_test(RX__PARAM_CONTEXT,                         \
                                    (x1),                                      \
                                    (x2),                                      \
                                    op,                                        \
                                    #x1,                                       \
                                    #x2,                                       \
                                    __FILE__,                                  \
                                    __LINE__,                                  \
                                    severity)

#define RX__DEFINE_REAL_TEST(x1, x2, op, severity)                             \
    rx__assess_real_comparison_test(RX__PARAM_CONTEXT,                         \
                                    (x1),                                      \
                                    (x2),                                      \
                                    op,                                        \
                                    #x1,                                       \
                                    #x2,                                       \
                                    __FILE__,                                  \
                                    __LINE__,                                  \
                                    severity)

#define RX__DEFINE_REAL_FUZZY_TEST(x1, x2, tol, op, severity)                  \
    rx__assess_real_fuzzy_comparison_test(RX__PARAM_CONTEXT,                   \
                                          (x1),                                \
                                          (x2),                                \
                                          (tol),                               \
                                          op,                                  \
                                          #x1,                                 \
                                          #x2,                                 \
                                          __FILE__,                            \
                                          __LINE__,                            \
                                          severity)
#define RX__DEFINE_STR_TEST(s1, s2, str_case, op, severity)                    \
    rx__assess_str_comparison_test(RX__PARAM_CONTEXT,                          \
                                   (s1),                                       \
                                   (s2),                                       \
                                   str_case,                                   \
                                   op,                                         \
                                   #s1,                                        \
                                   #s2,                                        \
                                   __FILE__,                                   \
                                   __LINE__,                                   \
                                   severity)

#define RX_REQUIRE(condition, msg)                                             \
    RX__DEFINE_TEST(condition, RX__TRUE, msg, RX_FATAL)

#define RX_REQUIRE_TRUE(condition)                                             \
    RX__DEFINE_BOOL_TEST(condition, RX__TRUE, RX_FATAL)

#define RX_REQUIRE_FALSE(condition)                                            \
    RX__DEFINE_BOOL_TEST(condition, RX__FALSE, RX_FATAL)

#define RX_REQUIRE_INT_EQUAL(x1, x2)                                           \
    RX__DEFINE_INT_TEST(x1, x2, RX__OP_EQUAL, RX_FATAL)

#define RX_REQUIRE_INT_NOT_EQUAL(x1, x2)                                       \
    RX__DEFINE_INT_TEST(x1, x2, RX__OP_NOT_EQUAL, RX_FATAL)

#define RX_REQUIRE_INT_GREATER(x1, x2)                                         \
    RX__DEFINE_INT_TEST(x1, x2, RX__OP_GREATER, RX_FATAL)

#define RX_REQUIRE_INT_LESSER(x1, x2)                                          \
    RX__DEFINE_INT_TEST(x1, x2, RX__OP_LESSER, RX_FATAL)

#define RX_REQUIRE_INT_GREATER_OR_EQUAL(x1, x2)                                \
    RX__DEFINE_INT_TEST(x1, x2, RX__OP_GREATER_OR_EQUAL, RX_FATAL)

#define RX_REQUIRE_INT_LESSER_OR_EQUAL(x1, x2)                                 \
    RX__DEFINE_INT_TEST(x1, x2, RX__OP_LESSER_OR_EQUAL, RX_FATAL)

#define RX_REQUIRE_UINT_EQUAL(x1, x2)                                          \
    RX__DEFINE_UINT_TEST(x1, x2, RX__OP_EQUAL, RX_FATAL)

#define RX_REQUIRE_UINT_NOT_EQUAL(x1, x2)                                      \
    RX__DEFINE_UINT_TEST(x1, x2, RX__OP_NOT_EQUAL, RX_FATAL)

#define RX_REQUIRE_UINT_GREATER(x1, x2)                                        \
    RX__DEFINE_UINT_TEST(x1, x2, RX__OP_GREATER, RX_FATAL)

#define RX_REQUIRE_UINT_LESSER(x1, x2)                                         \
    RX__DEFINE_UINT_TEST(x1, x2, RX__OP_LESSER, RX_FATAL)

#define RX_REQUIRE_UINT_GREATER_OR_EQUAL(x1, x2)                               \
    RX__DEFINE_UINT_TEST(x1, x2, RX__OP_GREATER_OR_EQUAL, RX_FATAL)

#define RX_REQUIRE_UINT_LESSER_OR_EQUAL(x1, x2)                                \
    RX__DEFINE_UINT_TEST(x1, x2, RX__OP_LESSER_OR_EQUAL, RX_FATAL)

#define RX_REQUIRE_REAL_EQUAL(x1, x2)                                          \
    RX__DEFINE_REAL_TEST(x1, x2, RX__OP_EQUAL, RX_FATAL)

#define RX_REQUIRE_REAL_NOT_EQUAL(x1, x2)                                      \
    RX__DEFINE_REAL_TEST(x1, x2, RX__OP_NOT_EQUAL, RX_FATAL)

#define RX_REQUIRE_REAL_GREATER(x1, x2)                                        \
    RX__DEFINE_REAL_TEST(x1, x2, RX__OP_GREATER, RX_FATAL)

#define RX_REQUIRE_REAL_LESSER(x1, x2)                                         \
    RX__DEFINE_REAL_TEST(x1, x2, RX__OP_LESSER, RX_FATAL)

#define RX_REQUIRE_REAL_GREATER_OR_EQUAL(x1, x2)                               \
    RX__DEFINE_REAL_TEST(x1, x2, RX__OP_GREATER_OR_EQUAL, RX_FATAL)

#define RX_REQUIRE_REAL_LESSER_OR_EQUAL(x1, x2)                                \
    RX__DEFINE_REAL_TEST(x1, x2, RX__OP_LESSER_OR_EQUAL, RX_FATAL)

#define RX_REQUIRE_REAL_ALMOST_EQUAL(x1, x2, tol)                              \
    RX__DEFINE_REAL_FUZZY_TEST(x1, x2, tol, RX__OP_EQUAL, RX_FATAL)

#define RX_REQUIRE_REAL_NOT_ALMOST_EQUAL(x1, x2, tol)                          \
    RX__DEFINE_REAL_FUZZY_TEST(x1, x2, tol, RX__OP_NOT_EQUAL, RX_FATAL)

#define RX_REQUIRE_STR_EQUAL(s1, s2)                                           \
    RX__DEFINE_STR_TEST(s1, s2, RX__STR_CASE_OBEY, RX__OP_EQUAL, RX_FATAL)

#define RX_REQUIRE_STR_NOT_EQUAL(s1, s2)                                       \
    RX__DEFINE_STR_TEST(s1, s2, RX__STR_CASE_OBEY, RX__OP_NOT_EQUAL, RX_FATAL)

#define RX_REQUIRE_STR_EQUAL_NO_CASE(s1, s2)                                   \
    RX__DEFINE_STR_TEST(s1, s2, RX__STR_CASE_IGNORE, RX__OP_EQUAL, RX_FATAL)

#define RX_REQUIRE_STR_NOT_EQUAL_NO_CASE(s1, s2)                               \
    RX__DEFINE_STR_TEST(s1, s2, RX__STR_CASE_IGNORE, RX__OP_NOT_EQUAL, RX_FATAL)

#define RX_CHECK(condition, msg)                                               \
    RX__DEFINE_TEST(condition, RX__TRUE, msg, RX_NONFATAL)

#define RX_CHECK_TRUE(condition)                                               \
    RX__DEFINE_BOOL_TEST(condition, RX__TRUE, RX_NONFATAL)

#define RX_CHECK_FALSE(condition)                                              \
    RX__DEFINE_BOOL_TEST(condition, RX__FALSE, RX_NONFATAL)

#define RX_CHECK_INT_EQUAL(x1, x2)                                             \
    RX__DEFINE_INT_TEST(x1, x2, RX__OP_EQUAL, RX_NONFATAL)

#define RX_CHECK_INT_NOT_EQUAL(x1, x2)                                         \
    RX__DEFINE_INT_TEST(x1, x2, RX__OP_NOT_EQUAL, RX_NONFATAL)

#define RX_CHECK_INT_GREATER(x1, x2)                                           \
    RX__DEFINE_INT_TEST(x1, x2, RX__OP_GREATER, RX_NONFATAL)

#define RX_CHECK_INT_LESSER(x1, x2)                                            \
    RX__DEFINE_INT_TEST(x1, x2, RX__OP_LESSER, RX_NONFATAL)

#define RX_CHECK_INT_GREATER_OR_EQUAL(x1, x2)                                  \
    RX__DEFINE_INT_TEST(x1, x2, RX__OP_GREATER_OR_EQUAL, RX_NONFATAL)

#define RX_CHECK_INT_LESSER_OR_EQUAL(x1, x2)                                   \
    RX__DEFINE_INT_TEST(x1, x2, RX__OP_LESSER_OR_EQUAL, RX_NONFATAL)

#define RX_CHECK_UINT_EQUAL(x1, x2)                                            \
    RX__DEFINE_UINT_TEST(x1, x2, RX__OP_EQUAL, RX_NONFATAL)

#define RX_CHECK_UINT_NOT_EQUAL(x1, x2)                                        \
    RX__DEFINE_UINT_TEST(x1, x2, RX__OP_NOT_EQUAL, RX_NONFATAL)

#define RX_CHECK_UINT_GREATER(x1, x2)                                          \
    RX__DEFINE_UINT_TEST(x1, x2, RX__OP_GREATER, RX_NONFATAL)

#define RX_CHECK_UINT_LESSER(x1, x2)                                           \
    RX__DEFINE_UINT_TEST(x1, x2, RX__OP_LESSER, RX_NONFATAL)

#define RX_CHECK_UINT_GREATER_OR_EQUAL(x1, x2)                                 \
    RX__DEFINE_UINT_TEST(x1, x2, RX__OP_GREATER_OR_EQUAL, RX_NONFATAL)

#define RX_CHECK_UINT_LESSER_OR_EQUAL(x1, x2)                                  \
    RX__DEFINE_UINT_TEST(x1, x2, RX__OP_LESSER_OR_EQUAL, RX_NONFATAL)

#define RX_CHECK_REAL_EQUAL(x1, x2)                                            \
    RX__DEFINE_REAL_TEST(x1, x2, RX__OP_EQUAL, RX_NONFATAL)

#define RX_CHECK_REAL_NOT_EQUAL(x1, x2)                                        \
    RX__DEFINE_REAL_TEST(x1, x2, RX__OP_NOT_EQUAL, RX_NONFATAL)

#define RX_CHECK_REAL_GREATER(x1, x2)                                          \
    RX__DEFINE_REAL_TEST(x1, x2, RX__OP_GREATER, RX_NONFATAL)

#define RX_CHECK_REAL_LESSER(x1, x2)                                           \
    RX__DEFINE_REAL_TEST(x1, x2, RX__OP_LESSER, RX_NONFATAL)

#define RX_CHECK_REAL_GREATER_OR_EQUAL(x1, x2)                                 \
    RX__DEFINE_REAL_TEST(x1, x2, RX__OP_GREATER_OR_EQUAL, RX_NONFATAL)

#define RX_CHECK_REAL_LESSER_OR_EQUAL(x1, x2)                                  \
    RX__DEFINE_REAL_TEST(x1, x2, RX__OP_LESSER_OR_EQUAL, RX_NONFATAL)

#define RX_CHECK_REAL_ALMOST_EQUAL(x1, x2, tol)                                \
    RX__DEFINE_REAL_FUZZY_TEST(x1, x2, tol, RX__OP_EQUAL, RX_NONFATAL)

#define RX_CHECK_REAL_NOT_ALMOST_EQUAL(x1, x2, tol)                            \
    RX__DEFINE_REAL_FUZZY_TEST(x1, x2, tol, RX__OP_NOT_EQUAL, RX_NONFATAL)

#define RX_CHECK_STR_EQUAL(s1, s2)                                             \
    RX__DEFINE_STR_TEST(s1, s2, RX__STR_CASE_OBEY, RX__OP_EQUAL, RX_NONFATAL)

#define RX_CHECK_STR_NOT_EQUAL(s1, s2)                                         \
    RX__DEFINE_STR_TEST(                                                       \
        s1, s2, RX__STR_CASE_OBEY, RX__OP_NOT_EQUAL, RX_NONFATAL)

#define RX_CHECK_STR_EQUAL_NO_CASE(s1, s2)                                     \
    RX__DEFINE_STR_TEST(s1, s2, RX__STR_CASE_IGNORE, RX__OP_EQUAL, RX_NONFATAL)

#define RX_CHECK_STR_NOT_EQUAL_NO_CASE(s1, s2)                                 \
    RX__DEFINE_STR_TEST(                                                       \
        s1, s2, RX__STR_CASE_IGNORE, RX__OP_NOT_EQUAL, RX_NONFATAL)

enum rx_status {
    RX_SUCCESS = 0,
    RX_ERROR = -1,
    RX_ERROR_ALLOCATION = -2,
    RX_ERROR_MAX_SIZE_EXCEEDED = -3
};

enum rx_severity { RX_NONFATAL = 0, RX_FATAL = 1 };

typedef enum rx_status (*rx_set_up_fn)(RX__DEFINE_PARAMS(void));
typedef void (*rx_tear_down_fn)(RX__DEFINE_PARAMS(void));
typedef void (*rx_test_case_run_fn)(RX__DEFINE_PARAMS(void));

struct rx_test_case {
    const char *name;
    const char *suite_name;
    void *data;
    rx_set_up_fn set_up;
    rx_tear_down_fn tear_down;
    rx_test_case_run_fn run;
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
    size_t test_count;
    size_t failure_count;
    struct rx_failure *failures;
    uint64_t elapsed;
};

struct rx_context {
    jmp_buf env;
    struct rx_summary *summary;
};

#ifdef __cplusplus
extern "C" {
#endif

RX__SCOPE void
rx_abort_test(struct rx_context *context);

RX__SCOPE enum rx_status
rx_handle_test_result(struct rx_context *context,
                      int result,
                      const char *file,
                      int line,
                      enum rx_severity severity,
                      const char *failure_msg,
                      const char *diagnostic_msg);

RX__SCOPE enum rx_status
rx_summary_initialize(struct rx_summary *summary,
                      const struct rx_test_case *test_case);

RX__SCOPE void
rx_summary_terminate(struct rx_summary *summary);

RX__SCOPE void
rx_summary_print(const struct rx_summary *summary);

RX__SCOPE enum rx_status
rx_test_case_run(struct rx_summary *summary,
                 const struct rx_test_case *test_case);

RX__SCOPE void
rx_test_cases_enumerate(size_t *test_case_count,
                        struct rx_test_case *test_cases);

RX__SCOPE enum rx_status
rx_run(int argc,
       const char **argv,
       size_t test_case_count,
       struct rx_test_case *test_cases);

#ifdef __cplusplus
}
#endif

/* Implementation                                                  O-(''Q)
   -------------------------------------------------------------------------- */

#if defined(RX_ENABLE_DEBUGGING) || (!defined(RX_DISABLE_DEBUGGING)            \
                                     && (defined(DEBUG) || !defined(NDEBUG)))
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

#define RX__UNUSED(x) (void)(x)

#define RX__REQUIRE_SEMICOLON void rx__dummy(void)

#define RX__STRINGIFY(x) #x

#define RX__EXPAND(x) x

#define RX__CONCAT_(a, b) a##b
#define RX__CONCAT(a, b) RX__CONCAT_(a, b)

#define RX__ARG(                                                               \
     _0,  _1,  _2,  _3,  _4,  _5,  _6,  _7,                                    \
     _8,  _9, _10, _11, _12, _13, _14, _15,                                    \
    _16, ...) _16

#define RX__COUNT_ARGS(...)                                                    \
    RX__EXPAND(RX__ARG(                                                        \
        __VA_ARGS__,                                                           \
        16, 15, 14, 13, 12, 11, 10,  9,                                        \
         8,  7,  6,  5,  4,  3,  2,  1,                                        \
         0,))

#define RX__APPLY_1(x, first) x(first)
#define RX__APPLY_2(x, first, ...) x(first) RX__APPLY_1(x, __VA_ARGS__)
#define RX__APPLY_3(x, first, ...) x(first) RX__APPLY_2(x, __VA_ARGS__)
#define RX__APPLY_4(x, first, ...) x(first) RX__APPLY_3(x, __VA_ARGS__)
#define RX__APPLY_5(x, first, ...) x(first) RX__APPLY_4(x, __VA_ARGS__)
#define RX__APPLY_6(x, first, ...) x(first) RX__APPLY_5(x, __VA_ARGS__)
#define RX__APPLY_7(x, first, ...) x(first) RX__APPLY_6(x, __VA_ARGS__)
#define RX__APPLY_8(x, first, ...) x(first) RX__APPLY_7(x, __VA_ARGS__)
#define RX__APPLY_9(x, first, ...) x(first) RX__APPLY_8(x, __VA_ARGS__)
#define RX__APPLY_10(x, first, ...) x(first) RX__APPLY_9(x, __VA_ARGS__)
#define RX__APPLY_11(x, first, ...) x(first) RX__APPLY_10(x, __VA_ARGS__)
#define RX__APPLY_12(x, first, ...) x(first) RX__APPLY_11(x, __VA_ARGS__)
#define RX__APPLY_13(x, first, ...) x(first) RX__APPLY_12(x, __VA_ARGS__)
#define RX__APPLY_14(x, first, ...) x(first) RX__APPLY_13(x, __VA_ARGS__)
#define RX__APPLY_15(x, first, ...) x(first) RX__APPLY_14(x, __VA_ARGS__)
#define RX__APPLY_16(x, first, ...) x(first) RX__APPLY_15(x, __VA_ARGS__)

#define RX__APPLY(x, ...)                                                      \
    RX__EXPAND(                                                                \
        RX__CONCAT(RX__APPLY_, RX__COUNT_ARGS(__VA_ARGS__))(x, __VA_ARGS__))

#define RX__STRUCT_INITIALIZE_STATIC_HAS_VARIADIC_ARGS(...)                    \
    RX__EXPAND(RX__ARG(                                                        \
        __VA_ARGS__,                                                           \
        1, 1, 1, 1, 1, 1, 1, 1,                                                \
        1, 1, 1, 1, 1, 0, 0, 0,))

/* Initializing data at compile-time in C99 can be done conveniently using
  designated initializers but these are not available in C++ prior to C++20.
  As a workaround, we use a feature specific to C++ that allows running
  a function at file scope to initialize the content of our static data. */

#define RX__STRUCT_INITIALIZE_STATIC_0(id, type, assigner)                     \
    static const type id = {0}

#ifdef __cplusplus
    #define RX__STRUCT_DEFAULT_MEMBER_ASSIGN(x) out x;
    #define RX__STRUCT_INITIALIZE_STATIC_1(id, type, assigner, ...)            \
        static const type id##_initialize()                                    \
        {                                                                      \
            type out = {0};                                                    \
            RX__APPLY(assigner, __VA_ARGS__);                                  \
            return out;                                                        \
        }                                                                      \
        static const type id = id##_initialize()
#else
    #define RX__STRUCT_DEFAULT_MEMBER_ASSIGN(x) x,
    #define RX__STRUCT_INITIALIZE_STATIC_1(id, type, assigner, ...)            \
        static const type id = { RX__APPLY(assigner, __VA_ARGS__) }
#endif

#define RX__STRUCT_INITIALIZE_STATIC(...)                                      \
    RX__EXPAND(                                                                \
        RX__CONCAT(                                                            \
            RX__STRUCT_INITIALIZE_STATIC_,                                     \
            RX__STRUCT_INITIALIZE_STATIC_HAS_VARIADIC_ARGS(__VA_ARGS__)        \
        )(__VA_ARGS__))

#define RX__SET_UP_GET_WRAPPER_ID(id)                                          \
    rx__set_up_wrapper_##id
#define RX__TEAR_DOWN_GET_WRAPPER_ID(id)                                       \
    rx__tear_down_wrapper_##id
#define RX__FIXTURE_GET_DATA_TYPE(id)                                          \
    rx__fixture_data_type_##id
#define RX__FIXTURE_DESC_GET_ID(id)                                            \
    rx__fixture_desc_##id
#define RX__TEST_SUITE_DESC_GET_ID(id)                                         \
    rx__test_suite_desc_##id
#define RX__TEST_CASE_DESC_GET_ID(suite_id, id)                                \
    rx__test_case_desc_##suite_id##_##id
#define RX__TEST_SUITE_DESC_PTR_GET_ID(id)                                     \
    rx__test_suite_desc_ptr_##id
#define RX__TEST_CASE_DESC_PTR_GET_ID(suite_id, id)                            \
    rx__test_case_desc_ptr_##suite_id##_##id
#define RX__TEST_SUITE_CONFIG_GET_ID(id)                                       \
    rx__test_suite_config_##id
#define RX__TEST_CASE_CONFIG_GET_ID(suite_id, id)                              \
    rx__test_case_config_##suite_id##_##id
#define RX__TEST_CASE_DATA_GET_ID(suite_id, id)                                \
    rx__test_case_data_##suite_id##_##id
#define RX__TEST_CASE_GET_ID(suite_id, id)                                     \
    suite_id##_##id

typedef intmax_t rx__int;
typedef uintmax_t rx__uint;
typedef long double rx__real;

/* Logger                                                          O-(''Q)
   -------------------------------------------------------------------------- */

#if !defined(RX_DISABLE_LOG_STYLING) && defined(RX__PLATFORM_UNIX)             \
    && defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 1
#include <unistd.h>
#define RX__LOG_STYLING 1
#else
#define RX__LOG_STYLING 0
#endif

#if defined(RX_SET_LOGGING_LEVEL_DEBUG)
    #define RX__LOGGING_LEVEL RX__LOG_LEVEL_DEBUG
#elif defined(RX_SET_LOGGING_LEVEL_TRACE)
    #define RX__LOGGING_LEVEL RX__LOG_LEVEL_TRACE
#elif defined(RX_SET_LOGGING_LEVEL_INFO)
    #define RX__LOGGING_LEVEL RX__LOG_LEVEL_INFO
#elif defined(RX_SET_LOGGING_LEVEL_WARNING)
    #define RX__LOGGING_LEVEL RX__LOG_LEVEL_WARNING
#elif defined(RX_SET_LOGGING_LEVEL_ERROR)
    #define RX__LOGGING_LEVEL RX__LOG_LEVEL_ERROR
#elif defined(RX_ENABLE_DEBUGGING)                                             \
    || (!defined(RX_DISABLE_DEBUGGING)                                         \
        && (defined(DEBUG) || !defined(NDEBUG)))
    #define RX__LOGGING_LEVEL RX__LOG_LEVEL_DEBUG
#else
    #define RX__LOGGING_LEVEL RX__LOG_LEVEL_WARNING
#endif

#ifdef RX_DISABLE_LOGGING
    #define RX__LOGGING 0
#else
    #define RX__LOGGING 1
#endif

#ifndef RX_LOG
    #define RX_LOG(level, ...)                                                 \
        do {                                                                   \
            if (RX__LOGGING && level <= RX__LOGGING_LEVEL) {                   \
                rx__log(level, __FILE__, __LINE__, __VA_ARGS__);               \
            }                                                                  \
        } while (0)
#endif

#define RX__LOG_DEBUG(...) RX_LOG(RX__LOG_LEVEL_DEBUG, __VA_ARGS__)

#define RX__LOG_TRACE(...) RX_LOG(RX__LOG_LEVEL_TRACE, __VA_ARGS__)

#define RX__LOG_INFO(...) RX_LOG(RX__LOG_LEVEL_INFO, __VA_ARGS__)

#define RX__LOG_WARNING(...) RX_LOG(RX__LOG_LEVEL_WARNING, __VA_ARGS__)

#define RX__LOG_ERROR(...) RX_LOG(RX__LOG_LEVEL_ERROR, __VA_ARGS__)

enum rx__log_level {
    RX__LOG_LEVEL_ERROR = 0,
    RX__LOG_LEVEL_WARNING = 1,
    RX__LOG_LEVEL_INFO = 2,
    RX__LOG_LEVEL_TRACE = 3,
    RX__LOG_LEVEL_DEBUG = 4
};

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
rx__log_level_get_name(const char **name, enum rx__log_level level)
{
    RX_ASSERT(name != NULL);

    switch (level) {
        case RX__LOG_LEVEL_ERROR:
            *name = "error";
            return;
        case RX__LOG_LEVEL_WARNING:
            *name = "warning";
            return;
        case RX__LOG_LEVEL_INFO:
            *name = "info";
            return;
        case RX__LOG_LEVEL_TRACE:
            *name = "trace";
            return;
        case RX__LOG_LEVEL_DEBUG:
            *name = "debug";
            return;
        default:
            RX_ASSERT(0);
    }
}

#if RX__LOG_STYLING
static void
rx__log_level_get_style(enum rx__log_style *style, enum rx__log_level level)
{
    RX_ASSERT(style != NULL);

    switch (level) {
        case RX__LOG_LEVEL_ERROR:
            *style = RX__LOG_STYLE_BRIGHT_RED;
            return;
        case RX__LOG_LEVEL_WARNING:
            *style = RX__LOG_STYLE_BRIGHT_YELLOW;
            return;
        case RX__LOG_LEVEL_INFO:
            *style = RX__LOG_STYLE_BRIGHT_GREEN;
            return;
        case RX__LOG_LEVEL_TRACE:
            *style = RX__LOG_STYLE_BRIGHT_CYAN;
            return;
        case RX__LOG_LEVEL_DEBUG:
            *style = RX__LOG_STYLE_BRIGHT_MAGENTA;
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

static void
rx__log(enum rx__log_level level,
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
    if (isatty(fileno(stderr))) {
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

/* Timer                                                           O-(''Q)
   -------------------------------------------------------------------------- */

#define RX__TICKS_PER_SECOND 1000000000ull

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
                RX__LOG_ERROR("failed to retrieve the time's frequency\n");
                return RX_ERROR;
            }

            time_to_nano = (double)RX__TICKS_PER_SECOND / frequency.QuadPart;
        }

        if (!QueryPerformanceCounter(&counter)) {
            RX__LOG_ERROR("failed to retrieve the current time\n");
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
        static double time_to_nano;

        if (time_to_nano == 0.0) {
            mach_timebase_info_data_t info;

            if (mach_timebase_info(&info) != KERN_SUCCESS) {
                RX__LOG_ERROR("failed to retrieve the current time\n");
                return RX_ERROR;
            }

            time_to_nano = (double)info.numer / info.denom;
        }

        *time = mach_absolute_time() * time_to_nano;
        return RX_SUCCESS;
    }
#elif defined(RX__PLATFORM_UNIX)
    #if defined(RX__USE_CLOCK_GETTIME)
    {
        struct timespec t;

        if (clock_gettime(RX__CLOCK_ID, &t) != 0) {
            RX__LOG_ERROR("failed to retrieve the current time\n");
            return RX_ERROR;
        }

        *time = (uint64_t)t.tv_sec * 1000000000ull + (uint64_t)t.tv_nsec;
        return RX_SUCCESS;
    }
    #else
    {
        struct timeval t;

        if (gettimeofday(&t, NULL) != 0) {
            RX__LOG_ERROR("failed to retrieve the current time\n");
            return RX_ERROR;
        }

        *time = (uint64_t)t.tv_sec * 1000000000ull
                + (uint64_t)t.tv_usec * 1000ull;
        return RX_SUCCESS;
    }
    #endif
#endif

    RX__LOG_ERROR("platform not supported\n");
    return RX_ERROR;
}

/* Test Failure Array                                              O-(''Q)
   -------------------------------------------------------------------------- */

/* Simple implementation for dynamic arrays that can grow and stretch at
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
   represents the actual array exposed to the user. */

#define RX__DYN_ARRAY_GET_BLOCK(buf)                                           \
    ((void *)&((struct rx__dyn_array_header *)(buf))[-1])
#define RX__DYN_ARRAY_GET_HEADER(block)                                        \
    ((struct rx__dyn_array_header *)(block))
#define RX__DYN_ARRAY_GET_BUFFER(block)                                        \
    ((void *)&((struct rx__dyn_array_header *)(block))[1])
#define RX__DYN_ARRAY_GET_CONST_BLOCK(buf)                                     \
    ((const void *)&((const struct rx__dyn_array_header *)(buf))[-1])
#define RX__DYN_ARRAY_GET_CONST_HEADER(block)                                  \
    ((const struct rx__dyn_array_header *)(block))

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
        RX__LOG_TRACE("the requested capacity is too large\n");
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
        RX__LOG_TRACE("failed to reallocate the block\n");
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
        RX__LOG_TRACE("failed to reserve a large enough capacity for the "
                      "test failure array (requested capacity: %zu)\n",
                      size);
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
    if (array == NULL) {
        return;
    }

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
        RX__LOG_TRACE("failed to reserve a large enough capacity for the "
                      "test failure array (requested capacity: %zu)\n",
                      RX__DYN_ARRAY_GET_HEADER(block)->size + size);
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

/* Test Discovery                                                  O-(''Q)
   -------------------------------------------------------------------------- */

/* Compiler-specific code that allows grouping test suite and test case
  descriptions into specific data sections, thus enabling automatic discovery
  by iterating over any data registered in these sections. */

#if defined(__GNUC__)
    #define RX__TEST_SUITE_SECTION_BEGIN __start_rxsuite
    #define RX__TEST_SUITE_SECTION_END __stop_rxsuite
    #define RX__TEST_CASE_SECTION_BEGIN __start_rxcase
    #define RX__TEST_CASE_SECTION_END __stop_rxcase

    extern const struct rx__test_suite_desc *RX__TEST_SUITE_SECTION_BEGIN;
    extern const struct rx__test_suite_desc *RX__TEST_SUITE_SECTION_END;
    extern const struct rx__test_case_desc *RX__TEST_CASE_SECTION_BEGIN;
    extern const struct rx__test_case_desc *RX__TEST_CASE_SECTION_END;

    /* Dummy pointers are required to avoid the compiler optimizing away
       empty sections in release mode. */
    __attribute__((used, section("rxsuite")))
    const struct rx__test_suite_desc *rx__dummy_suite = NULL;

    __attribute__((used, section("rxcase")))
    const struct rx__test_case_desc *rx__dummy_case = NULL;

    #define RX__TEST_SUITE_DESC_DEFINE_PTR(name)                               \
        __attribute__((used, section("rxsuite")))                              \
        const struct rx__test_suite_desc                                       \
        *RX__TEST_SUITE_DESC_PTR_GET_ID(name)                                  \
            = &RX__TEST_SUITE_DESC_GET_ID(name)

    #define RX__TEST_CASE_DESC_DEFINE_PTR(suite_name, name)                    \
        __attribute__((used, section("rxcase")))                               \
        const struct rx__test_case_desc                                        \
        *RX__TEST_CASE_DESC_PTR_GET_ID(suite_name, name)                       \
            = &RX__TEST_CASE_DESC_GET_ID(suite_name, name)
#elif defined(_MSC_VER)
    #define RX__TEST_SUITE_SECTION_BEGIN rx__test_suite_section_begin
    #define RX__TEST_SUITE_SECTION_END rx__test_suite_section_end
    #define RX__TEST_CASE_SECTION_BEGIN rx__test_case_section_begin
    #define RX__TEST_CASE_SECTION_END rx__test_case_section_end

    /* All the pointers set in the custom memory section need to be tagged
       to prevent MSVC to optimize them away. */
    #if defined(_WIN64)
        #define RX__MSVC_FORCE_LINKING(id)                                     \
            __pragma(comment(linker, "/include:" RX__STRINGIFY(id)))
    #else
        #define RX__MSVC_FORCE_LINKING(id)                                     \
            __pragma(comment(linker, "/include:_" RX__STRINGIFY(id)))
    #endif

    __pragma(section("rxsuite$a", read))
    __pragma(section("rxsuite$s", read))
    __pragma(section("rxsuite$z", read))
    __pragma(section("rxcase$a", read))
    __pragma(section("rxcase$s", read))
    __pragma(section("rxcase$z", read))

    RX__MSVC_FORCE_LINKING(RX__TEST_SUITE_SECTION_BEGIN)                       \
    __declspec(allocate("rxsuite$a"))                                          \
    const struct rx__test_suite_desc *RX__TEST_SUITE_SECTION_BEGIN;

    RX__MSVC_FORCE_LINKING(RX__TEST_SUITE_SECTION_END)                         \
    __declspec(allocate("rxsuite$z"))                                          \
    const struct rx__test_suite_desc *RX__TEST_SUITE_SECTION_END;

    RX__MSVC_FORCE_LINKING(RX__TEST_CASE_SECTION_BEGIN)                        \
    __declspec(allocate("rxcase$a"))                                           \
    const struct rx__test_case_desc *RX__TEST_CASE_SECTION_BEGIN;

    RX__MSVC_FORCE_LINKING(RX__TEST_CASE_SECTION_END)                          \
    __declspec(allocate("rxcase$z"))                                           \
    const struct rx__test_case_desc *RX__TEST_CASE_SECTION_END;

    #define RX__TEST_SUITE_DESC_DEFINE_PTR(name)                               \
        RX__MSVC_FORCE_LINKING(RX__TEST_SUITE_DESC_PTR_GET_ID(name))           \
        __declspec(allocate("rxsuite$s"))                                      \
        const struct rx__test_suite_desc                                       \
        *RX__TEST_SUITE_DESC_PTR_GET_ID(name)                                  \
            = &RX__TEST_SUITE_DESC_GET_ID(name)

    #define RX__TEST_CASE_DESC_DEFINE_PTR(suite_name, name)                    \
        RX__MSVC_FORCE_LINKING(RX__TEST_CASE_DESC_PTR_GET_ID(suite_name, name))\
        __declspec(allocate("rxcase$s"))                                       \
        const struct rx__test_case_desc                                        \
        *RX__TEST_CASE_DESC_PTR_GET_ID(suite_name, name)                       \
            = &RX__TEST_CASE_DESC_GET_ID(suite_name, name)
#endif

/* Test Configuration                                              O-(''Q)
   -------------------------------------------------------------------------- */

#define RX__CONFIG_MEMBERS                                                     \
    RX__CONFIG_MEMBER(rx_set_up_fn, set_up)                                    \
    RX__CONFIG_MEMBER(rx_tear_down_fn, tear_down)

/* Providing a designated initializer-like API like `DEFINE_DATA(.foo = 1.23);`,
   that tracks whether a member is explicitely defined or not, builds upon the
   `RX__STRUCT_INITIALIZE_STATIC` macro but provides an alternative way of
   setting members that also sets a ‘defined’ flag alongside each member that
   is being set through this macro.

   How the members and their respective boolean flags are set depends on
   the language.

   The designated initializer syntax available in C99 states that implicit
   initializers initialize struct elements in order of declaration after any
   previous designator used. So if we define a struct where each member value
   is followed by a corresponding boolean flag, as in

   struct data {
       double foo;
       int foo_defined;
   };

   then we are able to expand the macro to `.foo = 1.23, 1`, that is being used
   to fill up the structure as expected.

   C++ requires a different trick where the values and boolean flags are
   separatated into two named structs, such as

   struct data {
       struct {
           double foo;
       } value;
       struct {
           int foo;
       } defined;
   };

   In this scenario, the macro prefixes each given assignment to produce
   an expression like `out.value.foo = 1.23; out.defined.foo = 1.23 ? 1 : 1`
   that is used to initialize the structure inside the function defined by
   the C++ version of the macro `RX__STRUCT_INITIALIZE_STATIC`. */

#ifdef __cplusplus
    struct rx__config {
        struct {
    #define RX__CONFIG_MEMBER(type, name) type name;
            RX__CONFIG_MEMBERS
    #undef RX__CONFIG_MEMBER
        } value;
        struct {
    #define RX__CONFIG_MEMBER(type, name) int name;
            RX__CONFIG_MEMBERS
    #undef RX__CONFIG_MEMBER
        } defined;
    };

    #define RX__CONFIG_MEMBER_IS_DEFINED(data, name) data->defined . name
    #define RX__CONFIG_MEMBER_ACCESS_VALUE(data, name) data->value . name
    #define RX__CONFIG_MEMBER_ASSIGN(x) out.value x; out.defined x ? 1 : 1;
#else
    struct rx__config {
    #define RX__CONFIG_MEMBER(type, name) type name; int name##_defined;
        RX__CONFIG_MEMBERS
    #undef RX__CONFIG_MEMBER
    };

    #define RX__CONFIG_MEMBER_IS_DEFINED(data, name) data->name##_defined
    #define RX__CONFIG_MEMBER_ACCESS_VALUE(data, name) data->name
    #define RX__CONFIG_MEMBER_ASSIGN(x) x, 1,
#endif

#define RX__CONFIG_INITIALIZE_STATIC_HAS_VARIADIC_ARGS(...)                    \
    RX__EXPAND(RX__ARG(                                                        \
        __VA_ARGS__,                                                           \
        1, 1, 1, 1, 1, 1, 1, 1,                                                \
        1, 1, 1, 1, 1, 1, 1, 0,))

#define RX__CONFIG_INITIALIZE_STATIC_0(id)                                     \
    RX__STRUCT_INITIALIZE_STATIC_0(                                            \
        id, struct rx__config, RX__CONFIG_MEMBER_ASSIGN)

#define RX__CONFIG_INITIALIZE_STATIC_1(id, ...)                                \
    RX__STRUCT_INITIALIZE_STATIC_1(                                            \
        id, struct rx__config, RX__CONFIG_MEMBER_ASSIGN, __VA_ARGS__)

#define RX__CONFIG_INITIALIZE_STATIC(...)                                      \
    RX__EXPAND(                                                                \
        RX__CONCAT(                                                            \
            RX__CONFIG_INITIALIZE_STATIC_,                                     \
            RX__CONFIG_INITIALIZE_STATIC_HAS_VARIADIC_ARGS(__VA_ARGS__)        \
        )(__VA_ARGS__))

/* Fixture Macros                                                  O-(''Q)
   -------------------------------------------------------------------------- */

struct rx__fixture_desc {
    rx_set_up_fn set_up;
    rx_tear_down_fn tear_down;
};

#define RX__FIXTURE_HAS_VARIADIC_ARGS(...)                                     \
    RX__EXPAND(RX__ARG(                                                        \
        __VA_ARGS__,                                                           \
        1, 1, 1, 1, 1, 1, 1, 1,                                                \
        1, 1, 1, 1, 1, 1, 0, 0,))

#define RX__FIXTURE_(id, type)                                                 \
    typedef type RX__FIXTURE_GET_DATA_TYPE(id);

#define RX__FIXTURE_0(id, type)                                                \
    RX__STRUCT_INITIALIZE_STATIC(RX__FIXTURE_DESC_GET_ID(id),                  \
                                 struct rx__fixture_desc,                      \
                                 RX__STRUCT_DEFAULT_MEMBER_ASSIGN);            \
    RX__FIXTURE_(id, type)

#define RX__FIXTURE_1(id, type, ...)                                           \
    RX__STRUCT_INITIALIZE_STATIC(RX__FIXTURE_DESC_GET_ID(id),                  \
                                 struct rx__fixture_desc,                      \
                                 RX__STRUCT_DEFAULT_MEMBER_ASSIGN,             \
                                 __VA_ARGS__);                                 \
    RX__FIXTURE_(id, type)

#define RX__FIXTURE_VOID_HAS_VARIADIC_ARGS(...)                                \
    RX__EXPAND(RX__ARG(                                                        \
        __VA_ARGS__,                                                           \
        1, 1, 1, 1, 1, 1, 1, 1,                                                \
        1, 1, 1, 1, 1, 1, 1, 0,))

#define RX__FIXTURE_VOID_0(id)                                                 \
    RX__FIXTURE_0(id, void *)

#define RX__FIXTURE_VOID_1(id, ...)                                            \
    RX__FIXTURE_1(id, void *, __VA_ARGS__)

/* Test Suite Macros                                               O-(''Q)
   -------------------------------------------------------------------------- */

struct rx__test_suite_desc {
    const char *name;
    const struct rx__config *config;
};

#define RX__TEST_SUITE_HAS_VARIADIC_ARGS(...)                                  \
    RX__EXPAND(RX__ARG(                                                        \
        __VA_ARGS__,                                                           \
        1, 1, 1, 1, 1, 1, 1, 1,                                                \
        1, 1, 1, 1, 1, 1, 0, 0,))

#define RX__TEST_SUITE_(id)                                                    \
    static const struct rx__test_suite_desc RX__TEST_SUITE_DESC_GET_ID(id)     \
        = {#id, &RX__TEST_SUITE_CONFIG_GET_ID(id)};                            \
    RX__TEST_SUITE_DESC_DEFINE_PTR(id)

#define RX__TEST_SUITE_0(id)                                                   \
    RX__CONFIG_INITIALIZE_STATIC(                                              \
        RX__TEST_SUITE_CONFIG_GET_ID(id));                                     \
    RX_TEST_SUITE_(id);

#define RX__TEST_SUITE_1(id, ...)                                              \
    RX__CONFIG_INITIALIZE_STATIC(                                              \
        RX__TEST_SUITE_CONFIG_GET_ID(id), __VA_ARGS__);                        \
    RX_TEST_SUITE_(id);

/* Test Case Macros                                                O-(''Q)
   -------------------------------------------------------------------------- */

struct rx__test_case_desc {
    const char *name;
    const char *suite_name;
    void *data;
    const struct rx__fixture_desc *fixture;
    const struct rx__config *config;
    const rx_test_case_run_fn run;
};

#define RX__TEST_CASE_HAS_VARIADIC_ARGS(...)                                   \
    RX__EXPAND(RX__ARG(                                                        \
        __VA_ARGS__,                                                           \
        1, 1, 1, 1, 1, 1, 1, 1,                                                \
        1, 1, 1, 1, 1, 1, 0, 0,))

#define RX__TEST_CASE_(suite_id, id, type, data, fixture)                      \
    static void RX__TEST_CASE_GET_ID(suite_id, id)(RX__DEFINE_PARAMS(type));   \
    static const struct rx__test_case_desc                                     \
    RX__TEST_CASE_DESC_GET_ID(suite_id, id)                                    \
        = {#id,                                                                \
           #suite_id,                                                          \
           data,                                                               \
           fixture,                                                            \
           &RX__TEST_CASE_CONFIG_GET_ID(suite_id, id),                         \
           (rx_test_case_run_fn)RX__TEST_CASE_GET_ID(suite_id, id)};           \
    RX__TEST_CASE_DESC_DEFINE_PTR(suite_id, id);                               \
    static void RX__TEST_CASE_GET_ID(suite_id, id)(RX__DEFINE_PARAMS(type))

#define RX__TEST_CASE_0(suite_id, id)                                          \
    RX__CONFIG_INITIALIZE_STATIC(                                              \
        RX__TEST_CASE_CONFIG_GET_ID(suite_id, id));                            \
    RX__TEST_CASE_(suite_id, id, void *, NULL, NULL)

#define RX__TEST_CASE_1(suite_id, id, ...)                                     \
    RX__CONFIG_INITIALIZE_STATIC(                                              \
        RX__TEST_CASE_CONFIG_GET_ID(suite_id, id), __VA_ARGS__);               \
    RX__TEST_CASE_(suite_id, id, void *, NULL, NULL)

#define RX__TEST_CASE_FIXTURE_HAS_VARIADIC_ARGS(...)                           \
    RX__EXPAND(RX__ARG(                                                        \
        __VA_ARGS__,                                                           \
        1, 1, 1, 1, 1, 1, 1, 1,                                                \
        1, 1, 1, 1, 1, 0, 0, 0,))

#define RX__TEST_CASE_FIXTURE_(suite_id, id, fixture)                          \
    static RX__FIXTURE_GET_DATA_TYPE(fixture)                                  \
    RX__TEST_CASE_DATA_GET_ID(suite_id, id);                                   \

#define RX__TEST_CASE_FIXTURE_0(suite_id, id, fixture)                         \
    RX__CONFIG_INITIALIZE_STATIC(                                              \
        RX__TEST_CASE_CONFIG_GET_ID(suite_id, id));                            \
    RX__TEST_CASE_FIXTURE_(suite_id, id, fixture)                              \
    RX__TEST_CASE_(suite_id,                                                   \
                   id,                                                         \
                   RX__FIXTURE_GET_DATA_TYPE(fixture),                         \
                   &RX__TEST_CASE_DATA_GET_ID(suite_id, id),                   \
                   &RX__FIXTURE_DESC_GET_ID(fixture))

#define RX__TEST_CASE_FIXTURE_1(suite_id, id, fixture, ...)                    \
    RX__CONFIG_INITIALIZE_STATIC(                                              \
        RX__TEST_CASE_CONFIG_GET_ID(suite_id, id));                            \
    RX__TEST_CASE_FIXTURE_(suite_id, id, fixture)                              \
    RX__TEST_CASE_(suite_id,                                                   \
                   id,                                                         \
                   RX__FIXTURE_GET_DATA_TYPE(fixture),                         \
                   &RX__TEST_CASE_DATA_GET_ID(suite_id, id),                   \
                   &RX__FIXTURE_DESC_GET_ID(fixture),                          \
                   __VA_ARGS__)

/* Operators                                                       O-(''Q)
   -------------------------------------------------------------------------- */

#if defined(__GNUC__)
    #define RX__REAL_OP_EQUAL(out, a, b)                                       \
        _Pragma("GCC diagnostic push")                                         \
        _Pragma("GCC diagnostic ignored \"-Wfloat-equal\"")                    \
        (out) = (a) == (b);                                                    \
        _Pragma("GCC diagnostic pop")

    #define RX__REAL_OP_NOT_EQUAL(out, a, b)                                   \
        _Pragma("GCC diagnostic push")                                         \
        _Pragma("GCC diagnostic ignored \"-Wfloat-equal\"")                    \
        (out) = (a) != (b);                                                    \
        _Pragma("GCC diagnostic pop")
#else
    #define RX__REAL_OP_EQUAL(out, a, b) (out) = (a) == (b);
    #define RX__REAL_OP_NOT_EQUAL(out, a, b) (out) = (a) != (b);
#endif

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

/* Strings                                                         O-(''Q)
   -------------------------------------------------------------------------- */

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

static enum rx_status
rx__str_create(char **s, const char *fmt, ...)
{
    enum rx_status status;
    va_list args;
    int size;

    RX_ASSERT(s != NULL);
    RX_ASSERT(fmt != NULL);

    va_start(args, fmt);
    size = vsnprintf(NULL, 0, fmt, args);
    va_end(args);
    if (size < 0) {
        RX__LOG_TRACE("invalid string formatting\n");
        return RX_ERROR;
    }

    *s = (char *)RX_MALLOC(sizeof **s * ((size_t)size + 1));
    if (*s == NULL) {
        RX__LOG_TRACE("failed to allocate the string (%zu bytes)\n",
                      sizeof **s * ((size_t)size + 1));
        return RX_ERROR_ALLOCATION;
    }

    va_start(args, fmt);
    size = vsnprintf(*s, (size_t)size + 1, fmt, args);
    va_end(args);
    if (size < 0) {
        RX__LOG_TRACE("unexpected string formatting error\n");
        status = RX_ERROR;
        goto undo;
    }

    return RX_SUCCESS;

undo:
    RX_ASSERT(status != RX_SUCCESS);

    RX_FREE(*s);
    return status;
}

static enum rx_status
rx__str_copy(char **s, const char *original)
{
    size_t size;

    size = strlen(original) + 1;

    *s = (char *)RX_MALLOC(sizeof **s * size);
    if (*s == NULL) {
        RX__LOG_TRACE("failed to allocate the string (%zu bytes)\n",
                      sizeof **s * size);
        return RX_ERROR_ALLOCATION;
    }

    memcpy(*s, original, size);
    return RX_SUCCESS;
}

/* Helpers                                                         O-(''Q)
   -------------------------------------------------------------------------- */

static int
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

    out = strcmp(aa->name, bb->name);
    if (out != 0) {
        return out;
    }

    return out;
}

static void
rx__real_are_almost_equal(int *result, rx__real a, rx__real b, rx__real tol)
{
    rx__real diff;

    diff = fabsl(a - b);
    if (diff <= tol) {
        *result = 1;
        return;
    }

    *result = diff <= fmaxl(fabsl(a), fabsl(b)) * tol;
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

RX__MAYBE_UNUSED RX__SCOPE enum rx_status
rx__test_cases_run(size_t test_case_count, struct rx_test_case *test_cases)
{
    size_t i;
    enum rx_status status;
    struct rx_summary summary;

    if (test_case_count == 0) {
        RX__LOG_INFO("nothing to run\n");
        status = RX_SUCCESS;
        goto exit;
    }

    RX_ASSERT(test_cases != NULL);

    for (i = 0; i < test_case_count; ++i) {
        const struct rx_test_case *test_case;

        test_case = &test_cases[i];

        RX_ASSERT(test_case->name != NULL);
        RX_ASSERT(test_case->suite_name != NULL);

        status = rx_summary_initialize(&summary, test_case);
        if (status != RX_SUCCESS) {
            RX__LOG_ERROR("failed to initialize the summary "
                          "(suite: \"%s\", case: \"%s\")\n",
                          test_case->suite_name,
                          test_case->name);
            goto exit;
        }

        status = rx_test_case_run(&summary, test_case);
        if (status != RX_SUCCESS) {
            RX__LOG_ERROR("failed to run a test case "
                          "(suite: \"%s\", case: \"%s\")\n",
                          test_case->suite_name,
                          test_case->name);
            goto summary_undo;
        }

        rx_summary_print(&summary);
        rx_summary_terminate(&summary);
    }

    goto exit;

summary_undo:
    rx_summary_terminate(&summary);

exit:
    return status;
}

/* Test assessments                                                O-(''Q)
   -------------------------------------------------------------------------- */

RX__MAYBE_UNUSED static void
rx__assess_test(struct rx_context *context,
                int value,
                int expected,
                const char *failure_msg,
                const char *file,
                int line,
                enum rx_severity severity)
{
    int result;

    RX_ASSERT(context != NULL);
    RX_ASSERT(failure_msg != NULL);
    RX_ASSERT(file != NULL);

    result = ((value && expected) || (!value && !expected));

    if (rx_handle_test_result(
            context, result, file, line, severity, failure_msg, NULL)
        != RX_SUCCESS) {
        RX__LOG_TRACE("failed to handle the test result for the test "
                      "located at %s:%d\n",
                      file,
                      line);
    }

    if (!result && severity == RX_FATAL) {
        rx_abort_test(context);
    }
}

RX__MAYBE_UNUSED static void
rx__assess_bool_test(struct rx_context *context,
                     int x,
                     int expected,
                     const char *expr,
                     const char *file,
                     int line,
                     enum rx_severity severity)
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
        if (rx__str_create(&failure_msg,
                           "‘%s’ is expected to be %s",
                           expr,
                           expected ? "true" : "false")
            != RX_SUCCESS) {
            RX__LOG_TRACE("failed to create the failure message for the "
                          "boolean test located at %s:%d\n",
                          file,
                          line);
            failure_msg = NULL;
        }

        if (rx__str_create(&diagnostic_msg, "%d == %d", x, expected)
            != RX_SUCCESS) {
            RX__LOG_TRACE("failed to create the diagnostic message for the "
                          "boolean test located at %s:%d\n",
                          file,
                          line);
            diagnostic_msg = NULL;
        }
    }

    if (rx_handle_test_result(
            context, result, file, line, severity, failure_msg, diagnostic_msg)
        != RX_SUCCESS) {
        RX__LOG_TRACE("failed to handle the test result for the boolean test "
                      "located at %s:%d\n",
                      file,
                      line);
    }

    if (failure_msg != NULL) {
        RX_FREE(failure_msg);
    }

    if (diagnostic_msg != NULL) {
        RX_FREE(diagnostic_msg);
    }

    if (!result && severity == RX_FATAL) {
        rx_abort_test(context);
    }
}

RX__MAYBE_UNUSED static void
rx__assess_int_comparison_test(struct rx_context *context,
                               rx__int x1,
                               rx__int x2,
                               enum rx__op op,
                               const char *expr1,
                               const char *expr2,
                               const char *file,
                               int line,
                               enum rx_severity severity)
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
            break;
    }

    if (result) {
        failure_msg = NULL;
        diagnostic_msg = NULL;
    } else {
        const char *op_name;
        const char *op_symbol;

        rx__op_get_name(&op_name, op);
        rx__op_get_symbol(&op_symbol, op);

        if (rx__str_create(&failure_msg,
                           "‘%s’ is expected to be %s ‘%s’",
                           expr1,
                           op_name,
                           expr2)
            != RX_SUCCESS) {
            RX__LOG_TRACE("failed to create the failure message for the "
                          "integer comparison test located at %s:%d\n",
                          file,
                          line);
            failure_msg = NULL;
        }

        if (rx__str_create(&diagnostic_msg, "%jd %s %jd", x1, op_symbol, x2)
            != RX_SUCCESS) {
            RX__LOG_TRACE("failed to create the diagnostic message for the "
                          "integer comparison test located at %s:%d\n",
                          file,
                          line);
            diagnostic_msg = NULL;
        }
    }

    if (rx_handle_test_result(
            context, result, file, line, severity, failure_msg, diagnostic_msg)
        != RX_SUCCESS) {
        RX__LOG_TRACE("failed to handle the test result for the integer "
                      "comparison test located at %s:%d\n",
                      file,
                      line);
    }

    if (failure_msg != NULL) {
        RX_FREE(failure_msg);
    }

    if (diagnostic_msg != NULL) {
        RX_FREE(diagnostic_msg);
    }

    if (!result && severity == RX_FATAL) {
        rx_abort_test(context);
    }
}

RX__MAYBE_UNUSED static void
rx__assess_uint_comparison_test(struct rx_context *context,
                                rx__uint x1,
                                rx__uint x2,
                                enum rx__op op,
                                const char *expr1,
                                const char *expr2,
                                const char *file,
                                int line,
                                enum rx_severity severity)
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
            break;
    }

    if (result) {
        failure_msg = NULL;
        diagnostic_msg = NULL;
    } else {
        const char *op_name;
        const char *op_symbol;

        rx__op_get_name(&op_name, op);
        rx__op_get_symbol(&op_symbol, op);

        if (rx__str_create(&failure_msg,
                           "‘%s’ is expected to be %s ‘%s’",
                           expr1,
                           op_name,
                           expr2)
            != RX_SUCCESS) {
            RX__LOG_TRACE("failed to create the failure message for the "
                          "unsigned integer comparison test located at %s:%d\n",
                          file,
                          line);
            failure_msg = NULL;
        }

        if (rx__str_create(&diagnostic_msg, "%ju %s %ju", x1, op_symbol, x2)
            != RX_SUCCESS) {
            RX__LOG_TRACE("failed to create the diagnostic message for the "
                          "unsigned integer comparison test located at %s:%d\n",
                          file,
                          line);
            diagnostic_msg = NULL;
        }
    }

    if (rx_handle_test_result(
            context, result, file, line, severity, failure_msg, diagnostic_msg)
        != RX_SUCCESS) {
        RX__LOG_TRACE("failed to handle the test result for the unsigned "
                      "integer comparison test located at %s:%d\n",
                      file,
                      line);
    }

    if (failure_msg != NULL) {
        RX_FREE(failure_msg);
    }

    if (diagnostic_msg != NULL) {
        RX_FREE(diagnostic_msg);
    }

    if (!result && severity == RX_FATAL) {
        rx_abort_test(context);
    }
}

RX__MAYBE_UNUSED static void
rx__assess_real_comparison_test(struct rx_context *context,
                                rx__real x1,
                                rx__real x2,
                                enum rx__op op,
                                const char *expr1,
                                const char *expr2,
                                const char *file,
                                int line,
                                enum rx_severity severity)
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
            RX__REAL_OP_EQUAL(result, x1, x2);
            break;
        case RX__OP_NOT_EQUAL:
            RX__REAL_OP_NOT_EQUAL(result, x1, x2);
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
            break;
    }

    if (result) {
        failure_msg = NULL;
        diagnostic_msg = NULL;
    } else {
        const char *op_name;
        const char *op_symbol;

        rx__op_get_name(&op_name, op);
        rx__op_get_symbol(&op_symbol, op);

        if (rx__str_create(&failure_msg,
                           "‘%s’ is expected to be %s ‘%s’",
                           expr1,
                           op_name,
                           expr2)
            != RX_SUCCESS) {
            RX__LOG_TRACE("failed to create the failure message for the real "
                          "comparison test located at %s:%d\n",
                          file,
                          line);
            failure_msg = NULL;
        }

        if (rx__str_create(&diagnostic_msg, "%Lf %s %Lf", x1, op_symbol, x2)
            != RX_SUCCESS) {
            RX__LOG_TRACE("failed to create the diagnostic message for the "
                          "real comparison test located at %s:%d\n",
                          file,
                          line);
            diagnostic_msg = NULL;
        }
    }

    if (rx_handle_test_result(
            context, result, file, line, severity, failure_msg, diagnostic_msg)
        != RX_SUCCESS) {
        RX__LOG_TRACE("failed to handle the test result for the real "
                      "comparison test located at %s:%d\n",
                      file,
                      line);
    }

    if (failure_msg != NULL) {
        RX_FREE(failure_msg);
    }

    if (diagnostic_msg != NULL) {
        RX_FREE(diagnostic_msg);
    }

    if (!result && severity == RX_FATAL) {
        rx_abort_test(context);
    }
}

RX__MAYBE_UNUSED static void
rx__assess_real_fuzzy_comparison_test(struct rx_context *context,
                                      rx__real x1,
                                      rx__real x2,
                                      rx__real tol,
                                      enum rx__op op,
                                      const char *expr1,
                                      const char *expr2,
                                      const char *file,
                                      int line,
                                      enum rx_severity severity)
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
            rx__real_are_almost_equal(&result, x1, x2, tol);
            break;
        case RX__OP_NOT_EQUAL:
            rx__real_are_almost_equal(&result, x1, x2, tol);
            result = !result;
            break;
        default:
            RX_ASSERT(0);
            break;
    }

    if (result) {
        failure_msg = NULL;
        diagnostic_msg = NULL;
    } else {
        const char *op_name;
        const char *op_symbol;

        rx__op_get_name(&op_name, op);
        rx__op_get_symbol(&op_symbol, op);

        if (rx__str_create(&failure_msg,
                           "‘%s’ is expected to be almost %s ‘%s’ "
                           "(tolerance: %Lf)",
                           expr1,
                           op_name,
                           expr2,
                           tol)
            != RX_SUCCESS) {
            RX__LOG_TRACE("failed to create the failure message for the real "
                          "almost equal test located at %s:%d\n",
                          file,
                          line);
            failure_msg = NULL;
        }

        if (rx__str_create(&diagnostic_msg, "%Lf %s %Lf", x1, op_symbol, x2)
            != RX_SUCCESS) {
            RX__LOG_TRACE("failed to create the diagnostic message for the "
                          "real almost equal test located at %s:%d\n",
                          file,
                          line);
            diagnostic_msg = NULL;
        }
    }

    if (rx_handle_test_result(
            context, result, file, line, severity, failure_msg, diagnostic_msg)
        != RX_SUCCESS) {
        RX__LOG_TRACE("failed to handle the test result for the real "
                      "almost equal test located at %s:%d\n",
                      file,
                      line);
    }

    if (failure_msg != NULL) {
        RX_FREE(failure_msg);
    }

    if (diagnostic_msg != NULL) {
        RX_FREE(diagnostic_msg);
    }

    if (!result && severity == RX_FATAL) {
        rx_abort_test(context);
    }
}

RX__MAYBE_UNUSED static void
rx__assess_str_comparison_test(struct rx_context *context,
                               const char *s1,
                               const char *s2,
                               enum rx__str_case str_case,
                               enum rx__op op,
                               const char *expr1,
                               const char *expr2,
                               const char *file,
                               int line,
                               enum rx_severity severity)
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
            break;
    }

    if (result) {
        failure_msg = NULL;
        diagnostic_msg = NULL;
    } else {
        const char *str_case_type;
        const char *op_name;
        const char *op_symbol;

        rx__str_case_get_type(&str_case_type, str_case);
        rx__op_get_name(&op_name, op);
        rx__op_get_symbol(&op_symbol, op);

        if (rx__str_create(&failure_msg,
                           "‘%s’ is expected to be %s ‘%s’ (case: %s)",
                           expr1,
                           op_name,
                           expr2,
                           str_case_type)
            != RX_SUCCESS) {
            RX__LOG_TRACE("failed to create the failure message for the "
                          "string comparison test located at %s:%d\n",
                          file,
                          line);
            failure_msg = NULL;
        }

        if (rx__str_create(
                &diagnostic_msg, "\"%s\" %s \"%s\"", s1, op_symbol, s2)
            != RX_SUCCESS) {
            RX__LOG_TRACE("failed to create the diagnostic message for the "
                          "string comparison test located at %s:%d\n",
                          file,
                          line);
            diagnostic_msg = NULL;
        }
    }

    if (rx_handle_test_result(
            context, result, file, line, severity, failure_msg, diagnostic_msg)
        != RX_SUCCESS) {
        RX__LOG_TRACE("failed to handle the test result for the string "
                      "comparison test located at %s:%d\n",
                      file,
                      line);
    }

    if (failure_msg != NULL) {
        RX_FREE(failure_msg);
    }

    if (diagnostic_msg != NULL) {
        RX_FREE(diagnostic_msg);
    }

    if (!result && severity == RX_FATAL) {
        rx_abort_test(context);
    }
}

/* Public API                                                      O-(''Q)
   -------------------------------------------------------------------------- */

RX__MAYBE_UNUSED RX__SCOPE void
rx_abort_test(struct rx_context *context)
{
    longjmp(context->env, 1);
}

RX__MAYBE_UNUSED RX__SCOPE enum rx_status
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

    RX_ASSERT(context != NULL);
    RX_ASSERT(context->summary != NULL);
    RX_ASSERT(context->summary->failures != NULL);
    RX_ASSERT(file != NULL);

    summary = context->summary;

    ++summary->test_count;

    if (result) {
        return RX_SUCCESS;
    }

    status
        = rx__test_failure_array_extend_back(&failure, &summary->failures, 1);
    if (status != RX_SUCCESS) {
        RX__LOG_ERROR("failed to extend the test failure array for the test "
                      "located at %s:%d\n",
                      file,
                      line);
        return status;
    }

    rx__test_failure_array_get_size(&summary->failure_count, summary->failures);

    {
        char *buf;

        status = rx__str_copy(&buf, file);
        if (status != RX_SUCCESS) {
            RX__LOG_WARNING("failed to allocate the file name for the "
                            "test located at %s:%d\n",
                            file,
                            line);
        }

        failure->file = buf;
    }

    failure->line = line;
    failure->severity = severity;

    if (failure_msg == NULL) {
        failure->msg = NULL;
    } else {
        char *buf;

        status = rx__str_copy(&buf, failure_msg);
        if (status != RX_SUCCESS) {
            RX__LOG_WARNING("failed to allocate the failure message for the "
                            "test located at %s:%d\n",
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
            RX__LOG_WARNING("failed to allocate the diagnostic message for the "
                            "test located at %s:%d\n",
                            file,
                            line);
            failure->diagnostic_msg = NULL;
        } else {
            failure->diagnostic_msg = buf;
        }
    }

    return RX_SUCCESS;
}

RX__MAYBE_UNUSED RX__SCOPE enum rx_status
rx_summary_initialize(struct rx_summary *summary,
                      const struct rx_test_case *test_case)
{
    enum rx_status status;

    RX_ASSERT(summary != NULL);
    RX_ASSERT(test_case != NULL);

    summary->test_case = test_case;
    summary->test_count = 0;
    summary->elapsed = 0;

    status = rx__test_failure_array_create(&summary->failures, 0);
    if (status != RX_SUCCESS) {
        RX__LOG_ERROR("failed to create the test failure array "
                      "(suite: \"%s\", case: \"%s\")\n",
                      test_case->suite_name,
                      test_case->name);
        return status;
    }

    rx__test_failure_array_get_size(&summary->failure_count, summary->failures);
    RX_ASSERT(summary->failure_count == 0);
    return RX_SUCCESS;
}

RX__MAYBE_UNUSED RX__SCOPE void
rx_summary_terminate(struct rx_summary *summary)
{
    size_t i;

    RX_ASSERT(summary != NULL);
    RX_ASSERT(summary->failures != NULL);

    for (i = 0; i < summary->failure_count; ++i) {
        const struct rx_failure *failure;

        failure = &summary->failures[i];

        RX_FREE((void *)(uintptr_t)failure->file);

        if (failure->msg != NULL) {
            RX_FREE((void *)(uintptr_t)failure->msg);
        }

        if (failure->diagnostic_msg != NULL) {
            RX_FREE((void *)(uintptr_t)failure->diagnostic_msg);
        }
    }

    rx__test_failure_array_destroy(summary->failures);
}

RX__MAYBE_UNUSED RX__SCOPE void
rx_summary_print(const struct rx_summary *summary)
{
    size_t i;
    int passed;
    const char *style_begin;
    const char *style_end;

    RX_ASSERT(summary != NULL);
    RX_ASSERT(summary->test_case != NULL);
    RX_ASSERT(summary->test_case->name != NULL);
    RX_ASSERT(summary->test_case->suite_name != NULL);
    RX_ASSERT(summary->failures != NULL);

    passed = summary->failure_count == 0;

#if RX__LOG_STYLING
    if (isatty(fileno(stderr))) {
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

RX__MAYBE_UNUSED RX__SCOPE enum rx_status
rx_test_case_run(struct rx_summary *summary,
                 const struct rx_test_case *test_case)
{
    struct rx_context context;
    uint64_t time_begin;
    uint64_t time_end;

    RX_ASSERT(summary != NULL);
    RX_ASSERT(test_case != NULL);
    RX_ASSERT(test_case->name != NULL);
    RX_ASSERT(test_case->suite_name != NULL);
    RX_ASSERT(test_case->run != NULL);

    context.summary = summary;

    if (test_case->set_up != NULL) {
        enum rx_status status;

        status = test_case->set_up(&context, test_case->data);
        if (status != RX_SUCCESS) {
            RX__LOG_ERROR("failed to set-up the fixture "
                          "(suite: \"%s\", case: \"%s\")\n",
                          test_case->suite_name,
                          test_case->name);
            return status;
        }
    }

    if (rx__get_real_time(&time_begin) != RX_SUCCESS) {
        time_begin = (uint64_t)-1;
    }

    if (setjmp(context.env) == 0) {
        test_case->run(&context, test_case->data);
    }

    if (time_begin == (uint64_t)-1
        || rx__get_real_time(&time_end) != RX_SUCCESS) {
        RX__LOG_WARNING("failed to measure the time elapsed "
                        "(suite: \"%s\", case: \"%s\")\n",
                        test_case->suite_name,
                        test_case->name);
        summary->elapsed = 0;
    } else {
        RX_ASSERT(time_end >= time_begin);
        summary->elapsed = time_end - time_begin;
    }

    if (test_case->tear_down != NULL) {
        test_case->tear_down(&context, test_case->data);
    }

    return RX_SUCCESS;
}

RX__MAYBE_UNUSED RX__SCOPE void
rx_test_cases_enumerate(size_t *test_case_count,
                        struct rx_test_case *test_cases)
{
    size_t count;
    const struct rx__test_case_desc **c_it;

    if (test_cases == NULL) {
        RX_ASSERT(test_case_count != NULL);

        *test_case_count = 0;
        for (c_it = &RX__TEST_CASE_SECTION_BEGIN;
             c_it != &RX__TEST_CASE_SECTION_END;
             ++c_it) {
            if (*c_it != NULL) {
                ++(*test_case_count);
            }
        }

        return;
    }

    count = 0;
    for (c_it = &RX__TEST_CASE_SECTION_BEGIN;
         c_it != &RX__TEST_CASE_SECTION_END;
         ++c_it) {
        const struct rx__test_suite_desc **s_it;
        struct rx_test_case *test_case;

        if (*c_it == NULL) {
            continue;
        }

        for (s_it = &RX__TEST_SUITE_SECTION_BEGIN;
             s_it != &RX__TEST_SUITE_SECTION_END;
             ++s_it) {
            if (*s_it == NULL) {
                continue;
            }

            if (strcmp((*s_it)->name, (*c_it)->suite_name) == 0) {
                break;
            }
        }

        test_case = &test_cases[count];

        test_case->name = (*c_it)->name;
        test_case->suite_name = (*c_it)->suite_name;
        test_case->data = (*c_it)->data;
        test_case->run = (*c_it)->run;

        if ((*c_it)->fixture == NULL) {
            test_case->set_up = NULL;
            test_case->tear_down = NULL;
        } else {
            test_case->set_up = (*c_it)->fixture->set_up;
            test_case->tear_down = (*c_it)->fixture->tear_down;
        }

        if (s_it != &RX__TEST_SUITE_SECTION_END) {
#define RX__CONFIG_MEMBER(type, name)                                          \
    if (RX__CONFIG_MEMBER_IS_DEFINED((*s_it)->config, name)) {                 \
        test_case->name                                                        \
            = RX__CONFIG_MEMBER_ACCESS_VALUE((*s_it)->config, name);           \
    }
            RX__CONFIG_MEMBERS
#undef RX__CONFIG_MEMBER
        }

#define RX__CONFIG_MEMBER(type, name)                                          \
    if (RX__CONFIG_MEMBER_IS_DEFINED((*c_it)->config, name)) {                 \
        test_case->name                                                        \
            = RX__CONFIG_MEMBER_ACCESS_VALUE((*c_it)->config, name);           \
    }
        RX__CONFIG_MEMBERS
#undef RX__CONFIG_MEMBER

        ++count;
    }

    /* Objects that are defined in a custom memory section can only be retrieved
       in an undefined order, so these need to be manually sorted afterwards
       in a sensible way.
    */
    qsort(test_cases, count, sizeof *test_cases, rx__compare_test_cases);
}

RX__MAYBE_UNUSED RX__SCOPE enum rx_status
rx_run(int argc,
       const char **argv,
       size_t test_case_count,
       struct rx_test_case *test_cases)
{
    enum rx_status out;

    RX__UNUSED(argc);
    RX__UNUSED(argv);

    if (test_cases != NULL) {
        return rx__test_cases_run(test_case_count, test_cases);
    }

    rx_test_cases_enumerate(&test_case_count, NULL);
    if (test_case_count == 0) {
        return rx__test_cases_run(0, NULL);
    }

    test_cases = (struct rx_test_case *)RX_MALLOC(sizeof *test_cases
                                                  * test_case_count);
    if (test_cases == NULL) {
        RX__LOG_TRACE("failed to allocate the test cases\n");
        return RX_ERROR_ALLOCATION;
    }

    rx_test_cases_enumerate(&test_case_count, test_cases);
    out = rx__test_cases_run(test_case_count, test_cases);
    RX_FREE(test_cases);
    return out;
}

#endif /* REXO_REXO_H */
