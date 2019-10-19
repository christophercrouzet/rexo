#include <assert.h>

#include <rexo.h>

static int step = 0;

int
main(int argc, const char **argv)
{
    assert(step++ == 0);
    assert(rx_run(argc, argv, 0, NULL) == RX_SUCCESS);
    assert(step++ == 1);
    return 0;
}
