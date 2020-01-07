#include <stdlib.h>

#include <rexo.h>

#define ASSERT(x)                                                              \
    (void)(                                                                    \
        (x)                                                                    \
        || (printf(__FILE__ ":%d: assertion ‘" #x "’ failed\n", __LINE__), 0)  \
        || (abort(), 0))

static int step = 0;

struct my_data {
    int value;
};

RX_SET_UP(my_set_up, struct my_data)
{
    ++step;
    ASSERT(step == 2);

    RX_DATA->value = 123;
    return RX_SUCCESS;
}

RX_TEAR_DOWN(my_tear_down, struct my_data)
{
    ++step;
    ASSERT(step == 4);

    ASSERT(RX_DATA->value == 123);
}

RX_FIXTURE(my_fixture,
           struct my_data,
           .set_up = my_set_up,
           .tear_down = my_tear_down);

RX_TEST_CASE_FIXTURE(my_test_suite, my_test_case, my_fixture)
{
    ++step;
    ASSERT(step == 3);

    ASSERT(RX_DATA->value == 123);

    RX_INT_REQUIRE_EQUAL(42, 42);
}

static struct my_data my_data;

static const struct rx_test_case my_test_cases[] = {
    {
        "my_test_case",
        "my_test_suite",
        (rx_test_case_run_fn)my_test_suite_my_test_case,
        &my_data,
        {(rx_set_up_fn)my_set_up, (rx_tear_down_fn)my_tear_down},
        {0},
    },
};

int
main(int argc, const char **argv)
{
    ++step;
    ASSERT(step == 1);

    ASSERT(rx_run(argc, argv, 1, my_test_cases) == RX_SUCCESS);

    ++step;
    ASSERT(step == 5);

    return 0;
}
