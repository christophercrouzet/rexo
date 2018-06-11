Rexo
====

Rexo is a simple unit testing framework for C/C++.

It is written in C89 at the exception of a couple of features borrowed from C99.


## Features

* mostly C89-compliant
* diagonal and orthogonal APIs
* fixtures
* simple


## Minimal Example

```C
#include <rexo/rexo.h>

RX_TEST_CASE(testBasics)
{
    RX_CHECK_INT_EQUAL(1, 1);
}

static const struct RxTestCase cases[]
    = {{"basics", testBasics}};

static const struct RxTestSuite suites[]
    = {{"example", sizeof cases / sizeof cases[0], cases, setUp, tearDown}};

int
main(int argc, const char **ppArgv)
{
    rxRun(sizeof suites / sizeof suites[0], suites, argc, ppArgv);
    return 0;
}
```
