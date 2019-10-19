#include <assert.h>

#include <rexo.h>

static int step = 0;

struct my_data {
    int value;
};

RX_FIXTURE(my_fixture, struct my_data);

RX_TEST_CASE_FIXTURE(my_test_suite, my_test_case, my_fixture)
{
    assert(step++ == 1);
    RX_DATA->value = 123;
}

int
main(int argc, const char **argv)
{
    assert(step++ == 0);
    assert(rx_run(argc, argv, 0, NULL) == RX_SUCCESS);
    assert(step++ == 2);
    return 0;
}
