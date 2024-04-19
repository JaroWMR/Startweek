// Includes: own header file, hardware headers, minigame headers, framework headers, system headers (sorted alpabetically)
#include "statemachine.h"

#include "buttonMatrix.h"
#include "buzzers.h"
#include "genericGpio.h"
#include "gps.h"
#include "gyroCompass.h"
#include "ledCircle.h"
#include "ledMatrix.h"
#include "potmeter.h"
#include "sevenSegment.h"

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


// The stack size of all threads
#define STACKSIZE 1024

// Thread priority values (lower value is higher priority)
#define TSTARTBUTTON_PRIORITY 4
#define TGYRO_PRIORITY 4
#define TGPS_PRIORITY 4
#define TBTNMATRIX_OUT_PRIORITY 7
#define TBTNMATRIX_IN_PRIORITY 4
#define TSWITCHES_PRIORITY 4
#define TPOTMETER_PRIORITY 4
#define TBUZZERS_PRIORITY 7
#define TLEDMATRIX_PRIORITY 7
#define TLEDCIRCLE_PRIORITY 7
#define TSEVENSEG_PRIORITY 7
#define TABCBTN_PRIORITY 4

// Thread IDs
extern const k_tid_t tstartbutton_id;
extern const k_tid_t tgyro_id;
extern const k_tid_t tgps_id;
extern const k_tid_t tbtnmatrix_out_id;
extern const k_tid_t tbtnmatrix_in_id;
extern const k_tid_t tswitches_id;
extern const k_tid_t tpotmeter_id;
extern const k_tid_t tbuzzers_id;
extern const k_tid_t tledmatrix_id;
extern const k_tid_t tledcircle_id;
extern const k_tid_t tsevenseg_id;
extern const k_tid_t tabcbtn_id;

// Setup state machine
struct state;
typedef void state_fn(struct state *);

struct state {
	state_fn *next;
	int i;
};

state_fn init_state, walk_state, mg1_state, mg2_state, mg3_state, mg4_state, mg5_state, mg6_state, mg7_state, mg8_state, mg9_state, mg10_state, exit_state;

// State functions
void init_state(struct state *state) {
	printf("Initialization\n");

	// Disable all io threads
	k_thread_suspend(tstartbutton_id);
	k_thread_suspend(tgyro_id);
	k_thread_suspend(tgps_id);
	k_thread_suspend(tbtnmatrix_out_id);
	k_thread_suspend(tbtnmatrix_in_id);
	k_thread_suspend(tswitches_id);
	k_thread_suspend(tpotmeter_id);
	k_thread_suspend(tbuzzers_id);
	k_thread_suspend(tledmatrix_id);
	k_thread_suspend(tledcircle_id);
	k_thread_suspend(tsevenseg_id);
	k_thread_suspend(tabcbtn_id);

	state->next = walk_state;
}

void walk_state(struct state *state) {
	printf("Walking\n");
	state->next = mg1_state;
}

void mg1_state(struct state *state) { // Makes use of button and led
	// Initialise state, enable and disable corresponding threads
	printf("Minigame 1\n");

	// Enable required threads
	//k_thread_resume(tstartbutton_id);
	//k_thread_resume(tgyro_id);
	//k_thread_resume(tledmatrix_id);
	/* Temporarily enabling GPS in this mingame so I can test the driver **************************************/
	k_thread_resume(tgps_id);

	int ret = playMg1();

	// Disable required threads after finishing
	k_thread_suspend(tstartbutton_id);
	k_thread_suspend(tgyro_id);
	k_thread_suspend(tledmatrix_id);
	/* Temporarily enabling GPS in this mingame so I can test the driver **************************************/
	k_thread_suspend(tgps_id);

	state->next = mg2_state;
}

void mg2_state(struct state *state) { // Makes use of gyro and buzzer
	printf("Minigame 2\n");

	// Enable required threads
	k_thread_resume(tstartbutton_id);
	k_thread_resume(tbtnmatrix_in_id);
	k_thread_resume(tbtnmatrix_out_id);
	k_thread_resume(tbuzzers_id);

	int ret = playMg2();

	// Disable required threads after finishing
	k_thread_suspend(tstartbutton_id);
	k_thread_suspend(tbtnmatrix_in_id);
	k_thread_suspend(tbtnmatrix_out_id);
	k_thread_suspend(tbuzzers_id);

	state->next = mg3_state;
}

void mg3_state(struct state *state) { // Makes use of gyro and buzzer
	printf("Minigame 3\n");

	// Enable required threads
	k_thread_resume(tstartbutton_id);
	k_thread_resume(tbtnmatrix_in_id);
	k_thread_resume(tbtnmatrix_out_id);
	k_thread_resume(tbuzzers_id);

	int ret = playMg3();

	// Disable required threads after finishing
	k_thread_suspend(tstartbutton_id);
	k_thread_suspend(tbtnmatrix_in_id);
	k_thread_suspend(tbtnmatrix_out_id);
	k_thread_suspend(tbuzzers_id);

	state->next = mg4_state;
}

void mg4_state(struct state *state) { // Makes use of gyro and buzzer
	printf("Minigame 4\n");

	// Enable required threads
	k_thread_resume(tstartbutton_id);
	k_thread_resume(tbuzzers_id);
	k_thread_resume(tabcbtn_id);

	int ret = playMg4();

	// Disable required threads after finishing
	k_thread_suspend(tstartbutton_id);
	k_thread_suspend(tbuzzers_id);
	k_thread_suspend(tabcbtn_id);

	state->next = mg5_state;
}

void mg5_state(struct state *state) { // Makes use of gyro and buzzer
	printf("Minigame 5\n");

	// Enable required threads
	k_thread_resume(tstartbutton_id);
	k_thread_resume(tbtnmatrix_in_id);
	k_thread_resume(tbtnmatrix_out_id);

	int ret = playMg5();

	// Disable required threads after finishing
	k_thread_suspend(tstartbutton_id);
	k_thread_suspend(tbtnmatrix_in_id);
	k_thread_suspend(tbtnmatrix_out_id);

	state->next = mg6_state;
}

void mg6_state(struct state *state) { // Makes use of gyro and buzzer
	printf("Minigame 6\n");

	// Enable required threads
	k_thread_resume(tstartbutton_id);
	k_thread_resume(tsevenseg_id);
	k_thread_resume(tabcbtn_id);

	int ret = playMg6();

	// Disable required threads after finishing
	k_thread_suspend(tstartbutton_id);
	k_thread_suspend(tsevenseg_id);
	k_thread_suspend(tabcbtn_id);;

	state->next = mg7_state;
}

void mg7_state(struct state *state) { // Makes use of gyro and buzzer
	printf("Minigame 7\n");

	// Enable required threads
	k_thread_resume(tstartbutton_id);
	k_thread_resume(tgyro_id);
	k_thread_resume(tledmatrix_id);

	int ret = playMg7();

	// Disable required threads after finishing
	k_thread_suspend(tstartbutton_id);
	k_thread_suspend(tgyro_id);
	k_thread_suspend(tledmatrix_id);

	state->next = mg8_state;
}

void mg8_state(struct state *state) { // Makes use of gyro and buzzer
	printf("Minigame 8\n");

	// Enable required threads
	k_thread_resume(tstartbutton_id);
	k_thread_resume(tledmatrix_id);
	k_thread_resume(tabcbtn_id);

	int ret = playMg8();

	// Disable required threads after finishing
	k_thread_suspend(tstartbutton_id);
	k_thread_suspend(tledmatrix_id);
	k_thread_suspend(tabcbtn_id);

	state->next = mg9_state;
}

void mg9_state(struct state *state) { // Makes use of gyro and buzzer
	printf("Minigame 9\n");

	// Enable required threads
	k_thread_resume(tstartbutton_id);
	k_thread_resume(tswitches_id);

	int ret = playMg9();

	// Disable required threads after finishing
	k_thread_suspend(tstartbutton_id);
	k_thread_suspend(tswitches_id);

	state->next = mg10_state;
}

void mg10_state(struct state *state) { // Makes use of gyro and buzzer
	printf("Minigame 10\n");

	// Enable required threads
	k_thread_resume(tstartbutton_id);
	k_thread_resume(tbtnmatrix_in_id);
	k_thread_resume(tswitches_id);
	k_thread_resume(tpotmeter_id);
	k_thread_resume(tabcbtn_id);

	int ret = playMg10();

	// Disable required threads after finishing
	k_thread_suspend(tstartbutton_id);
	k_thread_suspend(tbtnmatrix_in_id);
	k_thread_suspend(tswitches_id);
	k_thread_suspend(tpotmeter_id);
	k_thread_suspend(tabcbtn_id);

	state->next = exit_state;
}

void exit_state(struct state *state) {
	printf("Exit state");
	k_thread_suspend(tstartbutton_id);
	k_thread_suspend(tgyro_id);
	k_thread_suspend(tgps_id);
	k_thread_suspend(tbtnmatrix_out_id);
	k_thread_suspend(tbtnmatrix_in_id);
	k_thread_suspend(tswitches_id);
	k_thread_suspend(tpotmeter_id);
	k_thread_suspend(tbuzzers_id);
	k_thread_suspend(tledmatrix_id);
	k_thread_suspend(tledcircle_id);
	k_thread_suspend(tsevenseg_id);
	k_thread_suspend(tabcbtn_id);
	state->next = 0;
}

void startStatemachine() {
	struct state state = {init_state, 0};
	while (state.next) state.next(&state);
}

// Define the threads
// Input threads
K_THREAD_DEFINE(tstartbutton_id, STACKSIZE, tstartbutton, NULL, NULL, NULL, TSTARTBUTTON_PRIORITY, 0,0);
K_THREAD_DEFINE(tgyro_id, STACKSIZE, tgyro, NULL, NULL, NULL, TGYRO_PRIORITY, 0,0);
K_THREAD_DEFINE(tbtnmatrix_in_id, STACKSIZE, tbtnmatrix_in, NULL, NULL, NULL, TBTNMATRIX_IN_PRIORITY, 0, 0);
K_THREAD_DEFINE(tswitches_id, STACKSIZE, tswitches, NULL, NULL, NULL, TSWITCHES_PRIORITY, 0, 0);
K_THREAD_DEFINE(tpotmeter_id, STACKSIZE, tpotmeter, NULL, NULL, NULL, TPOTMETER_PRIORITY, 0, 0);
K_THREAD_DEFINE(tabcbtn_id, STACKSIZE, tabcbtn, NULL, NULL, NULL, TABCBTN_PRIORITY, 0, 0);
// Output threads
K_THREAD_DEFINE(tgps_id, STACKSIZE, tgps, NULL, NULL, NULL, TGPS_PRIORITY, 0, 0);
K_THREAD_DEFINE(tbtnmatrix_out_id, STACKSIZE, tbtnmatrix_out, NULL, NULL, NULL, TBTNMATRIX_OUT_PRIORITY, 0, 0);
K_THREAD_DEFINE(tbuzzers_id, STACKSIZE, tbuzzers, NULL, NULL, NULL, TBUZZERS_PRIORITY, 0, 0);
K_THREAD_DEFINE(tledmatrix_id, STACKSIZE, tledmatrix, NULL, NULL, NULL, TLEDMATRIX_PRIORITY, 0, 0);
K_THREAD_DEFINE(tledcircle_id, STACKSIZE, tledcircle, NULL, NULL, NULL, TLEDCIRCLE_PRIORITY, 0, 0);
K_THREAD_DEFINE(tsevenseg_id, STACKSIZE, tsevenseg, NULL, NULL, NULL, TSEVENSEG_PRIORITY, 0, 0);