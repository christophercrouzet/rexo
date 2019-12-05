#include <stdlib.h>

#include <rexo.h>

#define ASSERT(x)                                                              \
    (void)(                                                                    \
        (x)                                                                    \
        || (printf(__FILE__ ":%d: assertion ‘" #x "’ failed\n", __LINE__), 0)  \
        || (abort(), 0))

static int step = 0;

RX_SET_UP(my_set_up, void *)
{
    ++step;
    ASSERT(step == 2);

    return RX_SUCCESS;
}

RX_TEAR_DOWN(my_tear_down, void *)
{
    ++step;
    ASSERT(step == 4);
}

RX_FIXTURE_VOID(my_fixture, .set_up = my_set_up, .tear_down = my_tear_down);

RX_TEST_CASE_FIXTURE(my_test_suite, my_test_case, my_fixture)
{
    ++step;
    ASSERT(step == 3);
}

int
main(int argc, const char **argv)
{
    ++step;
    ASSERT(step == 1);

    ASSERT(rx_run(argc, argv, 0, NULL) == RX_SUCCESS);

    ++step;
    ASSERT(step == 5);

    return 0;
}
