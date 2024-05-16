#include "idle.h"
#include "gps.h"
#include "threads.h"
#include "circleMatrix.h"
#include "locations.h"
#include <zephyr/kernel.h>
#include <stdio.h>

#define REQUIRED_DIST_METERS 30

unsigned idleThreadCount = 1;
char *idleThreads[1] = {"ledcircle"};

void getIdleThreads(char ***names, unsigned *amount) {
	*names = idleThreads;
	*amount = idleThreadCount;
}

int playIdle() {

	while(1) {
	//for (int i = 0; i < 10; i++) {
		printf("Looping idle\n");

		int64_t currLatNanoDeg = getLatitude();
		int64_t currLonNanoDeg = getLongitude();
		if (currLatNanoDeg == 0 && currLonNanoDeg == 0) {
			printf("GPS does not have a fix!\n");
		} else {
			long double currLat = nanoDegToLdDeg(getLatitude());
			long double currLon = nanoDegToLdDeg(getLongitude());
			long double vnrLat = nanoDegToLdDeg(LAT_DB);
			long double vnrLon = nanoDegToLdDeg(LON_DB);
			long double dist = getDistanceMeters(currLat,currLon,vnrLat, vnrLon);
			long double angle = getAngle(currLat, currLon, vnrLat, vnrLon);

			printf("Distance (rounded to whole meters): %d\n", (int)round(dist));
			printf("Angle: %Lf\n", angle);
		}
		uint8_t ledData[8] = {{0b11111111}};
		ledcircleSetMutexValue(ledData);
		k_msleep(500);
		for (int i = 0; i < 8; i++) {
			ledData[i] = 0;
		}
		ledcircleSetMutexValue(ledData);
		k_msleep(500);
	}


	/******/
	// Create and randomize array of coordinates
	int64_t lats[NR_OF_LOCS] = {LAT_LOC_A, LAT_LOC_B};
	int64_t lons[NR_OF_LOCS] = {LON_LOC_A, LON_LOC_B};
	// How to use the random generator to randomize?

	// Loop that iterates through the array of coords
	for (int i = 0; i < NR_OF_LOCS; i++) {
		int distMeters = 99;	// Initialize to a value outside the expected range
		while(distMeters > REQUIRED_DIST_METERS) {
			int64_t currLat = getLatitude();
			int64_t currLon = getLongitude();
			if ( currLat == 0 && currLon == 0) {
				printf("GPS does not have a lock!\n");
				continue;
			}
			distMeters = getDistanceMeters(nanoDegToLdDeg(currLon), nanoDegToLdDeg(currLat), nanoDegToLdDeg(lons[i]), nanoDegToLdDeg(lats[i]));
		}
	}
	// Inside that a loop that checks if arrived and updates leds
	/******/
}