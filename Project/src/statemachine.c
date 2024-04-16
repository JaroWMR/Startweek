#include "statemachine.h"
#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/gpio.h>

// The stack size of all threads
#define STACKSIZE 1024

// Thread priority values (lower value is higher priority)
#define TBUTTON_PRIORITY 4
#define TGYRO_PRIORITY 4
#define TBUZZER_PRIORITY 6
#define TLED_PRIORITY 7

// Thread IDs
extern const k_tid_t tbutton_id;
extern const k_tid_t tgyro_id;
extern const k_tid_t tled_id;
extern const k_tid_t tbuzzer_id;

// Setup state machine
struct state;
typedef void state_fn(struct state *);

struct state {
	state_fn *next;
	int i;
};

state_fn init_state, walk_state, mg1_state, mg2_state, finish_state;

// Thread functions
void tbutton(void) { // button thread
	while (1) {
		printf("Polling button\n");
		k_msleep(1);
	}
}

void tgyro(void) { // Gyro thread
	while (1) {
		printf("Polling gyro\n");
		k_msleep(1);
	}
}

void tled(void) // led thread
{
	while (1) printf("Writing LED\n");
}

void tbuzzer(void) // buzzer thread
{
	while (1) printf("Controlling buzzer\n");
}

void init_state(struct state *state) {
	printf("%s %i\n", __func__, ++state->i);
	state->next = walk_state;
}

void walk_state(struct state *state) {
	printf("%s %i\n", __func__, ++state->i);
	state->next = mg1_state;
}

void mg1_state(struct state *state) { // Makes use of button and led
	// Initialise state, enable and disable corresponding threads
	printf("%s %i\n", __func__, ++state->i);
	k_thread_resume(tbutton_id);
	k_thread_suspend(tgyro_id);
	k_thread_resume(tled_id);
	k_thread_suspend(tbuzzer_id);

	// State loop
	for (int i = 0; i < 500; i++) {
		printf("Looping mg1, %d\n", i);
		k_msleep(10);
	}
	state->next = mg2_state;
}

void mg2_state(struct state *state) { // Makes use of gyro and buzzer
	printf("%s %i\n", __func__, ++state->i);
	state->next = finish_state;
}

void finish_state(struct state *state) {
	printf("%s %i\n", __func__, ++state->i);
	k_thread_suspend(tbutton_id);
	k_thread_suspend(tgyro_id);
	k_thread_suspend(tled_id);
	k_thread_suspend(tbuzzer_id);
	state->next = 0;
}

void startStatemachine() {
	struct state state = {init_state, 0};
	while (state.next) state.next(&state);
}

// Define the threads
// Input threads
K_THREAD_DEFINE(tbutton_id, STACKSIZE, tbutton, NULL, NULL, NULL, TBUTTON_PRIORITY, 0,0);
K_THREAD_DEFINE(tgyro_id, STACKSIZE, tgyro, NULL, NULL, NULL, TGYRO_PRIORITY, 0,0);
// Output threads
K_THREAD_DEFINE(tled_id, STACKSIZE, tled, NULL, NULL, NULL, TLED_PRIORITY, 0, 0);
K_THREAD_DEFINE(tbuzzer_id, STACKSIZE, tbuzzer, NULL, NULL, NULL, TBUZZER_PRIORITY, 0, 0);