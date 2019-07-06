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

#define RXP_UNUSED(x) (void)(x)

#ifdef __GNUC__
#define RXP_MAYBE_UNUSED __attribute__((unused))
#else
#define RXP_MAYBE_UNUSED
#endif

struct RxContext;

#define RXP_FALSE ((int)0)
#define RXP_TRUE ((int)1)

#define RXP_TEST_CASE_PARAMETERS struct RxContext *pContext RXP_MAYBE_UNUSED

#define RX_TEST_CASE(name) static void name(RXP_TEST_CASE_PARAMETERS)

#define RX_FIXTURE pContext->pFixture

#define RXP_DEFINE_TEST(condition, expected, message, severity)                \
    rxpAssessTest(pContext,                                                    \
                  !!(condition),                                               \
                  expected,                                                    \
                  message,                                                     \
                  __FILE__,                                                    \
                  __LINE__,                                                    \
                  severity)

#define RXP_DEFINE_BOOL_TEST(condition, expected, severity)                    \
    rxpAssessBooleanTest(pContext,                                             \
                         !!(condition),                                        \
                         expected,                                             \
                         #condition,                                           \
                         __FILE__,                                             \
                         __LINE__,                                             \
                         severity)

#define RXP_DEFINE_INT_TEST(value1, value2, op, severity)                      \
    rxpAssessIntegerComparisonTest(pContext,                                   \
                                   (value1),                                   \
                                   (value2),                                   \
                                   op,                                         \
                                   #value1,                                    \
                                   #value2,                                    \
                                   __FILE__,                                   \
                                   __LINE__,                                   \
                                   severity)

#define RXP_DEFINE_UINT_TEST(value1, value2, op, severity)                     \
    rxpAssessUnsignedIntegerComparisonTest(pContext,                           \
                                           (value1),                           \
                                           (value2),                           \
                                           op,                                 \
                                           #value1,                            \
                                           #value2,                            \
                                           __FILE__,                           \
                                           __LINE__,                           \
                                           severity)

#define RXP_DEFINE_FP_TEST(value1, value2, op, severity)                       \
    rxpAssessFloatingPointComparisonTest(pContext,                             \
                                         (value1),                             \
                                         (value2),                             \
                                         op,                                   \
                                         #value1,                              \
                                         #value2,                              \
                                         __FILE__,                             \
                                         __LINE__,                             \
                                         severity)

#define RXP_DEFINE_FP_FUZZY_TEST(value1, value2, tolerance, op, severity)      \
    rxpAssessFloatingPointFuzzyComparisonTest(pContext,                        \
                                              (value1),                        \
                                              (value2),                        \
                                              (tolerance),                     \
                                              op,                              \
                                              #value1,                         \
                                              #value2,                         \
                                              __FILE__,                        \
                                              __LINE__,                        \
                                              severity)
#define RXP_DEFINE_STR_TEST(str1, str2, stringCase, op, severity)              \
    rxpAssessStringComparisonTest(pContext,                                    \
                                  (str1),                                      \
                                  (str2),                                      \
                                  stringCase,                                  \
                                  op,                                          \
                                  #str1,                                       \
                                  #str2,                                       \
                                  __FILE__,                                    \
                                  __LINE__,                                    \
                                  severity)

#define RX_REQUIRE(condition, message)                                         \
    RXP_DEFINE_TEST(condition, RXP_TRUE, message, RX_FATAL)

#define RX_REQUIRE_TRUE(condition)                                             \
    RXP_DEFINE_BOOL_TEST(condition, RXP_TRUE, RX_FATAL)

#define RX_REQUIRE_FALSE(condition)                                            \
    RXP_DEFINE_BOOL_TEST(condition, RXP_FALSE, RX_FATAL)

#define RX_REQUIRE_INT_EQUAL(value1, value2)                                   \
    RXP_DEFINE_INT_TEST(value1, value2, RXP_OP_EQUAL, RX_FATAL)

#define RX_REQUIRE_INT_NOT_EQUAL(value1, value2)                               \
    RXP_DEFINE_INT_TEST(value1, value2, RXP_OP_NOT_EQUAL, RX_FATAL)

#define RX_REQUIRE_INT_GREATER(value1, value2)                                 \
    RXP_DEFINE_INT_TEST(value1, value2, RXP_OP_GREATER, RX_FATAL)

#define RX_REQUIRE_INT_LESSER(value1, value2)                                  \
    RXP_DEFINE_INT_TEST(value1, value2, RXP_OP_LESSER, RX_FATAL)

#define RX_REQUIRE_INT_GREATER_OR_EQUAL(value1, value2)                        \
    RXP_DEFINE_INT_TEST(value1, value2, RXP_OP_GREATER_OR_EQUAL, RX_FATAL)

#define RX_REQUIRE_INT_LESSER_OR_EQUAL(value1, value2)                         \
    RXP_DEFINE_INT_TEST(value1, value2, RXP_OP_LESSER_OR_EQUAL, RX_FATAL)

#define RX_REQUIRE_UINT_EQUAL(value1, value2)                                  \
    RXP_DEFINE_UINT_TEST(value1, value2, RXP_OP_EQUAL, RX_FATAL)

#define RX_REQUIRE_UINT_NOT_EQUAL(value1, value2)                              \
    RXP_DEFINE_UINT_TEST(value1, value2, RXP_OP_NOT_EQUAL, RX_FATAL)

#define RX_REQUIRE_UINT_GREATER(value1, value2)                                \
    RXP_DEFINE_UINT_TEST(value1, value2, RXP_OP_GREATER, RX_FATAL)

#define RX_REQUIRE_UINT_LESSER(value1, value2)                                 \
    RXP_DEFINE_UINT_TEST(value1, value2, RXP_OP_LESSER, RX_FATAL)

#define RX_REQUIRE_UINT_GREATER_OR_EQUAL(value1, value2)                       \
    RXP_DEFINE_UINT_TEST(value1, value2, RXP_OP_GREATER_OR_EQUAL, RX_FATAL)

#define RX_REQUIRE_UINT_LESSER_OR_EQUAL(value1, value2)                        \
    RXP_DEFINE_UINT_TEST(value1, value2, RXP_OP_LESSER_OR_EQUAL, RX_FATAL)

#define RX_REQUIRE_FP_EQUAL(value1, value2)                                    \
    RXP_DEFINE_FP_TEST(value1, value2, RXP_OP_EQUAL, RX_FATAL)

#define RX_REQUIRE_FP_NOT_EQUAL(value1, value2)                                \
    RXP_DEFINE_FP_TEST(value1, value2, RXP_OP_NOT_EQUAL, RX_FATAL)

#define RX_REQUIRE_FP_GREATER(value1, value2)                                  \
    RXP_DEFINE_FP_TEST(value1, value2, RXP_OP_GREATER, RX_FATAL)

#define RX_REQUIRE_FP_LESSER(value1, value2)                                   \
    RXP_DEFINE_FP_TEST(value1, value2, RXP_OP_LESSER, RX_FATAL)

#define RX_REQUIRE_FP_GREATER_OR_EQUAL(value1, value2)                         \
    RXP_DEFINE_FP_TEST(value1, value2, RXP_OP_GREATER_OR_EQUAL, RX_FATAL)

#define RX_REQUIRE_FP_LESSER_OR_EQUAL(value1, value2)                          \
    RXP_DEFINE_FP_TEST(value1, value2, RXP_OP_LESSER_OR_EQUAL, RX_FATAL)

#define RX_REQUIRE_FP_ALMOST_EQUAL(value1, value2, tolerance)                  \
    RXP_DEFINE_FP_FUZZY_TEST(value1, value2, tolerance, RXP_OP_EQUAL, RX_FATAL)

#define RX_REQUIRE_FP_NOT_ALMOST_EQUAL(value1, value2, tolerance)              \
    RXP_DEFINE_FP_FUZZY_TEST(                                                  \
        value1, value2, tolerance, RXP_OP_NOT_EQUAL, RX_FATAL)

#define RX_REQUIRE_STR_EQUAL(str1, str2)                                       \
    RXP_DEFINE_STR_TEST(                                                       \
        str1, str2, RXP_STRING_CASE_OBEY, RXP_OP_EQUAL, RX_FATAL)

#define RX_REQUIRE_STR_NOT_EQUAL(str1, str2)                                   \
    RXP_DEFINE_STR_TEST(                                                       \
        str1, str2, RXP_STRING_CASE_OBEY, RXP_OP_NOT_EQUAL, RX_FATAL)

#define RX_REQUIRE_STR_EQUAL_NO_CASE(str1, str2)                               \
    RXP_DEFINE_STR_TEST(                                                       \
        str1, str2, RXP_STRING_CASE_IGNORE, RXP_OP_EQUAL, RX_FATAL)

#define RX_REQUIRE_STR_NOT_EQUAL_NO_CASE(str1, str2)                           \
    RXP_DEFINE_STR_TEST(                                                       \
        str1, str2, RXP_STRING_CASE_IGNORE, RXP_OP_NOT_EQUAL, RX_FATAL)

#define RX_CHECK(condition, message)                                           \
    RXP_DEFINE_TEST(condition, RXP_TRUE, message, RX_NONFATAL)

#define RX_CHECK_TRUE(condition)                                               \
    RXP_DEFINE_BOOL_TEST(condition, RXP_TRUE, RX_NONFATAL)

#define RX_CHECK_FALSE(condition)                                              \
    RXP_DEFINE_BOOL_TEST(condition, RXP_FALSE, RX_NONFATAL)

#define RX_CHECK_INT_EQUAL(value1, value2)                                     \
    RXP_DEFINE_INT_TEST(value1, value2, RXP_OP_EQUAL, RX_NONFATAL)

#define RX_CHECK_INT_NOT_EQUAL(value1, value2)                                 \
    RXP_DEFINE_INT_TEST(value1, value2, RXP_OP_NOT_EQUAL, RX_NONFATAL)

#define RX_CHECK_INT_GREATER(value1, value2)                                   \
    RXP_DEFINE_INT_TEST(value1, value2, RXP_OP_GREATER, RX_NONFATAL)

#define RX_CHECK_INT_LESSER(value1, value2)                                    \
    RXP_DEFINE_INT_TEST(value1, value2, RXP_OP_LESSER, RX_NONFATAL)

#define RX_CHECK_INT_GREATER_OR_EQUAL(value1, value2)                          \
    RXP_DEFINE_INT_TEST(value1, value2, RXP_OP_GREATER_OR_EQUAL, RX_NONFATAL)

#define RX_CHECK_INT_LESSER_OR_EQUAL(value1, value2)                           \
    RXP_DEFINE_INT_TEST(value1, value2, RXP_OP_LESSER_OR_EQUAL, RX_NONFATAL)

#define RX_CHECK_UINT_EQUAL(value1, value2)                                    \
    RXP_DEFINE_UINT_TEST(value1, value2, RXP_OP_EQUAL, RX_NONFATAL)

#define RX_CHECK_UINT_NOT_EQUAL(value1, value2)                                \
    RXP_DEFINE_UINT_TEST(value1, value2, RXP_OP_NOT_EQUAL, RX_NONFATAL)

#define RX_CHECK_UINT_GREATER(value1, value2)                                  \
    RXP_DEFINE_UINT_TEST(value1, value2, RXP_OP_GREATER, RX_NONFATAL)

#define RX_CHECK_UINT_LESSER(value1, value2)                                   \
    RXP_DEFINE_UINT_TEST(value1, value2, RXP_OP_LESSER, RX_NONFATAL)

#define RX_CHECK_UINT_GREATER_OR_EQUAL(value1, value2)                         \
    RXP_DEFINE_UINT_TEST(value1, value2, RXP_OP_GREATER_OR_EQUAL, RX_NONFATAL)

#define RX_CHECK_UINT_LESSER_OR_EQUAL(value1, value2)                          \
    RXP_DEFINE_UINT_TEST(value1, value2, RXP_OP_LESSER_OR_EQUAL, RX_NONFATAL)

#define RX_CHECK_FP_EQUAL(value1, value2)                                      \
    RXP_DEFINE_FP_TEST(value1, value2, RXP_OP_EQUAL, RX_NONFATAL)

#define RX_CHECK_FP_NOT_EQUAL(value1, value2)                                  \
    RXP_DEFINE_FP_TEST(value1, value2, RXP_OP_NOT_EQUAL, RX_NONFATAL)

#define RX_CHECK_FP_GREATER(value1, value2)                                    \
    RXP_DEFINE_FP_TEST(value1, value2, RXP_OP_GREATER, RX_NONFATAL)

#define RX_CHECK_FP_LESSER(value1, value2)                                     \
    RXP_DEFINE_FP_TEST(value1, value2, RXP_OP_LESSER, RX_NONFATAL)

#define RX_CHECK_FP_GREATER_OR_EQUAL(value1, value2)                           \
    RXP_DEFINE_FP_TEST(value1, value2, RXP_OP_GREATER_OR_EQUAL, RX_NONFATAL)

#define RX_CHECK_FP_LESSER_OR_EQUAL(value1, value2)                            \
    RXP_DEFINE_FP_TEST(value1, value2, RXP_OP_LESSER_OR_EQUAL, RX_NONFATAL)

#define RX_CHECK_FP_ALMOST_EQUAL(value1, value2, tolerance)                    \
    RXP_DEFINE_FP_FUZZY_TEST(                                                  \
        value1, value2, tolerance, RXP_OP_EQUAL, RX_NONFATAL)

#define RX_CHECK_FP_NOT_ALMOST_EQUAL(value1, value2, tolerance)                \
    RXP_DEFINE_FP_FUZZY_TEST(                                                  \
        value1, value2, tolerance, RXP_OP_NOT_EQUAL, RX_NONFATAL)

#define RX_CHECK_STR_EQUAL(str1, str2)                                         \
    RXP_DEFINE_STR_TEST(                                                       \
        str1, str2, RXP_STRING_CASE_OBEY, RXP_OP_EQUAL, RX_NONFATAL)

#define RX_CHECK_STR_NOT_EQUAL(str1, str2)                                     \
    RXP_DEFINE_STR_TEST(                                                       \
        str1, str2, RXP_STRING_CASE_OBEY, RXP_OP_NOT_EQUAL, RX_NONFATAL)

#define RX_CHECK_STR_EQUAL_NO_CASE(str1, str2)                                 \
    RXP_DEFINE_STR_TEST(                                                       \
        str1, str2, RXP_STRING_CASE_IGNORE, RXP_OP_EQUAL, RX_NONFATAL)

#define RX_CHECK_STR_NOT_EQUAL_NO_CASE(str1, str2)                             \
    RXP_DEFINE_STR_TEST(                                                       \
        str1, str2, RXP_STRING_CASE_IGNORE, RXP_OP_NOT_EQUAL, RX_NONFATAL)

enum RxLogLevel {
    RX_LOG_LEVEL_ERROR = 0,
    RX_LOG_LEVEL_WARNING = 1,
    RX_LOG_LEVEL_INFO = 2,
    RX_LOG_LEVEL_TRACE = 3,
    RX_LOG_LEVEL_DEBUG = 4
};

enum RxStatus {
    RX_SUCCESS = 0,
    RX_ERROR = -1,
    RX_ERROR_INVALID_VALUE = -2,
    RX_ERROR_ALLOCATION = -3,
    RX_ERROR_MAX_SIZE_EXCEEDED = -4
};

enum RxSeverity { RX_NONFATAL = 0, RX_FATAL = 1 };

typedef void (*RxPfnTestCase)(RXP_TEST_CASE_PARAMETERS);
typedef enum RxStatus (*RxPfnTestSuiteSetUp)(void **ppFixture);
typedef void (*RxPfnTestSuiteTearDown)(void *pFixture);

struct RxTestCase {
    const char *pName;
    RxPfnTestCase pfn;
};

struct RxTestSuite {
    const char *pName;
    size_t caseCount;
    const struct RxTestCase *pCases;
    RxPfnTestSuiteSetUp pfnSetUp;
    RxPfnTestSuiteTearDown pfnTearDown;
};

struct RxTestFailure {
    const char *pFile;
    int line;
    enum RxSeverity severity;
    const char *pMessage;
    const char *pDiagnosticMessage;
};

struct RxTestCaseReport {
    const struct RxTestSuite *pSuite;
    const struct RxTestCase *pCase;
    size_t testCount;
    size_t failureCount;
    struct RxTestFailure *pFailures;
    uint64_t elapsed;
};

struct RxTestSuiteReport {
    const struct RxTestSuite *pSuite;
    size_t caseReportCount;
    struct RxTestCaseReport *pCaseReports;
};

struct RxContext {
    jmp_buf env;
    struct RxTestCaseReport *pCaseReport;
    void *pFixture;
};

#ifdef __cplusplus
extern "C" {
#endif

RXP_SCOPE void
rxGetLogLevelName(const char **ppString, enum RxLogLevel level);

RXP_SCOPE void
rxAbortTest(struct RxContext *pContext);

RXP_SCOPE enum RxStatus
rxHandleTestResult(struct RxContext *pContext,
                   int result,
                   const char *pFile,
                   int line,
                   enum RxSeverity severity,
                   const char *pFailureMessage,
                   const char *pDiagnosticMessage);

RXP_SCOPE enum RxStatus
rxInitializeTestCaseReport(struct RxTestCaseReport *pReport,
                           const struct RxTestSuite *pSuite,
                           const struct RxTestCase *pCase);

RXP_SCOPE void
rxTerminateTestCaseReport(struct RxTestCaseReport *pReport);

RXP_SCOPE enum RxStatus
rxRunTestCase(struct RxTestCaseReport *pReport,
              const struct RxTestSuite *pSuite,
              const struct RxTestCase *pCase);

RXP_SCOPE void
rxPrintTestCaseRunSummary(const struct RxTestCaseReport *pReport);

RXP_SCOPE enum RxStatus
rxRun(size_t suiteCount,
      const struct RxTestSuite *pSuites,
      int argc,
      const char **ppArgv);

#ifdef __cplusplus
}
#endif

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

#if !defined(RX_DISABLE_LOG_STYLING) && defined(RXP_PLATFORM_UNIX)             \
    && defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 1
#include <unistd.h>
#define RXP_LOG_STYLING 1
#else
#define RXP_LOG_STYLING 0
#endif

#if defined(RX_SET_LOGGING_LEVEL_DEBUG)
#define RXP_LOGGING_LEVEL RX_LOG_LEVEL_DEBUG
#elif defined(RX_SET_LOGGING_LEVEL_TRACE)
#define RXP_LOGGING_LEVEL RX_LOG_LEVEL_TRACE
#elif defined(RX_SET_LOGGING_LEVEL_INFO)
#define RXP_LOGGING_LEVEL RX_LOG_LEVEL_INFO
#elif defined(RX_SET_LOGGING_LEVEL_WARNING)
#define RXP_LOGGING_LEVEL RX_LOG_LEVEL_WARNING
#elif defined(RX_SET_LOGGING_LEVEL_ERROR)
#define RXP_LOGGING_LEVEL RX_LOG_LEVEL_ERROR
#elif defined(RX_ENABLE_DEBUGGING)                                             \
    || (!defined(RX_DISABLE_DEBUGGING)                                         \
        && (defined(DEBUG) || !defined(NDEBUG)))
#define RXP_LOGGING_LEVEL RX_LOG_LEVEL_DEBUG
#else
#define RXP_LOGGING_LEVEL RX_LOG_LEVEL_WARNING
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

#define RXP_LOG_DEBUG(...) RX_LOG(RX_LOG_LEVEL_DEBUG, __VA_ARGS__)

#define RXP_LOG_TRACE(...) RX_LOG(RX_LOG_LEVEL_TRACE, __VA_ARGS__)

#define RXP_LOG_INFO(...) RX_LOG(RX_LOG_LEVEL_INFO, __VA_ARGS__)

#define RXP_LOG_WARNING(...) RX_LOG(RX_LOG_LEVEL_WARNING, __VA_ARGS__)

#define RXP_LOG_ERROR(...) RX_LOG(RX_LOG_LEVEL_ERROR, __VA_ARGS__)

#if RXP_LOG_STYLING
enum RxpLoggerStyle {
    RXP_STYLE_RESET = 0,
    RXP_STYLE_BLACK = 1,
    RXP_STYLE_RED = 2,
    RXP_STYLE_GREEN = 3,
    RXP_STYLE_YELLOW = 4,
    RXP_STYLE_BLUE = 5,
    RXP_STYLE_MAGENTA = 6,
    RXP_STYLE_CYAN = 7,
    RXP_STYLE_BRIGHT_BLACK = 8,
    RXP_STYLE_BRIGHT_RED = 9,
    RXP_STYLE_BRIGHT_GREEN = 10,
    RXP_STYLE_BRIGHT_YELLOW = 11,
    RXP_STYLE_BRIGHT_BLUE = 12,
    RXP_STYLE_BRIGHT_MAGENTA = 13,
    RXP_STYLE_BRIGHT_CYAN = 14
};
#endif /* RXP_LOG_STYLING */

static void
rxpGetLogLevelName(const char **ppName, enum RxLogLevel level)
{
    RX_ASSERT(ppName != NULL);

    switch (level) {
        case RX_LOG_LEVEL_ERROR:
            *ppName = "error";
            return;
        case RX_LOG_LEVEL_WARNING:
            *ppName = "warning";
            return;
        case RX_LOG_LEVEL_INFO:
            *ppName = "info";
            return;
        case RX_LOG_LEVEL_TRACE:
            *ppName = "trace";
            return;
        case RX_LOG_LEVEL_DEBUG:
            *ppName = "debug";
            return;
        default:
            RX_ASSERT(0);
    }
}

#if RXP_LOG_STYLING
static void
rxpGetLogLevelStyle(enum RxpLoggerStyle *pStyle, enum RxLogLevel level)
{
    RX_ASSERT(pStyle != NULL);

    switch (level) {
        case RX_LOG_LEVEL_ERROR:
            *pStyle = RXP_STYLE_BRIGHT_RED;
            return;
        case RX_LOG_LEVEL_WARNING:
            *pStyle = RXP_STYLE_BRIGHT_YELLOW;
            return;
        case RX_LOG_LEVEL_INFO:
            *pStyle = RXP_STYLE_BRIGHT_GREEN;
            return;
        case RX_LOG_LEVEL_TRACE:
            *pStyle = RXP_STYLE_BRIGHT_CYAN;
            return;
        case RX_LOG_LEVEL_DEBUG:
            *pStyle = RXP_STYLE_BRIGHT_MAGENTA;
            return;
        default:
            RX_ASSERT(0);
    };
}

static void
rxpGetStyleAnsiCode(const char **ppCode, enum RxpLoggerStyle style)
{
    RX_ASSERT(ppCode != NULL);

    switch (style) {
        case RXP_STYLE_RESET:
            *ppCode = "\x1b[0m";
            return;
        case RXP_STYLE_BLACK:
            *ppCode = "\x1b[30m";
            return;
        case RXP_STYLE_RED:
            *ppCode = "\x1b[31m";
            return;
        case RXP_STYLE_GREEN:
            *ppCode = "\x1b[32m";
            return;
        case RXP_STYLE_YELLOW:
            *ppCode = "\x1b[33m";
            return;
        case RXP_STYLE_BLUE:
            *ppCode = "\x1b[34m";
            return;
        case RXP_STYLE_MAGENTA:
            *ppCode = "\x1b[35m";
            return;
        case RXP_STYLE_CYAN:
            *ppCode = "\x1b[36m";
            return;
        case RXP_STYLE_BRIGHT_BLACK:
            *ppCode = "\x1b[1;30m";
            return;
        case RXP_STYLE_BRIGHT_RED:
            *ppCode = "\x1b[1;31m";
            return;
        case RXP_STYLE_BRIGHT_GREEN:
            *ppCode = "\x1b[1;32m";
            return;
        case RXP_STYLE_BRIGHT_YELLOW:
            *ppCode = "\x1b[1;33m";
            return;
        case RXP_STYLE_BRIGHT_BLUE:
            *ppCode = "\x1b[1;34m";
            return;
        case RXP_STYLE_BRIGHT_MAGENTA:
            *ppCode = "\x1b[1;35m";
            return;
        case RXP_STYLE_BRIGHT_CYAN:
            *ppCode = "\x1b[1;36m";
            return;
        default:
            RX_ASSERT(0);
    }
}
#endif /* RXP_LOG_STYLING */

static void
rxpLog(enum RxLogLevel level,
       const char *pFile,
       int line,
       const char *pFormat,
       ...)
{
    const char *pLevelName;
    const char *pLevelStyleStart;
    const char *pLevelStyleEnd;
    va_list args;

    RX_ASSERT(pFile != NULL);
    RX_ASSERT(pFormat != NULL);

    rxpGetLogLevelName(&pLevelName, level);

#if RXP_LOG_STYLING
    if (isatty(fileno(stderr))) {
        enum RxpLoggerStyle levelStyle;

        rxpGetLogLevelStyle(&levelStyle, level);
        rxpGetStyleAnsiCode(&pLevelStyleStart, levelStyle);
        rxpGetStyleAnsiCode(&pLevelStyleEnd, RXP_STYLE_RESET);
    } else {
        pLevelStyleStart = pLevelStyleEnd = "";
    }
#else
    pLevelStyleStart = pLevelStyleEnd = "";
#endif /* RXP_LOG_STYLING */

    va_start(args, pFormat);
    fprintf(stderr,
            "%s:%d: %s%s%s: ",
            pFile,
            line,
            pLevelStyleStart,
            pLevelName,
            pLevelStyleEnd);
    vfprintf(stderr, pFormat, args);
    va_end(args);
}

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
#define RXP_TIMER_USE_CLOCK_GETTIME
#if defined(CLOCK_MONOTONIC_RAW)
#define RXP_TIMER_CLOCK_ID CLOCK_MONOTONIC_RAW
#elif defined(CLOCK_MONOTONIC)
#define RXP_TIMER_CLOCK_ID CLOCK_MONOTONIC
#else
#define RXP_TIMER_CLOCK_ID CLOCK_REALTIME
#endif
#else
#include <sys/time.h>
#endif
#else
typedef char rxp_timer_unsupported_platform[-1];
#endif

static enum RxStatus
rxpGetRealTime(uint64_t *pTime)
{
    RX_ASSERT(pTime != NULL);

#if defined(RXP_PLATFORM_WINDOWS)
    {
        static double timeToNano;
        LARGE_INTEGER time;

        if (timeToNano == 0.0) {
            LARGE_INTEGER frequency;

            if (!QueryPerformanceFrequency(&frequency)) {
                RXP_LOG_ERROR("failed to retrieve the time's frequency\n");
                return RX_ERROR;
            }

            timeToNano = (double)RXP_TICKS_PER_SECOND / frequency.QuadPart;
        }

        if (!QueryPerformanceCounter(&time)) {
            RXP_LOG_ERROR("failed to retrieve the current time\n");
            return RX_ERROR;
        }

        *pTime = time.QuadPart * timeToNano;
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

        *pTime = mach_absolute_time() * timeToNano;
        return RX_SUCCESS;
    }
#elif defined(RXP_PLATFORM_UNIX)
#if defined(RXP_TIMER_USE_CLOCK_GETTIME)
    {
        struct timespec time;

        if (clock_gettime(RXP_TIMER_CLOCK_ID, &time) != 0) {
            RXP_LOG_ERROR("failed to retrieve the current time\n");
            return RX_ERROR;
        }

        *pTime = (uint64_t)time.tv_sec * 1000000000ull + (uint64_t)time.tv_nsec;
        return RX_SUCCESS;
    }
#else
    {
        struct timeval time;

        if (gettimeofday(&time, NULL) != 0) {
            RXP_LOG_ERROR("failed to retrieve the current time\n");
            return RX_ERROR;
        }

        *pTime = (uint64_t)time.tv_sec * 1000000000ull
                 + (uint64_t)time.tv_usec * 1000ull;
        return RX_SUCCESS;
    }
#endif
#endif

    RXP_LOG_ERROR("platform not supported\n");
    return RX_ERROR;
}

#define RXP_DYNAMICARRAY_GET_BLOCK(pBuffer)                                    \
    ((void *)&((struct RxpDynamicArrayHeader *)(pBuffer))[-1])
#define RXP_DYNAMICARRAY_GET_HEADER(pBlock)                                    \
    ((struct RxpDynamicArrayHeader *)(pBlock))
#define RXP_DYNAMICARRAY_GET_BUFFER(pBlock)                                    \
    ((void *)&((struct RxpDynamicArrayHeader *)(pBlock))[1])
#define RXP_DYNAMICARRAY_GET_CONST_BLOCK(pBuffer)                              \
    ((const void *)&((const struct RxpDynamicArrayHeader *)pBuffer)[-1])
#define RXP_DYNAMICARRAY_GET_CONST_HEADER(pBlock)                              \
    ((const struct RxpDynamicArrayHeader *)(pBlock))

#define RXP_DYNAMICARRAY_DEFINE_MAX_CAPACITY(name, type)                       \
    static const size_t rxpMax##name##Capacity                                 \
        = (((size_t)-1 - sizeof(struct RxpDynamicArrayHeader))                 \
           / sizeof(type));

#define RXP_DYNAMICARRAY_DEFINE_CREATE_FUNCTION(name, type)                    \
    static enum RxStatus rxpCreate##name(type **ppArray, size_t size)          \
    {                                                                          \
        enum RxStatus status;                                                  \
        void *pBlock;                                                          \
                                                                               \
        RX_ASSERT(ppArray != NULL);                                            \
                                                                               \
        pBlock = NULL;                                                         \
                                                                               \
        status = rxpEnsureDynamicArrayHasEnoughCapacity(                       \
            &pBlock, 0, (size_t)size, rxpMax##name##Capacity, sizeof(type));   \
        if (status != RX_SUCCESS) {                                            \
            RXP_LOG_ERROR("failed to reserve a large enough capacity for the " \
                          "type ‘" #type "’ (requested capacity: %zu)\n",      \
                          (size_t)size);                                       \
            return status;                                                     \
        }                                                                      \
                                                                               \
        RX_ASSERT(pBlock != NULL);                                             \
                                                                               \
        RXP_DYNAMICARRAY_GET_HEADER(pBlock)->size = (size_t)size;              \
        RXP_DYNAMICARRAY_GET_HEADER(pBlock)->capacity = (size_t)size;          \
        *ppArray = (type *)RXP_DYNAMICARRAY_GET_BUFFER(pBlock);                \
        return RX_SUCCESS;                                                     \
    }

#define RXP_DYNAMICARRAY_DEFINE_DESTROY_FUNCTION(name, type)                   \
    static void rxpDestroy##name(type *pArray)                                 \
    {                                                                          \
        if (pArray == NULL) {                                                  \
            return;                                                            \
        }                                                                      \
                                                                               \
        RX_FREE(RXP_DYNAMICARRAY_GET_BLOCK(pArray));                           \
    }

#define RXP_DYNAMICARRAY_DEFINE_GET_SIZE_FUNCTION(name, type)                  \
    static void rxpGet##name##Size(size_t *pSize, const type *pArray)          \
    {                                                                          \
        RX_ASSERT(pArray != NULL);                                             \
                                                                               \
        *pSize = (size_t)RXP_DYNAMICARRAY_GET_CONST_HEADER(                    \
                     RXP_DYNAMICARRAY_GET_CONST_BLOCK(pArray))                 \
                     ->size;                                                   \
    }

#define RXP_DYNAMICARRAY_DEFINE_EXTEND_FUNCTION(name, type)                    \
    static enum RxStatus rxpExtend##name(                                      \
        type **ppSlice, type **ppArray, size_t position, size_t size)          \
    {                                                                          \
        enum RxStatus status;                                                  \
        void *pBlock;                                                          \
                                                                               \
        RX_ASSERT(ppArray != NULL);                                            \
        RX_ASSERT(*ppArray != NULL);                                           \
                                                                               \
        pBlock = RXP_DYNAMICARRAY_GET_BLOCK(*ppArray);                         \
                                                                               \
        if (position > RXP_DYNAMICARRAY_GET_HEADER(pBlock)->size) {            \
            position = (size_t)RXP_DYNAMICARRAY_GET_HEADER(pBlock)->size;      \
        }                                                                      \
                                                                               \
        status = rxpEnsureDynamicArrayHasEnoughCapacity(                       \
            &pBlock,                                                           \
            RXP_DYNAMICARRAY_GET_HEADER(pBlock)->capacity,                     \
            RXP_DYNAMICARRAY_GET_HEADER(pBlock)->size + (size_t)size,          \
            rxpMax##name##Capacity,                                            \
            sizeof(type));                                                     \
        if (status != RX_SUCCESS) {                                            \
            RXP_LOG_ERROR(                                                     \
                "failed to reserve a large enough capacity for the "           \
                "type ‘" #type "’ (requested capacity: %zu)\n",                \
                RXP_DYNAMICARRAY_GET_HEADER(pBlock)->size + (size_t)size);     \
            return status;                                                     \
        }                                                                      \
                                                                               \
        RX_ASSERT(pBlock != NULL);                                             \
                                                                               \
        *ppArray = (type *)RXP_DYNAMICARRAY_GET_BUFFER(pBlock);                \
                                                                               \
        memmove(&(*ppArray)[(size_t)position + (size_t)size],                  \
                &(*ppArray)[(size_t)position],                                 \
                sizeof(type)                                                   \
                    * (RXP_DYNAMICARRAY_GET_HEADER(pBlock)->size               \
                       - (size_t)position));                                   \
                                                                               \
        if (ppSlice != NULL) {                                                 \
            *ppSlice = &(*ppArray)[position];                                  \
        }                                                                      \
                                                                               \
        RXP_DYNAMICARRAY_GET_HEADER(pBlock)->size += (size_t)size;             \
        return RX_SUCCESS;                                                     \
    }

#define RXP_DYNAMICARRAY_DEFINE_EXTEND_BACK_FUNCTION(name, type)               \
    static enum RxStatus rxpExtend##name##Back(                                \
        type **ppSlice, type **ppArray, size_t size)                           \
    {                                                                          \
        return rxpExtend##name(ppSlice,                                        \
                               ppArray,                                        \
                               (size_t)RXP_DYNAMICARRAY_GET_HEADER(            \
                                   RXP_DYNAMICARRAY_GET_BLOCK(*ppArray))       \
                                   ->size,                                     \
                               size);                                          \
    }

#define RXP_MAKE_DYNAMIC_ARRAY(name, type)                                     \
    RXP_DYNAMICARRAY_DEFINE_MAX_CAPACITY(name, type)                           \
    RXP_DYNAMICARRAY_DEFINE_CREATE_FUNCTION(name, type)                        \
    RXP_DYNAMICARRAY_DEFINE_DESTROY_FUNCTION(name, type)                       \
    RXP_DYNAMICARRAY_DEFINE_GET_SIZE_FUNCTION(name, type)                      \
    RXP_DYNAMICARRAY_DEFINE_EXTEND_FUNCTION(name, type)                        \
    RXP_DYNAMICARRAY_DEFINE_EXTEND_BACK_FUNCTION(name, type)

struct RxpDynamicArrayHeader {
    size_t size;
    size_t capacity;
};

static void
rxpGetDynamicArrayNewCapacity(size_t *pNewCapacity,
                              size_t current,
                              size_t requested,
                              size_t max)
{
    *pNewCapacity = current + current / 2 + 1;
    if (*pNewCapacity < current) {
        *pNewCapacity = max;
        return;
    }

    if (*pNewCapacity < requested) {
        *pNewCapacity = requested;
    }
}

static enum RxStatus
rxpEnsureDynamicArrayHasEnoughCapacity(void **ppBlock,
                                       size_t currentCapacity,
                                       size_t requestedCapacity,
                                       size_t maxCapacity,
                                       size_t elementSize)
{
    void *pBlock;
    size_t newCapacity;

    RX_ASSERT(ppBlock != NULL);
    RX_ASSERT(elementSize > 0);

    if (requestedCapacity > maxCapacity) {
        RXP_LOG_TRACE("the requested capacity is too large\n");
        return RX_ERROR_MAX_SIZE_EXCEEDED;
    }

    if (*ppBlock != NULL && currentCapacity >= requestedCapacity) {
        return RX_SUCCESS;
    }

    rxpGetDynamicArrayNewCapacity(
        &newCapacity, currentCapacity, requestedCapacity, maxCapacity);
    RX_ASSERT(newCapacity >= requestedCapacity);
    RX_ASSERT(newCapacity <= maxCapacity);

    pBlock = RX_REALLOC(
        *ppBlock,
        sizeof(struct RxpDynamicArrayHeader) + elementSize * newCapacity);
    if (pBlock == NULL) {
        RXP_LOG_TRACE("failed to reallocate the block\n");
        return RX_ERROR_ALLOCATION;
    }

    RXP_DYNAMICARRAY_GET_HEADER(pBlock)->capacity = newCapacity;
    *ppBlock = pBlock;
    return RX_SUCCESS;
}

enum RxpStringCase { RXP_STRING_CASE_OBEY = 0, RXP_STRING_CASE_IGNORE = 1 };

enum RxpComparisonOp {
    RXP_OP_EQUAL = 0,
    RXP_OP_NOT_EQUAL = 1,
    RXP_OP_GREATER = 2,
    RXP_OP_LESSER = 3,
    RXP_OP_GREATER_OR_EQUAL = 4,
    RXP_OP_LESSER_OR_EQUAL = 5
};

RXP_MAKE_DYNAMIC_ARRAY(TestFailureArray, struct RxTestFailure)

static void
rxpGetStringCaseType(const char **ppString, enum RxpStringCase stringCase)
{
    RX_ASSERT(ppString != NULL);

    switch (stringCase) {
        case RXP_STRING_CASE_OBEY:
            *ppString = "obey";
            return;
        case RXP_STRING_CASE_IGNORE:
            *ppString = "ignore";
            return;
        default:
            RX_ASSERT(0);
            *ppString = "invalid";
    }
}

static void
rxpGetComparisonOpSymbol(const char **ppString, enum RxpComparisonOp op)
{
    RX_ASSERT(ppString != NULL);

    switch (op) {
        case RXP_OP_EQUAL:
            *ppString = "==";
            return;
        case RXP_OP_NOT_EQUAL:
            *ppString = "!=";
            return;
        case RXP_OP_GREATER:
            *ppString = ">";
            return;
        case RXP_OP_LESSER:
            *ppString = "<";
            return;
        case RXP_OP_GREATER_OR_EQUAL:
            *ppString = ">=";
            return;
        case RXP_OP_LESSER_OR_EQUAL:
            *ppString = "<=";
            return;
        default:
            RX_ASSERT(0);
            *ppString = "invalid";
    }
}

static void
rxpGetComparisonOpName(const char **ppString, enum RxpComparisonOp op)
{
    RX_ASSERT(ppString != NULL);

    switch (op) {
        case RXP_OP_EQUAL:
            *ppString = "equal to";
            return;
        case RXP_OP_NOT_EQUAL:
            *ppString = "not equal to";
            return;
        case RXP_OP_GREATER:
            *ppString = "greater than";
            return;
        case RXP_OP_LESSER:
            *ppString = "less than";
            return;
        case RXP_OP_GREATER_OR_EQUAL:
            *ppString = "greater than or equal to";
            return;
        case RXP_OP_LESSER_OR_EQUAL:
            *ppString = "less than or equal to";
            return;
        default:
            RX_ASSERT(0);
            *ppString = "invalid";
    }
}

static enum RxStatus
rxpMakeString(char **ppString, const char *pFormat, ...)
{
    enum RxStatus status;
    va_list args;
    int size;

    RX_ASSERT(ppString != NULL);
    RX_ASSERT(pFormat != NULL);

    status = RX_SUCCESS;

    va_start(args, pFormat);
    size = vsnprintf(NULL, 0, pFormat, args);
    va_end(args);
    if (size < 0) {
        RXP_LOG_TRACE("invalid string formatting\n");
        status = RX_ERROR;
        goto exit;
    }

    *ppString = (char *)RX_MALLOC(sizeof **ppString * ((size_t)size + 1));
    if (*ppString == NULL) {
        RXP_LOG_TRACE("failed to allocate the string (%zu bytes)\n",
                      sizeof **ppString * ((size_t)size + 1));
        status = RX_ERROR_ALLOCATION;
        goto exit;
    }

    va_start(args, pFormat);
    size = vsnprintf(*ppString, (size_t)size + 1, pFormat, args);
    va_end(args);
    if (size < 0) {
        RXP_LOG_TRACE("unexpected string formatting error\n");
        status = RX_ERROR;
        goto undo;
    }

    RX_ASSERT(status == RX_SUCCESS);
    goto exit;

undo:
    RX_FREE(*ppString);

exit:
    return status;
}

static enum RxStatus
rxpMakeStringCopy(char **ppString, const char *pOriginal)
{
    size_t size;

    size = strlen(pOriginal) + 1;

    *ppString = (char *)RX_MALLOC(sizeof **ppString * size);
    if (*ppString == NULL) {
        RXP_LOG_TRACE("failed to allocate the string (%zu bytes)\n",
                      sizeof **ppString * size);
        return RX_ERROR_ALLOCATION;
    }

    memcpy(*ppString, pOriginal, size);
    return RX_SUCCESS;
}

static void
rxpAreFloatingPointAlmostEqual(int *pResult,
                               long double value1,
                               long double value2,
                               long double tolerance)
{
    long double difference;

    difference = fabsl(value1 - value2);
    if (difference <= tolerance) {
        *pResult = 1;
        return;
    }

    *pResult = difference <= fmaxl(fabsl(value1), fabsl(value2)) * tolerance;
}

static void
rxpAreStringsEqual(int *pResult, const char *pString1, const char *pString2)
{
    while (*pString1 != '\0') {
        if (*pString1 != *pString2) {
            *pResult = 0;
            return;
        }

        ++pString1;
        ++pString2;
    }

    *pResult = *pString1 == *pString2;
}

static void
rxpAreStringsEqualNoCase(int *pResult,
                         const char *pString1,
                         const char *pString2)
{
    while (*pString1 != '\0') {
        if (tolower(*pString1) != tolower(*pString2)) {
            *pResult = 0;
            return;
        }

        ++pString1;
        ++pString2;
    }

    *pResult = tolower(*pString1) == tolower(*pString2);
}

RXP_MAYBE_UNUSED static void
rxpAssessTest(struct RxContext *pContext,
              int value,
              int expected,
              const char *pFailureMessage,
              const char *pFile,
              int line,
              enum RxSeverity severity)
{
    int result;

    RX_ASSERT(pContext != NULL);
    RX_ASSERT(pContext->pCaseReport != NULL);
    RX_ASSERT(pFailureMessage != NULL);
    RX_ASSERT(pFile != NULL);

    result = ((value && expected) || (!value && !expected));

    if (rxHandleTestResult(
            pContext, result, pFile, line, severity, pFailureMessage, NULL)
        != RX_SUCCESS) {
        RXP_LOG_TRACE("failed to handle the test result for the test "
                      "located at %s:%d\n",
                      pFile,
                      line);
    }

    if (!result && severity == RX_FATAL) {
        rxAbortTest(pContext);
    }
}

RXP_MAYBE_UNUSED static void
rxpAssessBooleanTest(struct RxContext *pContext,
                     int value,
                     int expected,
                     const char *pExpression,
                     const char *pFile,
                     int line,
                     enum RxSeverity severity)
{
    int result;
    char *pFailureMessage;
    char *pDiagnosticMessage;

    RX_ASSERT(pContext != NULL);
    RX_ASSERT(pContext->pCaseReport != NULL);
    RX_ASSERT(pExpression != NULL);
    RX_ASSERT(pFile != NULL);

    result = ((value && expected) || (!value && !expected));

    if (result) {
        pFailureMessage = NULL;
        pDiagnosticMessage = NULL;
    } else {
        if (rxpMakeString(&pFailureMessage,
                          "‘%s’ is expected to be %s",
                          pExpression,
                          expected ? "true" : "false")
            != RX_SUCCESS) {
            RXP_LOG_TRACE("failed to create the failure message for the "
                          "boolean test located at %s:%d\n",
                          pFile,
                          line);
            pFailureMessage = NULL;
        }

        if (rxpMakeString(&pDiagnosticMessage, "%d == %d", value, expected)
            != RX_SUCCESS) {
            RXP_LOG_TRACE("failed to create the diagnostic message for the "
                          "boolean test located at %s:%d\n",
                          pFile,
                          line);
            pDiagnosticMessage = NULL;
        }
    }

    if (rxHandleTestResult(pContext,
                           result,
                           pFile,
                           line,
                           severity,
                           pFailureMessage,
                           pDiagnosticMessage)
        != RX_SUCCESS) {
        RXP_LOG_TRACE("failed to handle the test result for the boolean test "
                      "located at %s:%d\n",
                      pFile,
                      line);
    }

    if (pFailureMessage != NULL) {
        RX_FREE(pFailureMessage);
    }

    if (pDiagnosticMessage != NULL) {
        RX_FREE(pDiagnosticMessage);
    }

    if (!result && severity == RX_FATAL) {
        rxAbortTest(pContext);
    }
}

RXP_MAYBE_UNUSED static void
rxpAssessIntegerComparisonTest(struct RxContext *pContext,
                               intmax_t value1,
                               intmax_t value2,
                               enum RxpComparisonOp op,
                               const char *pExpression1,
                               const char *pExpression2,
                               const char *pFile,
                               int line,
                               enum RxSeverity severity)
{
    int result;
    char *pFailureMessage;
    char *pDiagnosticMessage;

    RX_ASSERT(pContext != NULL);
    RX_ASSERT(pContext->pCaseReport != NULL);
    RX_ASSERT(pExpression1 != NULL);
    RX_ASSERT(pExpression2 != NULL);
    RX_ASSERT(pFile != NULL);

    switch (op) {
        case RXP_OP_EQUAL:
            result = value1 == value2;
            break;
        case RXP_OP_NOT_EQUAL:
            result = value1 != value2;
            break;
        case RXP_OP_GREATER:
            result = value1 > value2;
            break;
        case RXP_OP_LESSER:
            result = value1 < value2;
            break;
        case RXP_OP_GREATER_OR_EQUAL:
            result = value1 >= value2;
            break;
        case RXP_OP_LESSER_OR_EQUAL:
            result = value1 <= value2;
            break;
        default:
            RX_ASSERT(0);
            break;
    }

    if (result) {
        pFailureMessage = NULL;
        pDiagnosticMessage = NULL;
    } else {
        const char *pOpName;
        const char *pOpSymbol;

        rxpGetComparisonOpName(&pOpName, op);
        rxpGetComparisonOpSymbol(&pOpSymbol, op);

        if (rxpMakeString(&pFailureMessage,
                          "‘%s’ is expected to be %s ‘%s’",
                          pExpression1,
                          pOpName,
                          pExpression2)
            != RX_SUCCESS) {
            RXP_LOG_TRACE("failed to create the failure message for the "
                          "integer comparison test located at %s:%d\n",
                          pFile,
                          line);
            pFailureMessage = NULL;
        }

        if (rxpMakeString(
                &pDiagnosticMessage, "%jd %s %jd", value1, pOpSymbol, value2)
            != RX_SUCCESS) {
            RXP_LOG_TRACE("failed to create the diagnostic message for the "
                          "integer comparison test located at %s:%d\n",
                          pFile,
                          line);
            pDiagnosticMessage = NULL;
        }
    }

    if (rxHandleTestResult(pContext,
                           result,
                           pFile,
                           line,
                           severity,
                           pFailureMessage,
                           pDiagnosticMessage)
        != RX_SUCCESS) {
        RXP_LOG_TRACE("failed to handle the test result for the integer "
                      "comparison test located at %s:%d\n",
                      pFile,
                      line);
    }

    if (pFailureMessage != NULL) {
        RX_FREE(pFailureMessage);
    }

    if (pDiagnosticMessage != NULL) {
        RX_FREE(pDiagnosticMessage);
    }

    if (!result && severity == RX_FATAL) {
        rxAbortTest(pContext);
    }
}

RXP_MAYBE_UNUSED static void
rxpAssessUnsignedIntegerComparisonTest(struct RxContext *pContext,
                                       uintmax_t value1,
                                       uintmax_t value2,
                                       enum RxpComparisonOp op,
                                       const char *pExpression1,
                                       const char *pExpression2,
                                       const char *pFile,
                                       int line,
                                       enum RxSeverity severity)
{
    int result;
    char *pFailureMessage;
    char *pDiagnosticMessage;

    RX_ASSERT(pContext != NULL);
    RX_ASSERT(pContext->pCaseReport != NULL);
    RX_ASSERT(pExpression1 != NULL);
    RX_ASSERT(pExpression2 != NULL);
    RX_ASSERT(pFile != NULL);

    switch (op) {
        case RXP_OP_EQUAL:
            result = value1 == value2;
            break;
        case RXP_OP_NOT_EQUAL:
            result = value1 != value2;
            break;
        case RXP_OP_GREATER:
            result = value1 > value2;
            break;
        case RXP_OP_LESSER:
            result = value1 < value2;
            break;
        case RXP_OP_GREATER_OR_EQUAL:
            result = value1 >= value2;
            break;
        case RXP_OP_LESSER_OR_EQUAL:
            result = value1 <= value2;
            break;
        default:
            RX_ASSERT(0);
            break;
    }

    if (result) {
        pFailureMessage = NULL;
        pDiagnosticMessage = NULL;
    } else {
        const char *pOpName;
        const char *pOpSymbol;

        rxpGetComparisonOpName(&pOpName, op);
        rxpGetComparisonOpSymbol(&pOpSymbol, op);

        if (rxpMakeString(&pFailureMessage,
                          "‘%s’ is expected to be %s ‘%s’",
                          pExpression1,
                          pOpName,
                          pExpression2)
            != RX_SUCCESS) {
            RXP_LOG_TRACE("failed to create the failure message for the "
                          "unsigned integer comparison test located at %s:%d\n",
                          pFile,
                          line);
            pFailureMessage = NULL;
        }

        if (rxpMakeString(
                &pDiagnosticMessage, "%ju %s %ju", value1, pOpSymbol, value2)
            != RX_SUCCESS) {
            RXP_LOG_TRACE("failed to create the diagnostic message for the "
                          "unsigned integer comparison test located at %s:%d\n",
                          pFile,
                          line);
            pDiagnosticMessage = NULL;
        }
    }

    if (rxHandleTestResult(pContext,
                           result,
                           pFile,
                           line,
                           severity,
                           pFailureMessage,
                           pDiagnosticMessage)
        != RX_SUCCESS) {
        RXP_LOG_TRACE("failed to handle the test result for the unsigned "
                      "integer comparison test located at %s:%d\n",
                      pFile,
                      line);
    }

    if (pFailureMessage != NULL) {
        RX_FREE(pFailureMessage);
    }

    if (pDiagnosticMessage != NULL) {
        RX_FREE(pDiagnosticMessage);
    }

    if (!result && severity == RX_FATAL) {
        rxAbortTest(pContext);
    }
}

RXP_MAYBE_UNUSED static void
rxpAssessFloatingPointComparisonTest(struct RxContext *pContext,
                                     long double value1,
                                     long double value2,
                                     enum RxpComparisonOp op,
                                     const char *pExpression1,
                                     const char *pExpression2,
                                     const char *pFile,
                                     int line,
                                     enum RxSeverity severity)
{
    int result;
    char *pFailureMessage;
    char *pDiagnosticMessage;

    RX_ASSERT(pContext != NULL);
    RX_ASSERT(pContext->pCaseReport != NULL);
    RX_ASSERT(pExpression1 != NULL);
    RX_ASSERT(pExpression2 != NULL);
    RX_ASSERT(pFile != NULL);

    switch (op) {
        case RXP_OP_EQUAL:
            result = value1 == value2;
            break;
        case RXP_OP_NOT_EQUAL:
            result = value1 != value2;
            break;
        case RXP_OP_GREATER:
            result = value1 > value2;
            break;
        case RXP_OP_LESSER:
            result = value1 < value2;
            break;
        case RXP_OP_GREATER_OR_EQUAL:
            result = value1 >= value2;
            break;
        case RXP_OP_LESSER_OR_EQUAL:
            result = value1 <= value2;
            break;
        default:
            RX_ASSERT(0);
            break;
    }

    if (result) {
        pFailureMessage = NULL;
        pDiagnosticMessage = NULL;
    } else {
        const char *pOpName;
        const char *pOpSymbol;

        rxpGetComparisonOpName(&pOpName, op);
        rxpGetComparisonOpSymbol(&pOpSymbol, op);

        if (rxpMakeString(&pFailureMessage,
                          "‘%s’ is expected to be %s ‘%s’",
                          pExpression1,
                          pOpName,
                          pExpression2)
            != RX_SUCCESS) {
            RXP_LOG_TRACE("failed to create the failure message for the "
                          "floating-point comparison test located at %s:%d\n",
                          pFile,
                          line);
            pFailureMessage = NULL;
        }

        if (rxpMakeString(
                &pDiagnosticMessage, "%Lf %s %Lf", value1, pOpSymbol, value2)
            != RX_SUCCESS) {
            RXP_LOG_TRACE("failed to create the diagnostic message for the "
                          "floating-point comparison test located at %s:%d\n",
                          pFile,
                          line);
            pDiagnosticMessage = NULL;
        }
    }

    if (rxHandleTestResult(pContext,
                           result,
                           pFile,
                           line,
                           severity,
                           pFailureMessage,
                           pDiagnosticMessage)
        != RX_SUCCESS) {
        RXP_LOG_TRACE("failed to handle the test result for the floating-point "
                      "comparison test located at %s:%d\n",
                      pFile,
                      line);
    }

    if (pFailureMessage != NULL) {
        RX_FREE(pFailureMessage);
    }

    if (pDiagnosticMessage != NULL) {
        RX_FREE(pDiagnosticMessage);
    }

    if (!result && severity == RX_FATAL) {
        rxAbortTest(pContext);
    }
}

RXP_MAYBE_UNUSED static void
rxpAssessFloatingPointFuzzyComparisonTest(struct RxContext *pContext,
                                          long double value1,
                                          long double value2,
                                          long double tolerance,
                                          enum RxpComparisonOp op,
                                          const char *pExpression1,
                                          const char *pExpression2,
                                          const char *pFile,
                                          int line,
                                          enum RxSeverity severity)
{
    int result;
    char *pFailureMessage;
    char *pDiagnosticMessage;

    RX_ASSERT(pContext != NULL);
    RX_ASSERT(pContext->pCaseReport != NULL);
    RX_ASSERT(pExpression1 != NULL);
    RX_ASSERT(pExpression2 != NULL);
    RX_ASSERT(pFile != NULL);

    switch (op) {
        case RXP_OP_EQUAL:
            rxpAreFloatingPointAlmostEqual(&result, value1, value2, tolerance);
            break;
        case RXP_OP_NOT_EQUAL:
            rxpAreFloatingPointAlmostEqual(&result, value1, value2, tolerance);
            result = !result;
            break;
        default:
            RX_ASSERT(0);
            break;
    }

    if (result) {
        pFailureMessage = NULL;
        pDiagnosticMessage = NULL;
    } else {
        const char *pOpName;
        const char *pOpSymbol;

        rxpGetComparisonOpName(&pOpName, op);
        rxpGetComparisonOpSymbol(&pOpSymbol, op);

        if (rxpMakeString(&pFailureMessage,
                          "‘%s’ is expected to be almost %s ‘%s’ (tolerance: "
                          "%Lf)",
                          pExpression1,
                          pOpName,
                          pExpression2,
                          tolerance)
            != RX_SUCCESS) {
            RXP_LOG_TRACE("failed to create the failure message for the "
                          "floating-point almost equal test located at %s:%d\n",
                          pFile,
                          line);
            pFailureMessage = NULL;
        }

        if (rxpMakeString(
                &pDiagnosticMessage, "%Lf %s %Lf", value1, pOpSymbol, value2)
            != RX_SUCCESS) {
            RXP_LOG_TRACE("failed to create the diagnostic message for the "
                          "floating-point almost equal test located at %s:%d\n",
                          pFile,
                          line);
            pDiagnosticMessage = NULL;
        }
    }

    if (rxHandleTestResult(pContext,
                           result,
                           pFile,
                           line,
                           severity,
                           pFailureMessage,
                           pDiagnosticMessage)
        != RX_SUCCESS) {
        RXP_LOG_TRACE("failed to handle the test result for the floating-point "
                      "almost equal test located at %s:%d\n",
                      pFile,
                      line);
    }

    if (pFailureMessage != NULL) {
        RX_FREE(pFailureMessage);
    }

    if (pDiagnosticMessage != NULL) {
        RX_FREE(pDiagnosticMessage);
    }

    if (!result && severity == RX_FATAL) {
        rxAbortTest(pContext);
    }
}

RXP_MAYBE_UNUSED static void
rxpAssessStringComparisonTest(struct RxContext *pContext,
                              const char *pStr1,
                              const char *pStr2,
                              enum RxpStringCase stringCase,
                              enum RxpComparisonOp op,
                              const char *pExpression1,
                              const char *pExpression2,
                              const char *pFile,
                              int line,
                              enum RxSeverity severity)
{
    int result;
    char *pFailureMessage;
    char *pDiagnosticMessage;

    RX_ASSERT(pContext != NULL);
    RX_ASSERT(pContext->pCaseReport != NULL);
    RX_ASSERT(pStr1 != NULL);
    RX_ASSERT(pStr2 != NULL);
    RX_ASSERT(pExpression1 != NULL);
    RX_ASSERT(pExpression2 != NULL);
    RX_ASSERT(pFile != NULL);

    switch (op) {
        case RXP_OP_EQUAL:
            stringCase == RXP_STRING_CASE_OBEY
                ? rxpAreStringsEqual(&result, pStr1, pStr2)
                : rxpAreStringsEqualNoCase(&result, pStr1, pStr2);
            break;
        case RXP_OP_NOT_EQUAL:
            stringCase == RXP_STRING_CASE_OBEY
                ? rxpAreStringsEqual(&result, pStr1, pStr2)
                : rxpAreStringsEqualNoCase(&result, pStr1, pStr2);
            result = !result;
            break;
        default:
            RX_ASSERT(0);
            break;
    }

    if (result) {
        pFailureMessage = NULL;
        pDiagnosticMessage = NULL;
    } else {
        const char *pStringCaseType;
        const char *pOpName;
        const char *pOpSymbol;

        rxpGetStringCaseType(&pStringCaseType, stringCase);
        rxpGetComparisonOpName(&pOpName, op);
        rxpGetComparisonOpSymbol(&pOpSymbol, op);

        if (rxpMakeString(&pFailureMessage,
                          "‘%s’ is expected to be %s ‘%s’ (case: %s)",
                          pExpression1,
                          pOpName,
                          pExpression2,
                          pStringCaseType)
            != RX_SUCCESS) {
            RXP_LOG_TRACE("failed to create the failure message for the "
                          "string comparison test located at %s:%d\n",
                          pFile,
                          line);
            pFailureMessage = NULL;
        }

        if (rxpMakeString(&pDiagnosticMessage,
                          "\"%s\" %s \"%s\"",
                          pStr1,
                          pOpSymbol,
                          pStr2)
            != RX_SUCCESS) {
            RXP_LOG_TRACE("failed to create the diagnostic message for the "
                          "string comparison test located at %s:%d\n",
                          pFile,
                          line);
            pDiagnosticMessage = NULL;
        }
    }

    if (rxHandleTestResult(pContext,
                           result,
                           pFile,
                           line,
                           severity,
                           pFailureMessage,
                           pDiagnosticMessage)
        != RX_SUCCESS) {
        RXP_LOG_TRACE("failed to handle the test result for the string "
                      "comparison test located at %s:%d\n",
                      pFile,
                      line);
    }

    if (pFailureMessage != NULL) {
        RX_FREE(pFailureMessage);
    }

    if (pDiagnosticMessage != NULL) {
        RX_FREE(pDiagnosticMessage);
    }

    if (!result && severity == RX_FATAL) {
        rxAbortTest(pContext);
    }
}

static void
rxpValidateTestSuite(int *pValid, const struct RxTestSuite *pSuite)
{
    RX_ASSERT(pSuite != NULL);

    *pValid = RXP_FALSE;

    if (pSuite->pName == NULL) {
        RXP_LOG_TRACE("the member ‘pName’ is NULL\n");
        return;
    }

    if (pSuite->caseCount > 0 && pSuite->pCases == NULL) {
        RXP_LOG_TRACE(
            "the member ‘pCases’ is NULL when ‘caseCount’ is not zero\n");
        return;
    }

    *pValid = RXP_TRUE;
}

static void
rxpValidateTestCase(int *pValid, const struct RxTestCase *pCase)
{
    RX_ASSERT(pCase != NULL);

    *pValid = RXP_FALSE;

    if (pCase->pName == NULL) {
        RXP_LOG_TRACE("the member ‘pName’ is NULL\n");
        return;
    }

    if (pCase->pfn == NULL) {
        RXP_LOG_TRACE("the member ‘pfn’ is NULL\n");
        return;
    }

    *pValid = RXP_TRUE;
}

static void
rxpValidateTestCaseReport(int *pValid, const struct RxTestCaseReport *pReport)
{
    size_t failureCount;

    RX_ASSERT(pReport != NULL);

    *pValid = RXP_FALSE;

    if (pReport->pFailures == NULL) {
        RXP_LOG_TRACE("the member ‘pFailures’ is NULL\n");
        return;
    }

    rxpGetTestFailureArraySize(&failureCount, pReport->pFailures);
    if (pReport->failureCount != failureCount) {
        RXP_LOG_TRACE("the member ‘failureCount’ is different from the size of "
                      "the array ‘pFailures’\n");
        return;
    }

    *pValid = RXP_TRUE;
}

RXP_MAYBE_UNUSED RXP_SCOPE void
rxGetLogLevelName(const char **ppString, enum RxLogLevel level)
{
    RX_ASSERT(ppString != NULL);

    switch (level) {
        case RX_LOG_LEVEL_DEBUG:
            *ppString = "debug";
            return;
        case RX_LOG_LEVEL_TRACE:
            *ppString = "trace";
            return;
        case RX_LOG_LEVEL_INFO:
            *ppString = "info";
            return;
        case RX_LOG_LEVEL_WARNING:
            *ppString = "warning";
            return;
        case RX_LOG_LEVEL_ERROR:
            *ppString = "error";
            return;
        default:
            *ppString = "invalid";
    }
}

RXP_MAYBE_UNUSED RXP_SCOPE void
rxAbortTest(struct RxContext *pContext)
{
    longjmp(pContext->env, 1);
}

RXP_MAYBE_UNUSED RXP_SCOPE enum RxStatus
rxHandleTestResult(struct RxContext *pContext,
                   int result,
                   const char *pFile,
                   int line,
                   enum RxSeverity severity,
                   const char *pFailureMessage,
                   const char *pDiagnosticMessage)
{
    enum RxStatus status;
    int valid;
    struct RxTestCaseReport *pReport;
    struct RxTestFailure *pFailure;

    RX_ASSERT(pContext != NULL);
    RX_ASSERT(pFile != NULL);

    rxpValidateTestCaseReport(&valid, pContext->pCaseReport);
    RX_ASSERT(valid);

    pReport = pContext->pCaseReport;

    ++pReport->testCount;

    if (result) {
        return RX_SUCCESS;
    }

    status = rxpExtendTestFailureArrayBack(&pFailure, &pReport->pFailures, 1);
    if (status != RX_SUCCESS) {
        RXP_LOG_WARNING("failed to extend the test failure array for the test "
                        "located at %s:%d\n",
                        pFile,
                        line);
        return status;
    }

    rxpGetTestFailureArraySize(&pReport->failureCount, pReport->pFailures);

    {
        char *pBuffer;

        status = rxpMakeStringCopy(&pBuffer, pFile);
        if (status != RX_SUCCESS) {
            RXP_LOG_ERROR("failed to allocate the file name for the "
                          "test located at %s:%d\n",
                          pFile,
                          line);
        }

        pFailure->pFile = pBuffer;
    }

    pFailure->line = line;
    pFailure->severity = severity;

    if (pFailureMessage == NULL) {
        pFailure->pMessage = NULL;
    } else {
        char *pBuffer;

        status = rxpMakeStringCopy(&pBuffer, pFailureMessage);
        if (status != RX_SUCCESS) {
            RXP_LOG_WARNING("failed to allocate the failure message for the "
                            "test located at %s:%d\n",
                            pFile,
                            line);
            pFailure->pMessage = NULL;
        } else {
            pFailure->pMessage = pBuffer;
        }
    }

    if (pDiagnosticMessage == NULL) {
        pFailure->pDiagnosticMessage = NULL;
    } else {
        char *pBuffer;

        status = rxpMakeStringCopy(&pBuffer, pDiagnosticMessage);
        if (status != RX_SUCCESS) {
            RXP_LOG_WARNING("failed to allocate the diagnostic message for the "
                            "test located at %s:%d\n",
                            pFile,
                            line);
            pFailure->pDiagnosticMessage = NULL;
        } else {
            pFailure->pDiagnosticMessage = pBuffer;
        }
    }

    return RX_SUCCESS;
}

RXP_MAYBE_UNUSED RXP_SCOPE enum RxStatus
rxInitializeTestCaseReport(struct RxTestCaseReport *pReport,
                           const struct RxTestSuite *pSuite,
                           const struct RxTestCase *pCase)
{
    enum RxStatus status;

    RX_ASSERT(pReport != NULL);
    RX_ASSERT(pSuite != NULL);
    RX_ASSERT(pCase != NULL);

    pReport->pSuite = pSuite;
    pReport->pCase = pCase;
    pReport->testCount = 0;
    pReport->elapsed = 0;

    status = rxpCreateTestFailureArray(&pReport->pFailures, 0);
    if (status != RX_SUCCESS) {
        RXP_LOG_TRACE("failed to create the test failure array (suite: \"%s\", "
                      "case: \"%s\")\n",
                      pSuite->pName,
                      pCase->pName);
        return status;
    }

    rxpGetTestFailureArraySize(&pReport->failureCount, pReport->pFailures);
    RX_ASSERT(pReport->failureCount == 0);

    RX_ASSERT(status == RX_SUCCESS);
    return status;
}

RXP_MAYBE_UNUSED RXP_SCOPE void
rxTerminateTestCaseReport(struct RxTestCaseReport *pReport)
{
    size_t i;
    int valid;

    if (pReport == NULL) {
        RXP_LOG_WARNING("invalid argument ‘pReport’ (NULL)\n");
        return;
    }

    rxpValidateTestCaseReport(&valid, pReport);
    if (!valid) {
        RXP_LOG_WARNING("invalid argument ‘pReport’\n");
        return;
    }

    for (i = 0; i < pReport->failureCount; ++i) {
        const struct RxTestFailure *pFailure;

        pFailure = &pReport->pFailures[i];

        RX_FREE((void *)(uintptr_t)pFailure->pFile);

        if (pFailure->pMessage != NULL) {
            RX_FREE((void *)(uintptr_t)pFailure->pMessage);
        }

        if (pFailure->pDiagnosticMessage != NULL) {
            RX_FREE((void *)(uintptr_t)pFailure->pDiagnosticMessage);
        }
    }

    rxpDestroyTestFailureArray(pReport->pFailures);
}

RXP_MAYBE_UNUSED RXP_SCOPE enum RxStatus
rxRunTestCase(struct RxTestCaseReport *pReport,
              const struct RxTestSuite *pSuite,
              const struct RxTestCase *pCase)
{
    enum RxStatus status;
    int valid;
    struct RxContext context;
    uint64_t timeStart;
    uint64_t timeEnd;

    RX_ASSERT(pReport != NULL);
    RX_ASSERT(pSuite != NULL);
    RX_ASSERT(pCase != NULL);

    rxpValidateTestCaseReport(&valid, pReport);
    RX_ASSERT(valid);

    rxpValidateTestSuite(&valid, pSuite);
    RX_ASSERT(valid);

    rxpValidateTestCase(&valid, pCase);
    RX_ASSERT(valid);

    context.pCaseReport = pReport;

    if (pSuite->pfnSetUp != NULL) {
        status = pSuite->pfnSetUp(&context.pFixture);
        if (status != RX_SUCCESS) {
            RXP_LOG_ERROR(
                "failed to set-up the fixture (suite: \"%s\", case: \"%s\")\n",
                pSuite->pName,
                pCase->pName);
            return status;
        }
    } else {
        context.pFixture = NULL;
    }

    if (rxpGetRealTime(&timeStart) != RX_SUCCESS) {
        timeStart = (uint64_t)-1;
    }

    if (setjmp(context.env) == 0) {
        pCase->pfn(&context);
    }

    if (timeStart == (uint64_t)-1 || rxpGetRealTime(&timeEnd) != RX_SUCCESS) {
        RXP_LOG_WARNING("failed to measure the time elapsed (suite: \"%s\", "
                        "case: \"%s\")\n",
                        pSuite->pName,
                        pCase->pName);
        pReport->elapsed = 0;
    } else {
        RX_ASSERT(timeEnd >= timeStart);
        pReport->elapsed = timeEnd - timeStart;
    }

    if (pSuite->pfnTearDown != NULL) {
        pSuite->pfnTearDown(context.pFixture);
    }

    return RX_SUCCESS;
}

RXP_MAYBE_UNUSED RXP_SCOPE void
rxPrintTestCaseRunSummary(const struct RxTestCaseReport *pReport)
{
    size_t i;
    int valid;
    int passed;
    const char *pResultStyleStart;
    const char *pStyleEnd;

    if (pReport == NULL) {
        RXP_LOG_WARNING("invalid argument ‘pReport’ (NULL)\n");
        return;
    }

    rxpValidateTestCaseReport(&valid, pReport);
    if (!valid) {
        RXP_LOG_WARNING("invalid argument ‘pReport’\n");
        return;
    }

    passed = pReport->failureCount == 0;

#ifdef RXP_LOG_STYLING
    if (isatty(fileno(stderr))) {
        rxpGetStyleAnsiCode(
            &pResultStyleStart,
            passed ? RXP_STYLE_BRIGHT_GREEN : RXP_STYLE_BRIGHT_RED);
        rxpGetStyleAnsiCode(&pStyleEnd, RXP_STYLE_RESET);
    } else {
        pResultStyleStart = pStyleEnd = "";
    }
#else
    pResultStyleStart = pStyleEnd = "";
#endif /* RXP_LOG_STYLING */

    fprintf(stderr,
            "[%s%s%s] \"%s\" / \"%s\" (%f ms)\n",
            pResultStyleStart,
            passed ? "PASSED" : "FAILED",
            pStyleEnd,
            pReport->pSuite->pName,
            pReport->pCase->pName,
            (double)pReport->elapsed * (1000.0 / RXP_TICKS_PER_SECOND));

    for (i = 0; i < pReport->failureCount; ++i) {
        const struct RxTestFailure *pFailure;
        const char *pFailureMessage;

        pFailure = &pReport->pFailures[i];
        pFailureMessage = pFailure->pMessage == NULL ? "" : pFailure->pMessage;

        if (pFailure->pDiagnosticMessage != NULL) {
            fprintf(stderr,
                    "%s:%d: %s test failure: %s\n%s\n",
                    pFailure->pFile,
                    pFailure->line,
                    pFailure->severity == RX_FATAL ? "fatal" : "nonfatal",
                    pFailureMessage,
                    pFailure->pDiagnosticMessage);
        } else {
            fprintf(stderr,
                    "%s:%d: %s test failure: %s\n",
                    pFailure->pFile,
                    pFailure->line,
                    pFailure->severity == RX_FATAL ? "fatal" : "nonfatal",
                    pFailureMessage);
        }
    }
}

RXP_MAYBE_UNUSED RXP_SCOPE enum RxStatus
rxRun(size_t suiteCount,
      const struct RxTestSuite *pSuites,
      int argc,
      const char **ppArgv)
{
    size_t i;
    size_t j;
    enum RxStatus status;
    size_t allocatedCaseReportsCount;
    size_t initializedCaseReportCount;
    int valid;
    struct RxTestSuiteReport *pSuiteReports;

    RXP_UNUSED(argc);
    RXP_UNUSED(ppArgv);

    RX_ASSERT(pSuites != NULL);

    status = RX_SUCCESS;

    if (suiteCount == 0) {
        RXP_LOG_INFO("nothing to run\n");
        goto exit;
    }

    for (i = 0; i < suiteCount; ++i) {
        rxpValidateTestSuite(&valid, &pSuites[i]);
        RX_ASSERT(valid);

        for (j = 0; j < pSuites[i].caseCount; ++j) {
            rxpValidateTestCase(&valid, &pSuites[i].pCases[j]);
            RX_ASSERT(valid);
        }
    }

    pSuiteReports = (struct RxTestSuiteReport *)RX_MALLOC(sizeof *pSuiteReports
                                                          * suiteCount);
    if (pSuiteReports == NULL) {
        status = RX_ERROR_ALLOCATION;
        RXP_LOG_ERROR("failed to allocate the test suite reports\n");
        goto exit;
    }

    allocatedCaseReportsCount = 0;
    for (i = 0; i < suiteCount; ++i) {
        const struct RxTestSuite *pSuite;
        struct RxTestSuiteReport *pSuiteReport;

        pSuite = &pSuites[i];
        pSuiteReport = &pSuiteReports[i];

        pSuiteReport->pSuite = pSuite;
        pSuiteReport->caseReportCount = pSuite->caseCount;

        if (pSuite->caseCount == 0) {
            pSuiteReport->pCaseReports = NULL;
            continue;
        }

        pSuiteReport->pCaseReports = (struct RxTestCaseReport *)RX_MALLOC(
            sizeof *pSuiteReport->pCaseReports * pSuite->caseCount);
        if (pSuiteReport->pCaseReports == NULL) {
            status = RX_ERROR_ALLOCATION;
            RXP_LOG_ERROR(
                "failed to allocate the test case reports (suite: \"%s\")\n",
                pSuite->pName);
            goto suite_reports_cleanup;
        }

        ++allocatedCaseReportsCount;

        initializedCaseReportCount = 0;
        for (j = 0; j < pSuite->caseCount; ++j) {
            const struct RxTestCase *pCase;
            struct RxTestCaseReport *pCaseReport;

            pCase = &pSuite->pCases[j];
            pCaseReport = &pSuiteReport->pCaseReports[j];

            status = rxInitializeTestCaseReport(pCaseReport, pSuite, pCase);
            if (status != RX_SUCCESS) {
                RXP_LOG_ERROR("failed to initialize the test case report "
                              "(suite: \"%s\", case: \"%s\")\n",
                              pSuite->pName,
                              pCase->pName);
                goto suite_reports_cleanup;
            }

            ++initializedCaseReportCount;

            status = rxRunTestCase(pCaseReport, pSuite, pCase);
            if (status != RX_SUCCESS) {
                RXP_LOG_ERROR(
                    "failed to run a test case (suite: \"%s\", case: \"%s\")\n",
                    pSuite->pName,
                    pCase->pName);
                goto suite_reports_cleanup;
            }

            rxPrintTestCaseRunSummary(pCaseReport);
        }
    }

    RX_ASSERT(status == RX_SUCCESS);

suite_reports_cleanup:
    for (i = 0; i < allocatedCaseReportsCount; ++i) {
        for (j = 0; j < initializedCaseReportCount; ++j) {
            rxTerminateTestCaseReport(&pSuiteReports[i].pCaseReports[j]);
        }

        RX_FREE(pSuiteReports[i].pCaseReports);
    }

    RX_FREE(pSuiteReports);

exit:
    return status;
}

#endif /* REXO_REXO_H */
