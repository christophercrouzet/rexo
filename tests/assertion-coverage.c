#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <rexo.h>

RX_TEST_CASE(my_test_suite, all_check_successes)
{
    RX_CHECK(123 == 123);
    RX_BOOL_CHECK_TRUE(123 == 123);
    RX_BOOL_CHECK_FALSE(123 == 456);
    RX_INT_CHECK_EQUAL(123, 123);
    RX_INT_CHECK_NOT_EQUAL(123, 456);
    RX_INT_CHECK_GREATER(456, 123);
    RX_INT_CHECK_LESSER(123, 456);
    RX_INT_CHECK_GREATER_OR_EQUAL(456, 123);
    RX_INT_CHECK_LESSER_OR_EQUAL(123, 456);
    RX_UINT_CHECK_EQUAL(123, 123);
    RX_UINT_CHECK_NOT_EQUAL(123, 456);
    RX_UINT_CHECK_GREATER(456, 123);
    RX_UINT_CHECK_LESSER(123, 456);
    RX_UINT_CHECK_GREATER_OR_EQUAL(456, 123);
    RX_UINT_CHECK_LESSER_OR_EQUAL(123, 456);
    RX_REAL_CHECK_EQUAL(1.23, 1.23);
    RX_REAL_CHECK_NOT_EQUAL(1.23, 4.56);
    RX_REAL_CHECK_GREATER(4.56, 1.23);
    RX_REAL_CHECK_LESSER(1.23, 4.56);
    RX_REAL_CHECK_GREATER_OR_EQUAL(4.56, 1.23);
    RX_REAL_CHECK_LESSER_OR_EQUAL(1.23, 4.56);
    RX_REAL_CHECK_EQUAL_FUZZY(1.23, 1.23, 0.001);
    RX_REAL_CHECK_NOT_EQUAL_FUZZY(1.23, 4.56, 0.001);
    RX_STR_CHECK_EQUAL("hello", "hello");
    RX_STR_CHECK_NOT_EQUAL("hello", "world!");
    RX_STR_CHECK_EQUAL_NO_CASE("hello", "hello");
    RX_STR_CHECK_NOT_EQUAL_NO_CASE("hello", "world!");
}

RX_TEST_CASE(my_test_suite, all_require_successes)
{
    RX_REQUIRE(123 == 123);
    RX_BOOL_REQUIRE_TRUE(123 == 123);
    RX_BOOL_REQUIRE_FALSE(123 == 456);
    RX_INT_REQUIRE_EQUAL(123, 123);
    RX_INT_REQUIRE_NOT_EQUAL(123, 456);
    RX_INT_REQUIRE_GREATER(456, 123);
    RX_INT_REQUIRE_LESSER(123, 456);
    RX_INT_REQUIRE_GREATER_OR_EQUAL(456, 123);
    RX_INT_REQUIRE_LESSER_OR_EQUAL(123, 456);
    RX_UINT_REQUIRE_EQUAL(123, 123);
    RX_UINT_REQUIRE_NOT_EQUAL(123, 456);
    RX_UINT_REQUIRE_GREATER(456, 123);
    RX_UINT_REQUIRE_LESSER(123, 456);
    RX_UINT_REQUIRE_GREATER_OR_EQUAL(456, 123);
    RX_UINT_REQUIRE_LESSER_OR_EQUAL(123, 456);
    RX_REAL_REQUIRE_EQUAL(1.23, 1.23);
    RX_REAL_REQUIRE_NOT_EQUAL(1.23, 4.56);
    RX_REAL_REQUIRE_GREATER(4.56, 1.23);
    RX_REAL_REQUIRE_LESSER(1.23, 4.56);
    RX_REAL_REQUIRE_GREATER_OR_EQUAL(4.56, 1.23);
    RX_REAL_REQUIRE_LESSER_OR_EQUAL(1.23, 4.56);
    RX_REAL_REQUIRE_EQUAL_FUZZY(1.23, 1.23, 0.001);
    RX_REAL_REQUIRE_NOT_EQUAL_FUZZY(1.23, 4.56, 0.001);
    RX_STR_REQUIRE_EQUAL("hello", "hello");
    RX_STR_REQUIRE_NOT_EQUAL("hello", "world!");
    RX_STR_REQUIRE_EQUAL_NO_CASE("hello", "hello");
    RX_STR_REQUIRE_NOT_EQUAL_NO_CASE("hello", "world!");
}

RX_TEST_CASE(my_test_suite, all_check_failures)
{
    RX_CHECK(123 == 456);
    RX_BOOL_CHECK_TRUE(123 == 456);
    RX_BOOL_CHECK_FALSE(123 == 123);
    RX_INT_CHECK_EQUAL(123, 456);
    RX_INT_CHECK_NOT_EQUAL(123, 123);
    RX_INT_CHECK_GREATER(123, 456);
    RX_INT_CHECK_LESSER(456, 123);
    RX_INT_CHECK_GREATER_OR_EQUAL(123, 456);
    RX_INT_CHECK_LESSER_OR_EQUAL(456, 123);
    RX_UINT_CHECK_EQUAL(123, 456);
    RX_UINT_CHECK_NOT_EQUAL(123, 123);
    RX_UINT_CHECK_GREATER(123, 456);
    RX_UINT_CHECK_LESSER(456, 123);
    RX_UINT_CHECK_GREATER_OR_EQUAL(123, 456);
    RX_UINT_CHECK_LESSER_OR_EQUAL(456, 123);
    RX_REAL_CHECK_EQUAL(1.23, 4.56);
    RX_REAL_CHECK_NOT_EQUAL(1.23, 1.23);
    RX_REAL_CHECK_GREATER(1.23, 4.56);
    RX_REAL_CHECK_LESSER(4.56, 1.23);
    RX_REAL_CHECK_GREATER_OR_EQUAL(1.23, 4.56);
    RX_REAL_CHECK_LESSER_OR_EQUAL(4.56, 1.23);
    RX_REAL_CHECK_EQUAL_FUZZY(1.23, 4.56, 0.001);
    RX_REAL_CHECK_NOT_EQUAL_FUZZY(1.23, 1.23, 0.001);
    RX_STR_CHECK_EQUAL("hello", "world!");
    RX_STR_CHECK_NOT_EQUAL("hello", "hello");
    RX_STR_CHECK_EQUAL_NO_CASE("hello", "world!");
    RX_STR_CHECK_NOT_EQUAL_NO_CASE("hello", "hello");
}

RX_TEST_CASE(my_test_suite, require_failure)
{
    RX_REQUIRE(123 == 456);
}

RX_TEST_CASE(my_test_suite, bool_require_true_failure)
{
    RX_BOOL_REQUIRE_TRUE(123 == 456);
}

RX_TEST_CASE(my_test_suite, bool_require_false_failure)
{
    RX_BOOL_REQUIRE_FALSE(123 == 123);
}

RX_TEST_CASE(my_test_suite, int_require_equal_failure)
{
    RX_INT_REQUIRE_EQUAL(123, 456);
}

RX_TEST_CASE(my_test_suite, int_require_not_equal_failure)
{
    RX_INT_REQUIRE_NOT_EQUAL(123, 123);
}

RX_TEST_CASE(my_test_suite, int_require_greater_failure)
{
    RX_INT_REQUIRE_GREATER(123, 456);
}

RX_TEST_CASE(my_test_suite, int_require_lesser_failure)
{
    RX_INT_REQUIRE_LESSER(456, 123);
}

RX_TEST_CASE(my_test_suite, int_require_greater_or_equal_failure)
{
    RX_INT_REQUIRE_GREATER_OR_EQUAL(123, 456);
}

RX_TEST_CASE(my_test_suite, int_require_lesser_or_equal_failure)
{
    RX_INT_REQUIRE_LESSER_OR_EQUAL(456, 123);
}

RX_TEST_CASE(my_test_suite, uint_require_equal_failure)
{
    RX_UINT_REQUIRE_EQUAL(123, 456);
}

RX_TEST_CASE(my_test_suite, uint_require_not_equal_failure)
{
    RX_UINT_REQUIRE_NOT_EQUAL(123, 123);
}

RX_TEST_CASE(my_test_suite, uint_require_greater_failure)
{
    RX_UINT_REQUIRE_GREATER(123, 456);
}

RX_TEST_CASE(my_test_suite, uint_require_lesser_failure)
{
    RX_UINT_REQUIRE_LESSER(456, 123);
}

RX_TEST_CASE(my_test_suite, uint_require_greater_or_equal_failure)
{
    RX_UINT_REQUIRE_GREATER_OR_EQUAL(123, 456);
}

RX_TEST_CASE(my_test_suite, uint_require_lesser_or_equal_failure)
{
    RX_UINT_REQUIRE_LESSER_OR_EQUAL(456, 123);
}

RX_TEST_CASE(my_test_suite, real_require_equal_failure)
{
    RX_REAL_REQUIRE_EQUAL(1.23, 4.56);
}

RX_TEST_CASE(my_test_suite, real_require_not_equal_failure)
{
    RX_REAL_REQUIRE_NOT_EQUAL(1.23, 1.23);
}

RX_TEST_CASE(my_test_suite, real_require_greater_failure)
{
    RX_REAL_REQUIRE_GREATER(1.23, 4.56);
}

RX_TEST_CASE(my_test_suite, real_require_lesser_failure)
{
    RX_REAL_REQUIRE_LESSER(4.56, 1.23);
}

RX_TEST_CASE(my_test_suite, real_require_greater_or_equal_failure)
{
    RX_REAL_REQUIRE_GREATER_OR_EQUAL(1.23, 4.56);
}

RX_TEST_CASE(my_test_suite, real_require_lesser_or_equal_failure)
{
    RX_REAL_REQUIRE_LESSER_OR_EQUAL(4.56, 1.23);
}

RX_TEST_CASE(my_test_suite, real_require_equal_fuzzy_failure)
{
    RX_REAL_REQUIRE_EQUAL_FUZZY(1.23, 4.56, 0.001);
}

RX_TEST_CASE(my_test_suite, real_require_not_equal_fuzzy_failure)
{
    RX_REAL_REQUIRE_NOT_EQUAL_FUZZY(1.23, 1.23, 0.001);
}

RX_TEST_CASE(my_test_suite, str_require_equal_failure)
{
    RX_STR_REQUIRE_EQUAL("hello", "world!");
}

RX_TEST_CASE(my_test_suite, str_require_not_equal_failure)
{
    RX_STR_REQUIRE_NOT_EQUAL("hello", "hello");
}

RX_TEST_CASE(my_test_suite, str_require_equal_no_case_failure)
{
    RX_STR_REQUIRE_EQUAL_NO_CASE("hello", "world!");
}

RX_TEST_CASE(my_test_suite, str_require_not_equal_no_case_failure)
{
    RX_STR_REQUIRE_NOT_EQUAL_NO_CASE("hello", "hello");
}

int
main(int argc, const char **argv)
{
    size_t i;
    size_t test_case_count;
    struct rx_test_case *test_cases;
    size_t test_count;
    size_t nonfatal_failure_count;
    size_t fatal_failure_count;

    (void)(argc);
    (void)(argv);

    test_count = 0;
    nonfatal_failure_count = 0;
    fatal_failure_count = 0;

    rx_test_cases_enumerate(&test_case_count, NULL);
    test_cases
        = (struct rx_test_case *)malloc(sizeof *test_cases * test_case_count);
    if (test_cases == NULL) {
        printf("failed to allocate the test cases\n");
        return 1;
    }

    rx_test_cases_enumerate(&test_case_count, test_cases);
    for (i = 0; i < test_case_count; ++i) {
        size_t j;
        const struct rx_test_case *test_case;
        struct rx_summary summary;

        test_case = &test_cases[i];

        if (rx_summary_initialize(&summary, test_case) != RX_SUCCESS) {
            printf("failed to initialize a summary\n");
            return 1;
        }

        if (rx_test_case_run(&summary, test_case) != RX_SUCCESS) {
            printf("failed to run a test case\n");
            rx_summary_terminate(&summary);
            return 1;
        }

        test_count += summary.test_count;
        for (j = 0; j < summary.failure_count; ++j) {
            switch (summary.failures[j].severity) {
                case RX_NONFATAL:
                    ++nonfatal_failure_count;
                    break;
                case RX_FATAL:
                    ++fatal_failure_count;
                    break;
                default:
                    assert(0);
            }
        }

        rx_summary_terminate(&summary);
    }

    free(test_cases);

    assert(test_count == 108);
    assert(nonfatal_failure_count == 27);
    assert(fatal_failure_count == 27);
    return 0;
}
