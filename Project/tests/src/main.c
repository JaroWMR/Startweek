#include <zephyr/ztest.h>
#include "threads.h"
#include <stdio.h>

void test_main(void)
{
	disableAllThreads();
	printf("Initialization\n");


    //  struct power_sequence_state state;

    //  /* Only suites that use a predicate checking for phase == PWR_PHASE_0 will run. */
    //  state.phase = PWR_PHASE_0;
    //  ztest_run_all(&state, false, 1, 1);

    //  /* Only suites that use a predicate checking for phase == PWR_PHASE_1 will run. */
    //  state.phase = PWR_PHASE_1;
    //  ztest_run_all(&state, false, 1, 1);

    //  /* Only suites that use a predicate checking for phase == PWR_PHASE_2 will run. */
    //  state.phase = PWR_PHASE_2;
    //  ztest_run_all(&state, false, 1, 1);

    //  /* Check that all the suites in this binary ran at least once. */
    //  ztest_verify_all_test_suites_ran();
}