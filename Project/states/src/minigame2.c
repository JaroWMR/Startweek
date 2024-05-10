#include "minigame2.h"
#include <zephyr/kernel.h>
#include <stdio.h>
#include "threads.h"

char *mg2Threads[mg2ThreadCount] = {"startbtn", "btnmatrix_in", "btnmatrix_out", "buzzers"};

void getMg2Threads(char ***names, unsigned *amount) {
	*names = mg2Threads;
	*amount = mg2ThreadCount;
}

int playMg2() {
	uint8_t *values;
	// State loop

	while(true)//for (int i = 0; i < 10; i++)
	{	
		uint8_t data[4] = {0b00000000,0b00000000,0b00000000,0b00000000};
		printf("Looping mg2,\n");
		values = btnmatrix_inGetMutexValue();
		for (uint8_t section = 1; section < 5; section++) {
			for (uint8_t i = 1; i < 5; i++) {
				printf("value in loop:%d value:%d\n",section * 4 - i,values[section * 4 - i]);
				data[section-1] |= (values[section * 4 - i]) << (i-1);
			}
			printf("data[%d]: %d\n",section,data[section]);
		}
		btnmatrix_outSetMutexValue(data);
		k_msleep(100);
	}
	return 0;
}