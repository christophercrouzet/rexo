#define RX_DEBUG_LOGGING_LEVEL
#include <rexo/rexo.h>

#include <stddef.h>
#include <stdio.h>
#include <string.h>

#define PI 3.14159265358979323846

int
zero()
{
    return 0;
}

int
two()
{
    return 2;
}

float
zerof()
{
    return 0.0f;
}

float
twof()
{
    return 2.0f;
}

RX_TEST_CASE(successfulCustomMessage)
{
    RX_CHECK(5 > 2, "five should be greater than two");
}

RX_TEST_CASE(successfulBooleanTrue)
{
    RX_CHECK_TRUE(1);
    RX_CHECK_TRUE(two());
}

RX_TEST_CASE(successfulBooleanFalse)
{
    RX_CHECK_FALSE(0);
    RX_CHECK_FALSE(zero());
}

RX_TEST_CASE(successfulIntegerEqual)
{
    RX_CHECK_INT_EQUAL(zero(), 0);
    RX_CHECK_INT_EQUAL(2, two());
}

RX_TEST_CASE(successfulIntegerNotEqual)
{
    RX_CHECK_INT_NOT_EQUAL(zero(), 2);
    RX_CHECK_INT_NOT_EQUAL(0, two());
}

RX_TEST_CASE(successfulIntegerGreater)
{
    RX_CHECK_INT_GREATER(two(), 0);
    RX_CHECK_INT_GREATER(2, zero());
}

RX_TEST_CASE(successfulIntegerLesser)
{
    RX_CHECK_INT_LESSER(zero(), 2);
    RX_CHECK_INT_LESSER(0, two());
}

RX_TEST_CASE(successfulIntegerGreaterOrEqual)
{
    RX_CHECK_INT_GREATER_OR_EQUAL(two(), 2);
    RX_CHECK_INT_GREATER_OR_EQUAL(4, two());
}

RX_TEST_CASE(successfulIntegerLesserOrEqual)
{
    RX_CHECK_INT_LESSER_OR_EQUAL(2, two());
    RX_CHECK_INT_LESSER_OR_EQUAL(two(), 4);
}

RX_TEST_CASE(successfulUnsignedIntegerEqual)
{
    RX_CHECK_UINT_EQUAL((unsigned int)zero(), 0);
    RX_CHECK_UINT_EQUAL(2, (unsigned int)two());
}

RX_TEST_CASE(successfulUnsignedIntegerNotEqual)
{
    RX_CHECK_UINT_NOT_EQUAL((unsigned int)zero(), 2);
    RX_CHECK_UINT_NOT_EQUAL(0, (unsigned int)two());
}

RX_TEST_CASE(successfulUnsignedIntegerGreater)
{
    RX_CHECK_UINT_GREATER((unsigned int)two(), 0);
    RX_CHECK_UINT_GREATER(2, (unsigned int)zero());
}

RX_TEST_CASE(successfulUnsignedIntegerLesser)
{
    RX_CHECK_UINT_LESSER((unsigned int)zero(), 2);
    RX_CHECK_UINT_LESSER(0, (unsigned int)two());
}

RX_TEST_CASE(successfulUnsignedIntegerGreaterOrEqual)
{
    RX_CHECK_UINT_GREATER_OR_EQUAL((unsigned int)two(), 2);
    RX_CHECK_UINT_GREATER_OR_EQUAL(4, (unsigned int)two());
}

RX_TEST_CASE(successfulUnsignedIntegerLesserOrEqual)
{
    RX_CHECK_UINT_LESSER_OR_EQUAL(2, (unsigned int)two());
    RX_CHECK_UINT_LESSER_OR_EQUAL((unsigned int)two(), 4);
}

RX_TEST_CASE(successfulRealEqual)
{
    RX_CHECK_REAL_EQUAL(zerof(), 0.0f);
    RX_CHECK_REAL_EQUAL(2.0f, twof());
}

RX_TEST_CASE(successfulRealNotEqual)
{
    RX_CHECK_REAL_NOT_EQUAL(zerof(), 2.0f);
    RX_CHECK_REAL_NOT_EQUAL(0.0f, twof());
}

RX_TEST_CASE(successfulRealGreater)
{
    RX_CHECK_REAL_GREATER(twof(), 0.0f);
    RX_CHECK_REAL_GREATER(2.0f, zerof());
}

RX_TEST_CASE(successfulRealLesser)
{
    RX_CHECK_REAL_LESSER(zerof(), 2.0f);
    RX_CHECK_REAL_LESSER(0.0f, twof());
}

RX_TEST_CASE(successfulRealGreaterOrEqual)
{
    RX_CHECK_REAL_GREATER_OR_EQUAL(twof(), 2.0f);
    RX_CHECK_REAL_GREATER_OR_EQUAL(4.0f, twof());
}

RX_TEST_CASE(successfulRealLesserOrEqual)
{
    RX_CHECK_REAL_LESSER_OR_EQUAL(2.0f, twof());
    RX_CHECK_REAL_LESSER_OR_EQUAL(twof(), 4.0f);
}

RX_TEST_CASE(successfulRealAlmostEqual)
{
    RX_CHECK_REAL_ALMOST_EQUAL(sin(PI), 0.0, 1.0e-6);
    RX_CHECK_REAL_ALMOST_EQUAL(1.0, sin(PI * 0.5), 1.0e-6);
}

RX_TEST_CASE(successfulRealAlmostNotEqual)
{
    RX_CHECK_REAL_NOT_ALMOST_EQUAL(zerof(), 0.1, 1.0e-6);
    RX_CHECK_REAL_NOT_ALMOST_EQUAL(2.1, twof(), 1.0e-6);
}

RX_TEST_CASE(successfulStringEqual)
{
    static const char *pString = "abc";

    RX_CHECK_STR_EQUAL(pString, "abc");
}

RX_TEST_CASE(successfulStringNotEqual)
{
    static const char *pString = "abcd";

    RX_CHECK_STR_NOT_EQUAL(pString, "abc");
}

RX_TEST_CASE(successfulStringEqualNoCase)
{
    static const char *pString = "AbC";

    RX_CHECK_STR_EQUAL_NO_CASE(pString, "abc");
}

RX_TEST_CASE(successfulStringNotEqualNoCase)
{
    static const char *pString = "AbcD";

    RX_CHECK_STR_NOT_EQUAL_NO_CASE(pString, "abc");
}

RX_TEST_CASE(unsuccessfulCustomMessage)
{
    RX_CHECK(5 < 2, "five should be smaller than two");
}

RX_TEST_CASE(unsuccessfulBooleanTrue)
{
    RX_CHECK_TRUE(0);
    RX_CHECK_TRUE(zero());
}

RX_TEST_CASE(unsuccessfulBooleanFalse)
{
    RX_CHECK_FALSE(1);
    RX_CHECK_FALSE(two());
}

RX_TEST_CASE(unsuccessfulIntegerEqual)
{
    RX_CHECK_INT_EQUAL(zero(), 2);
    RX_CHECK_INT_EQUAL(0, two());
}

RX_TEST_CASE(unsuccessfulIntegerNotEqual)
{
    RX_CHECK_INT_NOT_EQUAL(zero(), 0);
    RX_CHECK_INT_NOT_EQUAL(2, two());
}

RX_TEST_CASE(unsuccessfulIntegerGreater)
{
    RX_CHECK_INT_GREATER(two(), 2);
    RX_CHECK_INT_GREATER(0, zero());
}

RX_TEST_CASE(unsuccessfulIntegerLesser)
{
    RX_CHECK_INT_LESSER(2, zero());
    RX_CHECK_INT_LESSER(two(), 0);
}

RX_TEST_CASE(unsuccessfulIntegerGreaterOrEqual)
{
    RX_CHECK_INT_GREATER_OR_EQUAL(two(), 4);
    RX_CHECK_INT_GREATER_OR_EQUAL(0, two());
}

RX_TEST_CASE(unsuccessfulIntegerLesserOrEqual)
{
    RX_CHECK_INT_LESSER_OR_EQUAL(two(), 0);
    RX_CHECK_INT_LESSER_OR_EQUAL(4, two());
}

RX_TEST_CASE(unsuccessfulUnsignedIntegerEqual)
{
    RX_CHECK_UINT_EQUAL((unsigned int)zero(), 2);
    RX_CHECK_UINT_EQUAL(0, (unsigned int)two());
}

RX_TEST_CASE(unsuccessfulUnsignedIntegerNotEqual)
{
    RX_CHECK_UINT_NOT_EQUAL((unsigned int)zero(), 0);
    RX_CHECK_UINT_NOT_EQUAL(2, (unsigned int)two());
}

RX_TEST_CASE(unsuccessfulUnsignedIntegerGreater)
{
    RX_CHECK_UINT_GREATER((unsigned int)two(), 2);
    RX_CHECK_UINT_GREATER(0, (unsigned int)zero());
}

RX_TEST_CASE(unsuccessfulUnsignedIntegerLesser)
{
    RX_CHECK_UINT_LESSER(2, (unsigned int)zero());
    RX_CHECK_UINT_LESSER((unsigned int)two(), 0);
}

RX_TEST_CASE(unsuccessfulUnsignedIntegerGreaterOrEqual)
{
    RX_CHECK_UINT_GREATER_OR_EQUAL((unsigned int)two(), 4);
    RX_CHECK_UINT_GREATER_OR_EQUAL(0, (unsigned int)two());
}

RX_TEST_CASE(unsuccessfulUnsignedIntegerLesserOrEqual)
{
    RX_CHECK_UINT_LESSER_OR_EQUAL((unsigned int)two(), 0);
    RX_CHECK_UINT_LESSER_OR_EQUAL(4, (unsigned int)two());
}

RX_TEST_CASE(unsuccessfulRealEqual)
{
    RX_CHECK_REAL_EQUAL(zerof(), 2.0f);
    RX_CHECK_REAL_EQUAL(0.0f, twof());
}

RX_TEST_CASE(unsuccessfulRealNotEqual)
{
    RX_CHECK_REAL_NOT_EQUAL(zerof(), 0.0f);
    RX_CHECK_REAL_NOT_EQUAL(2.0f, twof());
}

RX_TEST_CASE(unsuccessfulRealGreater)
{
    RX_CHECK_REAL_GREATER(twof(), 2.0f);
    RX_CHECK_REAL_GREATER(0.0f, zerof());
}

RX_TEST_CASE(unsuccessfulRealLesser)
{
    RX_CHECK_REAL_LESSER(2.0f, zerof());
    RX_CHECK_REAL_LESSER(twof(), 0.0f);
}

RX_TEST_CASE(unsuccessfulRealGreaterOrEqual)
{
    RX_CHECK_REAL_GREATER_OR_EQUAL(twof(), 4.0f);
    RX_CHECK_REAL_GREATER_OR_EQUAL(0.0f, twof());
}

RX_TEST_CASE(unsuccessfulRealLesserOrEqual)
{
    RX_CHECK_REAL_LESSER_OR_EQUAL(twof(), 0.0f);
    RX_CHECK_REAL_LESSER_OR_EQUAL(4.0f, twof());
}

RX_TEST_CASE(unsuccessfulRealAlmostEqual)
{
    RX_CHECK_REAL_ALMOST_EQUAL(sin(PI), 1.0, 1.0e-6);
    RX_CHECK_REAL_ALMOST_EQUAL(0.0, sin(PI * 0.5), 1.0e-6);
}

RX_TEST_CASE(unsuccessfulRealAlmostNotEqual)
{
    RX_CHECK_REAL_NOT_ALMOST_EQUAL(zerof(), 0.1f, 1.0e-6f);
    RX_CHECK_REAL_NOT_ALMOST_EQUAL(2.0f, twof(), 1.0e-6f);
}

RX_TEST_CASE(unsuccessfulStringEqual)
{
    static const char *pString = "abc";

    RX_CHECK_STR_EQUAL(pString, "ABC");
}

RX_TEST_CASE(unsuccessfulStringNotEqual)
{
    static const char *pString = "abcd";

    RX_CHECK_STR_NOT_EQUAL(pString, "abcd");
}

RX_TEST_CASE(unsuccessfulStringEqualNoCase)
{
    static const char *pString = "AbC";

    RX_CHECK_STR_EQUAL_NO_CASE(pString, "AbCd");
}

RX_TEST_CASE(unsuccessfulStringNotEqualNoCase)
{
    static const char *pString = "AbcD";

    RX_CHECK_STR_NOT_EQUAL_NO_CASE(pString, "abcd");
}

enum RxStatus
setUp(void **ppFixture)
{
    int *pData;

    pData = (int *)malloc(sizeof(int));
    if (pData == NULL) {
        return RX_ERROR_ALLOCATION;
    }

    *pData = 123;

    *ppFixture = (void *)pData;
    return RX_SUCCESS;
}

void
tearDown(void *pFixture)
{
    free(pFixture);
}

RX_TEST_CASE(fixture)
{
    int *pData;

    pData = (int *)RX_FIXTURE;

    RX_CHECK_TRUE(*pData == 123);
}

RX_TEST_CASE(fatalFailure)
{
    RX_REQUIRE_TRUE(0);
    printf("This should never be reached!\n");
    abort();
}

static const struct RxTestCase successfulCases[]
    = {{"custom message", successfulCustomMessage},
       {"boolean true", successfulBooleanTrue},
       {"boolean false", successfulBooleanFalse},
       {"integer equal", successfulIntegerEqual},
       {"integer not equal", successfulIntegerNotEqual},
       {"integer greater", successfulIntegerGreater},
       {"integer lesser", successfulIntegerLesser},
       {"integer greater or equal", successfulIntegerGreaterOrEqual},
       {"integer lesser or equal", successfulIntegerLesserOrEqual},
       {"unsigned integer equal", successfulUnsignedIntegerEqual},
       {"unsigned integer not equal", successfulUnsignedIntegerNotEqual},
       {"unsigned integer greater", successfulUnsignedIntegerGreater},
       {"unsigned integer lesser", successfulUnsignedIntegerLesser},
       {"unsigned integer greater or equal",
        successfulUnsignedIntegerGreaterOrEqual},
       {"unsigned integer lesser or equal",
        successfulUnsignedIntegerLesserOrEqual},
       {"real equal", successfulRealEqual},
       {"real not equal", successfulRealNotEqual},
       {"real greater", successfulRealGreater},
       {"real lesser", successfulRealLesser},
       {"real greater or equal", successfulRealGreaterOrEqual},
       {"real lesser or equal", successfulRealLesserOrEqual},
       {"real equal", successfulRealAlmostEqual},
       {"real not equal", successfulRealAlmostNotEqual},
       {"string equal", successfulStringEqual},
       {"string not equal", successfulStringNotEqual},
       {"string equal no case", successfulStringEqualNoCase},
       {"string not equal no case", successfulStringNotEqualNoCase}};
static const struct RxTestCase unsuccessfulCases[]
    = {{"custom message", unsuccessfulCustomMessage},
       {"boolean true", unsuccessfulBooleanTrue},
       {"boolean false", unsuccessfulBooleanFalse},
       {"integer equal", unsuccessfulIntegerEqual},
       {"integer not equal", unsuccessfulIntegerNotEqual},
       {"integer greater", unsuccessfulIntegerGreater},
       {"integer lesser", unsuccessfulIntegerLesser},
       {"integer greater or equal", unsuccessfulIntegerGreaterOrEqual},
       {"integer lesser or equal", unsuccessfulIntegerLesserOrEqual},
       {"unsigned integer equal", unsuccessfulUnsignedIntegerEqual},
       {"unsigned integer not equal", unsuccessfulUnsignedIntegerNotEqual},
       {"unsigned integer greater", unsuccessfulUnsignedIntegerGreater},
       {"unsigned integer lesser", unsuccessfulUnsignedIntegerLesser},
       {"unsigned integer greater or equal",
        unsuccessfulUnsignedIntegerGreaterOrEqual},
       {"unsigned integer lesser or equal",
        unsuccessfulUnsignedIntegerLesserOrEqual},
       {"real equal", unsuccessfulRealEqual},
       {"real not equal", unsuccessfulRealNotEqual},
       {"real greater", unsuccessfulRealGreater},
       {"real lesser", unsuccessfulRealLesser},
       {"real greater or equal", unsuccessfulRealGreaterOrEqual},
       {"real lesser or equal", unsuccessfulRealLesserOrEqual},
       {"real equal", unsuccessfulRealAlmostEqual},
       {"real not equal", unsuccessfulRealAlmostNotEqual},
       {"string equal", unsuccessfulStringEqual},
       {"string not equal", unsuccessfulStringNotEqual},
       {"string equal no case", unsuccessfulStringEqualNoCase},
       {"string not equal no case", unsuccessfulStringNotEqualNoCase}};
static const struct RxTestCase miscCases[]
    = {{"fixture", fixture}, {"fatal failure", fatalFailure}};
static const struct RxTestSuite suites[]
    = {{"successes",
        sizeof successfulCases / sizeof successfulCases[0],
        successfulCases,
        NULL,
        NULL},
       {"failures",
        sizeof unsuccessfulCases / sizeof unsuccessfulCases[0],
        unsuccessfulCases,
        NULL,
        NULL},
       {"misc",
        sizeof miscCases / sizeof miscCases[0],
        miscCases,
        setUp,
        tearDown}};

int
main(int argc, const char **ppArgv)
{
    rxRun(sizeof suites / sizeof suites[0], suites, argc, ppArgv);
    return 0;
}
