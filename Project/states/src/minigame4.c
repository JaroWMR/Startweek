#include "minigame4.h"
#include <zephyr/kernel.h>
#include <stdio.h>

char *mg4Threads[mg4ThreadCount] = {"startbtn", "buzzers", "abcbtn"};

void getMg4Threads(char ***names, unsigned *amount) {
	*names = mg4Threads;
	*amount = mg4ThreadCount;
}

int playMg4() {
	// State loop
	for (int i = 0; i < 10; i++) {
		printf("Looping mg4, %d\n", i);
		k_msleep(10);
	}

	return 0;
}