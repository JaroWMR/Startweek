#include "threads.h"
#include <zephyr/kernel.h>
#include <zephyr/sys/util.h>

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

// Define the thread functions
// Input thread functions
void tstartbutton(void) {}
void tgyro(void) {}
void tbtnmatrix_in(void) {}
void tswitches(void) {}
void tpotmeter(void) {}
void tabcbtn(void) {}
// Output thread functions
void tbtnmatrix_out(void) {}
void tbuzzers(void) {}
void tledmatrix(void) {}
void tledcircle(void) {}
void tsevenseg(void) {}

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