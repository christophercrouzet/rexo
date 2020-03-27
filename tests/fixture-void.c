#include <stdlib.h>

#include <rexo.h>

#define ASSERT(x)                                                              \
    (void)(                                                                    \
        (x)                                                                    \
        || (printf(__FILE__ ":%d: assertion `" #x "` failed\n", __LINE__), 0)  \
        || (abort(), 0))

static int step = 0;

RX_SET_UP(my_set_up)
{
    ++step;
    ASSERT(step == 2);

    return RX_SUCCESS;
}

RX_TEAR_DOWN(my_tear_down)
{
    ++step;
    ASSERT(step == 4);
}

RX_VOID_FIXTURE(my_fixture, .set_up = my_set_up, .tear_down = my_tear_down);

RX_TEST_CASE(my_test_suite, my_test_case, .fixture = my_fixture)
{
    ++step;
    ASSERT(step == 3);

    RX_INT_REQUIRE_EQUAL(42, 42);
}

int
main(int argc, const char **argv)
{
    ++step;
    ASSERT(step == 1);

    ASSERT(rx_main(0, NULL, argc, argv) == RX_SUCCESS);

    ++step;
    ASSERT(step == 5);

    return 0;
}
