Guides
======

These guides take you over some concrete examples and how-tos covering most
of Rexo's features.


## Minimal

The easiest way to run a single test is to rely on the [framework][framework]
by defining a test case with the [`RX_TEST_CASE`][macro-rx_test_case] macro,
writing a test in there using one of the [assertion macros][assertion-macros]
available, and finally running that test case by calling
the [`rx_main`][fn-rx_main] function:

```c
#include <rexo.h>

/*
   Define a new test case 'bar' that is part of an implicit test suite 'foo'.

   The main purpose of the `RX_TEST_CASE` macro is to declare the test case's
   function.
*/
RX_TEST_CASE(foo, bar)
{
    /* Run a single test that compares two integer values for equality. */
    RX_INT_REQUIRE_EQUAL(2 * 3 * 7, 42);
}

int
main(int argc, const char **argv)
{
    /* Execute the main function that runs the test cases found. */
    return rx_main(0, NULL, argc, argv) == RX_SUCCESS ? 0 : 1;
}
```


> **Note:** Setting the `test_cases` argument from the [`rx_main`][fn-rx_main]
> function to `NULL` means that the function is responsible for finding all
> the test cases that were defined using the [framework][framework]'s automatic
> registration feature.

> **Note:** In the example above, the [`RX_TEST_CASE`][macro-rx_test_case] macro
> implicitly creates a test suite named _foo_.


## Runtime Configuration

When using the [framework][framework] and its automatic registration feature,
configuring test cases is done by passing optional arguments to the
[`RX_TEST_CASE`][macro-rx_test_case] and/or
[`RX_TEST_SUITE`][macro-rx_test_suite] macros.

```c
#include <stdio.h>

#include <rexo.h>

RX_TEST_SUITE(foo, .skip = 1);

/* Inherit the skip option from the test suite 'foo'. */
RX_TEST_CASE(foo, bar)
{
    printf("this does NOT print\n");
}

/* Override the skip option to run this specific test case. */
RX_TEST_CASE(foo, baz, .skip = 0)
{
    printf("this does print!\n");
}

int
main(int argc, const char **argv)
{
    return rx_main(0, NULL, argc, argv) == RX_SUCCESS ? 0 : 1;
}
```

This example explicitly creates a test suite named _foo_ with the `skip` option
enabled.

Any option defined on a test suite is to be inherited by all the test cases of
that test suite, unless overidden by a specific test case like here with _baz_.

> **Note:** For a list of all the runtime options available,
> see the [`rx_test_case_config`][struct-rx_test_case_config] struct.


## Fixtures

Often times a dataset is required to be prepared in advance, before test cases
should be run.

Like with every other xUnit-like frameworks, Rexo allows this do be done using
fixtures.

You can define a new fixture through the [`RX_FIXTURE`][macro-rx_fixture] macro
with optional set up and tear down functions, then reference this fixture by
setting the `fixture` option on the [`RX_TEST_CASE`][macro-rx_test_case] macro.


```c
#include <rexo.h>

/* Data structure to use at the core of our fixture. */
struct foo_data {
    const char *value;
};

/* Initialize the data structure. Its allocation is handled by Rexo. */
RX_SET_UP(foo_set_up)
{
    struct foo_data *data;

    /*
       The macro `RX_DATA` references our data as a pointer to `void` that
       needs to be cast to the correct type before being used.
    */
    data = (struct foo_data *)RX_DATA;

    /* Initialize it! */
    data->value = "world!";

    /* Let Rexo know that everything went well. */
    return RX_SUCCESS;
}

/* Define the fixture. */
RX_FIXTURE(foo_fixture, struct foo_data, .set_up = foo_set_up);

RX_TEST_CASE(foo, bar, .fixture = foo_fixture)
{
    struct foo_data *data;

    /* Here again, casting needs to be node before operating on the data. */
    data = (struct foo_data *)RX_DATA;

    /*
       Run a string equality test that will fail since 'Hello' isn't equal
       to the value 'world!' that we initialized earlier.
    */
    RX_STR_REQUIRE_EQUAL("Hello", data->value);
}

int
main(int argc, const char **argv)
{
    return rx_main(0, NULL, argc, argv) == RX_SUCCESS ? 0 : 1;
}
```


Fixture data can be accessed at any time using the [`RX_DATA`][macro-rx_data]
macro.


## Compile-Time Configuration

Configurations that applies globally are sometimes best done once and for all
during the compilation step.

Rexo offers a set of macros that, for example, allows redefining the `malloc`
and `assert` functions to use, or to set global flags:

```c
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

/* Define a custom `malloc` function. */
void *
my_malloc(size_t size)
{
    printf("something worth %ld bytes is being allocated\n", size);
    return malloc(size);
}

/* Override the default `malloc` function used by Rexo with ours. */
#define RX_MALLOC my_malloc

/* Override the default `assert` macro used by Rexo. */
#define RX_ASSERT(x)                                                           \
    (void)(                                                                    \
        (x)                                                                    \
        || (printf(__FILE__ ":%d: assertion `" #x "` failed\n", __LINE__), 0)  \
        || (abort(), 0))

/* Force Rexo's compatibility with C89. */
#define RX_ENABLE_C89_COMPAT

/* Proceed with including Rexo and writing tests as usual. */

#include <rexo.h>

RX_TEST_CASE(foo, bar)
{
    RX_INT_REQUIRE_EQUAL(2 * 3 * 7, 42);
}

int
main(int argc, const char **argv)
{
    return rx_main(0, NULL, argc, argv) == RX_SUCCESS ? 0 : 1;
}
```

> **Note:** The use of `#include <rexo.h>` needs to come after the definition
> of any compile-time option.

> **Note:** For a list of all the compile-time options available,
> see the [Compile-Time Configuration][compile-time-config] page.


## Explicit Registration

If you would like to not make use of the [framework][framework]'s automatic
registration feature, it is possible to register everything explicitly in
the same way as most C unit testing frameworks offer:

```c
#include <rexo.h>

struct foo_data {
    int value;
};

enum rx_status
foo_set_up(struct rx_context *RX_PARAM_CONTEXT, void *RX_PARAM_DATA)
{
    struct foo_data *data;

    /* Suppress the ‘defined but not used’ compiler warning. */
    (void)RX_PARAM_CONTEXT;

    data = (struct foo_data *)RX_DATA;
    data->value = 123;
    return RX_SUCCESS;
}

void
foo_bar(struct rx_context *RX_PARAM_CONTEXT, void *RX_PARAM_DATA)
{
    struct foo_data *data;

    data = (struct foo_data *)RX_DATA;
    RX_INT_REQUIRE_EQUAL(data->value, 123);
}

static const struct rx_test_case test_cases[] = {
    {  /* First test case's definition. */

        "foo",    /* Name of the test suite. */
        "bar",    /* Name of the test case. */
        foo_bar,  /* Function to run. */
        {         /* Test case's configuration. */

            0,  /* Option 'skip'. */
            {   /* Option 'fixture'. */

                sizeof(struct foo_data),  /* Size of the data in bytes. */
                {                         /* Fixture's configuration. */

                    foo_set_up,  /* Function to initialize the fixture. */
                    NULL         /* Function to clean up the fixture. */

                }

            }

        }

    }

};

/* Retrieve the number of test cases to run. */
static const size_t test_case_count
    = sizeof(test_cases) / sizeof(test_cases[0]);

int
main(int argc, const char **argv)
{
    /* Explicitly pass the test cases to be run. */
    return rx_main(test_case_count, test_cases, argc, argv) == RX_SUCCESS
        ? 0 : 1;
}
```

> **Note:** See the [rx_test_case][struct-rx_test_case] struct.

> **Note:** The function parameters `struct rx_context *` and `void *`
> are required to be respectively named using
> the [`RX_PARAM_CONTEXT`][macro-rx_param_context] and
> [`RX_PARAM_DATA`][macro-rx_param_data] macros.

> **Note:** Alternatively, the data parameter can be defined with its actual
> type instead of `void *`, thus removing the need to cast the data within
> the functions. Although this approach requires casting function pointers,
> which is not standard compliant and hence not used in the implementation
> of Rexo, in practice the behaviour is well-defined for the common platforms.


[assertion-macros]: ./reference/assertions.md
[compile-time-config]: ./compile-time-configuration.md
[framework]: ./reference/framework.md

[fn-rx_main]: ./reference/runner.md#rx_main
[macro-rx_data]: ./reference/building-blocks.md#rx_data
[macro-rx_param_context]: ./reference/building-blocks.md#rx_param_context
[macro-rx_param_data]: ./reference/building-blocks.md#rx_param_data
[macro-rx_fixture]: ./reference/framework.md#rx_fixture
[macro-rx_test_case]: ./reference/framework.md#rx_test_case
[macro-rx_test_suite]: ./reference/framework.md#rx_test_suite
[struct-rx_test_case_config]: ./reference/building-blocks.md#rx_test_case_config
[struct-rx_test_case]: ./reference/building-blocks.md#rx_test_case
