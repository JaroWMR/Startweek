#include "gyroCompass.h"
#include <stdio.h>
#include <zephyr/kernel.h>

void tgyro(void) { // Gyro thread
	while (1) {
		printf("Polling gyro\n");
		k_msleep(1);
	}
}