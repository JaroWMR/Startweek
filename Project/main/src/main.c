#include "statemachine.h"
#include "threads.h"
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
#define TMAIN_PRIORITY 8

void tmain(void) // Core thread
{
	printf("Main\n");
	// startStatemachine();
	magnetometer_init();
	gyroscope_init();
	double heading = 0;
while(1){
gyroCompass_get_heading(&heading);
k_sleep(K_MSEC(500));
}
	
	magnetometer_exit();
	gyroscope_exit();

	// double gyro[3];
	
	// gyroscope_get_acceleration(gyro);
	// gyroscope_get_gyro(gyro);
}

// Define the threads
K_THREAD_DEFINE(tmain_id, STACKSIZE, tmain, NULL, NULL, NULL, TMAIN_PRIORITY, 0, 0);
