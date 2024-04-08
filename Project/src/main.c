#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/gpio.h>

// The stack size of all threads
#define STACKSIZE 1024

// Thread priority values (lower value is higher priority)
#define TMAIN_PRIORITY 1
#define TBUTTON_PRIORITY 4
#define TGYRO_PRIORITY 4
#define TBUZZER_PRIORITY 6
#define TLED_PRIORITY 7

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
	printf("Button\n");
}

void tgyro(void) { // Gyro thread
	printf("Gyro\n");
}

void tled(void) // led thread
{
	printf("LED\n");
}

void tbuzzer(void) // buzzer thread
{
	printf("Buzzer\n");
}

void init_state(struct state *state) {
	printf("%s %i\n", __func__, ++state->i);
	state->next = walk_state;
}

void walk_state(struct state *state) {
	printf("%s %i\n", __func__, ++state->i);
	state->next = mg1_state;
}

void mg1_state(struct state *state) {
	printf("%s %i\n", __func__, ++state->i);
	state->next = mg2_state;
}

void mg2_state(struct state *state) {
	printf("%s %i\n", __func__, ++state->i);
	state->next = finish_state;
}

void finish_state(struct state *state) {
	printf("%s %i\n", __func__, ++state->i);
	state->next = 0;
}

void tmain(void) // Core thread
{
	//will probs contain a while loop
	printf("Main\n");
	struct state state = {init_state, 0};
	while (state.next) state.next(&state);
}

// Define the threads
K_THREAD_DEFINE(tmain_id, STACKSIZE, tmain, NULL, NULL, NULL, TMAIN_PRIORITY, 0, 0);
// Input threads
K_THREAD_DEFINE(tbutton_id, STACKSIZE, tbutton, NULL, NULL, NULL, TBUTTON_PRIORITY, 0,0);
K_THREAD_DEFINE(tgyro_id, STACKSIZE, tgyro, NULL, NULL, NULL, TGYRO_PRIORITY, 0,0);
// Output threads
K_THREAD_DEFINE(tled_id, STACKSIZE, tled, NULL, NULL, NULL, TLED_PRIORITY, 0, 0);
K_THREAD_DEFINE(tbuzzer_id, STACKSIZE, tbuzzer, NULL, NULL, NULL, TBUZZER_PRIORITY, 0, 0);