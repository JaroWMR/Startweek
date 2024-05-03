#include "statemachine.h"
#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/gpio.h>
#include "gyroCompass.h"

// The stack size of all threads
#define STACKSIZE 1024

// Thread priority values (lower value is higher priority)
#define TMAIN_PRIORITY 1

void tmain(void) // Core thread
{
	printf("Main\n");
	//startStatemachine();
	magnetometer_init();
	while(1){
		double heading = 0;
		magnetometer_get_heading(&heading);
		printf("heading = %.2f\n", heading);
		k_sleep(K_MSEC(1000));
	}
}

// Define the threads
K_THREAD_DEFINE(tmain_id, STACKSIZE, tmain, NULL, NULL, NULL, TMAIN_PRIORITY, 0, 0);
