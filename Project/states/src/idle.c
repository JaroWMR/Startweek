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

	//while(1) {
	for (int i = 0; i < 10; i++) {
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
		k_msleep(1000);
	}
}