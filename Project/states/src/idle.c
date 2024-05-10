#include "idle.h"
#include "gps.h"
#include "locations.h"
#include <zephyr/kernel.h>
#include <stdio.h>

unsigned idleThreadCount = 1;
char *idleThreads[1] = {"ledcircle"};

void getIdleThreads(char ***names, unsigned *amount) {
	*names = idleThreads;
	*amount = idleThreadCount;
}

int playIdle() {

	while(1) {
		printf("Looping mg1\n");

		int64_t currLatNanoDeg = getLatitude();
		int64_t currLonNanoDeg = getLongitude();
		if (currLatNanoDeg == 0 && currLonNanoDeg == 0) {
			printf("GPS does not have a fix!\n");
		} else {
			long double currLat = nanoDegToLdDeg(getLatitude());
			long double currLon = nanoDegToLdDeg(getLongitude());
			long double vnrLat = nanoDegToLdDeg(LAT_VNR);
			long double vnrLon = nanoDegToLdDeg(LON_VNR);
			long double dist = getDistanceMeters(currLat,currLon,vnrLat, vnrLon);

			printf("Distance (rounded to whole meters): %d\n", (int)round(dist));
		}
		k_msleep(1000);
	}
}