#define RX_DISABLE_TEST_DISCOVERY

#include <stdlib.h>

#include <rexo.h>

#define ASSERT(x)                                                              \
    (void)(                                                                    \
        (x)                                                                    \
        || (printf(__FILE__ ":%d: assertion `" #x "` failed\n", __LINE__), 0)  \
        || (abort(), 0))

static int step = 0;

RX_TEST_CASE(my_test_suite, my_test_case_1)
{
    ++step;
}

RX_TEST_CASE(my_test_suite, my_test_case_2)
{
    ++step;
    ASSERT(step == 2);

    RX_INT_REQUIRE_EQUAL(42, 42);
}

static const struct rx_test_case my_test_cases[] = {
    {
        "my_test_suite",
        "my_test_case_2",
        my_test_suite_my_test_case_2,
        {0, {0, {NULL, NULL}}},
    },
};

int
main(int argc, const char **argv)
{
    ++step;
    ASSERT(step == 1);

    ASSERT(rx_main(1, my_test_cases, argc, argv) == RX_SUCCESS);

    ++step;
    ASSERT(step == 3);

    return 0;
}
