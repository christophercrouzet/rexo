#include <assert.h>

#include <rexo.h>

static int step = 0;

int
main(int argc, const char **argv)
{
    ++step;
    assert(step == 1);

    assert(rx_run(argc, argv, 0, NULL) == RX_SUCCESS);

    ++step;
    assert(step == 2);

    return 0;
}
