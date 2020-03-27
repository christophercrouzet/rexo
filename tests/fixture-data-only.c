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

RX_FIXTURE(my_fixture, struct my_data);

RX_TEST_CASE(my_test_suite, my_test_case, .fixture = my_fixture)
{
    struct my_data *data;

    data = (struct my_data *)RX_DATA;

    ++step;
    ASSERT(step == 2);

    data->value = 123;
}

int
main(int argc, const char **argv)
{
    ++step;
    ASSERT(step == 1);

    ASSERT(rx_main(0, NULL, argc, argv) == RX_SUCCESS);

    ++step;
    ASSERT(step == 3);

    return 0;
}
