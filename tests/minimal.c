#include <stdlib.h>

#include <rexo.h>

#define ASSERT(x)                                                              \
    (void)(                                                                    \
        (x)                                                                    \
        || (printf(__FILE__ ":%d: assertion `" #x "` failed\n", __LINE__), 0)  \
        || (abort(), 0))

static int step = 0;

RX_TEST_CASE(my_test_suite, my_test_case)
{
    ++step;
    ASSERT(step == 2);

    RX_INT_REQUIRE_EQUAL(42, 42);
}

int
main(int argc, const char **argv)
{
    ++step;
    ASSERT(step == 1);

    ASSERT(rx_main(0, NULL, argc, argv) == RX_SUCCESS);

    ++step;
    ASSERT(step == 3);

    return 0;
}
