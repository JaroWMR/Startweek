#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/gpio.h>

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