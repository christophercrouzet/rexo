Compile-Time Configuration
==========================

One strength of single-file header libraries is that they allow to easily
change some behaviour at compile-time by allowing to redefine some macros
before including `rexo.h`.

This page lists all such macros that can be overridden if desired.

> **Note:** See the [guide][guide-compile-time-config] for a detailed example.


## Flag Macros

### `RX_ENABLE_EXTERNAL_LINKING`

Sets the storage class qualifier of the public functions to `extern`.

```c
#define RX_ENABLE_EXTERNAL_LINKING
```

If not set, the public functions are defined with the `static` qualifier
instead.


### `RX_ENABLE_NPRINTF`

Enables the usage of the standard `*nprintf` functions.

```c
#define RX_ENABLE_NPRINTF
```

If neither the `RX_ENABLE_NPRINTF` nor the `RX_DISABLE_NPRINTF`
macros are explicitly defined, the standard `*nprintf` functions are used
depending on the language (C or C++) and its version.


### `RX_DISABLE_NPRINTF`

Disables the usage of the standard `*nprintf` functions.

```c
#define RX_DISABLE_NPRINTF
```

If neither the `RX_ENABLE_NPRINTF` nor the `RX_DISABLE_NPRINTF`
macros are explicitly defined, the standard `*nprintf` functions are used
depending on the language (C or C++) and its version.

This macro takes precedence over
the [`RX_ENABLE_NPRINTF`][macro-rx_enable-nprintf] macro.


### `RX_ENABLE_VARIADIC_MACROS`

Enables the usage of variadic macros.

```c
#define RX_ENABLE_VARIADIC_MACROS
```

If neither the `RX_ENABLE_VARIADIC_MACROS` nor the `RX_DISABLE_VARIADIC_MACROS`
macros are explicitly defined, variadic macros are used depending on
the language (C or C++) and its version.

See the [variadic macros][gotcha-variadic-macros] gotcha.


### `RX_DISABLE_VARIADIC_MACROS`

Disables the usage of variadic macros.

```c
#define RX_DISABLE_VARIADIC_MACROS
```

If neither the `RX_ENABLE_VARIADIC_MACROS` nor the `RX_DISABLE_VARIADIC_MACROS`
macros are explicitly defined, variadic macros are used depending on
the language (C or C++) and its version.

This macro takes precedence over
the [`RX_ENABLE_VARIADIC_MACROS`][macro-rx_enable-variadic-macros] macro.

See the [variadic macros][gotcha-variadic-macros] gotcha.


### `RX_ENABLE_DEBUGGING`

Enables the debugging mode.

```c
#define RX_ENABLE_DEBUGGING
```

If neither the `RX_ENABLE_DEBUGGING` nor the `RX_DISABLE_DEBUGGING` macros are
explicitly defined, the debugging mode is enabled depending on the values of
the `DEBUG` and `NDEBUG` macros.

This takes precedence over
the [`RX_DISABLE_DEBUGGING`][macro-rx_disable_debugging] macro.


### `RX_DISABLE_DEBUGGING`

Disables the debugging mode.

```c
#define RX_DISABLE_DEBUGGING
```

If neither the `RX_ENABLE_DEBUGGING` nor the `RX_DISABLE_DEBUGGING` macros are
explicitly defined, the debugging mode is enabled depending on the values of
the `DEBUG` and `NDEBUG` macros.


### `RX_DISABLE_LOGGING`

Disables logging.

```c
#define RX_DISABLE_LOGGING
```

It suppresses any log that is intended to be displayed in the shell.


### `RX_SET_LOGGING_LEVEL`

Defines the logging level.

```c
#define RX_SET_LOGGING_LEVEL_NONE
#define RX_SET_LOGGING_LEVEL_FATAL
#define RX_SET_LOGGING_LEVEL_ERROR
#define RX_SET_LOGGING_LEVEL_WARNING
#define RX_SET_LOGGING_LEVEL_INFO
#define RX_SET_LOGGING_LEVEL_DEBUG
#define RX_SET_LOGGING_LEVEL_ALL
```

The logging level can be set to only output logs of a level greater or equal to
the given one, e.g.:  if the macro `RX_SET_LOGGING_LEVEL_INFO` is set,
then logs of the info, warning, and error levels are printed out.

See the [`enum rx_log_level`][enum-rx_log_level] enumerator for a description
of each level.


### `RX_DISABLE_LOG_STYLING`

Disables the styling of logs in the shell.

```c
#define RX_DISABLE_LOG_STYLING
```

If the [`RX_LOG`][macro-rx_log] macro hasn't been redefined, its default
implementation adds colours to the logs outputted to Unix shells in order to
visually highlight some bits of information and to help with readability.

The `RX_DISABLE_LOG_STYLING` macro ensures that no styling is ever applied to
the output logs.


### `RX_DISABLE_TEST_DISCOVERY`

Disables the automatic discovery of tests.

```c
#define RX_DISABLE_TEST_DISCOVERY
```


## Type Macros

### `RX_UINT32_TYPE`

Override the unsigned integer 32-bit type.

```c
#define RX_UINT32_TYPE
```

See the [`rx_uint32`][type-rx_uint32] type for more info.


### `RX_UINT64_TYPE`

Override the unsigned integer 64-bit type.

```c
#define RX_UINT64_TYPE
```

See the [`rx_uint64`][type-rx_uint64] type for more info.


### `RX_SIZE_TYPE`

Override the `size_t` type.

```c
#define RX_SIZE_TYPE
```

See the [`rx_size`][type-rx_size] type for more info.


## Function-Like Macros

### `RX_ASSERT`

Assertion macro.

```c
#define RX_ASSERT(condition)
```

Its purpose is to output an error when the argument `condition` evaluates
to zero. The default implementation runs this check only when the `NDEBUG` macro
is set, otherwise it does nothing.

If not redefined, the standard header file `assert.h` is included.


### `RX_MALLOC`

Allocation macro.

```c
#define RX_MALLOC(size)
```

Allocates the given `size` in bytes of uninitialized storage.

It returns a pointer to the allocated block of memory, or `NULL`
if the operation failed.

If not redefined, the standard header file `stdlib.h` is included.


### `RX_REALLOC`

Reallocation macro.

```c
#define RX_REALLOC(ptr, size)
```

Reallocates the given block of memory pointed by `ptr`. It is being done either
by expanding/shrinking the existing block of memory, if possible, or allocating
a new block of memory otherwise, before copying the data over and freeing
the previous block.

It returns a pointer to the reallocated block of memory.

If the operation failed, `NULL` is returned and the original block of memory
isn't freed.

If not redefined, the standard header file `stdlib.h` is included.


### `RX_FREE`

Deallocation macro.

```c
#define RX_FREE(ptr)
```

Deallocates the given block of memory pointed by `ptr`, that was previously
allocated through [`RX_MALLOC`][macro-rx_malloc]
or [`RX_REALLOC`][macro-rx_realloc].

If not redefined, the standard header file `stdlib.h` is included.


### `RX_LOG`

Logging macro.

```c
#define RX_LOG(level, format, ...)
```

Prints a message to `stderr` with a level defined through
[`enum rx_log_level`][enum-rx_log_level].

The content of the message is defined through a combination of the `format`
argument and the variadic arguments, in a similar fashion to the standard
`printf()` function.


[gotcha-variadic-macros]: ./gotchas.md#variadic-macros
[guide-compile-time-config]: ./guides.md#compile-time-configuration

[enum-rx_log_level]: ./reference/building-blocks.md#rx_log_level
[macro-rx_disable_debugging]: #rx_disable_debugging
[macro-rx_enable_debugging]: #rx_enable_debugging
[macro-rx_enable_nprintf]: #rx_enable_nprintf
[macro-rx_enable_variadic_macros]: #rx_enable_variadic_macros
[macro-rx_log]: #rx_log
[macro-rx_malloc]: #rx_malloc
[macro-rx_realloc]: #rx_realloc
[macro-rx_set_logging_level]: #rx_set_logging_level
[type-rx_size]: ./reference/building-blocks#rx_size
[type-rx_uint32]: ./reference/building-blocks.md#rx_uint32
[type-rx_uint64]: ./reference/building-blocks.md#rx_uint64
