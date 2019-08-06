/*
   The MIT License (MIT)

   Copyright (c) 2018 Christopher Crouzet

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

#ifdef RX_STATIC
#define RXP_SCOPE static
#else
#define RXP_SCOPE extern
#endif /* RX_STATIC */

#ifdef __GNUC__
#define RXP_MAYBE_UNUSED __attribute__((unused))
#else
#define RXP_MAYBE_UNUSED
#endif

struct RxContext;

#define RXP_FALSE ((int)0)
#define RXP_TRUE ((int)1)

#define RXP_TEST_CASE_PARAMETERS struct RxContext *context RXP_MAYBE_UNUSED

#define RX_TEST_CASE(name) static void name(RXP_TEST_CASE_PARAMETERS)

#define RX_FIXTURE context->fixture

#define RXP_DEFINE_TEST(condition, expected, msg, severity)                    \
    rxpAssessTest(context,                                                     \
                  !!(condition),                                               \
                  expected,                                                    \
                  msg,                                                         \
                  __FILE__,                                                    \
                  __LINE__,                                                    \
                  severity)

#define RXP_DEFINE_BOOL_TEST(condition, expected, severity)                    \
    rxpAssessBoolTest(context,                                                 \
                      !!(condition),                                           \
                      expected,                                                \
                      #condition,                                              \
                      __FILE__,                                                \
                      __LINE__,                                                \
                      severity)

#define RXP_DEFINE_INT_TEST(x1, x2, op, severity)                              \
    rxpAssessIntComparisonTest(context,                                        \
                               (x1),                                           \
                               (x2),                                           \
                               op,                                             \
                               #x1,                                            \
                               #x2,                                            \
                               __FILE__,                                       \
                               __LINE__,                                       \
                               severity)

#define RXP_DEFINE_UINT_TEST(x1, x2, op, severity)                             \
    rxpAssessUIntComparisonTest(context,                                       \
                               (x1),                                           \
                               (x2),                                           \
                               op,                                             \
                               #x1,                                            \
                               #x2,                                            \
                               __FILE__,                                       \
                               __LINE__,                                       \
                               severity)

#define RXP_DEFINE_REAL_TEST(x1, x2, op, severity)                             \
    rxpAssessRealComparisonTest(context,                                       \
                                (x1),                                          \
                                (x2),                                          \
                                op,                                            \
                                #x1,                                           \
                                #x2,                                           \
                                __FILE__,                                      \
                                __LINE__,                                      \
                                severity)

#define RXP_DEFINE_REAL_FUZZY_TEST(x1, x2, tol, op, severity)                  \
    rxpAssessRealFuzzyComparisonTest(context,                                  \
                                     (x1),                                     \
                                     (x2),                                     \
                                     (tol),                                    \
                                     op,                                       \
                                     #x1,                                      \
                                     #x2,                                      \
                                     __FILE__,                                 \
                                     __LINE__,                                 \
                                     severity)
#define RXP_DEFINE_STR_TEST(s1, s2, strCase, op, severity)                     \
    rxpAssessStrComparisonTest(context,                                        \
                               (s1),                                           \
                               (s2),                                           \
                               strCase,                                        \
                               op,                                             \
                               #s1,                                            \
                               #s2,                                            \
                               __FILE__,                                       \
                               __LINE__,                                       \
                               severity)

#define RX_REQUIRE(condition, msg)                                             \
    RXP_DEFINE_TEST(condition, RXP_TRUE, msg, RX_FATAL)

#define RX_REQUIRE_TRUE(condition)                                             \
    RXP_DEFINE_BOOL_TEST(condition, RXP_TRUE, RX_FATAL)

#define RX_REQUIRE_FALSE(condition)                                            \
    RXP_DEFINE_BOOL_TEST(condition, RXP_FALSE, RX_FATAL)

#define RX_REQUIRE_INT_EQUAL(x1, x2)                                           \
    RXP_DEFINE_INT_TEST(x1, x2, RXP_OP_EQUAL, RX_FATAL)

#define RX_REQUIRE_INT_NOT_EQUAL(x1, x2)                                       \
    RXP_DEFINE_INT_TEST(x1, x2, RXP_OP_NOT_EQUAL, RX_FATAL)

#define RX_REQUIRE_INT_GREATER(x1, x2)                                         \
    RXP_DEFINE_INT_TEST(x1, x2, RXP_OP_GREATER, RX_FATAL)

#define RX_REQUIRE_INT_LESSER(x1, x2)                                          \
    RXP_DEFINE_INT_TEST(x1, x2, RXP_OP_LESSER, RX_FATAL)

#define RX_REQUIRE_INT_GREATER_OR_EQUAL(x1, x2)                                \
    RXP_DEFINE_INT_TEST(x1, x2, RXP_OP_GREATER_OR_EQUAL, RX_FATAL)

#define RX_REQUIRE_INT_LESSER_OR_EQUAL(x1, x2)                                 \
    RXP_DEFINE_INT_TEST(x1, x2, RXP_OP_LESSER_OR_EQUAL, RX_FATAL)

#define RX_REQUIRE_UINT_EQUAL(x1, x2)                                          \
    RXP_DEFINE_UINT_TEST(x1, x2, RXP_OP_EQUAL, RX_FATAL)

#define RX_REQUIRE_UINT_NOT_EQUAL(x1, x2)                                      \
    RXP_DEFINE_UINT_TEST(x1, x2, RXP_OP_NOT_EQUAL, RX_FATAL)

#define RX_REQUIRE_UINT_GREATER(x1, x2)                                        \
    RXP_DEFINE_UINT_TEST(x1, x2, RXP_OP_GREATER, RX_FATAL)

#define RX_REQUIRE_UINT_LESSER(x1, x2)                                         \
    RXP_DEFINE_UINT_TEST(x1, x2, RXP_OP_LESSER, RX_FATAL)

#define RX_REQUIRE_UINT_GREATER_OR_EQUAL(x1, x2)                               \
    RXP_DEFINE_UINT_TEST(x1, x2, RXP_OP_GREATER_OR_EQUAL, RX_FATAL)

#define RX_REQUIRE_UINT_LESSER_OR_EQUAL(x1, x2)                                \
    RXP_DEFINE_UINT_TEST(x1, x2, RXP_OP_LESSER_OR_EQUAL, RX_FATAL)

#define RX_REQUIRE_REAL_EQUAL(x1, x2)                                          \
    RXP_DEFINE_REAL_TEST(x1, x2, RXP_OP_EQUAL, RX_FATAL)

#define RX_REQUIRE_REAL_NOT_EQUAL(x1, x2)                                      \
    RXP_DEFINE_REAL_TEST(x1, x2, RXP_OP_NOT_EQUAL, RX_FATAL)

#define RX_REQUIRE_REAL_GREATER(x1, x2)                                        \
    RXP_DEFINE_REAL_TEST(x1, x2, RXP_OP_GREATER, RX_FATAL)

#define RX_REQUIRE_REAL_LESSER(x1, x2)                                         \
    RXP_DEFINE_REAL_TEST(x1, x2, RXP_OP_LESSER, RX_FATAL)

#define RX_REQUIRE_REAL_GREATER_OR_EQUAL(x1, x2)                               \
    RXP_DEFINE_REAL_TEST(x1, x2, RXP_OP_GREATER_OR_EQUAL, RX_FATAL)

#define RX_REQUIRE_REAL_LESSER_OR_EQUAL(x1, x2)                                \
    RXP_DEFINE_REAL_TEST(x1, x2, RXP_OP_LESSER_OR_EQUAL, RX_FATAL)

#define RX_REQUIRE_REAL_ALMOST_EQUAL(x1, x2, tol)                              \
    RXP_DEFINE_REAL_FUZZY_TEST(x1, x2, tol, RXP_OP_EQUAL, RX_FATAL)

#define RX_REQUIRE_REAL_NOT_ALMOST_EQUAL(x1, x2, tol)                          \
    RXP_DEFINE_REAL_FUZZY_TEST(x1, x2, tol, RXP_OP_NOT_EQUAL, RX_FATAL)

#define RX_REQUIRE_STR_EQUAL(s1, s2)                                           \
    RXP_DEFINE_STR_TEST(s1, s2, RXP_STR_CASE_OBEY, RXP_OP_EQUAL, RX_FATAL)

#define RX_REQUIRE_STR_NOT_EQUAL(s1, s2)                                       \
    RXP_DEFINE_STR_TEST(s1, s2, RXP_STR_CASE_OBEY, RXP_OP_NOT_EQUAL, RX_FATAL)

#define RX_REQUIRE_STR_EQUAL_NO_CASE(s1, s2)                                   \
    RXP_DEFINE_STR_TEST(s1, s2, RXP_STR_CASE_IGNORE, RXP_OP_EQUAL, RX_FATAL)

#define RX_REQUIRE_STR_NOT_EQUAL_NO_CASE(s1, s2)                               \
    RXP_DEFINE_STR_TEST(s1, s2, RXP_STR_CASE_IGNORE, RXP_OP_NOT_EQUAL, RX_FATAL)

#define RX_CHECK(condition, msg)                                               \
    RXP_DEFINE_TEST(condition, RXP_TRUE, msg, RX_NONFATAL)

#define RX_CHECK_TRUE(condition)                                               \
    RXP_DEFINE_BOOL_TEST(condition, RXP_TRUE, RX_NONFATAL)

#define RX_CHECK_FALSE(condition)                                              \
    RXP_DEFINE_BOOL_TEST(condition, RXP_FALSE, RX_NONFATAL)

#define RX_CHECK_INT_EQUAL(x1, x2)                                             \
    RXP_DEFINE_INT_TEST(x1, x2, RXP_OP_EQUAL, RX_NONFATAL)

#define RX_CHECK_INT_NOT_EQUAL(x1, x2)                                         \
    RXP_DEFINE_INT_TEST(x1, x2, RXP_OP_NOT_EQUAL, RX_NONFATAL)

#define RX_CHECK_INT_GREATER(x1, x2)                                           \
    RXP_DEFINE_INT_TEST(x1, x2, RXP_OP_GREATER, RX_NONFATAL)

#define RX_CHECK_INT_LESSER(x1, x2)                                            \
    RXP_DEFINE_INT_TEST(x1, x2, RXP_OP_LESSER, RX_NONFATAL)

#define RX_CHECK_INT_GREATER_OR_EQUAL(x1, x2)                                  \
    RXP_DEFINE_INT_TEST(x1, x2, RXP_OP_GREATER_OR_EQUAL, RX_NONFATAL)

#define RX_CHECK_INT_LESSER_OR_EQUAL(x1, x2)                                   \
    RXP_DEFINE_INT_TEST(x1, x2, RXP_OP_LESSER_OR_EQUAL, RX_NONFATAL)

#define RX_CHECK_UINT_EQUAL(x1, x2)                                            \
    RXP_DEFINE_UINT_TEST(x1, x2, RXP_OP_EQUAL, RX_NONFATAL)

#define RX_CHECK_UINT_NOT_EQUAL(x1, x2)                                        \
    RXP_DEFINE_UINT_TEST(x1, x2, RXP_OP_NOT_EQUAL, RX_NONFATAL)

#define RX_CHECK_UINT_GREATER(x1, x2)                                          \
    RXP_DEFINE_UINT_TEST(x1, x2, RXP_OP_GREATER, RX_NONFATAL)

#define RX_CHECK_UINT_LESSER(x1, x2)                                           \
    RXP_DEFINE_UINT_TEST(x1, x2, RXP_OP_LESSER, RX_NONFATAL)

#define RX_CHECK_UINT_GREATER_OR_EQUAL(x1, x2)                                 \
    RXP_DEFINE_UINT_TEST(x1, x2, RXP_OP_GREATER_OR_EQUAL, RX_NONFATAL)

#define RX_CHECK_UINT_LESSER_OR_EQUAL(x1, x2)                                  \
    RXP_DEFINE_UINT_TEST(x1, x2, RXP_OP_LESSER_OR_EQUAL, RX_NONFATAL)

#define RX_CHECK_REAL_EQUAL(x1, x2)                                            \
    RXP_DEFINE_REAL_TEST(x1, x2, RXP_OP_EQUAL, RX_NONFATAL)

#define RX_CHECK_REAL_NOT_EQUAL(x1, x2)                                        \
    RXP_DEFINE_REAL_TEST(x1, x2, RXP_OP_NOT_EQUAL, RX_NONFATAL)

#define RX_CHECK_REAL_GREATER(x1, x2)                                          \
    RXP_DEFINE_REAL_TEST(x1, x2, RXP_OP_GREATER, RX_NONFATAL)

#define RX_CHECK_REAL_LESSER(x1, x2)                                           \
    RXP_DEFINE_REAL_TEST(x1, x2, RXP_OP_LESSER, RX_NONFATAL)

#define RX_CHECK_REAL_GREATER_OR_EQUAL(x1, x2)                                 \
    RXP_DEFINE_REAL_TEST(x1, x2, RXP_OP_GREATER_OR_EQUAL, RX_NONFATAL)

#define RX_CHECK_REAL_LESSER_OR_EQUAL(x1, x2)                                  \
    RXP_DEFINE_REAL_TEST(x1, x2, RXP_OP_LESSER_OR_EQUAL, RX_NONFATAL)

#define RX_CHECK_REAL_ALMOST_EQUAL(x1, x2, tol)                                \
    RXP_DEFINE_REAL_FUZZY_TEST(x1, x2, tol, RXP_OP_EQUAL, RX_NONFATAL)

#define RX_CHECK_REAL_NOT_ALMOST_EQUAL(x1, x2, tol)                            \
    RXP_DEFINE_REAL_FUZZY_TEST(x1, x2, tol, RXP_OP_NOT_EQUAL, RX_NONFATAL)

#define RX_CHECK_STR_EQUAL(s1, s2)                                             \
    RXP_DEFINE_STR_TEST(s1, s2, RXP_STR_CASE_OBEY, RXP_OP_EQUAL, RX_NONFATAL)

#define RX_CHECK_STR_NOT_EQUAL(s1, s2)                                         \
    RXP_DEFINE_STR_TEST(                                                       \
        s1, s2, RXP_STR_CASE_OBEY, RXP_OP_NOT_EQUAL, RX_NONFATAL)

#define RX_CHECK_STR_EQUAL_NO_CASE(s1, s2)                                     \
    RXP_DEFINE_STR_TEST(s1, s2, RXP_STR_CASE_IGNORE, RXP_OP_EQUAL, RX_NONFATAL)

#define RX_CHECK_STR_NOT_EQUAL_NO_CASE(s1, s2)                                 \
    RXP_DEFINE_STR_TEST(                                                       \
        s1, s2, RXP_STR_CASE_IGNORE, RXP_OP_NOT_EQUAL, RX_NONFATAL)

enum RxStatus {
    RX_SUCCESS = 0,
    RX_ERROR = -1,
    RX_ERROR_INVALID_VALUE = -2,
    RX_ERROR_ALLOCATION = -3,
    RX_ERROR_MAX_SIZE_EXCEEDED = -4
};

enum RxSeverity { RX_NONFATAL = 0, RX_FATAL = 1 };

typedef void (*RxTestCaseRunFn)(RXP_TEST_CASE_PARAMETERS);
typedef enum RxStatus (*RxTestSuiteSetUpFn)(void **fixture);
typedef void (*RxTestSuiteTearDownFn)(void *fixture);

struct RxTestCase {
    const char *name;
    RxTestCaseRunFn run;
};

struct RxTestSuite {
    const char *name;
    size_t testCaseCount;
    const struct RxTestCase *testCases;
    RxTestSuiteSetUpFn setUp;
    RxTestSuiteTearDownFn tearDown;
};

struct RxFailure {
    const char *file;
    int line;
    enum RxSeverity severity;
    const char *msg;
    const char *diagnosticMsg;
};

struct RxSummary {
    const struct RxTestSuite *testSuite;
    const struct RxTestCase *testCase;
    size_t testCount;
    size_t failureCount;
    struct RxFailure *failures;
    uint64_t elapsed;
};

struct RxContext {
    jmp_buf env;
    struct RxSummary *summary;
    void *fixture;
};

#ifdef __cplusplus
extern "C" {
#endif

RXP_SCOPE void
rxAbortTest(struct RxContext *context);

RXP_SCOPE enum RxStatus
rxHandleTestResult(struct RxContext *context,
                   int result,
                   const char *file,
                   int line,
                   enum RxSeverity severity,
                   const char *failureMsg,
                   const char *diagnosticMsg);

RXP_SCOPE enum RxStatus
rxSummaryInitialize(struct RxSummary *summary,
                    const struct RxTestSuite *testSuite,
                    const struct RxTestCase *testCase);

RXP_SCOPE void
rxSummaryTerminate(struct RxSummary *summary);

RXP_SCOPE void
rxSummaryPrint(const struct RxSummary *summary);

RXP_SCOPE enum RxStatus
rxTestCaseRun(struct RxSummary *summary,
              const struct RxTestCase *testCase,
              const struct RxTestSuite *testSuite);

RXP_SCOPE enum RxStatus
rxRun(size_t suiteCount,
      const struct RxTestSuite *testSuites,
      int argc,
      const char **argv);

#ifdef __cplusplus
}
#endif

/* Internal Implementation                                         O-(''Q)
   -------------------------------------------------------------------------- */

#if defined(RX_ENABLE_DEBUGGING)                                               \
    || (!defined(RX_DISABLE_DEBUGGING)                                         \
        && (defined(DEBUG) || !defined(NDEBUG)))
#define RXP_DEBUGGING 1
#else
#define RXP_DEBUGGING 0
#endif

#if defined(_WIN32)
#define RXP_PLATFORM_WINDOWS
#elif defined(__unix__) || defined(__APPLE__)
#define RXP_PLATFORM_UNIX
#if defined(__APPLE__)
#define RXP_PLATFORM_DARWIN
#elif defined(__linux__)
#define RXP_PLATFORM_LINUX
#endif
#endif

#ifndef RX_ASSERT
#include <assert.h>
#define RX_ASSERT assert
#endif /* RX_ASSERT */

#ifndef RX_MALLOC
#include <stdlib.h>
#define RX_MALLOC malloc
#endif /* RX_MALLOC */

#ifndef RX_REALLOC
#include <stdlib.h>
#define RX_REALLOC realloc
#endif /* RX_REALLOC */

#ifndef RX_FREE
#include <stdlib.h>
#define RX_FREE free
#endif /* RX_FREE */

#define RXP_UNUSED(x) (void)(x)

/* Logger                                                          O-(''Q)
   -------------------------------------------------------------------------- */

#if !defined(RX_DISABLE_LOG_STYLING) && defined(RXP_PLATFORM_UNIX)             \
    && defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 1
#include <unistd.h>
#define RXP_LOG_STYLING 1
#else
#define RXP_LOG_STYLING 0
#endif

#if defined(RX_SET_LOGGING_LEVEL_DEBUG)
#define RXP_LOGGING_LEVEL RXP_LOG_LEVEL_DEBUG
#elif defined(RX_SET_LOGGING_LEVEL_TRACE)
#define RXP_LOGGING_LEVEL RXP_LOG_LEVEL_TRACE
#elif defined(RX_SET_LOGGING_LEVEL_INFO)
#define RXP_LOGGING_LEVEL RXP_LOG_LEVEL_INFO
#elif defined(RX_SET_LOGGING_LEVEL_WARNING)
#define RXP_LOGGING_LEVEL RXP_LOG_LEVEL_WARNING
#elif defined(RX_SET_LOGGING_LEVEL_ERROR)
#define RXP_LOGGING_LEVEL RXP_LOG_LEVEL_ERROR
#elif defined(RX_ENABLE_DEBUGGING)                                             \
    || (!defined(RX_DISABLE_DEBUGGING)                                         \
        && (defined(DEBUG) || !defined(NDEBUG)))
#define RXP_LOGGING_LEVEL RXP_LOG_LEVEL_DEBUG
#else
#define RXP_LOGGING_LEVEL RXP_LOG_LEVEL_WARNING
#endif

#ifdef RX_DISABLE_LOGGING
#define RXP_LOGGING 0
#else
#define RXP_LOGGING 1
#endif /* RX_DISABLE_LOGGING */

#ifndef RX_LOG
#define RX_LOG(level, ...)                                                     \
    do {                                                                       \
        if (RXP_LOGGING && level <= RXP_LOGGING_LEVEL) {                       \
            rxpLog(level, __FILE__, __LINE__, __VA_ARGS__);                    \
        }                                                                      \
    } while (0)
#endif /* RX_LOG */

#define RXP_LOG_DEBUG(...) RX_LOG(RXP_LOG_LEVEL_DEBUG, __VA_ARGS__)

#define RXP_LOG_TRACE(...) RX_LOG(RXP_LOG_LEVEL_TRACE, __VA_ARGS__)

#define RXP_LOG_INFO(...) RX_LOG(RXP_LOG_LEVEL_INFO, __VA_ARGS__)

#define RXP_LOG_WARNING(...) RX_LOG(RXP_LOG_LEVEL_WARNING, __VA_ARGS__)

#define RXP_LOG_ERROR(...) RX_LOG(RXP_LOG_LEVEL_ERROR, __VA_ARGS__)

enum RxpLogLevel {
    RXP_LOG_LEVEL_ERROR = 0,
    RXP_LOG_LEVEL_WARNING = 1,
    RXP_LOG_LEVEL_INFO = 2,
    RXP_LOG_LEVEL_TRACE = 3,
    RXP_LOG_LEVEL_DEBUG = 4
};

#if RXP_LOG_STYLING
enum RxpLogStyle {
    RXP_LOG_STYLE_RESET = 0,
    RXP_LOG_STYLE_BLACK = 1,
    RXP_LOG_STYLE_RED = 2,
    RXP_LOG_STYLE_GREEN = 3,
    RXP_LOG_STYLE_YELLOW = 4,
    RXP_LOG_STYLE_BLUE = 5,
    RXP_LOG_STYLE_MAGENTA = 6,
    RXP_LOG_STYLE_CYAN = 7,
    RXP_LOG_STYLE_BRIGHT_BLACK = 8,
    RXP_LOG_STYLE_BRIGHT_RED = 9,
    RXP_LOG_STYLE_BRIGHT_GREEN = 10,
    RXP_LOG_STYLE_BRIGHT_YELLOW = 11,
    RXP_LOG_STYLE_BRIGHT_BLUE = 12,
    RXP_LOG_STYLE_BRIGHT_MAGENTA = 13,
    RXP_LOG_STYLE_BRIGHT_CYAN = 14
};
#endif /* RXP_LOG_STYLING */

static void
rxpLogLevelGetName(const char **name, enum RxpLogLevel level)
{
    RX_ASSERT(name != NULL);

    switch (level) {
        case RXP_LOG_LEVEL_ERROR:
            *name = "error";
            return;
        case RXP_LOG_LEVEL_WARNING:
            *name = "warning";
            return;
        case RXP_LOG_LEVEL_INFO:
            *name = "info";
            return;
        case RXP_LOG_LEVEL_TRACE:
            *name = "trace";
            return;
        case RXP_LOG_LEVEL_DEBUG:
            *name = "debug";
            return;
        default:
            RX_ASSERT(0);
    }
}

#if RXP_LOG_STYLING
static void
rxpLogLevelGetStyle(enum RxpLogStyle *style, enum RxpLogLevel level)
{
    RX_ASSERT(style != NULL);

    switch (level) {
        case RXP_LOG_LEVEL_ERROR:
            *style = RXP_LOG_STYLE_BRIGHT_RED;
            return;
        case RXP_LOG_LEVEL_WARNING:
            *style = RXP_LOG_STYLE_BRIGHT_YELLOW;
            return;
        case RXP_LOG_LEVEL_INFO:
            *style = RXP_LOG_STYLE_BRIGHT_GREEN;
            return;
        case RXP_LOG_LEVEL_TRACE:
            *style = RXP_LOG_STYLE_BRIGHT_CYAN;
            return;
        case RXP_LOG_LEVEL_DEBUG:
            *style = RXP_LOG_STYLE_BRIGHT_MAGENTA;
            return;
        default:
            RX_ASSERT(0);
    };
}

static void
rxpLogStyleGetAnsiCode(const char **code, enum RxpLogStyle style)
{
    RX_ASSERT(code != NULL);

    switch (style) {
        case RXP_LOG_STYLE_RESET:
            *code = "\x1b[0m";
            return;
        case RXP_LOG_STYLE_BLACK:
            *code = "\x1b[30m";
            return;
        case RXP_LOG_STYLE_RED:
            *code = "\x1b[31m";
            return;
        case RXP_LOG_STYLE_GREEN:
            *code = "\x1b[32m";
            return;
        case RXP_LOG_STYLE_YELLOW:
            *code = "\x1b[33m";
            return;
        case RXP_LOG_STYLE_BLUE:
            *code = "\x1b[34m";
            return;
        case RXP_LOG_STYLE_MAGENTA:
            *code = "\x1b[35m";
            return;
        case RXP_LOG_STYLE_CYAN:
            *code = "\x1b[36m";
            return;
        case RXP_LOG_STYLE_BRIGHT_BLACK:
            *code = "\x1b[1;30m";
            return;
        case RXP_LOG_STYLE_BRIGHT_RED:
            *code = "\x1b[1;31m";
            return;
        case RXP_LOG_STYLE_BRIGHT_GREEN:
            *code = "\x1b[1;32m";
            return;
        case RXP_LOG_STYLE_BRIGHT_YELLOW:
            *code = "\x1b[1;33m";
            return;
        case RXP_LOG_STYLE_BRIGHT_BLUE:
            *code = "\x1b[1;34m";
            return;
        case RXP_LOG_STYLE_BRIGHT_MAGENTA:
            *code = "\x1b[1;35m";
            return;
        case RXP_LOG_STYLE_BRIGHT_CYAN:
            *code = "\x1b[1;36m";
            return;
        default:
            RX_ASSERT(0);
    }
}
#endif /* RXP_LOG_STYLING */

static void
rxpLog(enum RxpLogLevel level, const char *file, int line, const char *fmt, ...)
{
    const char *levelName;
    const char *levelStyleStart;
    const char *levelStyleEnd;
    va_list args;

    RX_ASSERT(file != NULL);
    RX_ASSERT(fmt != NULL);

    rxpLogLevelGetName(&levelName, level);

#if RXP_LOG_STYLING
    if (isatty(fileno(stderr))) {
        enum RxpLogStyle levelStyle;

        rxpLogLevelGetStyle(&levelStyle, level);
        rxpLogStyleGetAnsiCode(&levelStyleStart, levelStyle);
        rxpLogStyleGetAnsiCode(&levelStyleEnd, RXP_LOG_STYLE_RESET);
    } else {
        levelStyleStart = levelStyleEnd = "";
    }
#else
    levelStyleStart = levelStyleEnd = "";
#endif /* RXP_LOG_STYLING */

    va_start(args, fmt);
    fprintf(stderr,
            "%s:%d: %s%s%s: ",
            file,
            line,
            levelStyleStart,
            levelName,
            levelStyleEnd);
    vfprintf(stderr, fmt, args);
    va_end(args);
}

/* Timer                                                           O-(''Q)
   -------------------------------------------------------------------------- */

#define RXP_TICKS_PER_SECOND 1000000000ull

#if defined(RXP_PLATFORM_WINDOWS)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#elif defined(RXP_PLATFORM_DARWIN)
#include <mach/mach_time.h>
#include <sys/resource.h>
#elif defined(RXP_PLATFORM_UNIX)
#include <sys/resource.h>
#if defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 199309L
#include <time.h>
#define RXP_USE_CLOCK_GETTIME
#if defined(CLOCK_MONOTONIC_RAW)
#define RXP_CLOCK_ID CLOCK_MONOTONIC_RAW
#elif defined(CLOCK_MONOTONIC)
#define RXP_CLOCK_ID CLOCK_MONOTONIC
#else
#define RXP_CLOCK_ID CLOCK_REALTIME
#endif
#else
#include <sys/time.h>
#endif
#else
typedef char rxp_unsupported_platform[-1];
#endif

static enum RxStatus
rxpGetRealTime(uint64_t *time)
{
    RX_ASSERT(time != NULL);

#if defined(RXP_PLATFORM_WINDOWS)
    {
        static double timeToNano;
        LARGE_INTEGER counter;

        if (timeToNano == 0.0) {
            LARGE_INTEGER frequency;

            if (!QueryPerformanceFrequency(&frequency)) {
                RXP_LOG_ERROR("failed to retrieve the time's frequency\n");
                return RX_ERROR;
            }

            timeToNano = (double)RXP_TICKS_PER_SECOND / frequency.QuadPart;
        }

        if (!QueryPerformanceCounter(&counter)) {
            RXP_LOG_ERROR("failed to retrieve the current time\n");
            return RX_ERROR;
        }

        *time = counter.QuadPart * timeToNano;
        return RX_SUCCESS;
    }
#elif defined(RXP_PLATFORM_DARWIN)
    /*
       Since Darwin 5.2, `clock_gettime()` can return high resolution times
       with the `CLOCK_UPTIME_RAW` clock but it internally only calls
       `mach_absolute_time()` with the overhead of converting the result into
       the `timespec` format.
    */
    {
        static double timeToNano;

        if (timeToNano == 0.0) {
            mach_timebase_info_data_t info;

            if (mach_timebase_info(&info) != KERN_SUCCESS) {
                RXP_LOG_ERROR("failed to retrieve the current time\n");
                return RX_ERROR;
            }

            timeToNano = (double)info.numer / info.denom;
        }

        *time = mach_absolute_time() * timeToNano;
        return RX_SUCCESS;
    }
#elif defined(RXP_PLATFORM_UNIX)
#if defined(RXP_USE_CLOCK_GETTIME)
    {
        struct timespec t;

        if (clock_gettime(RXP_CLOCK_ID, &t) != 0) {
            RXP_LOG_ERROR("failed to retrieve the current time\n");
            return RX_ERROR;
        }

        *time = (uint64_t)t.tv_sec * 1000000000ull + (uint64_t)t.tv_nsec;
        return RX_SUCCESS;
    }
#else
    {
        struct timeval t;

        if (gettimeofday(&t, NULL) != 0) {
            RXP_LOG_ERROR("failed to retrieve the current time\n");
            return RX_ERROR;
        }

        *time = (uint64_t)t.tv_sec * 1000000000ull
                + (uint64_t)t.tv_usec * 1000ull;
        return RX_SUCCESS;
    }
#endif
#endif

    RXP_LOG_ERROR("platform not supported\n");
    return RX_ERROR;
}

/* Dynamic Array                                                   O-(''Q)
   -------------------------------------------------------------------------- */

#define RXP_DYNARRAY_GET_BLOCK(buffer)                                         \
    ((void *)&((struct RxpDynArrayHeader *)(buffer))[-1])
#define RXP_DYNARRAY_GET_HEADER(block)                                         \
    ((struct RxpDynArrayHeader *)(block))
#define RXP_DYNARRAY_GET_BUFFER(block)                                         \
    ((void *)&((struct RxpDynArrayHeader *)(block))[1])
#define RXP_DYNARRAY_GET_CONST_BLOCK(buffer)                                   \
    ((const void *)&((const struct RxpDynArrayHeader *)(buffer))[-1])
#define RXP_DYNARRAY_GET_CONST_HEADER(block)                                   \
    ((const struct RxpDynArrayHeader *)(block))

struct RxpDynArrayHeader {
    size_t size;
    size_t capacity;
};

static const size_t rxpTestFailureArrayMaxCapacity
    = (((size_t)-1 - sizeof(struct RxpDynArrayHeader))
       / sizeof(struct RxFailure));

static void
rxpDynArrayGetNewCapacity(size_t *newCapacity,
                          size_t current,
                          size_t requested,
                          size_t max)
{
    *newCapacity = current + current / 2 + 1;
    if (*newCapacity < current) {
        *newCapacity = max;
        return;
    }

    if (*newCapacity < requested) {
        *newCapacity = requested;
    }
}

static enum RxStatus
rxpDynArrayEnsureHasEnoughCapacity(void **block,
                                   size_t currentCapacity,
                                   size_t requestedCapacity,
                                   size_t maxCapacity,
                                   size_t elementSize)
{
    void *buffer;
    size_t newCapacity;

    RX_ASSERT(block != NULL);
    RX_ASSERT(elementSize > 0);

    if (requestedCapacity > maxCapacity) {
        RXP_LOG_TRACE("the requested capacity is too large\n");
        return RX_ERROR_MAX_SIZE_EXCEEDED;
    }

    if (*block != NULL && currentCapacity >= requestedCapacity) {
        return RX_SUCCESS;
    }

    rxpDynArrayGetNewCapacity(
        &newCapacity, currentCapacity, requestedCapacity, maxCapacity);
    RX_ASSERT(newCapacity >= requestedCapacity);
    RX_ASSERT(newCapacity <= maxCapacity);

    buffer = RX_REALLOC(
        *block, sizeof(struct RxpDynArrayHeader) + elementSize * newCapacity);
    if (buffer == NULL) {
        RXP_LOG_TRACE("failed to reallocate the block\n");
        return RX_ERROR_ALLOCATION;
    }

    RXP_DYNARRAY_GET_HEADER(buffer)->capacity = newCapacity;
    *block = buffer;
    return RX_SUCCESS;
}

static enum RxStatus
rxpTestFailureArrayCreate(struct RxFailure **array, size_t size)
{
    enum RxStatus status;
    void *block;

    RX_ASSERT(array != NULL);

    block = NULL;

    status = rxpDynArrayEnsureHasEnoughCapacity(&block,
                                                0,
                                                (size_t)size,
                                                rxpTestFailureArrayMaxCapacity,
                                                sizeof(struct RxFailure));
    if (status != RX_SUCCESS) {
        RXP_LOG_ERROR("failed to reserve a large enough capacity for the "
                      "test failure array (requested capacity: %zu)\n",
                      (size_t)size);
        return status;
    }

    RX_ASSERT(block != NULL);

    RXP_DYNARRAY_GET_HEADER(block)->size = (size_t)size;
    RXP_DYNARRAY_GET_HEADER(block)->capacity = (size_t)size;
    *array = (struct RxFailure *)RXP_DYNARRAY_GET_BUFFER(block);
    return RX_SUCCESS;
}

static void
rxpTestFailureArrayDestroy(struct RxFailure *array)
{
    if (array == NULL) {
        return;
    }

    RX_FREE(RXP_DYNARRAY_GET_BLOCK(array));
}

static void
rxpTestFailureArrayGetSize(size_t *size, const struct RxFailure *array)
{
    RX_ASSERT(array != NULL);

    *size = (size_t)RXP_DYNARRAY_GET_CONST_HEADER(
        RXP_DYNARRAY_GET_CONST_BLOCK(array))->size;
}

static enum RxStatus
rxpTestFailureArrayExtendBack(struct RxFailure **slice,
                              struct RxFailure **array,
                              size_t size)
{
    enum RxStatus status;
    void *block;
    size_t pos;

    RX_ASSERT(array != NULL);
    RX_ASSERT(*array != NULL);

    block = RXP_DYNARRAY_GET_BLOCK(*array);
    status = rxpDynArrayEnsureHasEnoughCapacity(
        &block,
        RXP_DYNARRAY_GET_HEADER(block)->capacity,
        RXP_DYNARRAY_GET_HEADER(block)->size + (size_t)size,
        rxpTestFailureArrayMaxCapacity,
        sizeof(struct RxFailure));
    if (status != RX_SUCCESS) {
        RXP_LOG_ERROR("failed to reserve a large enough capacity for the "
                      "test failure array (requested capacity: %zu)\n",
                      RXP_DYNARRAY_GET_HEADER(block)->size + (size_t)size);
        return status;
    }

    RX_ASSERT(block != NULL);

    *array = (struct RxFailure *)RXP_DYNARRAY_GET_BUFFER(block);

    pos = (size_t)RXP_DYNARRAY_GET_HEADER(block)->size;
    memmove(&(*array)[(size_t)pos + (size_t)size],
            &(*array)[(size_t)pos],
            sizeof(struct RxFailure)
                * (RXP_DYNARRAY_GET_HEADER(block)->size - (size_t)pos));

    if (slice != NULL) {
        *slice = &(*array)[pos];
    }

    RXP_DYNARRAY_GET_HEADER(block)->size += (size_t)size;
    return RX_SUCCESS;
}

/* Internals                                                       O-(''Q)
   -------------------------------------------------------------------------- */

enum RxpOp {
    RXP_OP_EQUAL = 0,
    RXP_OP_NOT_EQUAL = 1,
    RXP_OP_GREATER = 2,
    RXP_OP_LESSER = 3,
    RXP_OP_GREATER_OR_EQUAL = 4,
    RXP_OP_LESSER_OR_EQUAL = 5
};

enum RxpStrCase { RXP_STR_CASE_OBEY = 0, RXP_STR_CASE_IGNORE = 1 };

typedef intmax_t RxpInt;
typedef uintmax_t RxpUInt;
typedef long double RxpReal;

static void
rxpOpGetSymbol(const char **symbol, enum RxpOp op)
{
    RX_ASSERT(symbol != NULL);

    switch (op) {
        case RXP_OP_EQUAL:
            *symbol = "==";
            return;
        case RXP_OP_NOT_EQUAL:
            *symbol = "!=";
            return;
        case RXP_OP_GREATER:
            *symbol = ">";
            return;
        case RXP_OP_LESSER:
            *symbol = "<";
            return;
        case RXP_OP_GREATER_OR_EQUAL:
            *symbol = ">=";
            return;
        case RXP_OP_LESSER_OR_EQUAL:
            *symbol = "<=";
            return;
        default:
            RX_ASSERT(0);
    }
}

static void
rxpOpGetName(const char **name, enum RxpOp op)
{
    RX_ASSERT(name != NULL);

    switch (op) {
        case RXP_OP_EQUAL:
            *name = "equal to";
            return;
        case RXP_OP_NOT_EQUAL:
            *name = "not equal to";
            return;
        case RXP_OP_GREATER:
            *name = "greater than";
            return;
        case RXP_OP_LESSER:
            *name = "less than";
            return;
        case RXP_OP_GREATER_OR_EQUAL:
            *name = "greater than or equal to";
            return;
        case RXP_OP_LESSER_OR_EQUAL:
            *name = "less than or equal to";
            return;
        default:
            RX_ASSERT(0);
    }
}

static void
rxpStrCaseGetType(const char **type, enum RxpStrCase strCase)
{
    RX_ASSERT(type != NULL);

    switch (strCase) {
        case RXP_STR_CASE_OBEY:
            *type = "obey";
            return;
        case RXP_STR_CASE_IGNORE:
            *type = "ignore";
            return;
        default:
            RX_ASSERT(0);
    }
}

static enum RxStatus
rxpStrCreate(char **s, const char *fmt, ...)
{
    enum RxStatus status;
    va_list args;
    int size;

    RX_ASSERT(s != NULL);
    RX_ASSERT(fmt != NULL);

    status = RX_SUCCESS;

    va_start(args, fmt);
    size = vsnprintf(NULL, 0, fmt, args);
    va_end(args);
    if (size < 0) {
        RXP_LOG_TRACE("invalid string formatting\n");
        status = RX_ERROR;
        goto exit;
    }

    *s = (char *)RX_MALLOC(sizeof **s * ((size_t)size + 1));
    if (*s == NULL) {
        RXP_LOG_TRACE("failed to allocate the string (%zu bytes)\n",
                      sizeof **s * ((size_t)size + 1));
        status = RX_ERROR_ALLOCATION;
        goto exit;
    }

    va_start(args, fmt);
    size = vsnprintf(*s, (size_t)size + 1, fmt, args);
    va_end(args);
    if (size < 0) {
        RXP_LOG_TRACE("unexpected string formatting error\n");
        status = RX_ERROR;
        goto undo;
    }

    RX_ASSERT(status == RX_SUCCESS);
    goto exit;

undo:
    RX_FREE(*s);

exit:
    return status;
}

static enum RxStatus
rxpStrCopy(char **s, const char *original)
{
    size_t size;

    size = strlen(original) + 1;

    *s = (char *)RX_MALLOC(sizeof **s * size);
    if (*s == NULL) {
        RXP_LOG_TRACE("failed to allocate the string (%zu bytes)\n",
                      sizeof **s * size);
        return RX_ERROR_ALLOCATION;
    }

    memcpy(*s, original, size);
    return RX_SUCCESS;
}

static void
rxpStrAreEqual(int *result, const char *a, const char *b)
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
rxpStrAreEqualNoCase(int *result, const char *a, const char *b)
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

static void
rxpRealAreAlmostEqual(int *result, RxpReal a, RxpReal b, RxpReal tol)
{
    RxpReal diff;

    diff = fabsl(a - b);
    if (diff <= tol) {
        *result = 1;
        return;
    }

    *result = diff <= fmaxl(fabsl(a), fabsl(b)) * tol;
}

RXP_MAYBE_UNUSED static void
rxpAssessTest(struct RxContext *context,
              int value,
              int expected,
              const char *failureMsg,
              const char *file,
              int line,
              enum RxSeverity severity)
{
    int result;

    RX_ASSERT(context != NULL);
    RX_ASSERT(context->summary != NULL);
    RX_ASSERT(failureMsg != NULL);
    RX_ASSERT(file != NULL);

    result = ((value && expected) || (!value && !expected));

    if (rxHandleTestResult(
            context, result, file, line, severity, failureMsg, NULL)
        != RX_SUCCESS) {
        RXP_LOG_TRACE("failed to handle the test result for the test "
                      "located at %s:%d\n",
                      file,
                      line);
    }

    if (!result && severity == RX_FATAL) {
        rxAbortTest(context);
    }
}

RXP_MAYBE_UNUSED static void
rxpAssessBoolTest(struct RxContext *context,
                  int x,
                  int expected,
                  const char *expr,
                  const char *file,
                  int line,
                  enum RxSeverity severity)
{
    int result;
    char *failureMsg;
    char *diagnosticMsg;

    RX_ASSERT(context != NULL);
    RX_ASSERT(context->summary != NULL);
    RX_ASSERT(expr != NULL);
    RX_ASSERT(file != NULL);

    result = ((x && expected) || (!x && !expected));

    if (result) {
        failureMsg = NULL;
        diagnosticMsg = NULL;
    } else {
        if (rxpStrCreate(&failureMsg,
                         "‘%s’ is expected to be %s",
                         expr,
                         expected ? "true" : "false")
            != RX_SUCCESS) {
            RXP_LOG_TRACE("failed to create the failure message for the "
                          "boolean test located at %s:%d\n",
                          file,
                          line);
            failureMsg = NULL;
        }

        if (rxpStrCreate(&diagnosticMsg, "%d == %d", x, expected)
            != RX_SUCCESS) {
            RXP_LOG_TRACE("failed to create the diagnostic message for the "
                          "boolean test located at %s:%d\n",
                          file,
                          line);
            diagnosticMsg = NULL;
        }
    }

    if (rxHandleTestResult(
            context, result, file, line, severity, failureMsg, diagnosticMsg)
        != RX_SUCCESS) {
        RXP_LOG_TRACE("failed to handle the test result for the boolean test "
                      "located at %s:%d\n",
                      file,
                      line);
    }

    if (failureMsg != NULL) {
        RX_FREE(failureMsg);
    }

    if (diagnosticMsg != NULL) {
        RX_FREE(diagnosticMsg);
    }

    if (!result && severity == RX_FATAL) {
        rxAbortTest(context);
    }
}

RXP_MAYBE_UNUSED static void
rxpAssessIntComparisonTest(struct RxContext *context,
                           RxpInt x1,
                           RxpInt x2,
                           enum RxpOp op,
                           const char *expr1,
                           const char *expr2,
                           const char *file,
                           int line,
                           enum RxSeverity severity)
{
    int result;
    char *failureMsg;
    char *diagnosticMsg;

    RX_ASSERT(context != NULL);
    RX_ASSERT(context->summary != NULL);
    RX_ASSERT(expr1 != NULL);
    RX_ASSERT(expr2 != NULL);
    RX_ASSERT(file != NULL);

    switch (op) {
        case RXP_OP_EQUAL:
            result = x1 == x2;
            break;
        case RXP_OP_NOT_EQUAL:
            result = x1 != x2;
            break;
        case RXP_OP_GREATER:
            result = x1 > x2;
            break;
        case RXP_OP_LESSER:
            result = x1 < x2;
            break;
        case RXP_OP_GREATER_OR_EQUAL:
            result = x1 >= x2;
            break;
        case RXP_OP_LESSER_OR_EQUAL:
            result = x1 <= x2;
            break;
        default:
            RX_ASSERT(0);
            break;
    }

    if (result) {
        failureMsg = NULL;
        diagnosticMsg = NULL;
    } else {
        const char *opName;
        const char *opSymbol;

        rxpOpGetName(&opName, op);
        rxpOpGetSymbol(&opSymbol, op);

        if (rxpStrCreate(&failureMsg,
                         "‘%s’ is expected to be %s ‘%s’",
                         expr1,
                         opName,
                         expr2)
            != RX_SUCCESS) {
            RXP_LOG_TRACE("failed to create the failure message for the "
                          "integer comparison test located at %s:%d\n",
                          file,
                          line);
            failureMsg = NULL;
        }

        if (rxpStrCreate(&diagnosticMsg, "%jd %s %jd", x1, opSymbol, x2)
            != RX_SUCCESS) {
            RXP_LOG_TRACE("failed to create the diagnostic message for the "
                          "integer comparison test located at %s:%d\n",
                          file,
                          line);
            diagnosticMsg = NULL;
        }
    }

    if (rxHandleTestResult(
            context, result, file, line, severity, failureMsg, diagnosticMsg)
        != RX_SUCCESS) {
        RXP_LOG_TRACE("failed to handle the test result for the integer "
                      "comparison test located at %s:%d\n",
                      file,
                      line);
    }

    if (failureMsg != NULL) {
        RX_FREE(failureMsg);
    }

    if (diagnosticMsg != NULL) {
        RX_FREE(diagnosticMsg);
    }

    if (!result && severity == RX_FATAL) {
        rxAbortTest(context);
    }
}

RXP_MAYBE_UNUSED static void
rxpAssessUIntComparisonTest(struct RxContext *context,
                            RxpUInt x1,
                            RxpUInt x2,
                            enum RxpOp op,
                            const char *expr1,
                            const char *expr2,
                            const char *file,
                            int line,
                            enum RxSeverity severity)
{
    int result;
    char *failureMsg;
    char *diagnosticMsg;

    RX_ASSERT(context != NULL);
    RX_ASSERT(context->summary != NULL);
    RX_ASSERT(expr1 != NULL);
    RX_ASSERT(expr2 != NULL);
    RX_ASSERT(file != NULL);

    switch (op) {
        case RXP_OP_EQUAL:
            result = x1 == x2;
            break;
        case RXP_OP_NOT_EQUAL:
            result = x1 != x2;
            break;
        case RXP_OP_GREATER:
            result = x1 > x2;
            break;
        case RXP_OP_LESSER:
            result = x1 < x2;
            break;
        case RXP_OP_GREATER_OR_EQUAL:
            result = x1 >= x2;
            break;
        case RXP_OP_LESSER_OR_EQUAL:
            result = x1 <= x2;
            break;
        default:
            RX_ASSERT(0);
            break;
    }

    if (result) {
        failureMsg = NULL;
        diagnosticMsg = NULL;
    } else {
        const char *opName;
        const char *opSymbol;

        rxpOpGetName(&opName, op);
        rxpOpGetSymbol(&opSymbol, op);

        if (rxpStrCreate(&failureMsg,
                         "‘%s’ is expected to be %s ‘%s’",
                         expr1,
                         opName,
                         expr2)
            != RX_SUCCESS) {
            RXP_LOG_TRACE("failed to create the failure message for the "
                          "unsigned integer comparison test located at %s:%d\n",
                          file,
                          line);
            failureMsg = NULL;
        }

        if (rxpStrCreate(&diagnosticMsg, "%ju %s %ju", x1, opSymbol, x2)
            != RX_SUCCESS) {
            RXP_LOG_TRACE("failed to create the diagnostic message for the "
                          "unsigned integer comparison test located at %s:%d\n",
                          file,
                          line);
            diagnosticMsg = NULL;
        }
    }

    if (rxHandleTestResult(
            context, result, file, line, severity, failureMsg, diagnosticMsg)
        != RX_SUCCESS) {
        RXP_LOG_TRACE("failed to handle the test result for the unsigned "
                      "integer comparison test located at %s:%d\n",
                      file,
                      line);
    }

    if (failureMsg != NULL) {
        RX_FREE(failureMsg);
    }

    if (diagnosticMsg != NULL) {
        RX_FREE(diagnosticMsg);
    }

    if (!result && severity == RX_FATAL) {
        rxAbortTest(context);
    }
}

RXP_MAYBE_UNUSED static void
rxpAssessRealComparisonTest(struct RxContext *context,
                            RxpReal x1,
                            RxpReal x2,
                            enum RxpOp op,
                            const char *expr1,
                            const char *expr2,
                            const char *file,
                            int line,
                            enum RxSeverity severity)
{
    int result;
    char *failureMsg;
    char *diagnosticMsg;

    RX_ASSERT(context != NULL);
    RX_ASSERT(context->summary != NULL);
    RX_ASSERT(expr1 != NULL);
    RX_ASSERT(expr2 != NULL);
    RX_ASSERT(file != NULL);

    switch (op) {
        case RXP_OP_EQUAL:
            result = x1 == x2;
            break;
        case RXP_OP_NOT_EQUAL:
            result = x1 != x2;
            break;
        case RXP_OP_GREATER:
            result = x1 > x2;
            break;
        case RXP_OP_LESSER:
            result = x1 < x2;
            break;
        case RXP_OP_GREATER_OR_EQUAL:
            result = x1 >= x2;
            break;
        case RXP_OP_LESSER_OR_EQUAL:
            result = x1 <= x2;
            break;
        default:
            RX_ASSERT(0);
            break;
    }

    if (result) {
        failureMsg = NULL;
        diagnosticMsg = NULL;
    } else {
        const char *opName;
        const char *opSymbol;

        rxpOpGetName(&opName, op);
        rxpOpGetSymbol(&opSymbol, op);

        if (rxpStrCreate(&failureMsg,
                         "‘%s’ is expected to be %s ‘%s’",
                         expr1,
                         opName,
                         expr2)
            != RX_SUCCESS) {
            RXP_LOG_TRACE("failed to create the failure message for the real "
                          "comparison test located at %s:%d\n",
                          file,
                          line);
            failureMsg = NULL;
        }

        if (rxpStrCreate(&diagnosticMsg, "%Lf %s %Lf", x1, opSymbol, x2)
            != RX_SUCCESS) {
            RXP_LOG_TRACE("failed to create the diagnostic message for the "
                          "real comparison test located at %s:%d\n",
                          file,
                          line);
            diagnosticMsg = NULL;
        }
    }

    if (rxHandleTestResult(
            context, result, file, line, severity, failureMsg, diagnosticMsg)
        != RX_SUCCESS) {
        RXP_LOG_TRACE("failed to handle the test result for the real "
                      "comparison test located at %s:%d\n",
                      file,
                      line);
    }

    if (failureMsg != NULL) {
        RX_FREE(failureMsg);
    }

    if (diagnosticMsg != NULL) {
        RX_FREE(diagnosticMsg);
    }

    if (!result && severity == RX_FATAL) {
        rxAbortTest(context);
    }
}

RXP_MAYBE_UNUSED static void
rxpAssessRealFuzzyComparisonTest(struct RxContext *context,
                                 RxpReal x1,
                                 RxpReal x2,
                                 RxpReal tol,
                                 enum RxpOp op,
                                 const char *expr1,
                                 const char *expr2,
                                 const char *file,
                                 int line,
                                 enum RxSeverity severity)
{
    int result;
    char *failureMsg;
    char *diagnosticMsg;

    RX_ASSERT(context != NULL);
    RX_ASSERT(context->summary != NULL);
    RX_ASSERT(expr1 != NULL);
    RX_ASSERT(expr2 != NULL);
    RX_ASSERT(file != NULL);

    switch (op) {
        case RXP_OP_EQUAL:
            rxpRealAreAlmostEqual(&result, x1, x2, tol);
            break;
        case RXP_OP_NOT_EQUAL:
            rxpRealAreAlmostEqual(&result, x1, x2, tol);
            result = !result;
            break;
        default:
            RX_ASSERT(0);
            break;
    }

    if (result) {
        failureMsg = NULL;
        diagnosticMsg = NULL;
    } else {
        const char *opName;
        const char *opSymbol;

        rxpOpGetName(&opName, op);
        rxpOpGetSymbol(&opSymbol, op);

        if (rxpStrCreate(&failureMsg,
                         "‘%s’ is expected to be almost %s ‘%s’ "
                         "(tolerance: %Lf)",
                         expr1,
                         opName,
                         expr2,
                         tol)
            != RX_SUCCESS) {
            RXP_LOG_TRACE("failed to create the failure message for the real "
                          "almost equal test located at %s:%d\n",
                          file,
                          line);
            failureMsg = NULL;
        }

        if (rxpStrCreate(&diagnosticMsg, "%Lf %s %Lf", x1, opSymbol, x2)
            != RX_SUCCESS) {
            RXP_LOG_TRACE("failed to create the diagnostic message for the "
                          "real almost equal test located at %s:%d\n",
                          file,
                          line);
            diagnosticMsg = NULL;
        }
    }

    if (rxHandleTestResult(
            context, result, file, line, severity, failureMsg, diagnosticMsg)
        != RX_SUCCESS) {
        RXP_LOG_TRACE("failed to handle the test result for the real "
                      "almost equal test located at %s:%d\n",
                      file,
                      line);
    }

    if (failureMsg != NULL) {
        RX_FREE(failureMsg);
    }

    if (diagnosticMsg != NULL) {
        RX_FREE(diagnosticMsg);
    }

    if (!result && severity == RX_FATAL) {
        rxAbortTest(context);
    }
}

RXP_MAYBE_UNUSED static void
rxpAssessStrComparisonTest(struct RxContext *context,
                           const char *s1,
                           const char *s2,
                           enum RxpStrCase strCase,
                           enum RxpOp op,
                           const char *expr1,
                           const char *expr2,
                           const char *file,
                           int line,
                           enum RxSeverity severity)
{
    int result;
    char *failureMsg;
    char *diagnosticMsg;

    RX_ASSERT(context != NULL);
    RX_ASSERT(context->summary != NULL);
    RX_ASSERT(s1 != NULL);
    RX_ASSERT(s2 != NULL);
    RX_ASSERT(expr1 != NULL);
    RX_ASSERT(expr2 != NULL);
    RX_ASSERT(file != NULL);

    switch (op) {
        case RXP_OP_EQUAL:
            strCase == RXP_STR_CASE_OBEY
                ? rxpStrAreEqual(&result, s1, s2)
                : rxpStrAreEqualNoCase(&result, s1, s2);
            break;
        case RXP_OP_NOT_EQUAL:
            strCase == RXP_STR_CASE_OBEY
                ? rxpStrAreEqual(&result, s1, s2)
                : rxpStrAreEqualNoCase(&result, s1, s2);
            result = !result;
            break;
        default:
            RX_ASSERT(0);
            break;
    }

    if (result) {
        failureMsg = NULL;
        diagnosticMsg = NULL;
    } else {
        const char *strCaseType;
        const char *opName;
        const char *opSymbol;

        rxpStrCaseGetType(&strCaseType, strCase);
        rxpOpGetName(&opName, op);
        rxpOpGetSymbol(&opSymbol, op);

        if (rxpStrCreate(&failureMsg,
                         "‘%s’ is expected to be %s ‘%s’ (case: %s)",
                         expr1,
                         opName,
                         expr2,
                         strCaseType)
            != RX_SUCCESS) {
            RXP_LOG_TRACE("failed to create the failure message for the "
                          "string comparison test located at %s:%d\n",
                          file,
                          line);
            failureMsg = NULL;
        }

        if (rxpStrCreate(&diagnosticMsg, "\"%s\" %s \"%s\"", s1, opSymbol, s2)
            != RX_SUCCESS) {
            RXP_LOG_TRACE("failed to create the diagnostic message for the "
                          "string comparison test located at %s:%d\n",
                          file,
                          line);
            diagnosticMsg = NULL;
        }
    }

    if (rxHandleTestResult(
            context, result, file, line, severity, failureMsg, diagnosticMsg)
        != RX_SUCCESS) {
        RXP_LOG_TRACE("failed to handle the test result for the string "
                      "comparison test located at %s:%d\n",
                      file,
                      line);
    }

    if (failureMsg != NULL) {
        RX_FREE(failureMsg);
    }

    if (diagnosticMsg != NULL) {
        RX_FREE(diagnosticMsg);
    }

    if (!result && severity == RX_FATAL) {
        rxAbortTest(context);
    }
}

static void
rxpTestSuiteAssert(const struct RxTestSuite *testSuite)
{
    RX_ASSERT(testSuite != NULL);
    RX_ASSERT(testSuite->name != NULL);
    RX_ASSERT(testSuite->testCaseCount == 0 || testSuite->testCases != NULL);
}

static void
rxpTestCaseAssert(const struct RxTestCase *testCase)
{
    RX_ASSERT(testCase != NULL);
    RX_ASSERT(testCase->name != NULL);
    RX_ASSERT(testCase->run != NULL);
}

static void
rxpSummaryAssert(const struct RxSummary *summary)
{
    size_t failureCount;

    RX_ASSERT(summary != NULL);
    RX_ASSERT(summary->failures != NULL);

    rxpTestFailureArrayGetSize(&failureCount, summary->failures);
    RX_ASSERT(summary->failureCount == failureCount);
}

/* Public API Implementation                                       O-(''Q)
   -------------------------------------------------------------------------- */

RXP_MAYBE_UNUSED RXP_SCOPE void
rxAbortTest(struct RxContext *context)
{
    longjmp(context->env, 1);
}

RXP_MAYBE_UNUSED RXP_SCOPE enum RxStatus
rxHandleTestResult(struct RxContext *context,
                   int result,
                   const char *file,
                   int line,
                   enum RxSeverity severity,
                   const char *failureMsg,
                   const char *diagnosticMsg)
{
    enum RxStatus status;
    struct RxSummary *summary;
    struct RxFailure *failure;

    RX_ASSERT(context != NULL);
    RX_ASSERT(file != NULL);

    rxpSummaryAssert(context->summary);

    summary = context->summary;

    ++summary->testCount;

    if (result) {
        return RX_SUCCESS;
    }

    status = rxpTestFailureArrayExtendBack(&failure, &summary->failures, 1);
    if (status != RX_SUCCESS) {
        RXP_LOG_WARNING("failed to extend the test failure array for the test "
                        "located at %s:%d\n",
                        file,
                        line);
        return status;
    }

    rxpTestFailureArrayGetSize(&summary->failureCount, summary->failures);

    {
        char *buffer;

        status = rxpStrCopy(&buffer, file);
        if (status != RX_SUCCESS) {
            RXP_LOG_ERROR("failed to allocate the file name for the "
                          "test located at %s:%d\n",
                          file,
                          line);
        }

        failure->file = buffer;
    }

    failure->line = line;
    failure->severity = severity;

    if (failureMsg == NULL) {
        failure->msg = NULL;
    } else {
        char *buffer;

        status = rxpStrCopy(&buffer, failureMsg);
        if (status != RX_SUCCESS) {
            RXP_LOG_WARNING("failed to allocate the failure message for the "
                            "test located at %s:%d\n",
                            file,
                            line);
            failure->msg = NULL;
        } else {
            failure->msg = buffer;
        }
    }

    if (diagnosticMsg == NULL) {
        failure->diagnosticMsg = NULL;
    } else {
        char *buffer;

        status = rxpStrCopy(&buffer, diagnosticMsg);
        if (status != RX_SUCCESS) {
            RXP_LOG_WARNING("failed to allocate the diagnostic message for the "
                            "test located at %s:%d\n",
                            file,
                            line);
            failure->diagnosticMsg = NULL;
        } else {
            failure->diagnosticMsg = buffer;
        }
    }

    return RX_SUCCESS;
}

RXP_MAYBE_UNUSED RXP_SCOPE enum RxStatus
rxSummaryInitialize(struct RxSummary *summary,
                    const struct RxTestSuite *testSuite,
                    const struct RxTestCase *testCase)
{
    enum RxStatus status;

    RX_ASSERT(summary != NULL);
    RX_ASSERT(testSuite != NULL);
    RX_ASSERT(testCase != NULL);

    summary->testSuite = testSuite;
    summary->testCase = testCase;
    summary->testCount = 0;
    summary->elapsed = 0;

    status = rxpTestFailureArrayCreate(&summary->failures, 0);
    if (status != RX_SUCCESS) {
        RXP_LOG_TRACE("failed to create the test failure array "
                      "(suite: \"%s\", case: \"%s\")\n",
                      testSuite->name,
                      testCase->name);
        return status;
    }

    rxpTestFailureArrayGetSize(&summary->failureCount, summary->failures);
    RX_ASSERT(summary->failureCount == 0);

    RX_ASSERT(status == RX_SUCCESS);
    return status;
}

RXP_MAYBE_UNUSED RXP_SCOPE void
rxSummaryTerminate(struct RxSummary *summary)
{
    size_t i;

    RX_ASSERT(summary != NULL);

    rxpSummaryAssert(summary);

    for (i = 0; i < summary->failureCount; ++i) {
        const struct RxFailure *failure;

        failure = &summary->failures[i];

        RX_FREE((void *)(uintptr_t)failure->file);

        if (failure->msg != NULL) {
            RX_FREE((void *)(uintptr_t)failure->msg);
        }

        if (failure->diagnosticMsg != NULL) {
            RX_FREE((void *)(uintptr_t)failure->diagnosticMsg);
        }
    }

    rxpTestFailureArrayDestroy(summary->failures);
}

RXP_MAYBE_UNUSED RXP_SCOPE void
rxSummaryPrint(const struct RxSummary *summary)
{
    size_t i;
    int passed;
    const char *styleStart;
    const char *styleEnd;

    RX_ASSERT(summary != NULL);

    rxpSummaryAssert(summary);

    passed = summary->failureCount == 0;

#if RXP_LOG_STYLING
    if (isatty(fileno(stderr))) {
        rxpLogStyleGetAnsiCode(
            &styleStart,
            passed ? RXP_LOG_STYLE_BRIGHT_GREEN : RXP_LOG_STYLE_BRIGHT_RED);
        rxpLogStyleGetAnsiCode(&styleEnd, RXP_LOG_STYLE_RESET);
    } else {
        styleStart = styleEnd = "";
    }
#else
    styleStart = styleEnd = "";
#endif /* RXP_LOG_STYLING */

    fprintf(stderr,
            "[%s%s%s] \"%s\" / \"%s\" (%f ms)\n",
            styleStart,
            passed ? "PASSED" : "FAILED",
            styleEnd,
            summary->testSuite->name,
            summary->testCase->name,
            (double)summary->elapsed * (1000.0 / RXP_TICKS_PER_SECOND));

    for (i = 0; i < summary->failureCount; ++i) {
        const struct RxFailure *failure;
        const char *failureMsg;

        failure = &summary->failures[i];
        failureMsg = failure->msg == NULL ? "" : failure->msg;

        if (failure->diagnosticMsg != NULL) {
            fprintf(stderr,
                    "%s:%d: %s test failure: %s\n%s\n",
                    failure->file,
                    failure->line,
                    failure->severity == RX_FATAL ? "fatal" : "nonfatal",
                    failureMsg,
                    failure->diagnosticMsg);
        } else {
            fprintf(stderr,
                    "%s:%d: %s test failure: %s\n",
                    failure->file,
                    failure->line,
                    failure->severity == RX_FATAL ? "fatal" : "nonfatal",
                    failureMsg);
        }
    }
}

RXP_MAYBE_UNUSED RXP_SCOPE enum RxStatus
rxTestCaseRun(struct RxSummary *summary,
              const struct RxTestCase *testCase,
              const struct RxTestSuite *testSuite)
{
    enum RxStatus status;
    struct RxContext context;
    uint64_t timeStart;
    uint64_t timeEnd;

    RX_ASSERT(summary != NULL);
    RX_ASSERT(testCase != NULL);
    RX_ASSERT(testSuite != NULL);

    rxpSummaryAssert(summary);
    rxpTestCaseAssert(testCase);
    rxpTestSuiteAssert(testSuite);

    context.summary = summary;

    if (testSuite->setUp != NULL) {
        status = testSuite->setUp(&context.fixture);
        if (status != RX_SUCCESS) {
            RXP_LOG_ERROR("failed to set-up the fixture "
                          "(suite: \"%s\", case: \"%s\")\n",
                          testSuite->name,
                          testCase->name);
            return status;
        }
    } else {
        context.fixture = NULL;
    }

    if (rxpGetRealTime(&timeStart) != RX_SUCCESS) {
        timeStart = (uint64_t)-1;
    }

    if (setjmp(context.env) == 0) {
        testCase->run(&context);
    }

    if (timeStart == (uint64_t)-1 || rxpGetRealTime(&timeEnd) != RX_SUCCESS) {
        RXP_LOG_WARNING("failed to measure the time elapsed "
                        "(suite: \"%s\", case: \"%s\")\n",
                        testSuite->name,
                        testCase->name);
        summary->elapsed = 0;
    } else {
        RX_ASSERT(timeEnd >= timeStart);
        summary->elapsed = timeEnd - timeStart;
    }

    if (testSuite->tearDown != NULL) {
        testSuite->tearDown(context.fixture);
    }

    return RX_SUCCESS;
}

RXP_MAYBE_UNUSED RXP_SCOPE enum RxStatus
rxRun(size_t suiteCount,
      const struct RxTestSuite *testSuites,
      int argc,
      const char **argv)
{
    size_t i;
    size_t j;
    enum RxStatus status;

    RXP_UNUSED(argc);
    RXP_UNUSED(argv);

    RX_ASSERT(testSuites != NULL);

    if (suiteCount == 0) {
        RXP_LOG_INFO("nothing to run\n");
        return RX_SUCCESS;
    }

    for (i = 0; i < suiteCount; ++i) {
        const struct RxTestSuite *testSuite;

        testSuite = &testSuites[i];
        rxpTestSuiteAssert(testSuite);

        for (j = 0; j < testSuite->testCaseCount; ++j) {
            const struct RxTestCase *testCase;
            struct RxSummary summary;

            testCase = &testSuite->testCases[j];
            rxpTestCaseAssert(testCase);

            status = rxSummaryInitialize(&summary, testSuite, testCase);
            if (status != RX_SUCCESS) {
                RXP_LOG_ERROR("failed to initialize the summary "
                              "(suite: \"%s\", case: \"%s\")\n",
                              testSuite->name,
                              testCase->name);
                return status;
            }

            status = rxTestCaseRun(&summary, testCase, testSuite);
            if (status != RX_SUCCESS) {
                RXP_LOG_ERROR("failed to run a test case "
                              "(suite: \"%s\", case: \"%s\")\n",
                              testSuite->name,
                              testCase->name);
                return status;
            }

            rxSummaryPrint(&summary);
            rxSummaryTerminate(&summary);
        }
    }

    return RX_SUCCESS;
}

#endif /* REXO_REXO_H */
