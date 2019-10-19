#include <assert.h>

#include <rexo.h>

static int step = 0;

RX_TEST_CASE(my_test_suite, my_test_case)
{
    assert(step++ == 1);
}

int
main(int argc, const char **argv)
{
    assert(step++ == 0);
    assert(rx_run(argc, argv, 0, NULL) == RX_SUCCESS);
    assert(step++ == 2);
    return 0;
}
