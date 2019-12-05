#define RX_ENABLE_C89_COMPAT

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

void
my_test_suite_my_test_case(struct rx_context *context, struct my_data *data)
{
    (void)context;

    ++step;
    ASSERT(step == 3);

    ASSERT(data->value == 123);
}

static struct my_data my_data;

static const struct rx_test_case my_test_cases[] = {
    {
        "my_test_case",
        "my_test_suite",
        (rx_test_case_run_fn)my_test_suite_my_test_case,
        &my_data,
        (rx_set_up_fn)my_set_up,
        (rx_tear_down_fn)my_tear_down,
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
