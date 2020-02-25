Automatic Registration
======================

Rexo aims at closing the gap between the C and C++ unit testing frameworks by
implementing one feature that is almost a given in C++ frameworks but that is
_rarely_ seen in their C counterparts: automatic registration of test suites,
test cases, and fixtures.


## Common Implementation

The automatic registration of such C++ frameworks is commonly achieved by
defining a function to run for each test case and by having these functions
added to a global array that can then be iterated over at runtime,
as demonstrated in this snippet:

```c
/* C++ only! */
#include <stdio.h>

struct test_case {
    const char *name;
    void (*run_fn)();
};

struct test_case *test_cases[128];
static int next_test_case_idx = 0;

static int
register_test_case(struct test_case *test_case)
{
    test_cases[next_test_case_idx++] = test_case;
    return 0;
}

#define TEST_CASE(name)                                                        \
    static void name();                                                        \
    struct test_case test_case_##name = {#name, name};                         \
    int dummy_##name = register_test_case(&test_case_##name);                  \
    static void name()

TEST_CASE(foo)
{
    printf("Hello, world, I'm foo!\n");
}

TEST_CASE(bar)
{
    printf("Hello, world, I'm bar!\n");
}

int
main(void)
{
    int i;

    for (i = 0; i < next_test_case_idx; ++i) {
        printf("running `%s`\n", test_cases[i]->name);
        test_cases[i]->run_fn();
    }

    return 0;
}
```

By the time that this program iterates over the test cases in `main`,
the two test cases _foo_ and _bar_ have already been added to the array through
a call to the `register_test_case` function.


## Enter C Land

Running the previous snippet is possible in C++ because it is able to call any
function _before_ executing `main`.

Alas, this doesn't work in C due to additional constraints to the language:

```c
#include <stdio.h>

static int
get_foo()
{
    return 123;
}

static int foo = get_foo(); /* only allowed in C++ :( */

int
main(void)
{
    printf("%d\n", foo);
    return 0;
}
```


Without any standard way of providing an automatic registration framework in C,
most minimalist samples from conventional frameworks look like something along
these lines:


```c
#include <stdlib.h>
#include <unittestframework.h>

struct my_string {
    int length;
    const char *value;
};

static int
set_up(void **data)
{
    struct my_string *s;

    s = (struct my_string *)malloc(sizeof *s);
    if (s == NULL) {
        return 1;
    }

    s->length = 13;
    s->value = "hello, world!";

    *data = (void *)s;
    return 0;
}

static void
tear_down(void *data)
{
    free(data);
}

static void
test_foo(void *data)
{
    struct my_string *s;

    s = (struct my_string *)data;
    ASSERT_INT_EQUAL(s->length, 13);
}

static void
test_bar(void *data)
{
    struct my_string *s;

    s = (struct my_string *)data;
    ASSERT_STR_EQUAL(s, "hello, world!");
}

static const struct test_cases cases[] = {
    {"foo", test_foo},
    {"bar", test_bar},
};

static const struct test_suite suites[]= {
    {"suite", sizeof cases / sizeof cases[0], cases, set_up, tear_down},
};

int
main(void)
{
    return run(sizeof suites / sizeof suites[0], suites);
}
```

Not only is this a lot of **boilerplate** but it is also fairly **error-prone**
since it is easy to add new tests while forgetting to register them further down
the line.


## Making It Work

One solution for C is to use a compiler extension that is expected to be
available for all compilers in a form or another and that provides **custom
memory sections**.

Custom memory sections allow grouping related objects in the same memory space
and to then iterate over them at runtime.

As an example, this is the gist of how such an implementation might look like
for the GNU-compliant compilers:

```c
#include <stdio.h>

struct test_case {
    const char *name;
};

const struct test_case test_case_foo = {"foo"};
const struct test_case test_case_bar = {"bar"};

const struct test_case *__start_rxcases;
const struct test_case *__stop_rxcases;

__attribute__((section("rxcases")))
const struct test_case *test_case_foo_ptr = &test_case_foo;

__attribute__((section("rxcases")))
const struct test_case *test_case_bar_ptr = &test_case_bar;

int
main(void)
{
    const struct test_case **it;

    for (it = &__start_rxcases; it < &__stop_rxcases; ++it) {
        if (*it != NULL) {
            printf("found `%s`\n", (*it)->name);
        }
    }

    return 0;
}
```


And here's the equivalent implementation, adapted to MSVC:

```c
#include <stdio.h>

struct test_case {
    const char *name;
};

const struct test_case test_case_foo = {"foo"};
const struct test_case test_case_bar = {"bar"};

__pragma(section("rxcases$a", read))
__pragma(section("rxcases$b", read))
__pragma(section("rxcases$c", read))

__declspec(allocate("rxcases$a"))
const struct test_case *section_begin = NULL;

__declspec(allocate("rxcases$c"))
const struct test_case *section_end = NULL;

__declspec(allocate("rxcases$b"))
const struct test_case *test_case_foo_ptr = &test_case_foo;

__declspec(allocate("rxcases$b"))
const struct test_case *test_case_bar_ptr = &test_case_bar;

int
main(void)
{
    const struct test_case **it;

    for (it = &section_begin + 1; it < &section_end; ++it) {
        if (*it != NULL) {
            printf("found `%s`\n", (*it)->name);
        }
    }

    return 0;
}
```
