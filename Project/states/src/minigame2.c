#include "minigame2.h"
#include <zephyr/kernel.h>
#include <stdio.h>

char *mg2Threads[mg2ThreadCount] = {"startbtn", "btnmatrix_in", "btnmatrix_out", "buzzers"};

void getMg2Threads(char ***names, unsigned *amount) {
	*names = mg2Threads;
	*amount = mg2ThreadCount;
}

int playMg2() {
	// State loop
	for (int i = 0; i < 10; i++) {
		printf("Looping mg2, %d\n", i);
		k_msleep(10);
	}

	return 0;
}