#include "minigame1.h"
#include <zephyr/kernel.h>
#include <stdio.h>

char *mg1Threads[mg1ThreadCount] = {"startbtn", "ledmatrix"};

void getMg1Threads(char ***names, unsigned *amount) {
	*names = mg1Threads;
	*amount = mg1ThreadCount;
}

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

		long double currLat = nanoDegToLdDeg(getLatitude());
		long double currLon = nanoDegToLdDeg(getLongitude());
		long double dbLat = nanoDegToLdDeg(db.latitude);
		long double dbLon = nanoDegToLdDeg(db.longitude);
		long double dist = getDistance(currLat,currLon,dbLat, dbLon);

		printf("Latitude: %lld\n", getLatitude());
		printf("Longitude: %lld\n", getLongitude());
		printf("Distance: %Lf\n", dist);
		k_msleep(1000);
	}

	return 0;
}