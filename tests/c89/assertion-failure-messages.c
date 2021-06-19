#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <rexo.h>

#define ASSERT(x)                                                              \
    (void)(                                                                    \
        (x)                                                                    \
        || (printf(__FILE__ ":%d: assertion `" #x "` failed\n", __LINE__), 0)  \
        || (abort(), 0))

RX_TEST_CASE(my_test_suite, require_failure_msg)
{
    RX_REQUIRE_MSG(
        123 == 456,
        "hello, world!");
}

RX_TEST_CASE(my_test_suite, bool_require_true_failure_msg)
{
    RX_BOOL_REQUIRE_TRUE_MSG(
        123 == 456,
        "hello, world!");
}

RX_TEST_CASE(my_test_suite, bool_require_false_failure_msg)
{
    RX_BOOL_REQUIRE_FALSE_MSG(
        123 == 123,
        "hello, world!");
}

RX_TEST_CASE(my_test_suite, int_require_equal_failure_msg)
{
    RX_INT_REQUIRE_EQUAL_MSG(
        123, 456,
        "hello, world!");
}

RX_TEST_CASE(my_test_suite, int_require_not_equal_failure_msg)
{
    RX_INT_REQUIRE_NOT_EQUAL_MSG(
        123, 123,
        "hello, world!");
}

RX_TEST_CASE(my_test_suite, int_require_greater_failure_msg)
{
    RX_INT_REQUIRE_GREATER_MSG(
        123, 456,
        "hello, world!");
}

RX_TEST_CASE(my_test_suite, int_require_lesser_failure_msg)
{
    RX_INT_REQUIRE_LESSER_MSG(
        456, 123,
        "hello, world!");
}

RX_TEST_CASE(my_test_suite, int_require_greater_or_equal_failure_msg)
{
    RX_INT_REQUIRE_GREATER_OR_EQUAL_MSG(
        123, 456,
        "hello, world!");
}

RX_TEST_CASE(my_test_suite, int_require_lesser_or_equal_failure_msg)
{
    RX_INT_REQUIRE_LESSER_OR_EQUAL_MSG(
        456, 123,
        "hello, world!");
}

RX_TEST_CASE(my_test_suite, uint_require_equal_failure_msg)
{
    RX_UINT_REQUIRE_EQUAL_MSG(
        123, 456,
        "hello, world!");
}

RX_TEST_CASE(my_test_suite, uint_require_not_equal_failure_msg)
{
    RX_UINT_REQUIRE_NOT_EQUAL_MSG(
        123, 123,
        "hello, world!");
}

RX_TEST_CASE(my_test_suite, uint_require_greater_failure_msg)
{
    RX_UINT_REQUIRE_GREATER_MSG(
        123, 456,
        "hello, world!");
}

RX_TEST_CASE(my_test_suite, uint_require_lesser_failure_msg)
{
    RX_UINT_REQUIRE_LESSER_MSG(
        456, 123,
        "hello, world!");
}

RX_TEST_CASE(my_test_suite, uint_require_greater_or_equal_failure_msg)
{
    RX_UINT_REQUIRE_GREATER_OR_EQUAL_MSG(
        123, 456,
        "hello, world!");
}

RX_TEST_CASE(my_test_suite, uint_require_lesser_or_equal_failure_msg)
{
    RX_UINT_REQUIRE_LESSER_OR_EQUAL_MSG(
        456, 123,
        "hello, world!");
}

RX_TEST_CASE(my_test_suite, real_require_equal_failure_msg)
{
    RX_REAL_REQUIRE_EQUAL_MSG(
        1.23, 4.56,
        "hello, world!");
}

RX_TEST_CASE(my_test_suite, real_require_not_equal_failure_msg)
{
    RX_REAL_REQUIRE_NOT_EQUAL_MSG(
        1.23, 1.23,
        "hello, world!");
}

RX_TEST_CASE(my_test_suite, real_require_greater_failure_msg)
{
    RX_REAL_REQUIRE_GREATER_MSG(
        1.23, 4.56,
        "hello, world!");
}

RX_TEST_CASE(my_test_suite, real_require_lesser_failure_msg)
{
    RX_REAL_REQUIRE_LESSER_MSG(
        4.56, 1.23,
        "hello, world!");
}

RX_TEST_CASE(my_test_suite, real_require_greater_or_equal_failure_msg)
{
    RX_REAL_REQUIRE_GREATER_OR_EQUAL_MSG(
        1.23, 4.56,
        "hello, world!");
}

RX_TEST_CASE(my_test_suite, real_require_lesser_or_equal_failure_msg)
{
    RX_REAL_REQUIRE_LESSER_OR_EQUAL_MSG(
        4.56, 1.23,
        "hello, world!");
}

RX_TEST_CASE(my_test_suite, real_require_fuzzy_equal_failure_msg)
{
    RX_REAL_REQUIRE_FUZZY_EQUAL_MSG(
        1.23, 4.56, 0.001,
        "hello, world!");
}

RX_TEST_CASE(my_test_suite, real_require_fuzzy_not_equal_failure_msg)
{
    RX_REAL_REQUIRE_FUZZY_NOT_EQUAL_MSG(
        1.23, 1.23, 0.001,
        "hello, world!");
}

RX_TEST_CASE(my_test_suite, str_require_equal_failure_msg)
{
    RX_STR_REQUIRE_EQUAL_MSG(
        "hello", "world!",
        "hello, world!");
}

RX_TEST_CASE(my_test_suite, str_require_not_equal_failure_msg)
{
    RX_STR_REQUIRE_NOT_EQUAL_MSG(
        "hello", "hello",
        "hello, world!");
}

RX_TEST_CASE(my_test_suite, str_require_equal_no_case_failure_msg)
{
    RX_STR_REQUIRE_EQUAL_NO_CASE_MSG(
        "hello", "world!",
        "hello, world!");
}

RX_TEST_CASE(my_test_suite, str_require_not_equal_no_case_failure_msg)
{
    RX_STR_REQUIRE_NOT_EQUAL_NO_CASE_MSG(
        "hello", "hello",
        "hello, world!");
}

RX_TEST_CASE(my_test_suite, ptr_require_equal_failure_msg)
{
    RX_PTR_REQUIRE_EQUAL_MSG(
        (void *)0x123, (void *)0x456,
        "hello, world!");
}

RX_TEST_CASE(my_test_suite, ptr_require_not_equal_failure_msg)
{
    RX_PTR_REQUIRE_NOT_EQUAL_MSG(
        (void *)0x123, (void *)0x123,
        "hello, world!");
}

RX_TEST_CASE(my_test_suite, ptr_require_aligned_failure_msg)
{
    RX_PTR_REQUIRE_ALIGNED_MSG(
        (void *)0x100, 100,
        "hello, world!");
}

RX_TEST_CASE(my_test_suite, all_check_failures_msg)
{
    RX_CHECK_MSG(
        123 == 456,
        "hello, world!");

    RX_BOOL_CHECK_TRUE_MSG(
        123 == 456,
        "hello, world!");

    RX_BOOL_CHECK_FALSE_MSG(
        123 == 123,
        "hello, world!");

    RX_INT_CHECK_EQUAL_MSG(
        123, 456,
        "hello, world!");

    RX_INT_CHECK_NOT_EQUAL_MSG(
        123, 123,
        "hello, world!");

    RX_INT_CHECK_GREATER_MSG(
        123, 456,
        "hello, world!");

    RX_INT_CHECK_LESSER_MSG(
        456, 123,
        "hello, world!");

    RX_INT_CHECK_GREATER_OR_EQUAL_MSG(
        123, 456,
        "hello, world!");

    RX_INT_CHECK_LESSER_OR_EQUAL_MSG(
        456, 123,
        "hello, world!");

    RX_UINT_CHECK_EQUAL_MSG(
        123, 456,
        "hello, world!");

    RX_UINT_CHECK_NOT_EQUAL_MSG(
        123, 123,
        "hello, world!");

    RX_UINT_CHECK_GREATER_MSG(
        123, 456,
        "hello, world!");

    RX_UINT_CHECK_LESSER_MSG(
        456, 123,
        "hello, world!");

    RX_UINT_CHECK_GREATER_OR_EQUAL_MSG(
        123, 456,
        "hello, world!");

    RX_UINT_CHECK_LESSER_OR_EQUAL_MSG(
        456, 123,
        "hello, world!");

    RX_REAL_CHECK_EQUAL_MSG(
        1.23, 4.56,
        "hello, world!");

    RX_REAL_CHECK_NOT_EQUAL_MSG(
        1.23, 1.23,
        "hello, world!");

    RX_REAL_CHECK_GREATER_MSG(
        1.23, 4.56,
        "hello, world!");

    RX_REAL_CHECK_LESSER_MSG(
        4.56, 1.23,
        "hello, world!");

    RX_REAL_CHECK_GREATER_OR_EQUAL_MSG(
        1.23, 4.56,
        "hello, world!");

    RX_REAL_CHECK_LESSER_OR_EQUAL_MSG(
        4.56, 1.23,
        "hello, world!");

    RX_REAL_CHECK_FUZZY_EQUAL_MSG(
        1.23, 4.56, 0.001,
        "hello, world!");

    RX_REAL_CHECK_FUZZY_NOT_EQUAL_MSG(
        1.23, 1.23, 0.001,
        "hello, world!");

    RX_STR_CHECK_EQUAL_MSG(
        "hello", "world!",
        "hello, world!");

    RX_STR_CHECK_NOT_EQUAL_MSG(
        "hello", "hello",
        "hello, world!");

    RX_STR_CHECK_EQUAL_NO_CASE_MSG(
        "hello", "world!",
        "hello, world!");

    RX_STR_CHECK_NOT_EQUAL_NO_CASE_MSG(
        "hello", "hello",
        "hello, world!");

    RX_PTR_CHECK_EQUAL_MSG(
        (void *)0x123, (void *)0x456,
        "hello, world!");

    RX_PTR_CHECK_NOT_EQUAL_MSG(
        (void *)0x123, (void *)0x123,
        "hello, world!");

    RX_PTR_CHECK_ALIGNED_MSG(
        (void *)0x100, 100,
        "hello, world!");
}

RX_TEST_CASE(my_test_suite, require_failure_msg_1)
{
    RX_REQUIRE_MSG_1(
        123 == 456,
        "hello, %s", "world!");
}

RX_TEST_CASE(my_test_suite, bool_require_true_failure_msg_1)
{
    RX_BOOL_REQUIRE_TRUE_MSG_1(
        123 == 456,
        "hello, %s", "world!");
}

RX_TEST_CASE(my_test_suite, bool_require_false_failure_msg_1)
{
    RX_BOOL_REQUIRE_FALSE_MSG_1(
        123 == 123,
        "hello, %s", "world!");
}

RX_TEST_CASE(my_test_suite, int_require_equal_failure_msg_1)
{
    RX_INT_REQUIRE_EQUAL_MSG_1(
        123, 456,
        "hello, %s", "world!");
}

RX_TEST_CASE(my_test_suite, int_require_not_equal_failure_msg_1)
{
    RX_INT_REQUIRE_NOT_EQUAL_MSG_1(
        123, 123,
        "hello, %s", "world!");
}

RX_TEST_CASE(my_test_suite, int_require_greater_failure_msg_1)
{
    RX_INT_REQUIRE_GREATER_MSG_1(
        123, 456,
        "hello, %s", "world!");
}

RX_TEST_CASE(my_test_suite, int_require_lesser_failure_msg_1)
{
    RX_INT_REQUIRE_LESSER_MSG_1(
        456, 123,
        "hello, %s", "world!");
}

RX_TEST_CASE(my_test_suite, int_require_greater_or_equal_failure_msg_1)
{
    RX_INT_REQUIRE_GREATER_OR_EQUAL_MSG_1(
        123, 456,
        "hello, %s", "world!");
}

RX_TEST_CASE(my_test_suite, int_require_lesser_or_equal_failure_msg_1)
{
    RX_INT_REQUIRE_LESSER_OR_EQUAL_MSG_1(
        456, 123,
        "hello, %s", "world!");
}

RX_TEST_CASE(my_test_suite, uint_require_equal_failure_msg_1)
{
    RX_UINT_REQUIRE_EQUAL_MSG_1(
        123, 456,
        "hello, %s", "world!");
}

RX_TEST_CASE(my_test_suite, uint_require_not_equal_failure_msg_1)
{
    RX_UINT_REQUIRE_NOT_EQUAL_MSG_1(
        123, 123,
        "hello, %s", "world!");
}

RX_TEST_CASE(my_test_suite, uint_require_greater_failure_msg_1)
{
    RX_UINT_REQUIRE_GREATER_MSG_1(
        123, 456,
        "hello, %s", "world!");
}

RX_TEST_CASE(my_test_suite, uint_require_lesser_failure_msg_1)
{
    RX_UINT_REQUIRE_LESSER_MSG_1(
        456, 123,
        "hello, %s", "world!");
}

RX_TEST_CASE(my_test_suite, uint_require_greater_or_equal_failure_msg_1)
{
    RX_UINT_REQUIRE_GREATER_OR_EQUAL_MSG_1(
        123, 456,
        "hello, %s", "world!");
}

RX_TEST_CASE(my_test_suite, uint_require_lesser_or_equal_failure_msg_1)
{
    RX_UINT_REQUIRE_LESSER_OR_EQUAL_MSG_1(
        456, 123,
        "hello, %s", "world!");
}

RX_TEST_CASE(my_test_suite, real_require_equal_failure_msg_1)
{
    RX_REAL_REQUIRE_EQUAL_MSG_1(
        1.23, 4.56,
        "hello, %s", "world!");
}

RX_TEST_CASE(my_test_suite, real_require_not_equal_failure_msg_1)
{
    RX_REAL_REQUIRE_NOT_EQUAL_MSG_1(
        1.23, 1.23,
        "hello, %s", "world!");
}

RX_TEST_CASE(my_test_suite, real_require_greater_failure_msg_1)
{
    RX_REAL_REQUIRE_GREATER_MSG_1(
        1.23, 4.56,
        "hello, %s", "world!");
}

RX_TEST_CASE(my_test_suite, real_require_lesser_failure_msg_1)
{
    RX_REAL_REQUIRE_LESSER_MSG_1(
        4.56, 1.23,
        "hello, %s", "world!");
}

RX_TEST_CASE(my_test_suite, real_require_greater_or_equal_failure_msg_1)
{
    RX_REAL_REQUIRE_GREATER_OR_EQUAL_MSG_1(
        1.23, 4.56,
        "hello, %s", "world!");
}

RX_TEST_CASE(my_test_suite, real_require_lesser_or_equal_failure_msg_1)
{
    RX_REAL_REQUIRE_LESSER_OR_EQUAL_MSG_1(
        4.56, 1.23,
        "hello, %s", "world!");
}

RX_TEST_CASE(my_test_suite, real_require_fuzzy_equal_failure_msg_1)
{
    RX_REAL_REQUIRE_FUZZY_EQUAL_MSG_1(
        1.23, 4.56, 0.001,
        "hello, %s", "world!");
}

RX_TEST_CASE(my_test_suite, real_require_fuzzy_not_equal_failure_msg_1)
{
    RX_REAL_REQUIRE_FUZZY_NOT_EQUAL_MSG_1(
        1.23, 1.23, 0.001,
        "hello, %s", "world!");
}

RX_TEST_CASE(my_test_suite, str_require_equal_failure_msg_1)
{
    RX_STR_REQUIRE_EQUAL_MSG_1(
        "hello", "world!",
        "hello, %s", "world!");
}

RX_TEST_CASE(my_test_suite, str_require_not_equal_failure_msg_1)
{
    RX_STR_REQUIRE_NOT_EQUAL_MSG_1(
        "hello", "hello",
        "hello, %s", "world!");
}

RX_TEST_CASE(my_test_suite, str_require_equal_no_case_failure_msg_1)
{
    RX_STR_REQUIRE_EQUAL_NO_CASE_MSG_1(
        "hello", "world!",
        "hello, %s", "world!");
}

RX_TEST_CASE(my_test_suite, str_require_not_equal_no_case_failure_msg_1)
{
    RX_STR_REQUIRE_NOT_EQUAL_NO_CASE_MSG_1(
        "hello", "hello",
        "hello, %s", "world!");
}

RX_TEST_CASE(my_test_suite, ptr_require_equal_failure_msg_1)
{
    RX_PTR_REQUIRE_EQUAL_MSG_1(
        (void *)0x123, (void *)0x456,
        "hello, %s", "world!");
}

RX_TEST_CASE(my_test_suite, ptr_require_not_equal_failure_msg_1)
{
    RX_PTR_REQUIRE_NOT_EQUAL_MSG_1(
        (void *)0x123, (void *)0x123,
        "hello, %s", "world!");
}

RX_TEST_CASE(my_test_suite, ptr_require_aligned_failure_msg_1)
{
    RX_PTR_REQUIRE_ALIGNED_MSG_1(
        (void *)0x100, 100,
        "hello, %s", "world!");
}

RX_TEST_CASE(my_test_suite, all_check_failures_msg_1)
{
    RX_CHECK_MSG_1(
        123 == 456,
        "hello, %s", "world!");

    RX_BOOL_CHECK_TRUE_MSG_1(
        123 == 456,
        "hello, %s", "world!");

    RX_BOOL_CHECK_FALSE_MSG_1(
        123 == 123,
        "hello, %s", "world!");

    RX_INT_CHECK_EQUAL_MSG_1(
        123, 456,
        "hello, %s", "world!");

    RX_INT_CHECK_NOT_EQUAL_MSG_1(
        123, 123,
        "hello, %s", "world!");

    RX_INT_CHECK_GREATER_MSG_1(
        123, 456,
        "hello, %s", "world!");

    RX_INT_CHECK_LESSER_MSG_1(
        456, 123,
        "hello, %s", "world!");

    RX_INT_CHECK_GREATER_OR_EQUAL_MSG_1(
        123, 456,
        "hello, %s", "world!");

    RX_INT_CHECK_LESSER_OR_EQUAL_MSG_1(
        456, 123,
        "hello, %s", "world!");

    RX_UINT_CHECK_EQUAL_MSG_1(
        123, 456,
        "hello, %s", "world!");

    RX_UINT_CHECK_NOT_EQUAL_MSG_1(
        123, 123,
        "hello, %s", "world!");

    RX_UINT_CHECK_GREATER_MSG_1(
        123, 456,
        "hello, %s", "world!");

    RX_UINT_CHECK_LESSER_MSG_1(
        456, 123,
        "hello, %s", "world!");

    RX_UINT_CHECK_GREATER_OR_EQUAL_MSG_1(
        123, 456,
        "hello, %s", "world!");

    RX_UINT_CHECK_LESSER_OR_EQUAL_MSG_1(
        456, 123,
        "hello, %s", "world!");

    RX_REAL_CHECK_EQUAL_MSG_1(
        1.23, 4.56,
        "hello, %s", "world!");

    RX_REAL_CHECK_NOT_EQUAL_MSG_1(
        1.23, 1.23,
        "hello, %s", "world!");

    RX_REAL_CHECK_GREATER_MSG_1(
        1.23, 4.56,
        "hello, %s", "world!");

    RX_REAL_CHECK_LESSER_MSG_1(
        4.56, 1.23,
        "hello, %s", "world!");

    RX_REAL_CHECK_GREATER_OR_EQUAL_MSG_1(
        1.23, 4.56,
        "hello, %s", "world!");

    RX_REAL_CHECK_LESSER_OR_EQUAL_MSG_1(
        4.56, 1.23,
        "hello, %s", "world!");

    RX_REAL_CHECK_FUZZY_EQUAL_MSG_1(
        1.23, 4.56, 0.001,
        "hello, %s", "world!");

    RX_REAL_CHECK_FUZZY_NOT_EQUAL_MSG_1(
        1.23, 1.23, 0.001,
        "hello, %s", "world!");

    RX_STR_CHECK_EQUAL_MSG_1(
        "hello", "world!",
        "hello, %s", "world!");

    RX_STR_CHECK_NOT_EQUAL_MSG_1(
        "hello", "hello",
        "hello, %s", "world!");

    RX_STR_CHECK_EQUAL_NO_CASE_MSG_1(
        "hello", "world!",
        "hello, %s", "world!");

    RX_STR_CHECK_NOT_EQUAL_NO_CASE_MSG_1(
        "hello", "hello",
        "hello, %s", "world!");

    RX_PTR_CHECK_EQUAL_MSG_1(
        (void *)0x123, (void *)0x456,
        "hello, %s", "world!");

    RX_PTR_CHECK_NOT_EQUAL_MSG_1(
        (void *)0x123, (void *)0x123,
        "hello, %s", "world!");

    RX_PTR_CHECK_ALIGNED_MSG_1(
        (void *)0x100, 100,
        "hello, %s", "world!");
}

RX_TEST_CASE(my_test_suite, require_failure_msg_2)
{
    RX_REQUIRE_MSG_2(
        123 == 456,
        "hello, %s%s", "world", "!");
}

RX_TEST_CASE(my_test_suite, bool_require_true_failure_msg_2)
{
    RX_BOOL_REQUIRE_TRUE_MSG_2(
        123 == 456,
        "hello, %s%s", "world", "!");
}

RX_TEST_CASE(my_test_suite, bool_require_false_failure_msg_2)
{
    RX_BOOL_REQUIRE_FALSE_MSG_2(
        123 == 123,
        "hello, %s%s", "world", "!");
}

RX_TEST_CASE(my_test_suite, int_require_equal_failure_msg_2)
{
    RX_INT_REQUIRE_EQUAL_MSG_2(
        123, 456,
        "hello, %s%s", "world", "!");
}

RX_TEST_CASE(my_test_suite, int_require_not_equal_failure_msg_2)
{
    RX_INT_REQUIRE_NOT_EQUAL_MSG_2(
        123, 123,
        "hello, %s%s", "world", "!");
}

RX_TEST_CASE(my_test_suite, int_require_greater_failure_msg_2)
{
    RX_INT_REQUIRE_GREATER_MSG_2(
        123, 456,
        "hello, %s%s", "world", "!");
}

RX_TEST_CASE(my_test_suite, int_require_lesser_failure_msg_2)
{
    RX_INT_REQUIRE_LESSER_MSG_2(
        456, 123,
        "hello, %s%s", "world", "!");
}

RX_TEST_CASE(my_test_suite, int_require_greater_or_equal_failure_msg_2)
{
    RX_INT_REQUIRE_GREATER_OR_EQUAL_MSG_2(
        123, 456,
        "hello, %s%s", "world", "!");
}

RX_TEST_CASE(my_test_suite, int_require_lesser_or_equal_failure_msg_2)
{
    RX_INT_REQUIRE_LESSER_OR_EQUAL_MSG_2(
        456, 123,
        "hello, %s%s", "world", "!");
}

RX_TEST_CASE(my_test_suite, uint_require_equal_failure_msg_2)
{
    RX_UINT_REQUIRE_EQUAL_MSG_2(
        123, 456,
        "hello, %s%s", "world", "!");
}

RX_TEST_CASE(my_test_suite, uint_require_not_equal_failure_msg_2)
{
    RX_UINT_REQUIRE_NOT_EQUAL_MSG_2(
        123, 123,
        "hello, %s%s", "world", "!");
}

RX_TEST_CASE(my_test_suite, uint_require_greater_failure_msg_2)
{
    RX_UINT_REQUIRE_GREATER_MSG_2(
        123, 456,
        "hello, %s%s", "world", "!");
}

RX_TEST_CASE(my_test_suite, uint_require_lesser_failure_msg_2)
{
    RX_UINT_REQUIRE_LESSER_MSG_2(
        456, 123,
        "hello, %s%s", "world", "!");
}

RX_TEST_CASE(my_test_suite, uint_require_greater_or_equal_failure_msg_2)
{
    RX_UINT_REQUIRE_GREATER_OR_EQUAL_MSG_2(
        123, 456,
        "hello, %s%s", "world", "!");
}

RX_TEST_CASE(my_test_suite, uint_require_lesser_or_equal_failure_msg_2)
{
    RX_UINT_REQUIRE_LESSER_OR_EQUAL_MSG_2(
        456, 123,
        "hello, %s%s", "world", "!");
}

RX_TEST_CASE(my_test_suite, real_require_equal_failure_msg_2)
{
    RX_REAL_REQUIRE_EQUAL_MSG_2(
        1.23, 4.56,
        "hello, %s%s", "world", "!");
}

RX_TEST_CASE(my_test_suite, real_require_not_equal_failure_msg_2)
{
    RX_REAL_REQUIRE_NOT_EQUAL_MSG_2(
        1.23, 1.23,
        "hello, %s%s", "world", "!");
}

RX_TEST_CASE(my_test_suite, real_require_greater_failure_msg_2)
{
    RX_REAL_REQUIRE_GREATER_MSG_2(
        1.23, 4.56,
        "hello, %s%s", "world", "!");
}

RX_TEST_CASE(my_test_suite, real_require_lesser_failure_msg_2)
{
    RX_REAL_REQUIRE_LESSER_MSG_2(
        4.56, 1.23,
        "hello, %s%s", "world", "!");
}

RX_TEST_CASE(my_test_suite, real_require_greater_or_equal_failure_msg_2)
{
    RX_REAL_REQUIRE_GREATER_OR_EQUAL_MSG_2(
        1.23, 4.56,
        "hello, %s%s", "world", "!");
}

RX_TEST_CASE(my_test_suite, real_require_lesser_or_equal_failure_msg_2)
{
    RX_REAL_REQUIRE_LESSER_OR_EQUAL_MSG_2(
        4.56, 1.23,
        "hello, %s%s", "world", "!");
}

RX_TEST_CASE(my_test_suite, real_require_fuzzy_equal_failure_msg_2)
{
    RX_REAL_REQUIRE_FUZZY_EQUAL_MSG_2(
        1.23, 4.56, 0.001,
        "hello, %s%s", "world", "!");
}

RX_TEST_CASE(my_test_suite, real_require_fuzzy_not_equal_failure_msg_2)
{
    RX_REAL_REQUIRE_FUZZY_NOT_EQUAL_MSG_2(
        1.23, 1.23, 0.001,
        "hello, %s%s", "world", "!");
}

RX_TEST_CASE(my_test_suite, str_require_equal_failure_msg_2)
{
    RX_STR_REQUIRE_EQUAL_MSG_2(
        "hello", "world!",
        "hello, %s%s", "world", "!");
}

RX_TEST_CASE(my_test_suite, str_require_not_equal_failure_msg_2)
{
    RX_STR_REQUIRE_NOT_EQUAL_MSG_2(
        "hello", "hello",
        "hello, %s%s", "world", "!");
}

RX_TEST_CASE(my_test_suite, str_require_equal_no_case_failure_msg_2)
{
    RX_STR_REQUIRE_EQUAL_NO_CASE_MSG_2(
        "hello", "world!",
        "hello, %s%s", "world", "!");
}

RX_TEST_CASE(my_test_suite, str_require_not_equal_no_case_failure_msg_2)
{
    RX_STR_REQUIRE_NOT_EQUAL_NO_CASE_MSG_2(
        "hello", "hello",
        "hello, %s%s", "world", "!");
}

RX_TEST_CASE(my_test_suite, ptr_require_equal_failure_msg_2)
{
    RX_PTR_REQUIRE_EQUAL_MSG_2(
        (void *)0x123, (void *)0x456,
        "hello, %s%s", "world", "!");
}

RX_TEST_CASE(my_test_suite, ptr_require_not_equal_failure_msg_2)
{
    RX_PTR_REQUIRE_NOT_EQUAL_MSG_2(
        (void *)0x123, (void *)0x123,
        "hello, %s%s", "world", "!");
}

RX_TEST_CASE(my_test_suite, ptr_require_aligned_failure_msg_2)
{
    RX_PTR_REQUIRE_ALIGNED_MSG_2(
        (void *)0x100, 100,
        "hello, %s%s", "world", "!");
}

RX_TEST_CASE(my_test_suite, all_check_failures_msg_2)
{
    RX_CHECK_MSG_2(
        123 == 456,
        "hello, %s%s", "world", "!");

    RX_BOOL_CHECK_TRUE_MSG_2(
        123 == 456,
        "hello, %s%s", "world", "!");

    RX_BOOL_CHECK_FALSE_MSG_2(
        123 == 123,
        "hello, %s%s", "world", "!");

    RX_INT_CHECK_EQUAL_MSG_2(
        123, 456,
        "hello, %s%s", "world", "!");

    RX_INT_CHECK_NOT_EQUAL_MSG_2(
        123, 123,
        "hello, %s%s", "world", "!");

    RX_INT_CHECK_GREATER_MSG_2(
        123, 456,
        "hello, %s%s", "world", "!");

    RX_INT_CHECK_LESSER_MSG_2(
        456, 123,
        "hello, %s%s", "world", "!");

    RX_INT_CHECK_GREATER_OR_EQUAL_MSG_2(
        123, 456,
        "hello, %s%s", "world", "!");

    RX_INT_CHECK_LESSER_OR_EQUAL_MSG_2(
        456, 123,
        "hello, %s%s", "world", "!");

    RX_UINT_CHECK_EQUAL_MSG_2(
        123, 456,
        "hello, %s%s", "world", "!");

    RX_UINT_CHECK_NOT_EQUAL_MSG_2(
        123, 123,
        "hello, %s%s", "world", "!");

    RX_UINT_CHECK_GREATER_MSG_2(
        123, 456,
        "hello, %s%s", "world", "!");

    RX_UINT_CHECK_LESSER_MSG_2(
        456, 123,
        "hello, %s%s", "world", "!");

    RX_UINT_CHECK_GREATER_OR_EQUAL_MSG_2(
        123, 456,
        "hello, %s%s", "world", "!");

    RX_UINT_CHECK_LESSER_OR_EQUAL_MSG_2(
        456, 123,
        "hello, %s%s", "world", "!");

    RX_REAL_CHECK_EQUAL_MSG_2(
        1.23, 4.56,
        "hello, %s%s", "world", "!");

    RX_REAL_CHECK_NOT_EQUAL_MSG_2(
        1.23, 1.23,
        "hello, %s%s", "world", "!");

    RX_REAL_CHECK_GREATER_MSG_2(
        1.23, 4.56,
        "hello, %s%s", "world", "!");

    RX_REAL_CHECK_LESSER_MSG_2(
        4.56, 1.23,
        "hello, %s%s", "world", "!");

    RX_REAL_CHECK_GREATER_OR_EQUAL_MSG_2(
        1.23, 4.56,
        "hello, %s%s", "world", "!");

    RX_REAL_CHECK_LESSER_OR_EQUAL_MSG_2(
        4.56, 1.23,
        "hello, %s%s", "world", "!");

    RX_REAL_CHECK_FUZZY_EQUAL_MSG_2(
        1.23, 4.56, 0.001,
        "hello, %s%s", "world", "!");

    RX_REAL_CHECK_FUZZY_NOT_EQUAL_MSG_2(
        1.23, 1.23, 0.001,
        "hello, %s%s", "world", "!");

    RX_STR_CHECK_EQUAL_MSG_2(
        "hello", "world!",
        "hello, %s%s", "world", "!");

    RX_STR_CHECK_NOT_EQUAL_MSG_2(
        "hello", "hello",
        "hello, %s%s", "world", "!");

    RX_STR_CHECK_EQUAL_NO_CASE_MSG_2(
        "hello", "world!",
        "hello, %s%s", "world", "!");

    RX_STR_CHECK_NOT_EQUAL_NO_CASE_MSG_2(
        "hello", "hello",
        "hello, %s%s", "world", "!");

    RX_PTR_CHECK_EQUAL_MSG_2(
        (void *)0x123, (void *)0x456,
        "hello, %s%s", "world", "!");

    RX_PTR_CHECK_NOT_EQUAL_MSG_2(
        (void *)0x123, (void *)0x123,
        "hello, %s%s", "world", "!");

    RX_PTR_CHECK_ALIGNED_MSG_2(
        (void *)0x100, 100,
        "hello, %s%s", "world", "!");
}

RX_TEST_CASE(my_test_suite, require_failure_msg_3)
{
    RX_REQUIRE_MSG_3(
        123 == 456,
        "%s, %s%s", "hello", "world", "!");
}

RX_TEST_CASE(my_test_suite, bool_require_true_failure_msg_3)
{
    RX_BOOL_REQUIRE_TRUE_MSG_3(
        123 == 456,
        "%s, %s%s", "hello", "world", "!");
}

RX_TEST_CASE(my_test_suite, bool_require_false_failure_msg_3)
{
    RX_BOOL_REQUIRE_FALSE_MSG_3(
        123 == 123,
        "%s, %s%s", "hello", "world", "!");
}

RX_TEST_CASE(my_test_suite, int_require_equal_failure_msg_3)
{
    RX_INT_REQUIRE_EQUAL_MSG_3(
        123, 456,
        "%s, %s%s", "hello", "world", "!");
}

RX_TEST_CASE(my_test_suite, int_require_not_equal_failure_msg_3)
{
    RX_INT_REQUIRE_NOT_EQUAL_MSG_3(
        123, 123,
        "%s, %s%s", "hello", "world", "!");
}

RX_TEST_CASE(my_test_suite, int_require_greater_failure_msg_3)
{
    RX_INT_REQUIRE_GREATER_MSG_3(
        123, 456,
        "%s, %s%s", "hello", "world", "!");
}

RX_TEST_CASE(my_test_suite, int_require_lesser_failure_msg_3)
{
    RX_INT_REQUIRE_LESSER_MSG_3(
        456, 123,
        "%s, %s%s", "hello", "world", "!");
}

RX_TEST_CASE(my_test_suite, int_require_greater_or_equal_failure_msg_3)
{
    RX_INT_REQUIRE_GREATER_OR_EQUAL_MSG_3(
        123, 456,
        "%s, %s%s", "hello", "world", "!");
}

RX_TEST_CASE(my_test_suite, int_require_lesser_or_equal_failure_msg_3)
{
    RX_INT_REQUIRE_LESSER_OR_EQUAL_MSG_3(
        456, 123,
        "%s, %s%s", "hello", "world", "!");
}

RX_TEST_CASE(my_test_suite, uint_require_equal_failure_msg_3)
{
    RX_UINT_REQUIRE_EQUAL_MSG_3(
        123, 456,
        "%s, %s%s", "hello", "world", "!");
}

RX_TEST_CASE(my_test_suite, uint_require_not_equal_failure_msg_3)
{
    RX_UINT_REQUIRE_NOT_EQUAL_MSG_3(
        123, 123,
        "%s, %s%s", "hello", "world", "!");
}

RX_TEST_CASE(my_test_suite, uint_require_greater_failure_msg_3)
{
    RX_UINT_REQUIRE_GREATER_MSG_3(
        123, 456,
        "%s, %s%s", "hello", "world", "!");
}

RX_TEST_CASE(my_test_suite, uint_require_lesser_failure_msg_3)
{
    RX_UINT_REQUIRE_LESSER_MSG_3(
        456, 123,
        "%s, %s%s", "hello", "world", "!");
}

RX_TEST_CASE(my_test_suite, uint_require_greater_or_equal_failure_msg_3)
{
    RX_UINT_REQUIRE_GREATER_OR_EQUAL_MSG_3(
        123, 456,
        "%s, %s%s", "hello", "world", "!");
}

RX_TEST_CASE(my_test_suite, uint_require_lesser_or_equal_failure_msg_3)
{
    RX_UINT_REQUIRE_LESSER_OR_EQUAL_MSG_3(
        456, 123,
        "%s, %s%s", "hello", "world", "!");
}

RX_TEST_CASE(my_test_suite, real_require_equal_failure_msg_3)
{
    RX_REAL_REQUIRE_EQUAL_MSG_3(
        1.23, 4.56,
        "%s, %s%s", "hello", "world", "!");
}

RX_TEST_CASE(my_test_suite, real_require_not_equal_failure_msg_3)
{
    RX_REAL_REQUIRE_NOT_EQUAL_MSG_3(
        1.23, 1.23,
        "%s, %s%s", "hello", "world", "!");
}

RX_TEST_CASE(my_test_suite, real_require_greater_failure_msg_3)
{
    RX_REAL_REQUIRE_GREATER_MSG_3(
        1.23, 4.56,
        "%s, %s%s", "hello", "world", "!");
}

RX_TEST_CASE(my_test_suite, real_require_lesser_failure_msg_3)
{
    RX_REAL_REQUIRE_LESSER_MSG_3(
        4.56, 1.23,
        "%s, %s%s", "hello", "world", "!");
}

RX_TEST_CASE(my_test_suite, real_require_greater_or_equal_failure_msg_3)
{
    RX_REAL_REQUIRE_GREATER_OR_EQUAL_MSG_3(
        1.23, 4.56,
        "%s, %s%s", "hello", "world", "!");
}

RX_TEST_CASE(my_test_suite, real_require_lesser_or_equal_failure_msg_3)
{
    RX_REAL_REQUIRE_LESSER_OR_EQUAL_MSG_3(
        4.56, 1.23,
        "%s, %s%s", "hello", "world", "!");
}

RX_TEST_CASE(my_test_suite, real_require_fuzzy_equal_failure_msg_3)
{
    RX_REAL_REQUIRE_FUZZY_EQUAL_MSG_3(
        1.23, 4.56, 0.001,
        "%s, %s%s", "hello", "world", "!");
}

RX_TEST_CASE(my_test_suite, real_require_fuzzy_not_equal_failure_msg_3)
{
    RX_REAL_REQUIRE_FUZZY_NOT_EQUAL_MSG_3(
        1.23, 1.23, 0.001,
        "%s, %s%s", "hello", "world", "!");
}

RX_TEST_CASE(my_test_suite, str_require_equal_failure_msg_3)
{
    RX_STR_REQUIRE_EQUAL_MSG_3(
        "hello", "world!",
        "%s, %s%s", "hello", "world", "!");
}

RX_TEST_CASE(my_test_suite, str_require_not_equal_failure_msg_3)
{
    RX_STR_REQUIRE_NOT_EQUAL_MSG_3(
        "hello", "hello",
        "%s, %s%s", "hello", "world", "!");
}

RX_TEST_CASE(my_test_suite, str_require_equal_no_case_failure_msg_3)
{
    RX_STR_REQUIRE_EQUAL_NO_CASE_MSG_3(
        "hello", "world!",
        "%s, %s%s", "hello", "world", "!");
}

RX_TEST_CASE(my_test_suite, str_require_not_equal_no_case_failure_msg_3)
{
    RX_STR_REQUIRE_NOT_EQUAL_NO_CASE_MSG_3(
        "hello", "hello",
        "%s, %s%s", "hello", "world", "!");
}

RX_TEST_CASE(my_test_suite, ptr_require_equal_failure_msg_3)
{
    RX_PTR_REQUIRE_EQUAL_MSG_3(
        (void *)0x123, (void *)0x456,
        "%s, %s%s", "hello", "world", "!");
}

RX_TEST_CASE(my_test_suite, ptr_require_not_equal_failure_msg_3)
{
    RX_PTR_REQUIRE_NOT_EQUAL_MSG_3(
        (void *)0x123, (void *)0x123,
        "%s, %s%s", "hello", "world", "!");
}

RX_TEST_CASE(my_test_suite, ptr_require_aligned_failure_msg_3)
{
    RX_PTR_REQUIRE_ALIGNED_MSG_3(
        (void *)0x100, 100,
        "%s, %s%s", "hello", "world", "!");
}

RX_TEST_CASE(my_test_suite, all_check_failures_msg_3)
{
    RX_CHECK_MSG_3(
        123 == 456,
        "%s, %s%s", "hello", "world", "!");

    RX_BOOL_CHECK_TRUE_MSG_3(
        123 == 456,
        "%s, %s%s", "hello", "world", "!");

    RX_BOOL_CHECK_FALSE_MSG_3(
        123 == 123,
        "%s, %s%s", "hello", "world", "!");

    RX_INT_CHECK_EQUAL_MSG_3(
        123, 456,
        "%s, %s%s", "hello", "world", "!");

    RX_INT_CHECK_NOT_EQUAL_MSG_3(
        123, 123,
        "%s, %s%s", "hello", "world", "!");

    RX_INT_CHECK_GREATER_MSG_3(
        123, 456,
        "%s, %s%s", "hello", "world", "!");

    RX_INT_CHECK_LESSER_MSG_3(
        456, 123,
        "%s, %s%s", "hello", "world", "!");

    RX_INT_CHECK_GREATER_OR_EQUAL_MSG_3(
        123, 456,
        "%s, %s%s", "hello", "world", "!");

    RX_INT_CHECK_LESSER_OR_EQUAL_MSG_3(
        456, 123,
        "%s, %s%s", "hello", "world", "!");

    RX_UINT_CHECK_EQUAL_MSG_3(
        123, 456,
        "%s, %s%s", "hello", "world", "!");

    RX_UINT_CHECK_NOT_EQUAL_MSG_3(
        123, 123,
        "%s, %s%s", "hello", "world", "!");

    RX_UINT_CHECK_GREATER_MSG_3(
        123, 456,
        "%s, %s%s", "hello", "world", "!");

    RX_UINT_CHECK_LESSER_MSG_3(
        456, 123,
        "%s, %s%s", "hello", "world", "!");

    RX_UINT_CHECK_GREATER_OR_EQUAL_MSG_3(
        123, 456,
        "%s, %s%s", "hello", "world", "!");

    RX_UINT_CHECK_LESSER_OR_EQUAL_MSG_3(
        456, 123,
        "%s, %s%s", "hello", "world", "!");

    RX_REAL_CHECK_EQUAL_MSG_3(
        1.23, 4.56,
        "%s, %s%s", "hello", "world", "!");

    RX_REAL_CHECK_NOT_EQUAL_MSG_3(
        1.23, 1.23,
        "%s, %s%s", "hello", "world", "!");

    RX_REAL_CHECK_GREATER_MSG_3(
        1.23, 4.56,
        "%s, %s%s", "hello", "world", "!");

    RX_REAL_CHECK_LESSER_MSG_3(
        4.56, 1.23,
        "%s, %s%s", "hello", "world", "!");

    RX_REAL_CHECK_GREATER_OR_EQUAL_MSG_3(
        1.23, 4.56,
        "%s, %s%s", "hello", "world", "!");

    RX_REAL_CHECK_LESSER_OR_EQUAL_MSG_3(
        4.56, 1.23,
        "%s, %s%s", "hello", "world", "!");

    RX_REAL_CHECK_FUZZY_EQUAL_MSG_3(
        1.23, 4.56, 0.001,
        "%s, %s%s", "hello", "world", "!");

    RX_REAL_CHECK_FUZZY_NOT_EQUAL_MSG_3(
        1.23, 1.23, 0.001,
        "%s, %s%s", "hello", "world", "!");

    RX_STR_CHECK_EQUAL_MSG_3(
        "hello", "world!",
        "%s, %s%s", "hello", "world", "!");

    RX_STR_CHECK_NOT_EQUAL_MSG_3(
        "hello", "hello",
        "%s, %s%s", "hello", "world", "!");

    RX_STR_CHECK_EQUAL_NO_CASE_MSG_3(
        "hello", "world!",
        "%s, %s%s", "hello", "world", "!");

    RX_STR_CHECK_NOT_EQUAL_NO_CASE_MSG_3(
        "hello", "hello",
        "%s, %s%s", "hello", "world", "!");

    RX_PTR_CHECK_EQUAL_MSG_3(
        (void *)0x123, (void *)0x456,
        "%s, %s%s", "hello", "world", "!");

    RX_PTR_CHECK_NOT_EQUAL_MSG_3(
        (void *)0x123, (void *)0x123,
        "%s, %s%s", "hello", "world", "!");

    RX_PTR_CHECK_ALIGNED_MSG_3(
        (void *)0x100, 100,
        "%s, %s%s", "hello", "world", "!");
}

RX_TEST_CASE(my_test_suite, require_failure_msg_4)
{
    RX_REQUIRE_MSG_4(
        123 == 456,
        "%s%s %s%s", "hello", ",", "world", "!");
}

RX_TEST_CASE(my_test_suite, bool_require_true_failure_msg_4)
{
    RX_BOOL_REQUIRE_TRUE_MSG_4(
        123 == 456,
        "%s%s %s%s", "hello", ",", "world", "!");
}

RX_TEST_CASE(my_test_suite, bool_require_false_failure_msg_4)
{
    RX_BOOL_REQUIRE_FALSE_MSG_4(
        123 == 123,
        "%s%s %s%s", "hello", ",", "world", "!");
}

RX_TEST_CASE(my_test_suite, int_require_equal_failure_msg_4)
{
    RX_INT_REQUIRE_EQUAL_MSG_4(
        123, 456,
        "%s%s %s%s", "hello", ",", "world", "!");
}

RX_TEST_CASE(my_test_suite, int_require_not_equal_failure_msg_4)
{
    RX_INT_REQUIRE_NOT_EQUAL_MSG_4(
        123, 123,
        "%s%s %s%s", "hello", ",", "world", "!");
}

RX_TEST_CASE(my_test_suite, int_require_greater_failure_msg_4)
{
    RX_INT_REQUIRE_GREATER_MSG_4(
        123, 456,
        "%s%s %s%s", "hello", ",", "world", "!");
}

RX_TEST_CASE(my_test_suite, int_require_lesser_failure_msg_4)
{
    RX_INT_REQUIRE_LESSER_MSG_4(
        456, 123,
        "%s%s %s%s", "hello", ",", "world", "!");
}

RX_TEST_CASE(my_test_suite, int_require_greater_or_equal_failure_msg_4)
{
    RX_INT_REQUIRE_GREATER_OR_EQUAL_MSG_4(
        123, 456,
        "%s%s %s%s", "hello", ",", "world", "!");
}

RX_TEST_CASE(my_test_suite, int_require_lesser_or_equal_failure_msg_4)
{
    RX_INT_REQUIRE_LESSER_OR_EQUAL_MSG_4(
        456, 123,
        "%s%s %s%s", "hello", ",", "world", "!");
}

RX_TEST_CASE(my_test_suite, uint_require_equal_failure_msg_4)
{
    RX_UINT_REQUIRE_EQUAL_MSG_4(
        123, 456,
        "%s%s %s%s", "hello", ",", "world", "!");
}

RX_TEST_CASE(my_test_suite, uint_require_not_equal_failure_msg_4)
{
    RX_UINT_REQUIRE_NOT_EQUAL_MSG_4(
        123, 123,
        "%s%s %s%s", "hello", ",", "world", "!");
}

RX_TEST_CASE(my_test_suite, uint_require_greater_failure_msg_4)
{
    RX_UINT_REQUIRE_GREATER_MSG_4(
        123, 456,
        "%s%s %s%s", "hello", ",", "world", "!");
}

RX_TEST_CASE(my_test_suite, uint_require_lesser_failure_msg_4)
{
    RX_UINT_REQUIRE_LESSER_MSG_4(
        456, 123,
        "%s%s %s%s", "hello", ",", "world", "!");
}

RX_TEST_CASE(my_test_suite, uint_require_greater_or_equal_failure_msg_4)
{
    RX_UINT_REQUIRE_GREATER_OR_EQUAL_MSG_4(
        123, 456,
        "%s%s %s%s", "hello", ",", "world", "!");
}

RX_TEST_CASE(my_test_suite, uint_require_lesser_or_equal_failure_msg_4)
{
    RX_UINT_REQUIRE_LESSER_OR_EQUAL_MSG_4(
        456, 123,
        "%s%s %s%s", "hello", ",", "world", "!");
}

RX_TEST_CASE(my_test_suite, real_require_equal_failure_msg_4)
{
    RX_REAL_REQUIRE_EQUAL_MSG_4(
        1.23, 4.56,
        "%s%s %s%s", "hello", ",", "world", "!");
}

RX_TEST_CASE(my_test_suite, real_require_not_equal_failure_msg_4)
{
    RX_REAL_REQUIRE_NOT_EQUAL_MSG_4(
        1.23, 1.23,
        "%s%s %s%s", "hello", ",", "world", "!");
}

RX_TEST_CASE(my_test_suite, real_require_greater_failure_msg_4)
{
    RX_REAL_REQUIRE_GREATER_MSG_4(
        1.23, 4.56,
        "%s%s %s%s", "hello", ",", "world", "!");
}

RX_TEST_CASE(my_test_suite, real_require_lesser_failure_msg_4)
{
    RX_REAL_REQUIRE_LESSER_MSG_4(
        4.56, 1.23,
        "%s%s %s%s", "hello", ",", "world", "!");
}

RX_TEST_CASE(my_test_suite, real_require_greater_or_equal_failure_msg_4)
{
    RX_REAL_REQUIRE_GREATER_OR_EQUAL_MSG_4(
        1.23, 4.56,
        "%s%s %s%s", "hello", ",", "world", "!");
}

RX_TEST_CASE(my_test_suite, real_require_lesser_or_equal_failure_msg_4)
{
    RX_REAL_REQUIRE_LESSER_OR_EQUAL_MSG_4(
        4.56, 1.23,
        "%s%s %s%s", "hello", ",", "world", "!");
}

RX_TEST_CASE(my_test_suite, real_require_fuzzy_equal_failure_msg_4)
{
    RX_REAL_REQUIRE_FUZZY_EQUAL_MSG_4(
        1.23, 4.56, 0.001,
        "%s%s %s%s", "hello", ",", "world", "!");
}

RX_TEST_CASE(my_test_suite, real_require_fuzzy_not_equal_failure_msg_4)
{
    RX_REAL_REQUIRE_FUZZY_NOT_EQUAL_MSG_4(
        1.23, 1.23, 0.001,
        "%s%s %s%s", "hello", ",", "world", "!");
}

RX_TEST_CASE(my_test_suite, str_require_equal_failure_msg_4)
{
    RX_STR_REQUIRE_EQUAL_MSG_4(
        "hello", "world!",
        "%s%s %s%s", "hello", ",", "world", "!");
}

RX_TEST_CASE(my_test_suite, str_require_not_equal_failure_msg_4)
{
    RX_STR_REQUIRE_NOT_EQUAL_MSG_4(
        "hello", "hello",
        "%s%s %s%s", "hello", ",", "world", "!");
}

RX_TEST_CASE(my_test_suite, str_require_equal_no_case_failure_msg_4)
{
    RX_STR_REQUIRE_EQUAL_NO_CASE_MSG_4(
        "hello", "world!",
        "%s%s %s%s", "hello", ",", "world", "!");
}

RX_TEST_CASE(my_test_suite, str_require_not_equal_no_case_failure_msg_4)
{
    RX_STR_REQUIRE_NOT_EQUAL_NO_CASE_MSG_4(
        "hello", "hello",
        "%s%s %s%s", "hello", ",", "world", "!");
}

RX_TEST_CASE(my_test_suite, ptr_require_equal_failure_msg_4)
{
    RX_PTR_REQUIRE_EQUAL_MSG_4(
        (void *)0x123, (void *)0x456,
        "%s%s %s%s", "hello", ",", "world", "!");
}

RX_TEST_CASE(my_test_suite, ptr_require_not_equal_failure_msg_4)
{
    RX_PTR_REQUIRE_NOT_EQUAL_MSG_4(
        (void *)0x123, (void *)0x123,
        "%s%s %s%s", "hello", ",", "world", "!");
}

RX_TEST_CASE(my_test_suite, ptr_require_aligned_failure_msg_4)
{
    RX_PTR_REQUIRE_ALIGNED_MSG_4(
        (void *)0x100, 100,
        "%s%s %s%s", "hello", ",", "world", "!");
}

RX_TEST_CASE(my_test_suite, all_check_failures_msg_4)
{
    RX_CHECK_MSG_4(
        123 == 456,
        "%s%s %s%s", "hello", ",", "world", "!");

    RX_BOOL_CHECK_TRUE_MSG_4(
        123 == 456,
        "%s%s %s%s", "hello", ",", "world", "!");

    RX_BOOL_CHECK_FALSE_MSG_4(
        123 == 123,
        "%s%s %s%s", "hello", ",", "world", "!");

    RX_INT_CHECK_EQUAL_MSG_4(
        123, 456,
        "%s%s %s%s", "hello", ",", "world", "!");

    RX_INT_CHECK_NOT_EQUAL_MSG_4(
        123, 123,
        "%s%s %s%s", "hello", ",", "world", "!");

    RX_INT_CHECK_GREATER_MSG_4(
        123, 456,
        "%s%s %s%s", "hello", ",", "world", "!");

    RX_INT_CHECK_LESSER_MSG_4(
        456, 123,
        "%s%s %s%s", "hello", ",", "world", "!");

    RX_INT_CHECK_GREATER_OR_EQUAL_MSG_4(
        123, 456,
        "%s%s %s%s", "hello", ",", "world", "!");

    RX_INT_CHECK_LESSER_OR_EQUAL_MSG_4(
        456, 123,
        "%s%s %s%s", "hello", ",", "world", "!");

    RX_UINT_CHECK_EQUAL_MSG_4(
        123, 456,
        "%s%s %s%s", "hello", ",", "world", "!");

    RX_UINT_CHECK_NOT_EQUAL_MSG_4(
        123, 123,
        "%s%s %s%s", "hello", ",", "world", "!");

    RX_UINT_CHECK_GREATER_MSG_4(
        123, 456,
        "%s%s %s%s", "hello", ",", "world", "!");

    RX_UINT_CHECK_LESSER_MSG_4(
        456, 123,
        "%s%s %s%s", "hello", ",", "world", "!");

    RX_UINT_CHECK_GREATER_OR_EQUAL_MSG_4(
        123, 456,
        "%s%s %s%s", "hello", ",", "world", "!");

    RX_UINT_CHECK_LESSER_OR_EQUAL_MSG_4(
        456, 123,
        "%s%s %s%s", "hello", ",", "world", "!");

    RX_REAL_CHECK_EQUAL_MSG_4(
        1.23, 4.56,
        "%s%s %s%s", "hello", ",", "world", "!");

    RX_REAL_CHECK_NOT_EQUAL_MSG_4(
        1.23, 1.23,
        "%s%s %s%s", "hello", ",", "world", "!");

    RX_REAL_CHECK_GREATER_MSG_4(
        1.23, 4.56,
        "%s%s %s%s", "hello", ",", "world", "!");

    RX_REAL_CHECK_LESSER_MSG_4(
        4.56, 1.23,
        "%s%s %s%s", "hello", ",", "world", "!");

    RX_REAL_CHECK_GREATER_OR_EQUAL_MSG_4(
        1.23, 4.56,
        "%s%s %s%s", "hello", ",", "world", "!");

    RX_REAL_CHECK_LESSER_OR_EQUAL_MSG_4(
        4.56, 1.23,
        "%s%s %s%s", "hello", ",", "world", "!");

    RX_REAL_CHECK_FUZZY_EQUAL_MSG_4(
        1.23, 4.56, 0.001,
        "%s%s %s%s", "hello", ",", "world", "!");

    RX_REAL_CHECK_FUZZY_NOT_EQUAL_MSG_4(
        1.23, 1.23, 0.001,
        "%s%s %s%s", "hello", ",", "world", "!");

    RX_STR_CHECK_EQUAL_MSG_4(
        "hello", "world!",
        "%s%s %s%s", "hello", ",", "world", "!");

    RX_STR_CHECK_NOT_EQUAL_MSG_4(
        "hello", "hello",
        "%s%s %s%s", "hello", ",", "world", "!");

    RX_STR_CHECK_EQUAL_NO_CASE_MSG_4(
        "hello", "world!",
        "%s%s %s%s", "hello", ",", "world", "!");

    RX_STR_CHECK_NOT_EQUAL_NO_CASE_MSG_4(
        "hello", "hello",
        "%s%s %s%s", "hello", ",", "world", "!");

    RX_PTR_CHECK_EQUAL_MSG_4(
        (void *)0x123, (void *)0x456,
        "%s%s %s%s", "hello", ",", "world", "!");

    RX_PTR_CHECK_NOT_EQUAL_MSG_4(
        (void *)0x123, (void *)0x123,
        "%s%s %s%s", "hello", ",", "world", "!");

    RX_PTR_CHECK_ALIGNED_MSG_4(
        (void *)0x100, 100,
        "%s%s %s%s", "hello", ",", "world", "!");
}

RX_TEST_CASE(my_test_suite, require_failure_msg_5)
{
    RX_REQUIRE_MSG_5(
        123 == 456,
        "%s%s %s%s%s", "hello", ",", "worl", "d", "!");
}

RX_TEST_CASE(my_test_suite, bool_require_true_failure_msg_5)
{
    RX_BOOL_REQUIRE_TRUE_MSG_5(
        123 == 456,
        "%s%s %s%s%s", "hello", ",", "worl", "d", "!");
}

RX_TEST_CASE(my_test_suite, bool_require_false_failure_msg_5)
{
    RX_BOOL_REQUIRE_FALSE_MSG_5(
        123 == 123,
        "%s%s %s%s%s", "hello", ",", "worl", "d", "!");
}

RX_TEST_CASE(my_test_suite, int_require_equal_failure_msg_5)
{
    RX_INT_REQUIRE_EQUAL_MSG_5(
        123, 456,
        "%s%s %s%s%s", "hello", ",", "worl", "d", "!");
}

RX_TEST_CASE(my_test_suite, int_require_not_equal_failure_msg_5)
{
    RX_INT_REQUIRE_NOT_EQUAL_MSG_5(
        123, 123,
        "%s%s %s%s%s", "hello", ",", "worl", "d", "!");
}

RX_TEST_CASE(my_test_suite, int_require_greater_failure_msg_5)
{
    RX_INT_REQUIRE_GREATER_MSG_5(
        123, 456,
        "%s%s %s%s%s", "hello", ",", "worl", "d", "!");
}

RX_TEST_CASE(my_test_suite, int_require_lesser_failure_msg_5)
{
    RX_INT_REQUIRE_LESSER_MSG_5(
        456, 123,
        "%s%s %s%s%s", "hello", ",", "worl", "d", "!");
}

RX_TEST_CASE(my_test_suite, int_require_greater_or_equal_failure_msg_5)
{
    RX_INT_REQUIRE_GREATER_OR_EQUAL_MSG_5(
        123, 456,
        "%s%s %s%s%s", "hello", ",", "worl", "d", "!");
}

RX_TEST_CASE(my_test_suite, int_require_lesser_or_equal_failure_msg_5)
{
    RX_INT_REQUIRE_LESSER_OR_EQUAL_MSG_5(
        456, 123,
        "%s%s %s%s%s", "hello", ",", "worl", "d", "!");
}

RX_TEST_CASE(my_test_suite, uint_require_equal_failure_msg_5)
{
    RX_UINT_REQUIRE_EQUAL_MSG_5(
        123, 456,
        "%s%s %s%s%s", "hello", ",", "worl", "d", "!");
}

RX_TEST_CASE(my_test_suite, uint_require_not_equal_failure_msg_5)
{
    RX_UINT_REQUIRE_NOT_EQUAL_MSG_5(
        123, 123,
        "%s%s %s%s%s", "hello", ",", "worl", "d", "!");
}

RX_TEST_CASE(my_test_suite, uint_require_greater_failure_msg_5)
{
    RX_UINT_REQUIRE_GREATER_MSG_5(
        123, 456,
        "%s%s %s%s%s", "hello", ",", "worl", "d", "!");
}

RX_TEST_CASE(my_test_suite, uint_require_lesser_failure_msg_5)
{
    RX_UINT_REQUIRE_LESSER_MSG_5(
        456, 123,
        "%s%s %s%s%s", "hello", ",", "worl", "d", "!");
}

RX_TEST_CASE(my_test_suite, uint_require_greater_or_equal_failure_msg_5)
{
    RX_UINT_REQUIRE_GREATER_OR_EQUAL_MSG_5(
        123, 456,
        "%s%s %s%s%s", "hello", ",", "worl", "d", "!");
}

RX_TEST_CASE(my_test_suite, uint_require_lesser_or_equal_failure_msg_5)
{
    RX_UINT_REQUIRE_LESSER_OR_EQUAL_MSG_5(
        456, 123,
        "%s%s %s%s%s", "hello", ",", "worl", "d", "!");
}

RX_TEST_CASE(my_test_suite, real_require_equal_failure_msg_5)
{
    RX_REAL_REQUIRE_EQUAL_MSG_5(
        1.23, 4.56,
        "%s%s %s%s%s", "hello", ",", "worl", "d", "!");
}

RX_TEST_CASE(my_test_suite, real_require_not_equal_failure_msg_5)
{
    RX_REAL_REQUIRE_NOT_EQUAL_MSG_5(
        1.23, 1.23,
        "%s%s %s%s%s", "hello", ",", "worl", "d", "!");
}

RX_TEST_CASE(my_test_suite, real_require_greater_failure_msg_5)
{
    RX_REAL_REQUIRE_GREATER_MSG_5(
        1.23, 4.56,
        "%s%s %s%s%s", "hello", ",", "worl", "d", "!");
}

RX_TEST_CASE(my_test_suite, real_require_lesser_failure_msg_5)
{
    RX_REAL_REQUIRE_LESSER_MSG_5(
        4.56, 1.23,
        "%s%s %s%s%s", "hello", ",", "worl", "d", "!");
}

RX_TEST_CASE(my_test_suite, real_require_greater_or_equal_failure_msg_5)
{
    RX_REAL_REQUIRE_GREATER_OR_EQUAL_MSG_5(
        1.23, 4.56,
        "%s%s %s%s%s", "hello", ",", "worl", "d", "!");
}

RX_TEST_CASE(my_test_suite, real_require_lesser_or_equal_failure_msg_5)
{
    RX_REAL_REQUIRE_LESSER_OR_EQUAL_MSG_5(
        4.56, 1.23,
        "%s%s %s%s%s", "hello", ",", "worl", "d", "!");
}

RX_TEST_CASE(my_test_suite, real_require_fuzzy_equal_failure_msg_5)
{
    RX_REAL_REQUIRE_FUZZY_EQUAL_MSG_5(
        1.23, 4.56, 0.001,
        "%s%s %s%s%s", "hello", ",", "worl", "d", "!");
}

RX_TEST_CASE(my_test_suite, real_require_fuzzy_not_equal_failure_msg_5)
{
    RX_REAL_REQUIRE_FUZZY_NOT_EQUAL_MSG_5(
        1.23, 1.23, 0.001,
        "%s%s %s%s%s", "hello", ",", "worl", "d", "!");
}

RX_TEST_CASE(my_test_suite, str_require_equal_failure_msg_5)
{
    RX_STR_REQUIRE_EQUAL_MSG_5(
        "hello", "world!",
        "%s%s %s%s%s", "hello", ",", "worl", "d", "!");
}

RX_TEST_CASE(my_test_suite, str_require_not_equal_failure_msg_5)
{
    RX_STR_REQUIRE_NOT_EQUAL_MSG_5(
        "hello", "hello",
        "%s%s %s%s%s", "hello", ",", "worl", "d", "!");
}

RX_TEST_CASE(my_test_suite, str_require_equal_no_case_failure_msg_5)
{
    RX_STR_REQUIRE_EQUAL_NO_CASE_MSG_5(
        "hello", "world!",
        "%s%s %s%s%s", "hello", ",", "worl", "d", "!");
}

RX_TEST_CASE(my_test_suite, str_require_not_equal_no_case_failure_msg_5)
{
    RX_STR_REQUIRE_NOT_EQUAL_NO_CASE_MSG_5(
        "hello", "hello",
        "%s%s %s%s%s", "hello", ",", "worl", "d", "!");
}

RX_TEST_CASE(my_test_suite, ptr_require_equal_failure_msg_5)
{
    RX_PTR_REQUIRE_EQUAL_MSG_5(
        (void *)0x123, (void *)0x456,
        "%s%s %s%s%s", "hello", ",", "worl", "d", "!");
}

RX_TEST_CASE(my_test_suite, ptr_require_not_equal_failure_msg_5)
{
    RX_PTR_REQUIRE_NOT_EQUAL_MSG_5(
        (void *)0x123, (void *)0x123,
        "%s%s %s%s%s", "hello", ",", "worl", "d", "!");
}

RX_TEST_CASE(my_test_suite, ptr_require_aligned_failure_msg_5)
{
    RX_PTR_REQUIRE_ALIGNED_MSG_5(
        (void *)0x100, 100,
        "%s%s %s%s%s", "hello", ",", "worl", "d", "!");
}

RX_TEST_CASE(my_test_suite, all_check_failures_msg_5)
{
    RX_CHECK_MSG_5(
        123 == 456,
        "%s%s %s%s%s", "hello", ",", "worl", "d", "!");

    RX_BOOL_CHECK_TRUE_MSG_5(
        123 == 456,
        "%s%s %s%s%s", "hello", ",", "worl", "d", "!");

    RX_BOOL_CHECK_FALSE_MSG_5(
        123 == 123,
        "%s%s %s%s%s", "hello", ",", "worl", "d", "!");

    RX_INT_CHECK_EQUAL_MSG_5(
        123, 456,
        "%s%s %s%s%s", "hello", ",", "worl", "d", "!");

    RX_INT_CHECK_NOT_EQUAL_MSG_5(
        123, 123,
        "%s%s %s%s%s", "hello", ",", "worl", "d", "!");

    RX_INT_CHECK_GREATER_MSG_5(
        123, 456,
        "%s%s %s%s%s", "hello", ",", "worl", "d", "!");

    RX_INT_CHECK_LESSER_MSG_5(
        456, 123,
        "%s%s %s%s%s", "hello", ",", "worl", "d", "!");

    RX_INT_CHECK_GREATER_OR_EQUAL_MSG_5(
        123, 456,
        "%s%s %s%s%s", "hello", ",", "worl", "d", "!");

    RX_INT_CHECK_LESSER_OR_EQUAL_MSG_5(
        456, 123,
        "%s%s %s%s%s", "hello", ",", "worl", "d", "!");

    RX_UINT_CHECK_EQUAL_MSG_5(
        123, 456,
        "%s%s %s%s%s", "hello", ",", "worl", "d", "!");

    RX_UINT_CHECK_NOT_EQUAL_MSG_5(
        123, 123,
        "%s%s %s%s%s", "hello", ",", "worl", "d", "!");

    RX_UINT_CHECK_GREATER_MSG_5(
        123, 456,
        "%s%s %s%s%s", "hello", ",", "worl", "d", "!");

    RX_UINT_CHECK_LESSER_MSG_5(
        456, 123,
        "%s%s %s%s%s", "hello", ",", "worl", "d", "!");

    RX_UINT_CHECK_GREATER_OR_EQUAL_MSG_5(
        123, 456,
        "%s%s %s%s%s", "hello", ",", "worl", "d", "!");

    RX_UINT_CHECK_LESSER_OR_EQUAL_MSG_5(
        456, 123,
        "%s%s %s%s%s", "hello", ",", "worl", "d", "!");

    RX_REAL_CHECK_EQUAL_MSG_5(
        1.23, 4.56,
        "%s%s %s%s%s", "hello", ",", "worl", "d", "!");

    RX_REAL_CHECK_NOT_EQUAL_MSG_5(
        1.23, 1.23,
        "%s%s %s%s%s", "hello", ",", "worl", "d", "!");

    RX_REAL_CHECK_GREATER_MSG_5(
        1.23, 4.56,
        "%s%s %s%s%s", "hello", ",", "worl", "d", "!");

    RX_REAL_CHECK_LESSER_MSG_5(
        4.56, 1.23,
        "%s%s %s%s%s", "hello", ",", "worl", "d", "!");

    RX_REAL_CHECK_GREATER_OR_EQUAL_MSG_5(
        1.23, 4.56,
        "%s%s %s%s%s", "hello", ",", "worl", "d", "!");

    RX_REAL_CHECK_LESSER_OR_EQUAL_MSG_5(
        4.56, 1.23,
        "%s%s %s%s%s", "hello", ",", "worl", "d", "!");

    RX_REAL_CHECK_FUZZY_EQUAL_MSG_5(
        1.23, 4.56, 0.001,
        "%s%s %s%s%s", "hello", ",", "worl", "d", "!");

    RX_REAL_CHECK_FUZZY_NOT_EQUAL_MSG_5(
        1.23, 1.23, 0.001,
        "%s%s %s%s%s", "hello", ",", "worl", "d", "!");

    RX_STR_CHECK_EQUAL_MSG_5(
        "hello", "world!",
        "%s%s %s%s%s", "hello", ",", "worl", "d", "!");

    RX_STR_CHECK_NOT_EQUAL_MSG_5(
        "hello", "hello",
        "%s%s %s%s%s", "hello", ",", "worl", "d", "!");

    RX_STR_CHECK_EQUAL_NO_CASE_MSG_5(
        "hello", "world!",
        "%s%s %s%s%s", "hello", ",", "worl", "d", "!");

    RX_STR_CHECK_NOT_EQUAL_NO_CASE_MSG_5(
        "hello", "hello",
        "%s%s %s%s%s", "hello", ",", "worl", "d", "!");

    RX_PTR_CHECK_EQUAL_MSG_5(
        (void *)0x123, (void *)0x456,
        "%s%s %s%s%s", "hello", ",", "worl", "d", "!");

    RX_PTR_CHECK_NOT_EQUAL_MSG_5(
        (void *)0x123, (void *)0x123,
        "%s%s %s%s%s", "hello", ",", "worl", "d", "!");

    RX_PTR_CHECK_ALIGNED_MSG_5(
        (void *)0x100, 100,
        "%s%s %s%s%s", "hello", ",", "worl", "d", "!");
}

RX_TEST_CASE(my_test_suite, require_failure_msg_6)
{
    RX_REQUIRE_MSG_6(
        123 == 456,
        "%s%s %s%s%s%s", "hello", ",", "wor", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, bool_require_true_failure_msg_6)
{
    RX_BOOL_REQUIRE_TRUE_MSG_6(
        123 == 456,
        "%s%s %s%s%s%s", "hello", ",", "wor", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, bool_require_false_failure_msg_6)
{
    RX_BOOL_REQUIRE_FALSE_MSG_6(
        123 == 123,
        "%s%s %s%s%s%s", "hello", ",", "wor", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, int_require_equal_failure_msg_6)
{
    RX_INT_REQUIRE_EQUAL_MSG_6(
        123, 456,
        "%s%s %s%s%s%s", "hello", ",", "wor", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, int_require_not_equal_failure_msg_6)
{
    RX_INT_REQUIRE_NOT_EQUAL_MSG_6(
        123, 123,
        "%s%s %s%s%s%s", "hello", ",", "wor", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, int_require_greater_failure_msg_6)
{
    RX_INT_REQUIRE_GREATER_MSG_6(
        123, 456,
        "%s%s %s%s%s%s", "hello", ",", "wor", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, int_require_lesser_failure_msg_6)
{
    RX_INT_REQUIRE_LESSER_MSG_6(
        456, 123,
        "%s%s %s%s%s%s", "hello", ",", "wor", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, int_require_greater_or_equal_failure_msg_6)
{
    RX_INT_REQUIRE_GREATER_OR_EQUAL_MSG_6(
        123, 456,
        "%s%s %s%s%s%s", "hello", ",", "wor", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, int_require_lesser_or_equal_failure_msg_6)
{
    RX_INT_REQUIRE_LESSER_OR_EQUAL_MSG_6(
        456, 123,
        "%s%s %s%s%s%s", "hello", ",", "wor", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, uint_require_equal_failure_msg_6)
{
    RX_UINT_REQUIRE_EQUAL_MSG_6(
        123, 456,
        "%s%s %s%s%s%s", "hello", ",", "wor", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, uint_require_not_equal_failure_msg_6)
{
    RX_UINT_REQUIRE_NOT_EQUAL_MSG_6(
        123, 123,
        "%s%s %s%s%s%s", "hello", ",", "wor", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, uint_require_greater_failure_msg_6)
{
    RX_UINT_REQUIRE_GREATER_MSG_6(
        123, 456,
        "%s%s %s%s%s%s", "hello", ",", "wor", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, uint_require_lesser_failure_msg_6)
{
    RX_UINT_REQUIRE_LESSER_MSG_6(
        456, 123,
        "%s%s %s%s%s%s", "hello", ",", "wor", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, uint_require_greater_or_equal_failure_msg_6)
{
    RX_UINT_REQUIRE_GREATER_OR_EQUAL_MSG_6(
        123, 456,
        "%s%s %s%s%s%s", "hello", ",", "wor", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, uint_require_lesser_or_equal_failure_msg_6)
{
    RX_UINT_REQUIRE_LESSER_OR_EQUAL_MSG_6(
        456, 123,
        "%s%s %s%s%s%s", "hello", ",", "wor", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, real_require_equal_failure_msg_6)
{
    RX_REAL_REQUIRE_EQUAL_MSG_6(
        1.23, 4.56,
        "%s%s %s%s%s%s", "hello", ",", "wor", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, real_require_not_equal_failure_msg_6)
{
    RX_REAL_REQUIRE_NOT_EQUAL_MSG_6(
        1.23, 1.23,
        "%s%s %s%s%s%s", "hello", ",", "wor", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, real_require_greater_failure_msg_6)
{
    RX_REAL_REQUIRE_GREATER_MSG_6(
        1.23, 4.56,
        "%s%s %s%s%s%s", "hello", ",", "wor", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, real_require_lesser_failure_msg_6)
{
    RX_REAL_REQUIRE_LESSER_MSG_6(
        4.56, 1.23,
        "%s%s %s%s%s%s", "hello", ",", "wor", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, real_require_greater_or_equal_failure_msg_6)
{
    RX_REAL_REQUIRE_GREATER_OR_EQUAL_MSG_6(
        1.23, 4.56,
        "%s%s %s%s%s%s", "hello", ",", "wor", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, real_require_lesser_or_equal_failure_msg_6)
{
    RX_REAL_REQUIRE_LESSER_OR_EQUAL_MSG_6(
        4.56, 1.23,
        "%s%s %s%s%s%s", "hello", ",", "wor", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, real_require_fuzzy_equal_failure_msg_6)
{
    RX_REAL_REQUIRE_FUZZY_EQUAL_MSG_6(
        1.23, 4.56, 0.001,
        "%s%s %s%s%s%s", "hello", ",", "wor", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, real_require_fuzzy_not_equal_failure_msg_6)
{
    RX_REAL_REQUIRE_FUZZY_NOT_EQUAL_MSG_6(
        1.23, 1.23, 0.001,
        "%s%s %s%s%s%s", "hello", ",", "wor", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, str_require_equal_failure_msg_6)
{
    RX_STR_REQUIRE_EQUAL_MSG_6(
        "hello", "world!",
        "%s%s %s%s%s%s", "hello", ",", "wor", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, str_require_not_equal_failure_msg_6)
{
    RX_STR_REQUIRE_NOT_EQUAL_MSG_6(
        "hello", "hello",
        "%s%s %s%s%s%s", "hello", ",", "wor", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, str_require_equal_no_case_failure_msg_6)
{
    RX_STR_REQUIRE_EQUAL_NO_CASE_MSG_6(
        "hello", "world!",
        "%s%s %s%s%s%s", "hello", ",", "wor", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, str_require_not_equal_no_case_failure_msg_6)
{
    RX_STR_REQUIRE_NOT_EQUAL_NO_CASE_MSG_6(
        "hello", "hello",
        "%s%s %s%s%s%s", "hello", ",", "wor", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, ptr_require_equal_failure_msg_6)
{
    RX_PTR_REQUIRE_EQUAL_MSG_6(
        (void *)0x123, (void *)0x456,
        "%s%s %s%s%s%s", "hello", ",", "wor", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, ptr_require_not_equal_failure_msg_6)
{
    RX_PTR_REQUIRE_NOT_EQUAL_MSG_6(
        (void *)0x123, (void *)0x123,
        "%s%s %s%s%s%s", "hello", ",", "wor", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, ptr_require_aligned_failure_msg_6)
{
    RX_PTR_REQUIRE_ALIGNED_MSG_6(
        (void *)0x100, 100,
        "%s%s %s%s%s%s", "hello", ",", "wor", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, all_check_failures_msg_6)
{
    RX_CHECK_MSG_6(
        123 == 456,
        "%s%s %s%s%s%s", "hello", ",", "wor", "l", "d", "!");

    RX_BOOL_CHECK_TRUE_MSG_6(
        123 == 456,
        "%s%s %s%s%s%s", "hello", ",", "wor", "l", "d", "!");

    RX_BOOL_CHECK_FALSE_MSG_6(
        123 == 123,
        "%s%s %s%s%s%s", "hello", ",", "wor", "l", "d", "!");

    RX_INT_CHECK_EQUAL_MSG_6(
        123, 456,
        "%s%s %s%s%s%s", "hello", ",", "wor", "l", "d", "!");

    RX_INT_CHECK_NOT_EQUAL_MSG_6(
        123, 123,
        "%s%s %s%s%s%s", "hello", ",", "wor", "l", "d", "!");

    RX_INT_CHECK_GREATER_MSG_6(
        123, 456,
        "%s%s %s%s%s%s", "hello", ",", "wor", "l", "d", "!");

    RX_INT_CHECK_LESSER_MSG_6(
        456, 123,
        "%s%s %s%s%s%s", "hello", ",", "wor", "l", "d", "!");

    RX_INT_CHECK_GREATER_OR_EQUAL_MSG_6(
        123, 456,
        "%s%s %s%s%s%s", "hello", ",", "wor", "l", "d", "!");

    RX_INT_CHECK_LESSER_OR_EQUAL_MSG_6(
        456, 123,
        "%s%s %s%s%s%s", "hello", ",", "wor", "l", "d", "!");

    RX_UINT_CHECK_EQUAL_MSG_6(
        123, 456,
        "%s%s %s%s%s%s", "hello", ",", "wor", "l", "d", "!");

    RX_UINT_CHECK_NOT_EQUAL_MSG_6(
        123, 123,
        "%s%s %s%s%s%s", "hello", ",", "wor", "l", "d", "!");

    RX_UINT_CHECK_GREATER_MSG_6(
        123, 456,
        "%s%s %s%s%s%s", "hello", ",", "wor", "l", "d", "!");

    RX_UINT_CHECK_LESSER_MSG_6(
        456, 123,
        "%s%s %s%s%s%s", "hello", ",", "wor", "l", "d", "!");

    RX_UINT_CHECK_GREATER_OR_EQUAL_MSG_6(
        123, 456,
        "%s%s %s%s%s%s", "hello", ",", "wor", "l", "d", "!");

    RX_UINT_CHECK_LESSER_OR_EQUAL_MSG_6(
        456, 123,
        "%s%s %s%s%s%s", "hello", ",", "wor", "l", "d", "!");

    RX_REAL_CHECK_EQUAL_MSG_6(
        1.23, 4.56,
        "%s%s %s%s%s%s", "hello", ",", "wor", "l", "d", "!");

    RX_REAL_CHECK_NOT_EQUAL_MSG_6(
        1.23, 1.23,
        "%s%s %s%s%s%s", "hello", ",", "wor", "l", "d", "!");

    RX_REAL_CHECK_GREATER_MSG_6(
        1.23, 4.56,
        "%s%s %s%s%s%s", "hello", ",", "wor", "l", "d", "!");

    RX_REAL_CHECK_LESSER_MSG_6(
        4.56, 1.23,
        "%s%s %s%s%s%s", "hello", ",", "wor", "l", "d", "!");

    RX_REAL_CHECK_GREATER_OR_EQUAL_MSG_6(
        1.23, 4.56,
        "%s%s %s%s%s%s", "hello", ",", "wor", "l", "d", "!");

    RX_REAL_CHECK_LESSER_OR_EQUAL_MSG_6(
        4.56, 1.23,
        "%s%s %s%s%s%s", "hello", ",", "wor", "l", "d", "!");

    RX_REAL_CHECK_FUZZY_EQUAL_MSG_6(
        1.23, 4.56, 0.001,
        "%s%s %s%s%s%s", "hello", ",", "wor", "l", "d", "!");

    RX_REAL_CHECK_FUZZY_NOT_EQUAL_MSG_6(
        1.23, 1.23, 0.001,
        "%s%s %s%s%s%s", "hello", ",", "wor", "l", "d", "!");

    RX_STR_CHECK_EQUAL_MSG_6(
        "hello", "world!",
        "%s%s %s%s%s%s", "hello", ",", "wor", "l", "d", "!");

    RX_STR_CHECK_NOT_EQUAL_MSG_6(
        "hello", "hello",
        "%s%s %s%s%s%s", "hello", ",", "wor", "l", "d", "!");

    RX_STR_CHECK_EQUAL_NO_CASE_MSG_6(
        "hello", "world!",
        "%s%s %s%s%s%s", "hello", ",", "wor", "l", "d", "!");

    RX_STR_CHECK_NOT_EQUAL_NO_CASE_MSG_6(
        "hello", "hello",
        "%s%s %s%s%s%s", "hello", ",", "wor", "l", "d", "!");

    RX_PTR_CHECK_EQUAL_MSG_6(
        (void *)0x123, (void *)0x456,
        "%s%s %s%s%s%s", "hello", ",", "wor", "l", "d", "!");

    RX_PTR_CHECK_NOT_EQUAL_MSG_6(
        (void *)0x123, (void *)0x123,
        "%s%s %s%s%s%s", "hello", ",", "wor", "l", "d", "!");

    RX_PTR_CHECK_ALIGNED_MSG_6(
        (void *)0x100, 100,
        "%s%s %s%s%s%s", "hello", ",", "wor", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, require_failure_msg_7)
{
    RX_REQUIRE_MSG_7(
        123 == 456,
        "%s%s %s%s%s%s%s", "hello", ",", "wo", "r", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, bool_require_true_failure_msg_7)
{
    RX_BOOL_REQUIRE_TRUE_MSG_7(
        123 == 456,
        "%s%s %s%s%s%s%s", "hello", ",", "wo", "r", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, bool_require_false_failure_msg_7)
{
    RX_BOOL_REQUIRE_FALSE_MSG_7(
        123 == 123,
        "%s%s %s%s%s%s%s", "hello", ",", "wo", "r", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, int_require_equal_failure_msg_7)
{
    RX_INT_REQUIRE_EQUAL_MSG_7(
        123, 456,
        "%s%s %s%s%s%s%s", "hello", ",", "wo", "r", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, int_require_not_equal_failure_msg_7)
{
    RX_INT_REQUIRE_NOT_EQUAL_MSG_7(
        123, 123,
        "%s%s %s%s%s%s%s", "hello", ",", "wo", "r", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, int_require_greater_failure_msg_7)
{
    RX_INT_REQUIRE_GREATER_MSG_7(
        123, 456,
        "%s%s %s%s%s%s%s", "hello", ",", "wo", "r", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, int_require_lesser_failure_msg_7)
{
    RX_INT_REQUIRE_LESSER_MSG_7(
        456, 123,
        "%s%s %s%s%s%s%s", "hello", ",", "wo", "r", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, int_require_greater_or_equal_failure_msg_7)
{
    RX_INT_REQUIRE_GREATER_OR_EQUAL_MSG_7(
        123, 456,
        "%s%s %s%s%s%s%s", "hello", ",", "wo", "r", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, int_require_lesser_or_equal_failure_msg_7)
{
    RX_INT_REQUIRE_LESSER_OR_EQUAL_MSG_7(
        456, 123,
        "%s%s %s%s%s%s%s", "hello", ",", "wo", "r", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, uint_require_equal_failure_msg_7)
{
    RX_UINT_REQUIRE_EQUAL_MSG_7(
        123, 456,
        "%s%s %s%s%s%s%s", "hello", ",", "wo", "r", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, uint_require_not_equal_failure_msg_7)
{
    RX_UINT_REQUIRE_NOT_EQUAL_MSG_7(
        123, 123,
        "%s%s %s%s%s%s%s", "hello", ",", "wo", "r", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, uint_require_greater_failure_msg_7)
{
    RX_UINT_REQUIRE_GREATER_MSG_7(
        123, 456,
        "%s%s %s%s%s%s%s", "hello", ",", "wo", "r", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, uint_require_lesser_failure_msg_7)
{
    RX_UINT_REQUIRE_LESSER_MSG_7(
        456, 123,
        "%s%s %s%s%s%s%s", "hello", ",", "wo", "r", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, uint_require_greater_or_equal_failure_msg_7)
{
    RX_UINT_REQUIRE_GREATER_OR_EQUAL_MSG_7(
        123, 456,
        "%s%s %s%s%s%s%s", "hello", ",", "wo", "r", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, uint_require_lesser_or_equal_failure_msg_7)
{
    RX_UINT_REQUIRE_LESSER_OR_EQUAL_MSG_7(
        456, 123,
        "%s%s %s%s%s%s%s", "hello", ",", "wo", "r", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, real_require_equal_failure_msg_7)
{
    RX_REAL_REQUIRE_EQUAL_MSG_7(
        1.23, 4.56,
        "%s%s %s%s%s%s%s", "hello", ",", "wo", "r", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, real_require_not_equal_failure_msg_7)
{
    RX_REAL_REQUIRE_NOT_EQUAL_MSG_7(
        1.23, 1.23,
        "%s%s %s%s%s%s%s", "hello", ",", "wo", "r", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, real_require_greater_failure_msg_7)
{
    RX_REAL_REQUIRE_GREATER_MSG_7(
        1.23, 4.56,
        "%s%s %s%s%s%s%s", "hello", ",", "wo", "r", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, real_require_lesser_failure_msg_7)
{
    RX_REAL_REQUIRE_LESSER_MSG_7(
        4.56, 1.23,
        "%s%s %s%s%s%s%s", "hello", ",", "wo", "r", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, real_require_greater_or_equal_failure_msg_7)
{
    RX_REAL_REQUIRE_GREATER_OR_EQUAL_MSG_7(
        1.23, 4.56,
        "%s%s %s%s%s%s%s", "hello", ",", "wo", "r", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, real_require_lesser_or_equal_failure_msg_7)
{
    RX_REAL_REQUIRE_LESSER_OR_EQUAL_MSG_7(
        4.56, 1.23,
        "%s%s %s%s%s%s%s", "hello", ",", "wo", "r", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, real_require_fuzzy_equal_failure_msg_7)
{
    RX_REAL_REQUIRE_FUZZY_EQUAL_MSG_7(
        1.23, 4.56, 0.001,
        "%s%s %s%s%s%s%s", "hello", ",", "wo", "r", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, real_require_fuzzy_not_equal_failure_msg_7)
{
    RX_REAL_REQUIRE_FUZZY_NOT_EQUAL_MSG_7(
        1.23, 1.23, 0.001,
        "%s%s %s%s%s%s%s", "hello", ",", "wo", "r", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, str_require_equal_failure_msg_7)
{
    RX_STR_REQUIRE_EQUAL_MSG_7(
        "hello", "world!",
        "%s%s %s%s%s%s%s", "hello", ",", "wo", "r", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, str_require_not_equal_failure_msg_7)
{
    RX_STR_REQUIRE_NOT_EQUAL_MSG_7(
        "hello", "hello",
        "%s%s %s%s%s%s%s", "hello", ",", "wo", "r", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, str_require_equal_no_case_failure_msg_7)
{
    RX_STR_REQUIRE_EQUAL_NO_CASE_MSG_7(
        "hello", "world!",
        "%s%s %s%s%s%s%s", "hello", ",", "wo", "r", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, str_require_not_equal_no_case_failure_msg_7)
{
    RX_STR_REQUIRE_NOT_EQUAL_NO_CASE_MSG_7(
        "hello", "hello",
        "%s%s %s%s%s%s%s", "hello", ",", "wo", "r", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, ptr_require_equal_failure_msg_7)
{
    RX_PTR_REQUIRE_EQUAL_MSG_7(
        (void *)0x123, (void *)0x456,
        "%s%s %s%s%s%s%s", "hello", ",", "wo", "r", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, ptr_require_not_equal_failure_msg_7)
{
    RX_PTR_REQUIRE_NOT_EQUAL_MSG_7(
        (void *)0x123, (void *)0x123,
        "%s%s %s%s%s%s%s", "hello", ",", "wo", "r", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, ptr_require_aligned_failure_msg_7)
{
    RX_PTR_REQUIRE_ALIGNED_MSG_7(
        (void *)0x100, 100,
        "%s%s %s%s%s%s%s", "hello", ",", "wo", "r", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, all_check_failures_msg_7)
{
    RX_CHECK_MSG_7(
        123 == 456,
        "%s%s %s%s%s%s%s", "hello", ",", "wo", "r", "l", "d", "!");

    RX_BOOL_CHECK_TRUE_MSG_7(
        123 == 456,
        "%s%s %s%s%s%s%s", "hello", ",", "wo", "r", "l", "d", "!");

    RX_BOOL_CHECK_FALSE_MSG_7(
        123 == 123,
        "%s%s %s%s%s%s%s", "hello", ",", "wo", "r", "l", "d", "!");

    RX_INT_CHECK_EQUAL_MSG_7(
        123, 456,
        "%s%s %s%s%s%s%s", "hello", ",", "wo", "r", "l", "d", "!");

    RX_INT_CHECK_NOT_EQUAL_MSG_7(
        123, 123,
        "%s%s %s%s%s%s%s", "hello", ",", "wo", "r", "l", "d", "!");

    RX_INT_CHECK_GREATER_MSG_7(
        123, 456,
        "%s%s %s%s%s%s%s", "hello", ",", "wo", "r", "l", "d", "!");

    RX_INT_CHECK_LESSER_MSG_7(
        456, 123,
        "%s%s %s%s%s%s%s", "hello", ",", "wo", "r", "l", "d", "!");

    RX_INT_CHECK_GREATER_OR_EQUAL_MSG_7(
        123, 456,
        "%s%s %s%s%s%s%s", "hello", ",", "wo", "r", "l", "d", "!");

    RX_INT_CHECK_LESSER_OR_EQUAL_MSG_7(
        456, 123,
        "%s%s %s%s%s%s%s", "hello", ",", "wo", "r", "l", "d", "!");

    RX_UINT_CHECK_EQUAL_MSG_7(
        123, 456,
        "%s%s %s%s%s%s%s", "hello", ",", "wo", "r", "l", "d", "!");

    RX_UINT_CHECK_NOT_EQUAL_MSG_7(
        123, 123,
        "%s%s %s%s%s%s%s", "hello", ",", "wo", "r", "l", "d", "!");

    RX_UINT_CHECK_GREATER_MSG_7(
        123, 456,
        "%s%s %s%s%s%s%s", "hello", ",", "wo", "r", "l", "d", "!");

    RX_UINT_CHECK_LESSER_MSG_7(
        456, 123,
        "%s%s %s%s%s%s%s", "hello", ",", "wo", "r", "l", "d", "!");

    RX_UINT_CHECK_GREATER_OR_EQUAL_MSG_7(
        123, 456,
        "%s%s %s%s%s%s%s", "hello", ",", "wo", "r", "l", "d", "!");

    RX_UINT_CHECK_LESSER_OR_EQUAL_MSG_7(
        456, 123,
        "%s%s %s%s%s%s%s", "hello", ",", "wo", "r", "l", "d", "!");

    RX_REAL_CHECK_EQUAL_MSG_7(
        1.23, 4.56,
        "%s%s %s%s%s%s%s", "hello", ",", "wo", "r", "l", "d", "!");

    RX_REAL_CHECK_NOT_EQUAL_MSG_7(
        1.23, 1.23,
        "%s%s %s%s%s%s%s", "hello", ",", "wo", "r", "l", "d", "!");

    RX_REAL_CHECK_GREATER_MSG_7(
        1.23, 4.56,
        "%s%s %s%s%s%s%s", "hello", ",", "wo", "r", "l", "d", "!");

    RX_REAL_CHECK_LESSER_MSG_7(
        4.56, 1.23,
        "%s%s %s%s%s%s%s", "hello", ",", "wo", "r", "l", "d", "!");

    RX_REAL_CHECK_GREATER_OR_EQUAL_MSG_7(
        1.23, 4.56,
        "%s%s %s%s%s%s%s", "hello", ",", "wo", "r", "l", "d", "!");

    RX_REAL_CHECK_LESSER_OR_EQUAL_MSG_7(
        4.56, 1.23,
        "%s%s %s%s%s%s%s", "hello", ",", "wo", "r", "l", "d", "!");

    RX_REAL_CHECK_FUZZY_EQUAL_MSG_7(
        1.23, 4.56, 0.001,
        "%s%s %s%s%s%s%s", "hello", ",", "wo", "r", "l", "d", "!");

    RX_REAL_CHECK_FUZZY_NOT_EQUAL_MSG_7(
        1.23, 1.23, 0.001,
        "%s%s %s%s%s%s%s", "hello", ",", "wo", "r", "l", "d", "!");

    RX_STR_CHECK_EQUAL_MSG_7(
        "hello", "world!",
        "%s%s %s%s%s%s%s", "hello", ",", "wo", "r", "l", "d", "!");

    RX_STR_CHECK_NOT_EQUAL_MSG_7(
        "hello", "hello",
        "%s%s %s%s%s%s%s", "hello", ",", "wo", "r", "l", "d", "!");

    RX_STR_CHECK_EQUAL_NO_CASE_MSG_7(
        "hello", "world!",
        "%s%s %s%s%s%s%s", "hello", ",", "wo", "r", "l", "d", "!");

    RX_STR_CHECK_NOT_EQUAL_NO_CASE_MSG_7(
        "hello", "hello",
        "%s%s %s%s%s%s%s", "hello", ",", "wo", "r", "l", "d", "!");

    RX_PTR_CHECK_EQUAL_MSG_7(
        (void *)0x123, (void *)0x456,
        "%s%s %s%s%s%s%s", "hello", ",", "wo", "r", "l", "d", "!");

    RX_PTR_CHECK_NOT_EQUAL_MSG_7(
        (void *)0x123, (void *)0x123,
        "%s%s %s%s%s%s%s", "hello", ",", "wo", "r", "l", "d", "!");

    RX_PTR_CHECK_ALIGNED_MSG_7(
        (void *)0x100, 100,
        "%s%s %s%s%s%s%s", "hello", ",", "wo", "r", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, require_failure_msg_8)
{
    RX_REQUIRE_MSG_8(
        123 == 456,
        "%s%s %s%s%s%s%s%s", "hello", ",", "w", "o", "r", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, bool_require_true_failure_msg_8)
{
    RX_BOOL_REQUIRE_TRUE_MSG_8(
        123 == 456,
        "%s%s %s%s%s%s%s%s", "hello", ",", "w", "o", "r", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, bool_require_false_failure_msg_8)
{
    RX_BOOL_REQUIRE_FALSE_MSG_8(
        123 == 123,
        "%s%s %s%s%s%s%s%s", "hello", ",", "w", "o", "r", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, int_require_equal_failure_msg_8)
{
    RX_INT_REQUIRE_EQUAL_MSG_8(
        123, 456,
        "%s%s %s%s%s%s%s%s", "hello", ",", "w", "o", "r", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, int_require_not_equal_failure_msg_8)
{
    RX_INT_REQUIRE_NOT_EQUAL_MSG_8(
        123, 123,
        "%s%s %s%s%s%s%s%s", "hello", ",", "w", "o", "r", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, int_require_greater_failure_msg_8)
{
    RX_INT_REQUIRE_GREATER_MSG_8(
        123, 456,
        "%s%s %s%s%s%s%s%s", "hello", ",", "w", "o", "r", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, int_require_lesser_failure_msg_8)
{
    RX_INT_REQUIRE_LESSER_MSG_8(
        456, 123,
        "%s%s %s%s%s%s%s%s", "hello", ",", "w", "o", "r", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, int_require_greater_or_equal_failure_msg_8)
{
    RX_INT_REQUIRE_GREATER_OR_EQUAL_MSG_8(
        123, 456,
        "%s%s %s%s%s%s%s%s", "hello", ",", "w", "o", "r", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, int_require_lesser_or_equal_failure_msg_8)
{
    RX_INT_REQUIRE_LESSER_OR_EQUAL_MSG_8(
        456, 123,
        "%s%s %s%s%s%s%s%s", "hello", ",", "w", "o", "r", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, uint_require_equal_failure_msg_8)
{
    RX_UINT_REQUIRE_EQUAL_MSG_8(
        123, 456,
        "%s%s %s%s%s%s%s%s", "hello", ",", "w", "o", "r", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, uint_require_not_equal_failure_msg_8)
{
    RX_UINT_REQUIRE_NOT_EQUAL_MSG_8(
        123, 123,
        "%s%s %s%s%s%s%s%s", "hello", ",", "w", "o", "r", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, uint_require_greater_failure_msg_8)
{
    RX_UINT_REQUIRE_GREATER_MSG_8(
        123, 456,
        "%s%s %s%s%s%s%s%s", "hello", ",", "w", "o", "r", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, uint_require_lesser_failure_msg_8)
{
    RX_UINT_REQUIRE_LESSER_MSG_8(
        456, 123,
        "%s%s %s%s%s%s%s%s", "hello", ",", "w", "o", "r", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, uint_require_greater_or_equal_failure_msg_8)
{
    RX_UINT_REQUIRE_GREATER_OR_EQUAL_MSG_8(
        123, 456,
        "%s%s %s%s%s%s%s%s", "hello", ",", "w", "o", "r", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, uint_require_lesser_or_equal_failure_msg_8)
{
    RX_UINT_REQUIRE_LESSER_OR_EQUAL_MSG_8(
        456, 123,
        "%s%s %s%s%s%s%s%s", "hello", ",", "w", "o", "r", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, real_require_equal_failure_msg_8)
{
    RX_REAL_REQUIRE_EQUAL_MSG_8(
        1.23, 4.56,
        "%s%s %s%s%s%s%s%s", "hello", ",", "w", "o", "r", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, real_require_not_equal_failure_msg_8)
{
    RX_REAL_REQUIRE_NOT_EQUAL_MSG_8(
        1.23, 1.23,
        "%s%s %s%s%s%s%s%s", "hello", ",", "w", "o", "r", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, real_require_greater_failure_msg_8)
{
    RX_REAL_REQUIRE_GREATER_MSG_8(
        1.23, 4.56,
        "%s%s %s%s%s%s%s%s", "hello", ",", "w", "o", "r", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, real_require_lesser_failure_msg_8)
{
    RX_REAL_REQUIRE_LESSER_MSG_8(
        4.56, 1.23,
        "%s%s %s%s%s%s%s%s", "hello", ",", "w", "o", "r", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, real_require_greater_or_equal_failure_msg_8)
{
    RX_REAL_REQUIRE_GREATER_OR_EQUAL_MSG_8(
        1.23, 4.56,
        "%s%s %s%s%s%s%s%s", "hello", ",", "w", "o", "r", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, real_require_lesser_or_equal_failure_msg_8)
{
    RX_REAL_REQUIRE_LESSER_OR_EQUAL_MSG_8(
        4.56, 1.23,
        "%s%s %s%s%s%s%s%s", "hello", ",", "w", "o", "r", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, real_require_fuzzy_equal_failure_msg_8)
{
    RX_REAL_REQUIRE_FUZZY_EQUAL_MSG_8(
        1.23, 4.56, 0.001,
        "%s%s %s%s%s%s%s%s", "hello", ",", "w", "o", "r", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, real_require_fuzzy_not_equal_failure_msg_8)
{
    RX_REAL_REQUIRE_FUZZY_NOT_EQUAL_MSG_8(
        1.23, 1.23, 0.001,
        "%s%s %s%s%s%s%s%s", "hello", ",", "w", "o", "r", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, str_require_equal_failure_msg_8)
{
    RX_STR_REQUIRE_EQUAL_MSG_8(
        "hello", "world!",
        "%s%s %s%s%s%s%s%s", "hello", ",", "w", "o", "r", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, str_require_not_equal_failure_msg_8)
{
    RX_STR_REQUIRE_NOT_EQUAL_MSG_8(
        "hello", "hello",
        "%s%s %s%s%s%s%s%s", "hello", ",", "w", "o", "r", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, str_require_equal_no_case_failure_msg_8)
{
    RX_STR_REQUIRE_EQUAL_NO_CASE_MSG_8(
        "hello", "world!",
        "%s%s %s%s%s%s%s%s", "hello", ",", "w", "o", "r", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, str_require_not_equal_no_case_failure_msg_8)
{
    RX_STR_REQUIRE_NOT_EQUAL_NO_CASE_MSG_8(
        "hello", "hello",
        "%s%s %s%s%s%s%s%s", "hello", ",", "w", "o", "r", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, ptr_require_equal_failure_msg_8)
{
    RX_PTR_REQUIRE_EQUAL_MSG_8(
        (void *)0x123, (void *)0x456,
        "%s%s %s%s%s%s%s%s", "hello", ",", "w", "o", "r", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, ptr_require_not_equal_failure_msg_8)
{
    RX_PTR_REQUIRE_NOT_EQUAL_MSG_8(
        (void *)0x123, (void *)0x123,
        "%s%s %s%s%s%s%s%s", "hello", ",", "w", "o", "r", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, ptr_require_aligned_failure_msg_8)
{
    RX_PTR_REQUIRE_ALIGNED_MSG_8(
        (void *)0x100, 100,
        "%s%s %s%s%s%s%s%s", "hello", ",", "w", "o", "r", "l", "d", "!");
}

RX_TEST_CASE(my_test_suite, all_check_failures_msg_8)
{
    RX_CHECK_MSG_8(
        123 == 456,
        "%s%s %s%s%s%s%s%s", "hello", ",", "w", "o", "r", "l", "d", "!");

    RX_BOOL_CHECK_TRUE_MSG_8(
        123 == 456,
        "%s%s %s%s%s%s%s%s", "hello", ",", "w", "o", "r", "l", "d", "!");

    RX_BOOL_CHECK_FALSE_MSG_8(
        123 == 123,
        "%s%s %s%s%s%s%s%s", "hello", ",", "w", "o", "r", "l", "d", "!");

    RX_INT_CHECK_EQUAL_MSG_8(
        123, 456,
        "%s%s %s%s%s%s%s%s", "hello", ",", "w", "o", "r", "l", "d", "!");

    RX_INT_CHECK_NOT_EQUAL_MSG_8(
        123, 123,
        "%s%s %s%s%s%s%s%s", "hello", ",", "w", "o", "r", "l", "d", "!");

    RX_INT_CHECK_GREATER_MSG_8(
        123, 456,
        "%s%s %s%s%s%s%s%s", "hello", ",", "w", "o", "r", "l", "d", "!");

    RX_INT_CHECK_LESSER_MSG_8(
        456, 123,
        "%s%s %s%s%s%s%s%s", "hello", ",", "w", "o", "r", "l", "d", "!");

    RX_INT_CHECK_GREATER_OR_EQUAL_MSG_8(
        123, 456,
        "%s%s %s%s%s%s%s%s", "hello", ",", "w", "o", "r", "l", "d", "!");

    RX_INT_CHECK_LESSER_OR_EQUAL_MSG_8(
        456, 123,
        "%s%s %s%s%s%s%s%s", "hello", ",", "w", "o", "r", "l", "d", "!");

    RX_UINT_CHECK_EQUAL_MSG_8(
        123, 456,
        "%s%s %s%s%s%s%s%s", "hello", ",", "w", "o", "r", "l", "d", "!");

    RX_UINT_CHECK_NOT_EQUAL_MSG_8(
        123, 123,
        "%s%s %s%s%s%s%s%s", "hello", ",", "w", "o", "r", "l", "d", "!");

    RX_UINT_CHECK_GREATER_MSG_8(
        123, 456,
        "%s%s %s%s%s%s%s%s", "hello", ",", "w", "o", "r", "l", "d", "!");

    RX_UINT_CHECK_LESSER_MSG_8(
        456, 123,
        "%s%s %s%s%s%s%s%s", "hello", ",", "w", "o", "r", "l", "d", "!");

    RX_UINT_CHECK_GREATER_OR_EQUAL_MSG_8(
        123, 456,
        "%s%s %s%s%s%s%s%s", "hello", ",", "w", "o", "r", "l", "d", "!");

    RX_UINT_CHECK_LESSER_OR_EQUAL_MSG_8(
        456, 123,
        "%s%s %s%s%s%s%s%s", "hello", ",", "w", "o", "r", "l", "d", "!");

    RX_REAL_CHECK_EQUAL_MSG_8(
        1.23, 4.56,
        "%s%s %s%s%s%s%s%s", "hello", ",", "w", "o", "r", "l", "d", "!");

    RX_REAL_CHECK_NOT_EQUAL_MSG_8(
        1.23, 1.23,
        "%s%s %s%s%s%s%s%s", "hello", ",", "w", "o", "r", "l", "d", "!");

    RX_REAL_CHECK_GREATER_MSG_8(
        1.23, 4.56,
        "%s%s %s%s%s%s%s%s", "hello", ",", "w", "o", "r", "l", "d", "!");

    RX_REAL_CHECK_LESSER_MSG_8(
        4.56, 1.23,
        "%s%s %s%s%s%s%s%s", "hello", ",", "w", "o", "r", "l", "d", "!");

    RX_REAL_CHECK_GREATER_OR_EQUAL_MSG_8(
        1.23, 4.56,
        "%s%s %s%s%s%s%s%s", "hello", ",", "w", "o", "r", "l", "d", "!");

    RX_REAL_CHECK_LESSER_OR_EQUAL_MSG_8(
        4.56, 1.23,
        "%s%s %s%s%s%s%s%s", "hello", ",", "w", "o", "r", "l", "d", "!");

    RX_REAL_CHECK_FUZZY_EQUAL_MSG_8(
        1.23, 4.56, 0.001,
        "%s%s %s%s%s%s%s%s", "hello", ",", "w", "o", "r", "l", "d", "!");

    RX_REAL_CHECK_FUZZY_NOT_EQUAL_MSG_8(
        1.23, 1.23, 0.001,
        "%s%s %s%s%s%s%s%s", "hello", ",", "w", "o", "r", "l", "d", "!");

    RX_STR_CHECK_EQUAL_MSG_8(
        "hello", "world!",
        "%s%s %s%s%s%s%s%s", "hello", ",", "w", "o", "r", "l", "d", "!");

    RX_STR_CHECK_NOT_EQUAL_MSG_8(
        "hello", "hello",
        "%s%s %s%s%s%s%s%s", "hello", ",", "w", "o", "r", "l", "d", "!");

    RX_STR_CHECK_EQUAL_NO_CASE_MSG_8(
        "hello", "world!",
        "%s%s %s%s%s%s%s%s", "hello", ",", "w", "o", "r", "l", "d", "!");

    RX_STR_CHECK_NOT_EQUAL_NO_CASE_MSG_8(
        "hello", "hello",
        "%s%s %s%s%s%s%s%s", "hello", ",", "w", "o", "r", "l", "d", "!");

    RX_PTR_CHECK_EQUAL_MSG_8(
        (void *)0x123, (void *)0x456,
        "%s%s %s%s%s%s%s%s", "hello", ",", "w", "o", "r", "l", "d", "!");

    RX_PTR_CHECK_NOT_EQUAL_MSG_8(
        (void *)0x123, (void *)0x123,
        "%s%s %s%s%s%s%s%s", "hello", ",", "w", "o", "r", "l", "d", "!");

    RX_PTR_CHECK_ALIGNED_MSG_8(
        (void *)0x100, 100,
        "%s%s %s%s%s%s%s%s", "hello", ",", "w", "o", "r", "l", "d", "!");
}

int
main(int argc, const char **argv)
{
    size_t i;
    rx_size test_case_count;
    struct rx_test_case *test_cases;

    (void)argc;
    (void)argv;

    rx_enumerate_test_cases(&test_case_count, NULL);
    test_cases
        = (struct rx_test_case *)malloc(sizeof *test_cases * test_case_count);
    if (test_cases == NULL) {
        printf("failed to allocate the test cases\n");
        return 1;
    }

    rx_enumerate_test_cases(&test_case_count, test_cases);
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

        for (j = 0; j < summary.failure_count; ++j) {
            ASSERT(
                strcmp(summary.failures[j].msg,
                "hello, world!") == 0);
        }

        rx_summary_terminate(&summary);
    }

    free(test_cases);
    return 0;
}
