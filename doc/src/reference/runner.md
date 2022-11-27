Runner
======

High-level API to run all the unit tests.


### `rx_main`

Runs the given tests.

```c
enum rx_status
rx_main(size_t test_case_count,
        const struct rx_test_case *test_cases,
        int argc,
        const char * const *argv)
```

The `rx_main` function can be seen as composing all the orthogonal calls into
a single higher level API for ease of use and is likely to be the only one
function needing to ever be called by most users.

If the `test_cases` argument is `NULL`, then the tests found through
the [framework][framework]'s automatic registration feature are used.

This function is implemented using the lower level API described in
[building blocks][building-blocks]. Use these directly instead of `rx_main`
if you'd like to further customize the process.


[building-blocks]: ./building-blocks.md
[framework]: ./framework.md
