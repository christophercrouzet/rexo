#include <assert.h>

#include <rexo.h>

static int step = 0;

RX_SET_UP(my_set_up, void *)
{
    assert(step++ == 1);
    return RX_SUCCESS;
}

RX_TEAR_DOWN(my_tear_down, void *)
{
    assert(step++ == 3);
}

RX_FIXTURE(my_fixture, void *, .set_up = my_set_up, .tear_down = my_tear_down);

RX_TEST_CASE_FIXTURE(my_test_suite, my_test_case, my_fixture)
{
    assert(step++ == 2);
}

int
main(int argc, const char **argv)
{
    assert(step++ == 0);
    assert(rx_run(argc, argv, 0, NULL) == RX_SUCCESS);
    assert(step++ == 4);
    return 0;
}
