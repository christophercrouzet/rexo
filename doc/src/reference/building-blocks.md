Building Blocks
===============

Core of Rexo—data structures, required macros, and lower level API calls.

Most of these are abstracted away when using the [runner][runner],
the [framework][framework]'s automatic registration feature,
and the [assertion macros][assertion-macros].


## Enumerators

### `rx_status`

Return codes.

```c
enum rx_status {
    RX_SUCCESS = 0,
    RX_ERROR = -1,
    RX_ERROR_ALLOCATION = -2,
    RX_ERROR_MAX_SIZE_EXCEEDED = -3
}
```

Error codes come in different categories that all evaluate to negative numbers.


### `rx_severity`

Severity levels for test failures.

```c
enum rx_severity { RX_NONFATAL = 0, RX_FATAL = 1 }
```

Nonfatal failures arise from `RX_CHECK*` assertions while `RX_REQUIRE*` trigger
fatal failures.

See also the [assertion macros][assertion-macros].


### `rx_log_level`

Logging level.

```c
enum rx_log_level {
    RX_LOG_LEVEL_NONE = 0,
    RX_LOG_LEVEL_FATAL = 1,
    RX_LOG_LEVEL_ERROR = 3,
    RX_LOG_LEVEL_WARNING = 4,
    RX_LOG_LEVEL_INFO = 5,
    RX_LOG_LEVEL_DEBUG = 6,
    RX_LOG_LEVEL_ALL = RX_LOG_LEVEL_DEBUG
};
```

The purpose of each level is defined as follows:

| level | description | example |
| :---: | ----------- | ------- |
| **fatal** | critical failure that causes premature termination of the application | lack of disk space, data corruption |
| **error** | failure that doesn't require the application to be prematurely terminated | unable to open a file, allocation failure |
| **warning** | situation that is not ideal but that is not an actual failure per se | use of a deprecated API, poor use of an API |
| **info** | event of general interest corresponding to normal application behaviour | addition of a database entry, statistics |
| **debug** | detailed information to help maintainers troubleshooting problems | allocation size, state of data |
| **trace** | control flow of the application to help maintainers pinpointing problems | entry/exit of functions, dump of argument values |


## Function Pointers

### `rx_set_up_fn`

Function part of the fixture feature, to be called before the function defining
the tests is run.

```c
typedef enum rx_status (*rx_set_up_fn)(struct rx_context *, void *)
```

The `struct rx_context *` parameter is reserved for the implementation and
shouldn't be directly accessed by the users.

The `void *` parameter is used to output any data initialized within
this function. The pointer can be accessed within the function's definition
using the [`RX_DATA`][macro-rx_data] macro.

When [explicitly registering tests][explicit-registration-guide], the macros
[`RX_PARAM_CONTEXT`][macro-rx_param_context]
and [`RX_PARAM_DATA`][macro-rx_param_data] need to be used to define
the parameter names.

The function is expected to return an error code if something went wrong,
or `RX_SUCCESS` otherwise.


### `rx_tear_down_fn`

Function part of the fixture feature, to be called after the function defining
the tests is run.

```c
typedef void (*rx_tear_down_fn)(struct rx_context *, void *)
```

The `struct rx_context *` parameter is reserved for the implementation and
shouldn't be directly accessed by the users.

The `void *` parameter is used to output any data initialized within
this function. The pointer can be accessed within the function's definition
using the [`RX_DATA`][macro-rx_data] macro.

When [explicitly registering tests][explicit-registration-guide], the macros
[`RX_PARAM_CONTEXT`][macro-rx_param_context]
and [`RX_PARAM_DATA`][macro-rx_param_data] need to be used to define
the parameter names.


### `rx_run_fn`

Function defining the tests to run.

```c
typedef void (*rx_run_fn)(struct rx_context *, void *)
```

The `struct rx_context *` parameter is reserved for the implementation and
shouldn't be directly accessed by the users.

The `void *` parameter is used to output any data initialized within
this function. The pointer can be accessed within the function's definition
using the [`RX_DATA`][macro-rx_data] macro.

When [explicitly registering tests][explicit-registration-guide], the macros
[`RX_PARAM_CONTEXT`][macro-rx_param_context]
and [`RX_PARAM_DATA`][macro-rx_param_data] need to be used to define
the parameter names.


## Parameters

### `RX_PARAM_CONTEXT`

Refers to the name expected for the context parameter.

```c
#define RX_PARAM_CONTEXT
```

The context parameter is used for the functions
[rx_set_up_fn][fnptr-rx_set_up_fn], [rx_tear_down_fn][fnptr-rx_tear_down_fn],
and [rx_run_fn][fnptr-rx_run_fn].

It is not to be directly used unless when
[explicitly registering tests][explicit-registration-guide]. In this case,
the macro `RX_PARAM_CONTEXT` is required to name
the `struct rx_context *` parameter.


### `RX_PARAM_DATA`

Refers to the name expected for the data parameter.

```c
#define RX_PARAM_DATA
```

The context parameter is used for the functions
[rx_set_up_fn][fnptr-rx_set_up_fn], [rx_tear_down_fn][fnptr-rx_tear_down_fn],
and [rx_run_fn][fnptr-rx_run_fn].

It is not to be directly used unless when
[explicitly registering tests][explicit-registration-guide]. In this case,
the macro `RX_PARAM_DATA` is required to name the `void *` parameter.


## Data Accessors

### `RX_DATA`

Access the data's pointer.

```c
#define RX_DATA
```

This macro can be used within the definitions of the functions
[rx_set_up_fn][fnptr-rx_set_up_fn], [rx_tear_down_fn][fnptr-rx_tear_down_fn],
and [rx_run_fn][fnptr-rx_run_fn].


## Types

### `rx_uint32`

Type for 32-bit unsigned integers.

```c
typedef TYPE rx_uint32;
```

The type is determined by the value of
the [`RX_UINT32_TYPE`][macro-rx_uint32_type] macro. If the macro isn't set,
`unsigned int` is used, which fits the common data models, that is ILP32
(most recent 32-bit systems), LP64 (Unix-like systems), and LLP64 (Windows).


### `rx_uint64`

Type for 64-bit unsigned integers.

```c
typedef TYPE rx_uint64;
```

The type is determined by the value of
the [`RX_UINT64_TYPE`][macro-rx_uint64_type] macro. If the macro isn't set,
`unsigned long long` is used, which fits the common data models, that is ILP32
(most recent 32-bit systems), LP64 (Unix-like systems), and LLP64 (Windows).


### `rx_size`

Type to use in place of `size_t`.

```c
typedef TYPE rx_size;
```

The type is determined by the value of the [`RX_SIZE_TYPE`][macro-rx_size_type]
macro. If the macro isn't set, either [`rx_uint32`][type-rx_uint32]
or [`rx_uint64`][type-rx_uint64] is used, depending on whether the environment
is running on a 32-bit or 64-bit platform.


## Structures

### `rx_test_case_config`

Configuration object to apply to a test case.

```c
struct rx_test_case_config {
    int skip;
    struct rx_fixture fixture;
}
```

In the event where a test case should be skipped by the runner,
the `skip` option can be used.

Fixtures are defined through the `fixture` option, see
the [`rx_fixture`][struct-rx_fixture] struct.

Filling the struct with the value `0` sets all the members to
their default values.


### `rx_fixture_config`

Configuration object to apply to a fixture.

```c
struct rx_fixture_config {
    rx_set_up_fn set_up;
    rx_tear_down_fn tear_down;
}
```

The `set_up` and `tear_down` options respectively define
the [rx_set_up_fn][fnptr-rx_set_up_fn] and
the [rx_tear_down_fn][fnptr-rx_tear_down_fn] functions.

Filling the struct with the value `0` sets all the members to
their default values.


### `rx_fixture`

Fixture defining data type size and function pointers to run before and after
the test is run.

```c
struct rx_fixture {
    rx_size size;
    struct rx_fixture_config config;
};
```

Any configuration can be set through the `config` option. See
the [`rx_fixture_config`][struct-rx_fixture_config] struct.


### `rx_test_case`

Definition of a single test case.

```c
struct rx_test_case {
    const char *suite_name;
    const char *name;
    rx_run_fn run;
    struct rx_test_case_config config;
};
```

The `run` function pointer needs to point to the function that contains
the tests for the test case. See the [`rx_run_fn`][fnptr-rx_run_fn] function.

Any configuration can be set through the `config` option. See
the [`rx_test_case_config`][struct-rx_test_case_config] struct.


### `rx_failure`

Information related to a test that failed.

```c
struct rx_failure {
    const char *file;
    int line;
    enum rx_severity severity;
    const char *msg;
    const char *diagnostic_msg;
}
```


### `rx_summary`

Report from running a test case.

```c
struct rx_summary {
    const struct rx_test_case *test_case;
    int skipped;
    const char *error;
    rx_size assessed_count;
    rx_size failure_count;
    struct rx_failure *failures;
    rx_uint64 elapsed;
}
```


### `rx_context`

Opaque data required internally by Rexo.

```c
struct rx_context
```


## Functions

### `rx_abort`

Aborts the execution of the test case being currently run.

```c
void
rx_abort(struct rx_context *context)
```


### `rx_handle_test_result`

Handles the result of a single test.

```c
enum rx_status
rx_handle_test_result(struct rx_context *context,
                      int result,
                      const char *file,
                      int line,
                      enum rx_severity severity,
                      const char *failure_msg,
                      const char *diagnostic_msg)
```

This records the result and any error or diagnostic messages related to it.


### `rx_summary_initialize`

Initializes a summary.

```c
enum rx_status
rx_summary_initialize(struct rx_summary *summary,
                      const struct rx_test_case *test_case)
```

The struct [`rx_summary`][struct-rx_summary] must be already manually
allocated beforehand.


### `rx_summary_terminate`

Terminates a summary.

```c
void
rx_summary_terminate(struct rx_summary *summary)
```

The struct [`rx_summary`][struct-rx_summary] must be manually freed
afterwards, if needed.


### `rx_summary_print`

Prints a summary covering the result of running a single test case.

```c
void
rx_summary_print(const struct rx_summary *summary)
```

The summary is printed out to `stderr` as it is not intended for further
processing, but to only show the progress of each test case as the results
come in.


### `rx_test_case_run`

Runs a single test case.

```c
enum rx_status
rx_test_case_run(struct rx_summary *summary,
                 const struct rx_test_case *test_case)
```

The `run` function set for the given test case is being executed with
the results are being stored in the `summary` argument.


### `rx_enumerate_test_cases`

Enumerates the test cases automatically registered.

```c
void
rx_enumerate_test_cases(size_t *test_case_count,
                        struct rx_test_case *test_cases)
```

If `test_cases` is `NULL`, then the number of test cases available is returned
in `test_case_count`. Otherwise, `test_case_count` must point to a variable set
by the user to the number of elements in the `test_cases` array, and on return
the variable is overwritten with the number of objects actually written to
`test_cases`.

If `test_case_count` is less than the number of test cases available,
at most `test_case_count` objects will be written.


[assertion-macros]: ./assertions.md
[explicit-registration-guide]: ../guides.md#explicit-registration
[framework]: ./framework.md
[runner]: ./runner.md

[fnptr-rx_run_fn]: #rx_run_fn
[fnptr-rx_set_up_fn]: #rx_set_up_fn
[fnptr-rx_tear_down_fn]: #rx_tear_down_fn
[macro-rx_data]: #rx_data
[macro-rx_param_context]: #rx_param_context
[macro-rx_param_data]: #rx_param_data
[macro-rx_size_type]: ../compile-time-configuration.md#rx_size_type
[macro-rx_uint32_type]: ../compile-time-configuration.md#rx_uint32_type
[macro-rx_uint64_type]: ../compile-time-configuration.md#rx_uint64_type
[type-rx_uint32]: #rx_uint32
[type-rx_uint64]: #rx_uint64
[struct-rx_fixture]: #rx_fixture
[struct-rx_fixture_config]: #rx_fixture_config
[struct-rx_summary]: #rx_summary
[struct-rx_test_case_config]: #rx_test_case_config
