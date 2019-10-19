#include <assert.h>

#include <rexo.h>

static int step = 0;

struct my_data {
    int value;
};

RX_SET_UP(my_set_up, struct my_data)
{
    assert(step++ == 1);
    RX_DATA->value = 123;
    return RX_SUCCESS;
}

RX_TEAR_DOWN(my_tear_down, struct my_data)
{
    assert(step++ == 3);
    assert(RX_DATA->value == 123);
}

RX_FIXTURE(my_fixture,
           struct my_data,
           .set_up = my_set_up,
           .tear_down = my_tear_down);

RX_TEST_CASE_FIXTURE(my_test_suite, my_test_case, my_fixture)
{
    assert(step++ == 2);
    assert(RX_DATA->value == 123);
}

int
main(int argc, const char **argv)
{
    assert(step++ == 0);
    assert(rx_run(argc, argv, 0, NULL) == RX_SUCCESS);
    assert(step++ == 4);
    return 0;
}
