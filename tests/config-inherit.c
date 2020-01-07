#include <stdlib.h>

#include <rexo.h>

#define ASSERT(x)                                                              \
    (void)(                                                                    \
        (x)                                                                    \
        || (printf(__FILE__ ":%d: assertion ‘" #x "’ failed\n", __LINE__), 0)  \
        || (abort(), 0))

static int step = 0;

RX_TEST_SUITE(my_test_suite, .skip = 1);

RX_TEST_CASE(my_test_suite, my_test_case_1)
{
    ++step;
    ASSERT(0);
}

RX_TEST_CASE(my_test_suite, my_test_case_2, .skip = 0)
{
    ++step;
    ASSERT(step == 2);
}

int
main(int argc, const char **argv)
{
    ++step;
    ASSERT(step == 1);

    ASSERT(rx_run(argc, argv, 0, NULL) == RX_SUCCESS);

    ++step;
    ASSERT(step == 3);

    return 0;
}
