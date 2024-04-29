// Includes: own header file, hardware headers, minigame headers, framework headers, system headers (sorted alpabetically)
#include "statemachine.h"
#include "threads.h"

#include "idle.h"
#include "minigame1.h"
#include "minigame2.h"
#include "minigame3.h"
#include "minigame4.h"
#include "minigame5.h"
#include "minigame6.h"
#include "minigame7.h"
#include "minigame8.h"
#include "minigame9.h"
#include "minigame10.h"

#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/printk.h>

#include <stdio.h>

// Setup state machine
struct state;
typedef void state_fn(struct state *);

struct state {
	state_fn *next;
	int i;
};

state_fn init_state, idle_state, mg1_state, mg2_state, mg3_state, mg4_state, mg5_state, mg6_state, mg7_state, mg8_state, mg9_state, mg10_state, exit_state;

// State functions
void init_state(struct state *state) {
	printf("Initialization\n");
	uint8_t ret = 0;
	ret = configure();
	if(ret != 0)
	{
		while(true)
		{
			__asm__ volatile ("nop");
		}
	}
	initialize();
	
	state->next = idle_state;
}

void idle_state(struct state *state) {
	char **names;
	unsigned amount;
	getIdleThreads(&names, &amount);
	enableThreads(names, amount);
	printf("Walking\n");
	disableThreads(names, amount);

	state->next = mg1_state;
}

void mg1_state(struct state *state) { // Makes use of button and led
	// Initialise state, enable and disable corresponding threads
	printf("Minigame 1\n");

	// Enable required threads
	// k_thread_resume(tstartbutton_id);
	// k_thread_resume(tgyro_id);
	// k_thread_resume(tledmatrix_id);

	// int ret = playMg1();

	// // Disable required threads after finishing
	// k_thread_suspend(tstartbutton_id);
	// k_thread_suspend(tgyro_id);
	// k_thread_suspend(tledmatrix_id);

	state->next = mg2_state;
}

void mg2_state(struct state *state) { // Makes use of gyro and buzzer
	printf("Minigame 2\n");

	// Enable required threads
	// k_thread_resume(tstartbutton_id);
	// k_thread_resume(tbtnmatrix_in_id);
	// k_thread_resume(tbtnmatrix_out_id);
	// k_thread_resume(tbuzzers_id);

	// int ret = playMg2();

	// // Disable required threads after finishing
	// k_thread_suspend(tstartbutton_id);
	// k_thread_suspend(tbtnmatrix_in_id);
	// k_thread_suspend(tbtnmatrix_out_id);
	// k_thread_suspend(tbuzzers_id);

	state->next = mg3_state;
}

void mg3_state(struct state *state) { // Makes use of gyro and buzzer
	printf("Minigame 3\n");

	// // Enable required threads
	// k_thread_resume(tstartbutton_id);
	// k_thread_resume(tbtnmatrix_in_id);
	// k_thread_resume(tbtnmatrix_out_id);
	// k_thread_resume(tbuzzers_id);

	// int ret = playMg3();

	// // Disable required threads after finishing
	// k_thread_suspend(tstartbutton_id);
	// k_thread_suspend(tbtnmatrix_in_id);
	// k_thread_suspend(tbtnmatrix_out_id);
	// k_thread_suspend(tbuzzers_id);

	state->next = mg4_state;
}

void mg4_state(struct state *state) { // Makes use of gyro and buzzer
	printf("Minigame 4\n");

	// // Enable required threads
	// k_thread_resume(tstartbutton_id);
	// k_thread_resume(tbuzzers_id);
	// k_thread_resume(tabcbtn_id);

	// int ret = playMg4();

	// // Disable required threads after finishing
	// k_thread_suspend(tstartbutton_id);
	// k_thread_suspend(tbuzzers_id);
	// k_thread_suspend(tabcbtn_id);

	state->next = mg5_state;
}

void mg5_state(struct state *state) { // Makes use of gyro and buzzer
	printf("Minigame 5\n");

	// // Enable required threads
	// k_thread_resume(tstartbutton_id);
	// k_thread_resume(tbtnmatrix_in_id);
	// k_thread_resume(tbtnmatrix_out_id);

	// int ret = playMg5();

	// // Disable required threads after finishing
	// k_thread_suspend(tstartbutton_id);
	// k_thread_suspend(tbtnmatrix_in_id);
	// k_thread_suspend(tbtnmatrix_out_id);

	state->next = mg6_state;
}

void mg6_state(struct state *state) { // Makes use of gyro and buzzer
	printf("Minigame 6\n");

	// // Enable required threads
	// k_thread_resume(tstartbutton_id);
	// k_thread_resume(tsevenseg_id);
	// k_thread_resume(tabcbtn_id);

	// int ret = playMg6();

	// // Disable required threads after finishing
	// k_thread_suspend(tstartbutton_id);
	// k_thread_suspend(tsevenseg_id);
	// k_thread_suspend(tabcbtn_id);;

	state->next = mg7_state;
}

void mg7_state(struct state *state) { // Makes use of gyro and buzzer
	printf("Minigame 7\n");

	// // Enable required threads
	// k_thread_resume(tstartbutton_id);
	// k_thread_resume(tgyro_id);
	// k_thread_resume(tledmatrix_id);

	// int ret = playMg7();

	// // Disable required threads after finishing
	// k_thread_suspend(tstartbutton_id);
	// k_thread_suspend(tgyro_id);
	// k_thread_suspend(tledmatrix_id);

	state->next = mg8_state;
}

void mg8_state(struct state *state) { // Makes use of gyro and buzzer
	printf("Minigame 8\n");

	// Enable required threads
	// k_thread_resume(tstartbutton_id);
	// k_thread_resume(tledmatrix_id);
	// k_thread_resume(tabcbtn_id);

	// int ret = playMg8();

	// // Disable required threads after finishing
	// k_thread_suspend(tstartbutton_id);
	// k_thread_suspend(tledmatrix_id);
	// k_thread_suspend(tabcbtn_id);

	state->next = mg9_state;
}

void mg9_state(struct state *state) { // Makes use of gyro and buzzer
	printf("Minigame 9\n");

	// // Enable required threads
	// k_thread_resume(tstartbutton_id);
	// k_thread_resume(tswitches_id);

	// int ret = playMg9();

	// // Disable required threads after finishing
	// k_thread_suspend(tstartbutton_id);
	// k_thread_suspend(tswitches_id);

	state->next = mg10_state;
}

void mg10_state(struct state *state) { // Makes use of gyro and buzzer
	printf("Minigame 10\n");

	// // Enable required threads
	// k_thread_resume(tstartbutton_id);
	// k_thread_resume(tbtnmatrix_in_id);
	// k_thread_resume(tswitches_id);
	// k_thread_resume(tpotmeter_id);
	// k_thread_resume(tabcbtn_id);

	// int ret = playMg10();

	// // Disable required threads after finishing
	// k_thread_suspend(tstartbutton_id);
	// k_thread_suspend(tbtnmatrix_in_id);
	// k_thread_suspend(tswitches_id);
	// k_thread_suspend(tpotmeter_id);
	// k_thread_suspend(tabcbtn_id);

	state->next = exit_state;
}

void exit_state(struct state *state) {
	printf("Exit state");
	// k_thread_suspend(tstartbutton_id);
	// k_thread_suspend(tgyro_id);
	// k_thread_suspend(tgps_id);
	// k_thread_suspend(tbtnmatrix_out_id);
	// k_thread_suspend(tbtnmatrix_in_id);
	// k_thread_suspend(tswitches_id);
	// k_thread_suspend(tpotmeter_id);
	// k_thread_suspend(tbuzzers_id);
	// k_thread_suspend(tledmatrix_id);
	// k_thread_suspend(tledcircle_id);
	// k_thread_suspend(tsevenseg_id);
	// k_thread_suspend(tabcbtn_id);
	state->next = 0;
}

void startStatemachine() {
	struct state state = {init_state, 0};
	while (state.next) state.next(&state);
}