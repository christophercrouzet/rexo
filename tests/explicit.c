#include <stdlib.h>

#include <rexo.h>

#define ASSERT(x)                                                              \
    (void)(                                                                    \
        (x)                                                                    \
        || (printf(__FILE__ ":%d: assertion `" #x "` failed\n", __LINE__), 0)  \
        || (abort(), 0))

static int step = 0;

struct my_data {
    int value;
};

enum rx_status
my_set_up(struct rx_context *RX_PARAM_CONTEXT, void *RX_PARAM_DATA)
{
    struct my_data *data;

    (void)RX_PARAM_CONTEXT;

    data = (struct my_data *)RX_DATA;

    ++step;
    ASSERT(step == 2);

    data->value = 123;
    return RX_SUCCESS;
}

void
my_tear_down(struct rx_context *RX_PARAM_CONTEXT, void *RX_PARAM_DATA)
{
    struct my_data *data;

    (void)RX_PARAM_CONTEXT;

    data = (struct my_data *)RX_DATA;

    ++step;
    ASSERT(step == 4);

    ASSERT(data->value == 123);
}

void
my_test_suite_my_test_case(struct rx_context *RX_PARAM_CONTEXT,
                           void *RX_PARAM_DATA)
{
    struct my_data *data;

    (void)RX_PARAM_CONTEXT;

    data = (struct my_data *)RX_DATA;

    ++step;
    ASSERT(step == 3);

    ASSERT(data->value == 123);

    RX_INT_REQUIRE_EQUAL(42, 42);
}

static const struct rx_test_case my_test_cases[] = {
    {
        "my_test_suite",
        "my_test_case",
        my_test_suite_my_test_case,
        {0, {sizeof(struct my_data), {my_set_up, my_tear_down}}},
    },
};

int
main(int argc, const char **argv)
{
    ++step;
    ASSERT(step == 1);

    ASSERT(rx_main(1, my_test_cases, argc, argv) == RX_SUCCESS);

    ++step;
    ASSERT(step == 5);

    return 0;
}
