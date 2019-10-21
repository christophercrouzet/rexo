#include <assert.h>

#include <rexo.h>

static int step = 0;

RX_SET_UP(my_set_up, void *)
{
    ++step;
    assert(step == 2);

    return RX_SUCCESS;
}

RX_TEAR_DOWN(my_tear_down, void *)
{
    ++step;
    assert(step == 4);
}

RX_FIXTURE_VOID(my_fixture, .set_up = my_set_up, .tear_down = my_tear_down);

RX_TEST_CASE_FIXTURE(my_test_suite, my_test_case, my_fixture)
{
    ++step;
    assert(step == 3);
}

int
main(int argc, const char **argv)
{
    ++step;
    assert(step == 1);

    assert(rx_run(argc, argv, 0, NULL) == RX_SUCCESS);

    ++step;
    assert(step == 5);

    return 0;
}
