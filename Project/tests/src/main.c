/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/ztest.h>
#include "walking.h"


ZTEST_SUITE(framework_tests, NULL, NULL, NULL, NULL, NULL);

/**
 * @brief Test Asserts
 *
 * This test verifies various assert macros provided by ztest.
 *
 */
ZTEST(framework_tests, test_one_one)
{
	zassert_true(1, "1 was false");
	zassert_false(0, "0 was true");
	zassert_is_null(NULL, "NULL was not NULL");
	zassert_not_null("foo", "\"foo\" was NULL");
	zassert_equal(1, 1, "1 was not equal to 1");
	zassert_equal_ptr(NULL, NULL, "NULL was not equal to NULL");

	int a = myFunction();
	zassert_equal(a,0);
}

ZTEST(framework_tests, test_one_two)
{
	int a = myFunction();
	a += a;
	zassert_equal(a,0);
}

ZTEST_SUITE(test_suite_two, NULL, NULL, NULL, NULL, NULL);

ZTEST(test_suite_two, test_two_one)
{
	int a = myFunction();
	a += a;
	zassert_equal(a,0);
}

ZTEST(test_suite_two, test_two_two)
{
	int a = myFunction();
	a += a;
	zassert_equal(a,0);
}