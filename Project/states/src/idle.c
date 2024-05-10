#include "idle.h"
#include "gps.h"
#include <zephyr/kernel.h>
#include <stdio.h>

unsigned idleThreadCount = 1;
char *idleThreads[1] = {"ledcircle"};

void getIdleThreads(char ***names, unsigned *amount) {
	*names = idleThreads;
	*amount = idleThreadCount;
}

int playIdle() {
	// Values for testing the GPS:
	int64_t dbLatNanoDeg = 51687593000;
	int64_t dbLonNanoDeg = 5286511000;
	int64_t vnrLatNanoDeg = 51536111110;
	int64_t vnrLonNanoDeg = 5979444440;
	

	while(1) {
		printf("Looping mg1\n");

		int64_t currLatNanoDeg = getLatitude();
		int64_t currLonNanoDeg = getLongitude();
		if (currLatNanoDeg == 0 && currLonNanoDeg == 0) {
			printf("GPS does not have a fix!\n");
		} else {
			long double currLat = nanoDegToLdDeg(getLatitude());
			long double currLon = nanoDegToLdDeg(getLongitude());
			long double vnrLat = nanoDegToLdDeg(vnrLatNanoDeg);
			long double vnrLon = nanoDegToLdDeg(vnrLonNanoDeg);
			long double dist = getDistance(currLat,currLon,vnrLat, vnrLon);

			printf("Latitude: %lld\n", getLatitude());
			printf("Longitude: %lld\n", getLongitude());
			printf("Distance: %Lf\n", dist);
		}
		k_msleep(1000);
	}
}