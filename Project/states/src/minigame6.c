#include "minigame6.h"
#include <zephyr/kernel.h>
#include <stdio.h>

char *mg6Threads[mg6ThreadCount] = {"startbtn", "sevenseg", "abcbtn"};

void getMg6Threads(char ***names, unsigned *amount) {
	*names = mg6Threads;
	*amount = mg6ThreadCount;
}

int playMg6() {
	// State loop
	for (int i = 0; i < 10; i++) {
		printf("Looping mg6, %d\n", i);
		k_msleep(10);
	}

	return 0;
}