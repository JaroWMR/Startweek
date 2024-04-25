#include "threads.h"

#include "buttonMatrix.h"
#include "buzzers.h"
#include "circleMatrix.h"
#include "genericGpio.h"
#include "gps.h"
#include "gyroCompass.h"
#include "lcd.h"
#include "ledMatrix.h"
#include "potmeter.h"
#include "sevenSegment.h"

#include <zephyr/kernel.h>
#include <zephyr/sys/util.h>

// Define the threads
// Input threads
K_THREAD_DEFINE(tstartbutton_id, STACKSIZE, tstartbutton, NULL, NULL, NULL, TSTARTBUTTON_PRIORITY, 0,0);
K_THREAD_DEFINE(tgyro_id, STACKSIZE, tgyro, NULL, NULL, NULL, TGYRO_PRIORITY, 0,0);
K_THREAD_DEFINE(tbtnmatrix_in_id, STACKSIZE, tbtnmatrix_in, NULL, NULL, NULL, TBTNMATRIX_IN_PRIORITY, 0, 0);
K_THREAD_DEFINE(tswitches_id, STACKSIZE, tswitches, NULL, NULL, NULL, TSWITCHES_PRIORITY, 0, 0);
K_THREAD_DEFINE(tpotmeter_id, STACKSIZE, tpotmeter, NULL, NULL, NULL, TPOTMETER_PRIORITY, 0, 0);
K_THREAD_DEFINE(tabcbtn_id, STACKSIZE, tabcbtn, NULL, NULL, NULL, TABCBTN_PRIORITY, 0, 0);
// Output threads
K_THREAD_DEFINE(tbtnmatrix_out_id, STACKSIZE, tbtnmatrix_out, NULL, NULL, NULL, TBTNMATRIX_OUT_PRIORITY, 0, 0);
K_THREAD_DEFINE(tbuzzers_id, STACKSIZE, tbuzzers, NULL, NULL, NULL, TBUZZERS_PRIORITY, 0, 0);
K_THREAD_DEFINE(tledmatrix_id, STACKSIZE, tledmatrix, NULL, NULL, NULL, TLEDMATRIX_PRIORITY, 0, 0);
K_THREAD_DEFINE(tledcircle_id, STACKSIZE, tledcircle, NULL, NULL, NULL, TLEDCIRCLE_PRIORITY, 0, 0);
K_THREAD_DEFINE(tsevenseg_id, STACKSIZE, tsevenseg, NULL, NULL, NULL, TSEVENSEG_PRIORITY, 0, 0);


void tstartbutton(void) {
	while (1) {
		printf("Call start button polling function here!\n");
	}
}

void tgyro(void) { 
	while (1) {
		printf("Call gyro polling function here!\n");
	}
}

void tbtnmatrix_in(void) { 
	while (1) {
		printf("Call button matrix polling function here!\n");
	}
}

void tswitches(void) { 
	while (1) {
		printf("Call switches polling function here!\n");
	}
}

void tpotmeter(void) { 
	while (1) {
		printf("Call potmeter polling function here!\n");
	}
}

void tabcbtn(void) { 
	while (1) {
		printf("Call abc button polling function here!\n");
	}
}

// Output thread functions
void tbtnmatrix_out(void) { 
	while (1) {
		printf("Call button matrix LED control function here!\n");
	}
}

void tbuzzers(void) { 
	while (1) {
		printf("Call buzzers control function here!\n");
	}
}

void tledmatrix(void) { 
	while (1) {
		printf("Call LED matrix control function here!\n");
	}
}

void tledcircle(void) { 
	while (1) {
		printf("Call LED circle control function here!\n");
	}
}

void tsevenseg(void) { 
	while (1) {
		printf("Call seven segment control function here!\n");
	}
}

void enableThreads(k_tid_t threadIds[THREAD_AMOUNT]) {
	for (int i = 0; i < THREAD_AMOUNT; i++) {
		// Resume each desired thread
		k_thread_resume(threadIds[i]);
	}
}

void disableThreads(k_tid_t threadIds[THREAD_AMOUNT]) {
	for (int i = 0; i < THREAD_AMOUNT; i++) {
		// Resume each desired thread
		k_thread_suspend(threadIds[i]);
	}
}