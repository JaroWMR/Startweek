#include "idle.h"

#include "threads.h"

#include <zephyr/kernel.h>

const unsigned idleRequiredThreadsCount = 1;
const k_tid_t idleRequiredThreads[THREAD_AMOUNT] = {tledcircle_id};