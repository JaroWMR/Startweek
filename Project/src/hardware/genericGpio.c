#include "genericGpio.h"
#include <zephyr/kernel.h>
#include <stdio.h>

void tstartbutton(void) { // startbutton thread
	while (1) {
		printf("Polling startbutton\n");
		k_msleep(1);
	}
}

void tswitches(void) // 5 switches thread
{
	while (1) {
		printf("Reading switches\n");
		k_msleep(1);
	}
}

void tabcbtn(void) // ABC buttons thread
{
	while (1) {
		printf("Polling ABC buttons\n");
		k_msleep(1);
	}
}