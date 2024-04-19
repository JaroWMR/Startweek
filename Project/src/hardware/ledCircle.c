#include "ledCircle.h"
#include <zephyr/kernel.h>
#include <stdio.h>

void tledcircle(void) // LED circle thread
{
	while (1) {
		printf("Controlling LED circle\n");
		k_msleep(1);
	}
}