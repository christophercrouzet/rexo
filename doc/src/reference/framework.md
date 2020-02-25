Framework
=========

The framework defines the go-to approach when wanting to use the mechanism
that automatically registers test suites, test cases, and fixtures.


### `RX_SET_UP`

Defines the initialization function of a fixture.

```c
#define RX_SET_UP(id)
```

The name for this function needs to be passed to the `id` parameter and can then
referenced through the `set_up` option available as part of
the [`RX_FIXTURE`][macro-rx_fixture] macro.


### `RX_TEAR_DOWN`

Defines the clean-up function of a fixture.

```c
#define RX_TEAR_DOWN(id)
```

The name for this function needs to be passed to the `id` parameter and can then
be referenced through the `tear_down` option available as part of
the [`RX_FIXTURE`][macro-rx_fixture] macro.


### `RX_FIXTURE`

Defines a fixture.

```c
#define RX_FIXTURE(id, type, ...)
```

The name for this fixture needs to be passed to the `id` parameter and can then
be referenced through the configuration of
the [`RX_TEST_SUITE`][macro-rx_test_suite] and
the [`RX_TEST_CASE`][macro-rx_test_case] macros.

For a list of all the options available through the variadic parameter, see
the [`rx_fixture_config`][struct-rx_fixture_config] struct.

If the fixture defines some data to be used by its test cases, its type needs
to be passed to the `type` parameter. Otherwise, the alternative
macro [`RX_VOID_FIXTURE`][macro-rx_void_fixture] should be used instead.


### `RX_VOID_FIXTURE`

Defines a fixture without any user data.

```c
#define RX_VOID_FIXTURE(id, ...)
```

The name for this fixture needs to be passed to the `id` parameter and can then
be referenced through the configuration of
the [`RX_TEST_SUITE`][macro-rx_test_suite] and
the [`RX_TEST_CASE`][macro-rx_test_case] macros.


### `RX_TEST_SUITE`

Defines a test suite.

```c
#define RX_TEST_SUITE(id, ...)
```

The name for this suite needs to be passed to the `id` parameter and can then be
referenced by the [`RX_TEST_CASE`][macro-rx_test_case] macro.

For a list of all the options available through the variadic parameter, see
the [`rx_test_case_config`][struct-rx_test_case_config] struct.


### `RX_TEST_CASE`

Defines a test case function.

```c
#define RX_TEST_CASE(suite_id, id, ...)
```

For a list of all the options available through the variadic parameter, see
the [`rx_test_case_config`][struct-rx_test_case_config] struct.


[macro-rx_fixture]: #rx_fixture
[macro-rx_test_case]: #rx_test_case
[macro-rx_test_suite]: #rx_test_suite
[macro-rx_void_fixture]: #rx_void_fixture
[struct-rx_fixture_config]: ./building-blocks.md#rx_fixture_config
[struct-rx_test_case_config]: ./building-blocks.md#rx_test_case_config
