#include "minigame1.h"
#include <zephyr/kernel.h>
#include <stdio.h>

//Temporary includes, remove afterwards
#include "gps.h"
#include <zephyr/drivers/gnss.h>
#include <zephyr/data/navigation.h>

int playMg1() {
	// State loop
	// for (int i = 0; i < 10; i++) {
	// 	printf("Looping mg1, %d\n", i);
	// 	k_msleep(10);
	// }
	// Temporary infinite loop for testing gps driver:
	struct navigation_data db;
	db.latitude = 51687593000;
	db.longitude = 5286511000;
	while(1) {
		printf("Looping mg1\n");
		printf("Latitude: %lld\n", getLatitude());
		printf("Longitude: %lld\n", getLongitude());
		struct gnss_data currLoc = getGnssData();
		uint64_t dist;
		int ret = navigation_distance(&dist, &currLoc.nav_data, &db);
		printf("Distance: %llu\n", dist);
		k_msleep(1000);
	}

	return 0;
}