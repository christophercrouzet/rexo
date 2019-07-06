#include <rexo/rexo.h>

#include <stddef.h>

#define PI 3.14159265358979323846

struct Fixture {
    int zero;
    unsigned int one;
};

enum RxStatus
setUp(void **ppFixture)
{
    struct Fixture *pData;

    pData = (struct Fixture *)malloc(sizeof *pData);
    if (pData == NULL) {
        return RX_ERROR_ALLOCATION;
    }

    pData->zero = 0;
    pData->one = 1;

    *ppFixture = (void *)pData;
    return RX_SUCCESS;
}

void
tearDown(void *pFixture)
{
    free(pFixture);
}

RX_TEST_CASE(testBasics)
{
    struct Fixture *pData;

    pData = (struct Fixture *)RX_FIXTURE;

    RX_CHECK_INT_EQUAL(pData->zero, 0);
    RX_CHECK_UINT_EQUAL(pData->one, 1);
    RX_CHECK_FP_ALMOST_EQUAL(sin(PI), 0.0, 1.0e-6);
    RX_CHECK_FP_ALMOST_EQUAL(sin(PI * 0.5), 1.0, 1.0e-6);
    RX_CHECK_STR_EQUAL_NO_CASE("abc", "ABC");
}

RX_TEST_CASE(testFailure)
{
    struct Fixture *pData;

    pData = (struct Fixture *)RX_FIXTURE;

    RX_CHECK_INT_EQUAL(pData->zero, 1);
}

static const struct RxTestCase cases[]
    = {{"basics", testBasics}, {"failure", testFailure}};

static const struct RxTestSuite suites[]
    = {{"example", sizeof cases / sizeof cases[0], cases, setUp, tearDown}};

int
main(int argc, const char **ppArgv)
{
    rxRun(sizeof suites / sizeof suites[0], suites, argc, ppArgv);
    return 0;
}
