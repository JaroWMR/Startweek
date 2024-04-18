#include "buttonMatrix.h"
#include <zephyr/kernel.h>
#include <stdio.h>

void tbtnmatrix_out(void) // button matrix output thread
{
	while (1) {
		printf("Writing button matrix LEDs\n");
		k_msleep(1);
	}
}

void tbtnmatrix_in(void) // button matrix input thread
{
	while (1) {
		printf("Reading button matrix buttons\n");
		k_msleep(1);
	}
}