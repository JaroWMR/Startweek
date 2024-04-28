#include <zephyr/kernel.h>

// Total number of threads
#define THREAD_AMOUNT 11

// The stack size of all threads
#define STACKSIZE 1024

// Thread priority values (lower value is higher priority)
#define TSTARTBUTTON_PRIORITY 4
#define TGYRO_PRIORITY 4
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
void tstartbutton(void);
void tgyro(void);
void tbtnmatrix_in(void);
void tswitches(void);
void tpotmeter(void);
void tabcbtn(void);
// Output thread functions
void tbtnmatrix_out(void);
void tbuzzers(void);
void tledmatrix(void);
void tledcircle(void);
void tsevenseg(void);

// Functions for enabling and disabling threads
void enableThreads(k_tid_t *, const unsigned);
void disableThreads(k_tid_t *, const unsigned);