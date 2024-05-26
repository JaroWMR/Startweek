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

// Dummy function that can be called until the actual direction (yaw) function is finished
int getDirOffsetDummy() {
	return 0;
}

int playIdle() {
	int i = 0;
	// while(1) {
	// //for (int i = 0; i < 10; i++) {
	// 	printf("Looping idle\n");

	// 	int64_t currLatNanoDeg = getLatitude();
	// 	int64_t currLonNanoDeg = getLongitude();
	// 	if (currLatNanoDeg == 0 && currLonNanoDeg == 0) {
	// 		printf("GPS does not have a fix!\n");
	// 	} else {
	// 		long double currLat = nanoDegToLdDeg(getLatitude());
	// 		long double currLon = nanoDegToLdDeg(getLongitude());
	// 		long double vnrLat = nanoDegToLdDeg(LAT_DB);
	// 		long double vnrLon = nanoDegToLdDeg(LON_DB);
	// 		long double dist = getDistanceMeters(currLat,currLon,vnrLat, vnrLon);
	// 		long double angle = getAngle(currLat, currLon, vnrLat, vnrLon);

	// 		printf("Distance (rounded to whole meters): %d\n", (int)round(dist));
	// 		printf("Angle: %Lf\n", angle);
	// 	}
	// 	ledCircleDemo();
	// }


	/******/
	// Create and randomize array of coordinates
	int64_t lats[NR_OF_LOCS] = {LAT_LOC_A, LAT_LOC_B};
	int64_t lons[NR_OF_LOCS] = {LON_LOC_A, LON_LOC_B};
	// How to use the random generator to randomize?

	// Loop that iterates through the array of coords
	for (int i = 0; i < NR_OF_LOCS; i++) {
		int distMeters = 99;	// Initialize to a value outside the expected range
		int dir = 0;			// Direction the user must head in
		while(distMeters > REQUIRED_DIST_METERS) {	// Device is too far away from next target
			int64_t currLat = getLatitude();
			int64_t currLon = getLongitude();
			//if ( currLat == 0 && currLon == 0) {	// GPS doesn't have lock
			//	printf("GPS does not have a lock!\n");
			//	continue;
			//}
			distMeters = getDistanceMeters(nanoDegToLdDeg(currLon), nanoDegToLdDeg(currLat), nanoDegToLdDeg(lons[i]), nanoDegToLdDeg(lats[i])); // Distance from current position to next location (meters)
			//dir = getAngle(currLat, currLon, lats[i], lons[i]);
			dir = round(getAngle(lats[0], lons[0], lats[1], lons[1]));
			printf("dir: %d\n", dir);
			dir += getDirOffsetDummy();
			// 
		}
	}
}

void setLedCircleDirWidth(unsigned dir, unsigned width) {
	// dir must be in degrees (so zero through 359)
	// widtb: total width of the 'band' in pixels

	const int nrPixels = 64;
	const int maxDir = 359;
	float centerFloat = (float)dir / maxDir;
	int centerPixel = round(centerFloat * nrPixels);

	int leftBound = centerPixel - (width / 2);
	if (leftBound < 0 ) {
		leftBound = nrPixels + leftBound;
	}
	int rightBound = centerPixel + (width / 2);
	if (rightBound > (nrPixels - 1)) {
		rightBound = rightBound - nrPixels;
	}
	bool overlap = false;
	if (leftBound > rightBound) {
		overlap = true;
	}

	const int nrBytes = 8;
	const int nrBits = 8;
	uint8_t outputValues[8] = {0,0,0,0,0,0,0,0};	// Initialize to zero (all LEDs off)
	for (int byteCount = 0; byteCount < nrBytes; byteCount++) {
		for (int bitCount = 0; bitCount < nrBits; bitCount++) {
			int bitIndex = 8 * byteCount + bitCount;
			//bool enableLed = (!overlap && (bitIndex >= leftBound) && (bitIndex >= rightBound)) || (overlap && (bitIndex >= leftBound || bitIndex <= rightBound));
			bool enableLed = false;
			if (!overlap) {
				if (bitIndex >= leftBound && bitIndex <= rightBound) {
					enableLed = true;
				}
			} else {
				if (bitIndex >= leftBound || bitIndex <= rightBound) {
					enableLed = true;
				}
			}
			//printf("Byte %d bit %d (bitindex %d): %d\n", byteCount, bitCount, bitIndex, enableLed);
			if (enableLed) {
				outputValues[byteCount] |= (1 << (7-bitCount));
			}

		}
	}
	ledcircleSetMutexValue(outputValues);
}

void ledCircleDemo() {
	// Just a small demo to show off the led circle in all its glory
	for (int i = 6; i < 40; i++) {
		setLedCircleDirWidth(0, i);
		k_msleep(4);
	}
	k_msleep(400);
	for (int i = 39; i >= 6; i--) {
		setLedCircleDirWidth(0, i);
		k_msleep(4);
	}
	k_msleep(400);
	for (int i = 0; i < 360; i++) {
		setLedCircleDirWidth(i, 10);
		k_msleep(2);
	}
	k_msleep(200);
	for (int i = 359; i >= 0; i--) {
		setLedCircleDirWidth(i, 10);
		k_msleep(2);
	}
	k_msleep(300);
}