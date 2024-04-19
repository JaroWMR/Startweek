#include "sevenSegment.h"
#include <zephyr/kernel.h>
#include <stdio.h>

void tsevenseg(void) // Seven segment displays thread
{
	while (1) {
		printf("Controlling seven segment displays\n");
		k_msleep(1);
	}
}