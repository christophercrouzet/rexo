Gotchas
=======

## Variadic Macros

Since variadic macros are not available as part of the C89 and C++98
specifications, having
the [variadic macros flag][macro-rx_enable_variadic_macros] disabled assumes
no support for variadic macros and defines an alternative set of macros
for each macro that would otherwise accept variadic arguments.

In other words, a macro documented as `RX_DO_DOMETHING(fixed_arg, ...)`
translates in fact to set of macros suffixed with the number of variadic
arguments such as:

* `RX_DO_DOMETHING(fixed_arg)`.
* `RX_DO_DOMETHING_1(fixed_arg, var_arg_1)`.
* `RX_DO_DOMETHING_2(fixed_arg, var_arg_1, var_arg_2)`.
* ...
* `RX_DO_DOMETHING_N(fixed_arg, var_arg_1, var_arg_2, ..., var_arg_n)`.

> **Note:** The set of macros suffixed with a number is available whether the
> [variadic macros flag][macro-rx_enable_variadic_macros] is enabled or not
> in order to provide a compatibility layer between the two modes if needed.
> The only difference comes from the base macro `RX_DO_DOMETHING` being
> defined with variadic arguments or only fixed arguments otherwise.


Examples of such macros can be found in the [framework][framework] and as part
of the [assertion macros][assertion-macros].


[assertion-macros]: ./reference/assertions.md
[framework]: ./reference/framework.md

[macro-rx_enable_variadic_macros]: ./compile-time-configuration.md#rx_enable_variadic_macros
