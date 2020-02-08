Rexo
====

Rexo is a neat unit testing framework for C and C++.

It offers the same [xUnit][xunit]-like structure than most other unit testing
frameworks but aims at providing a _truly_ polished API.


## Features

* **sleek**: polished API with great attention to details.
* **easy**: no learning curve, it's yet another framework based on xUnit
  with test suites, test cases, and fixtures.
* **convenient**: automatic registration of tests.
* **granular**: high level or low level API? You choose.
* **portable**: compatible with C89 (ANSI C) and C++.
* **simple**: straightforward implementation—KISS all the things!
* **cascading configuration**: configure a whole suite at once and/or tweak
  specific options for each case.
* **painless**: deployment couldn't be easier—it all fits into a single
  header file and has no external dependencies.


But also...

* fully standard compliant minus the optional automatic registration of tests
  that relies on a widespread compiler-specific feature.
* brings a designated initializer-like syntax to all C and C++ versions.
* test cases can override configuration attributes from their parent test suite.


## Roadmap

* run each test in isolated environment.
* implement a command-line option parser (e.g.: for test filtering).
* allow to choose the output format of the summary (e.g.: jUnit XML).
* support more assertion macros (e.g.: array comparison, signal handling).
* visual failure messages (e.g.: an arrow pointing where a string differs).


## Usage

```c
#include <rexo.h>

RX_TEST_CASE(numbers, magic)
{
    RX_INT_REQUIRE_EQUAL(2 * 3 * 7, 42);
}

int
main(int argc, const char **argv)
{
    return rx_run(argc, argv, 0, NULL) == RX_SUCCESS ? 0 : 1;
}
```


## License

[MIT][mit].


[mit]: https://choosealicense.com/licenses/mit
[xunit]: https://en.wikipedia.org/wiki/XUnit
