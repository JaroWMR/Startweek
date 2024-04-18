#include "ledMatrix.h"
#include <zephyr/kernel.h>
#include <stdio.h>

void tledmatrix(void) // LED matrix thread
{
	while (1) {
		printf("Controlling LED matrix\n");
		k_msleep(1);
	}
}