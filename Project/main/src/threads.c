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
		k_msleep(1);	// This delay should depend on how frequently this sensor / actuator is read / written
	}
}

void tbtnmatrix_in(void) { 
	while (1) {
		printf("Call button matrix polling function here!\n");
		k_msleep(1);	// This delay should depend on how frequently this sensor / actuator is read / written
	}
}

void tswitches(void) { 
	while (1) {
		printf("Call switches polling function here!\n");
		k_msleep(1);	// This delay should depend on how frequently this sensor / actuator is read / written
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
		k_msleep(1);	// This delay should depend on how frequently this sensor / actuator is read / written
	}
}

// Output thread functions
void tbtnmatrix_out(void) { 
	while (1) {
		printf("Call button matrix LED control function here!\n");
		k_msleep(1);	// This delay should depend on how frequently this sensor / actuator is read / written
	}
}

void tbuzzers(void) { 
	while (1) {
		printf("Call buzzers control function here!\n");
		k_msleep(1);	// This delay should depend on how frequently this sensor / actuator is read / written
	}
}

void tledmatrix(void) { 
	while (1) {
		printf("Call LED matrix control function here!\n");
		k_msleep(1);	// This delay should depend on how frequently this sensor / actuator is read / written
	}
}

void tledcircle(void) { 
	while (1) {
		printf("Call LED circle control function here!\n");
		k_msleep(1);	// This delay should depend on how frequently this sensor / actuator is read / written
	}
}

void tsevenseg(void) { 
	while (1) {
		printf("Call seven segment control function here!\n");
		k_msleep(1);	// This delay should depend on how frequently this sensor / actuator is read / written
	}
}

void enableThreads(char **names, unsigned amount) {
	for (int i = 0; i < amount; i++) {
		// Resume each desired thread
		printf("Enabling thread %s\n", names[i]);
		setThread(names[i], true);
	}
}

void disableThreads(char **names, unsigned amount) {
	for (int i = 0; i < amount; i++) {
		// Suspend each desired thread
		printf("Suspending thread %s\n", names[i]);
		setThread(names[i], false);
	}
}

void setThread(char* name, bool state) {
	if (strcmp(name, "startbtn") == 0) {
			extern const k_tid_t tstartbutton_id;
			state ? k_thread_resume(tstartbutton_id) : k_thread_suspend(tstartbutton_id);
	}
	if (strcmp(name, "btnmatrix_in") == 0) {
			extern const k_tid_t tbtnmatrix_in_id;
			state ? k_thread_resume(tbtnmatrix_in_id) : k_thread_suspend(tbtnmatrix_in_id);
	}
	if (strcmp(name, "switches") == 0) {
			extern const k_tid_t tswitches_id;
			state ? k_thread_resume(tswitches_id) : k_thread_suspend(tswitches_id);
	}
	if (strcmp(name, "potmeter") == 0) {
			extern const k_tid_t tpotmeter_id;
			state ? k_thread_resume(tpotmeter_id) : k_thread_suspend(tpotmeter_id);
	}
	if (strcmp(name, "abcbtn") == 0) {
			extern const k_tid_t tabcbtn_id;
			state ? k_thread_resume(tabcbtn_id) : k_thread_suspend(tabcbtn_id);
	}
	if (strcmp(name, "btnmatrix_out") == 0) {
			extern const k_tid_t tbtnmatrix_out_id;
			state ? k_thread_resume(tbtnmatrix_out_id) : k_thread_suspend(tbtnmatrix_out_id);
	}
	if (strcmp(name, "buzzers") == 0) {
			extern const k_tid_t tbuzzers_id;
			state ? k_thread_resume(tbuzzers_id) : k_thread_suspend(tbuzzers_id);
	}
	if (strcmp(name, "ledmatrix") == 0) {
			extern const k_tid_t tledmatrix_id;
			state ? k_thread_resume(tledmatrix_id) : k_thread_suspend(tledmatrix_id);
	}
	if (strcmp(name, "sevenseg") == 0) {
			extern const k_tid_t tsevenseg_id;
			state ? k_thread_resume(tsevenseg_id) : k_thread_suspend(tsevenseg_id);
	}
}

void disableAllThreads() {
	k_thread_suspend(tstartbutton_id);
	k_thread_suspend(tbtnmatrix_in_id);
	k_thread_suspend(tswitches_id);
	k_thread_suspend(tpotmeter_id);
	k_thread_suspend(tabcbtn_id);
	k_thread_suspend(tbtnmatrix_out_id);
	k_thread_suspend(tbuzzers_id);
	k_thread_suspend(tledmatrix_id);
	k_thread_suspend(tledcircle_id);
	k_thread_suspend(tsevenseg_id);
}