#include "minigame1.h"
#include <zephyr/kernel.h>
#include <stdio.h>

int playMg1() {
	// State loop
	for (int i = 0; i < 10; i++) {
		printf("Looping mg1, %d\n", i);
		k_msleep(10);
	}
}