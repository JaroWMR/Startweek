#include "minigame1.h"
#include <zephyr/kernel.h>
#include <stdio.h>

//Temporary include, remove afterwards
#include "gps.h"

int playMg1() {
	// State loop
	// for (int i = 0; i < 10; i++) {
	// 	printf("Looping mg1, %d\n", i);
	// 	k_msleep(10);
	// }
	// Temporary infinite loop for testing gps driver:
	while(1) {
		printf("Looping mg1\n");
		printf("Latitude: %lld\n", getLatitude());
		printf("Longitude: %lld\n", getLongitude());
		k_msleep(1000);
	}

	return 0;
}