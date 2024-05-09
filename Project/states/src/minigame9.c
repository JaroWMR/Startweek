#include "minigame9.h"
#include <zephyr/kernel.h>
#include <stdio.h>

char *mg9Threads[mg9ThreadCount] = {"startbtn", "switches"};

void getMg9Threads(char ***names, unsigned *amount) {
	*names = mg9Threads;
	*amount = mg9ThreadCount;
}

int playMg9() {
	// State loop
	for (int i = 0; i < 10; i++) {
		printf("Looping mg9, %d\n", i);
		k_msleep(10);
	}

	return 0;
}