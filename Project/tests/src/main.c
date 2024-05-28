#include <zephyr/ztest.h>
#include "threads.h"
#include <stdio.h>
#include "config.h"

void test_main(void)
{
	disableAllThreads();

	ztest_run_all(NULL, false, 1, 1);

	ztest_verify_all_test_suites_ran();
}