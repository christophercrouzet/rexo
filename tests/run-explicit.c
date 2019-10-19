#include <assert.h>

#include <rexo.h>

static int step = 0;

struct my_data {
    int value;
};

enum rx_status
my_set_up(struct rx_context *context, struct my_data *data)
{
    (void)context;

    assert(step++ == 1);
    data->value = 123;
    return RX_SUCCESS;
}

void
my_tear_down(struct rx_context *context, struct my_data *data)
{
    (void)context;

    assert(step++ == 3);
    assert(data->value == 123);
}

void
my_test_suite_my_test_case(struct rx_context *context, struct my_data *data)
{
    (void)context;

    assert(step++ == 2);
    assert(data->value == 123);
}

static struct my_data my_data = {
    123,
};

static struct rx_test_case my_test_case = {
    "my_test_case",
    "my_test_suite",
    &my_data,
    (rx_set_up_fn)my_set_up,
    (rx_tear_down_fn)my_tear_down,
    (rx_test_case_run_fn)my_test_suite_my_test_case,
};

int
main(int argc, const char **argv)
{
    assert(step++ == 0);
    assert(rx_run(argc, argv, 1, &my_test_case) == RX_SUCCESS);
    assert(step++ == 4);
    return 0;
}
