#include <assert.h>

#include <rexo.h>

static int step = 0;

struct my_data {
    int value;
};

RX_FIXTURE(my_fixture, struct my_data);

RX_TEST_CASE_FIXTURE(my_test_suite, my_test_case, my_fixture)
{
    ++step;
    assert(step == 2);

    RX_DATA->value = 123;
}

int
main(int argc, const char **argv)
{
    ++step;
    assert(step == 1);

    assert(rx_run(argc, argv, 0, NULL) == RX_SUCCESS);

    ++step;
    assert(step == 3);

    return 0;
}
