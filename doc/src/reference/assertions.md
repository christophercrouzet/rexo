Assertion Macros
================

Each assertion macro comes in two variants that define a different behaviour
when an assertion fails:

* `REQUIRE`: reports a fatal failure and aborts the current test case.
* `CHECK`: reports a nonfatal failure and keeps running other tests found within
           the same test case.

> **Note:** When in [C89 compatibility mode][macro-rx_enable_c89_compat],
> variadic macro arguments are not available as part of the language.
> See the associated [gotcha][gotcha-variadic-macros].


## Generic Assertions

```c
#define RX_REQUIRE(condition)
#define RX_REQUIRE_MSG(condition, msg, ...)
#define RX_CHECK(condition)
#define RX_CHECK_MSG(condition, msg, ...)
```

Ultimately, these two assertion macros could be enough to express all possible 
tests but it is recommended to use the more specialized assertions available.


## Boolean Assertions

```c
#define RX_BOOL_REQUIRE_TRUE(condition)
#define RX_BOOL_REQUIRE_TRUE_MSG(condition, msg, ...)
#define RX_BOOL_CHECK_TRUE(condition)
#define RX_BOOL_CHECK_TRUE_MSG(condition, msg, ...)

#define RX_BOOL_REQUIRE_FALSE(condition)
#define RX_BOOL_REQUIRE_FALSE_MSG(condition, msg, ...)
#define RX_BOOL_CHECK_FALSE(condition)
#define RX_BOOL_CHECK_FALSE_MSG(condition, msg, ...)
```


## Integer Assertions

```c
#define RX_INT_REQUIRE_EQUAL(x1, x2)
#define RX_INT_REQUIRE_EQUAL_MSG(x1, x2, msg, ...)
#define RX_INT_CHECK_EQUAL(x1, x2)
#define RX_INT_CHECK_EQUAL_MSG(x1, x2, msg, ...)

#define RX_INT_REQUIRE_NOT_EQUAL(x1, x2)
#define RX_INT_REQUIRE_NOT_EQUAL_MSG(x1, x2, msg, ...)
#define RX_INT_CHECK_NOT_EQUAL(x1, x2)
#define RX_INT_CHECK_NOT_EQUAL_MSG(x1, x2, msg, ...)

#define RX_INT_REQUIRE_GREATER(x1, x2)
#define RX_INT_REQUIRE_GREATER_MSG(x1, x2, msg, ...)
#define RX_INT_CHECK_GREATER(x1, x2)
#define RX_INT_CHECK_GREATER_MSG(x1, x2, msg, ...)

#define RX_INT_REQUIRE_LESSER(x1, x2)
#define RX_INT_REQUIRE_LESSER_MSG(x1, x2, msg, ...)
#define RX_INT_CHECK_LESSER(x1, x2)
#define RX_INT_CHECK_LESSER_MSG(x1, x2, msg, ...)

#define RX_INT_REQUIRE_GREATER_OR_EQUAL(x1, x2)
#define RX_INT_REQUIRE_GREATER_OR_EQUAL_MSG(x1, x2, msg, ...)
#define RX_INT_CHECK_GREATER_OR_EQUAL(x1, x2)
#define RX_INT_CHECK_GREATER_OR_EQUAL_MSG(x1, x2, msg, ...)

#define RX_INT_REQUIRE_LESSER_OR_EQUAL(x1, x2)
#define RX_INT_REQUIRE_LESSER_OR_EQUAL_MSG(x1, x2, msg, ...)
#define RX_INT_CHECK_LESSER_OR_EQUAL(x1, x2)
#define RX_INT_CHECK_LESSER_OR_EQUAL_MSG(x1, x2, msg, ...)
```


## Unsigned Integer Assertions

```c
#define RX_UINT_REQUIRE_EQUAL(x1, x2)
#define RX_UINT_REQUIRE_EQUAL_MSG(x1, x2, msg, ...)
#define RX_UINT_CHECK_EQUAL(x1, x2)
#define RX_UINT_CHECK_EQUAL_MSG(x1, x2, msg, ...)

#define RX_UINT_REQUIRE_NOT_EQUAL(x1, x2)
#define RX_UINT_REQUIRE_NOT_EQUAL_MSG(x1, x2, msg, ...)
#define RX_UINT_CHECK_NOT_EQUAL(x1, x2)
#define RX_UINT_CHECK_NOT_EQUAL_MSG(x1, x2, msg, ...)

#define RX_UINT_REQUIRE_GREATER(x1, x2)
#define RX_UINT_REQUIRE_GREATER_MSG(x1, x2, msg, ...)
#define RX_UINT_CHECK_GREATER(x1, x2)
#define RX_UINT_CHECK_GREATER_MSG(x1, x2, msg, ...)

#define RX_UINT_REQUIRE_LESSER(x1, x2)
#define RX_UINT_REQUIRE_LESSER_MSG(x1, x2, msg, ...)
#define RX_UINT_CHECK_LESSER(x1, x2)
#define RX_UINT_CHECK_LESSER_MSG(x1, x2, msg, ...)

#define RX_UINT_REQUIRE_GREATER_OR_EQUAL(x1, x2)
#define RX_UINT_REQUIRE_GREATER_OR_EQUAL_MSG(x1, x2, msg, ...)
#define RX_UINT_CHECK_GREATER_OR_EQUAL(x1, x2)
#define RX_UINT_CHECK_GREATER_OR_EQUAL_MSG(x1, x2, msg, ...)

#define RX_UINT_REQUIRE_LESSER_OR_EQUAL(x1, x2)
#define RX_UINT_REQUIRE_LESSER_OR_EQUAL_MSG(x1, x2, msg, ...)
#define RX_UINT_CHECK_LESSER_OR_EQUAL(x1, x2)
#define RX_UINT_CHECK_LESSER_OR_EQUAL_MSG(x1, x2, msg, ...)
```


## Floating-Point Assertions

```c
#define RX_REAL_REQUIRE_EQUAL(x1, x2)
#define RX_REAL_REQUIRE_EQUAL_MSG(x1, x2, msg, ...)
#define RX_REAL_CHECK_EQUAL(x1, x2)
#define RX_REAL_CHECK_EQUAL_MSG(x1, x2, msg, ...)

#define RX_REAL_REQUIRE_NOT_EQUAL(x1, x2)
#define RX_REAL_REQUIRE_NOT_EQUAL_MSG(x1, x2, msg, ...)
#define RX_REAL_CHECK_NOT_EQUAL(x1, x2)
#define RX_REAL_CHECK_NOT_EQUAL_MSG(x1, x2, msg, ...)

#define RX_REAL_REQUIRE_GREATER(x1, x2)
#define RX_REAL_REQUIRE_GREATER_MSG(x1, x2, msg, ...)
#define RX_REAL_CHECK_GREATER(x1, x2)
#define RX_REAL_CHECK_GREATER_MSG(x1, x2, msg, ...)

#define RX_REAL_REQUIRE_LESSER(x1, x2)
#define RX_REAL_REQUIRE_LESSER_MSG(x1, x2, msg, ...)
#define RX_REAL_CHECK_LESSER(x1, x2)
#define RX_REAL_CHECK_LESSER_MSG(x1, x2, msg, ...)

#define RX_REAL_REQUIRE_GREATER_OR_EQUAL(x1, x2)
#define RX_REAL_REQUIRE_GREATER_OR_EQUAL_MSG(x1, x2, msg, ...)
#define RX_REAL_CHECK_GREATER_OR_EQUAL(x1, x2)
#define RX_REAL_CHECK_GREATER_OR_EQUAL_MSG(x1, x2, msg, ...)

#define RX_REAL_REQUIRE_LESSER_OR_EQUAL(x1, x2)
#define RX_REAL_REQUIRE_LESSER_OR_EQUAL_MSG(x1, x2, msg, ...)
#define RX_REAL_CHECK_LESSER_OR_EQUAL(x1, x2)
#define RX_REAL_CHECK_LESSER_OR_EQUAL_MSG(x1, x2, msg, ...)

#define RX_REAL_REQUIRE_FUZZY_EQUAL(x1, x2, tol)
#define RX_REAL_REQUIRE_FUZZY_EQUAL_MSG(x1, x2, tol, msg, ...)
#define RX_REAL_CHECK_FUZZY_EQUAL(x1, x2, tol)
#define RX_REAL_CHECK_FUZZY_EQUAL_MSG(x1, x2, tol, msg, ...)

#define RX_REAL_REQUIRE_FUZZY_NOT_EQUAL(x1, x2, tol)
#define RX_REAL_REQUIRE_FUZZY_NOT_EQUAL_MSG(x1, x2, tol, msg, ...)
#define RX_REAL_CHECK_FUZZY_NOT_EQUAL(x1, x2, tol)
#define RX_REAL_CHECK_FUZZY_NOT_EQUAL_MSG(x1, x2, tol, msg, ...)
```

> **Note:** The `EQUAL` and `NOT_EQUAL` comparisons perform strict equality
> checks, which is usually not what you would want to do in the case of
> floating-point due to precision errors.
>
> Therefore, each of these two assertions come with a corresponding `FUZZY`
> alternative that takes a tolerance parameter and performs a relative
> comparison followed by an absolute one, similar to what is described in
> Bruce Dawson's article
> [Comparing Floating Point Numbers, 2012 Edition][comparing-fp].
>
> The fuzzy comparison test is described with the following logic:
> ```c
> int
> are_equal_fuzzy(float a, float b, float tol)
> {
>     float diff;
> 
>     diff = fabs(a - b);
>     if (diff <= tol) {
>         return 1;
>     }
> 
>     a = fabs(a);
>     b = fabs(b);
>     return diff <= (a > b ? a : b) * tol;
> }
> ```


## String Assertions

```c
#define RX_STR_REQUIRE_EQUAL(s1, s2)
#define RX_STR_REQUIRE_EQUAL_MSG(s1, s2, msg, ...)
#define RX_STR_CHECK_EQUAL(s1, s2)
#define RX_STR_CHECK_EQUAL_MSG(s1, s2, msg, ...)

#define RX_STR_REQUIRE_NOT_EQUAL(s1, s2)
#define RX_STR_REQUIRE_NOT_EQUAL_MSG(s1, s2, msg, ...)
#define RX_STR_CHECK_NOT_EQUAL(s1, s2)
#define RX_STR_CHECK_NOT_EQUAL_MSG(s1, s2, msg, ...)

#define RX_STR_REQUIRE_EQUAL_NO_CASE(s1, s2)
#define RX_STR_REQUIRE_EQUAL_NO_CASE_MSG(s1, s2, msg, ...)
#define RX_STR_CHECK_EQUAL_NO_CASE(s1, s2)
#define RX_STR_CHECK_EQUAL_NO_CASE_MSG(s1, s2, msg, ...)

#define RX_STR_REQUIRE_NOT_EQUAL_NO_CASE(s1, s2)
#define RX_STR_REQUIRE_NOT_EQUAL_NO_CASE_MSG(s1, s2, msg, ...)
#define RX_STR_CHECK_NOT_EQUAL_NO_CASE(s1, s2)
#define RX_STR_CHECK_NOT_EQUAL_NO_CASE_MSG(s1, s2, msg, ...)
```


## Pointer Assertions

```c
#define RX_PTR_REQUIRE_EQUAL(x1, x2)
#define RX_PTR_REQUIRE_EQUAL_MSG(x1, x2, msg, ...)
#define RX_PTR_CHECK_EQUAL(x1, x2)
#define RX_PTR_CHECK_EQUAL_MSG(x1, x2, msg, ...)

#define RX_PTR_REQUIRE_NOT_EQUAL(x1, x2)
#define RX_PTR_REQUIRE_NOT_EQUAL_MSG(x1, x2, msg, ...)
#define RX_PTR_CHECK_NOT_EQUAL(x1, x2)
#define RX_PTR_CHECK_NOT_EQUAL_MSG(x1, x2, msg, ...)

#define RX_PTR_REQUIRE_ALIGNED(x, alignment)
#define RX_PTR_REQUIRE_ALIGNED_MSG(x, alignment, msg, ...)
```


[gotcha-variadic-macros]: ../gotchas.md#variadic_macros_in_c89_compatibility_mode
[macro-rx_enable_c89_compat]: ../compile-time-configuration.md#rx_enable_c89_compat

[comparing-fp]: https://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition
