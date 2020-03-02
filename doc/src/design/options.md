Options Definition
==================

To build an expressive and intuitive framework, it was important to put
a system in place that makes the experience of configuring the different objects
as frictionless as possible.


## Named Arguments Syntax

When it comes to setting a bunch of options at once, the most user-friendly
approach is usually to define a struct where `0` (or `NULL`) corresponds to
the default value for each field, and to let the users initialize instances
of that struct by only setting the fields that need to have their value changed
from the defaults through.

This is exactly what the designated initializer syntax introduced with C99
offers:

```c
struct config {
    const char *foo;
    int bar;
    double baz;
};

/* Only setting the bar field leaves foo and baz to 0. */
struct config my_config = {.bar = 123};
```


Despite Rexo's intention of supporting C89 and C++ when both languages are
not compatible with the designated initializer syntax, it was still a strong
design goal to provide a similar approach for setting options.

This is done by using variadic macros where the optional arguments start with
a dot character:

```c
MY_MACRO(arg_1, arg_2, .option_1 = 123, .option_2 = "abc");
```


## Options Sharing

This designated initialized syntax can be used to set-up individual test cases
but, to avoid redundancy, it is possible to group test cases with a similar
configuration into a same test suite, configure only that test suite, and have
all of its test cases automatically inherit the configuration.

Not only that but it is still possible to override only specific options for
individual test cases:

```c
RX_TEST_SUITE(my_test_suite, .foo = 123, .bar = "abc");

RX_TEST_CASE(my_test_suite, my_test_case, .bar = "def", .baz = 1.23)
{
    /*
       foo is 123.
       bar is "def".
       baz is 1.23
    */
}
```


## Variadic Macros Fallback

Since C89 and C++98 don't support variadic macros, an alternative set of macros
is provided, each suffixed with a digit representing the number of options
to pass.

For example, the equivalent of the previous examples become:

```c
MY_MACRO(arg_1, arg_2)
MY_MACRO_1(arg_1, arg_2, .option_2 = "abc");
MY_MACRO_2(arg_1, arg_2, .option_1 = 123, .option_2 = "abc");

RX_TEST_SUITE_2(my_test_suite, .foo = 123, .bar = "abc");

RX_TEST_CASE_2(my_test_suite, my_test_case, .bar = "def", .baz = 1.23)
{
    /* ... */
}
```


[macro-rx_fixture]: ../reference/framework.md#rx_fixture
[macro-rx_void_fixture]: ../reference/framework.md#rx_void_fixture
[macro-rx_test_case]: ../reference/framework.md#rx_test_case
[macro-rx_test_case_fixture]: ../reference/framework.md#rx_test_case_fixture
[macro-rx_test_suite]: ../reference/framework.md#rx_test_suite
