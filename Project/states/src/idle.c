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
	int i = 0;
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
		uint8_t ledData[8] = {0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0};
		// for (int i = 0; i < 8; i++) {
		// 	for (int j = 7; j >= 0; j--) {
		// 		ledData[i] |= (1 << j);
		// 		ledcircleSetMutexValue(ledData);
		// 		k_msleep(200);
		// 	}
		// }
		// setLedCircleDirWidth(180, 10);
		// k_msleep(200);
		for (int i = 0; i < 360; i++) {
			setLedCircleDirWidth(i, 10);
			k_msleep(40);
		}
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

void setLedCircleDirWidth(unsigned dir, unsigned width) {
	// NOTE: two issues currently
	//	1. When the dir is low and width too great, the pixels below zero are not turend on (e.g. dir = 2, width = 10, the pixels below zero (359, 359 etc.) are not turned on)
	//	2. Vice versa but when dir is great (near 359)

	// dir must be in degrees (so zero through 359)
	// widtb: total width of the 'band' in pixels
	const int nrPixels = 64;
	const int maxDir = 359;
	float centerFloat = (float)dir / maxDir;
	int centerPixel = round(centerFloat * nrPixels);
	const int nrBytes = 8;
	const int nrBits = 8;
	uint8_t outputValues[8] = {0,0,0,0,0,0,0,0};	// Initialize to zero (all LEDs off)
	for (int byteCount = 0; byteCount < nrBytes; byteCount++) {
		for (int bitCount = 0; bitCount < nrBits; bitCount++) {
			int bitIndex = 8 * byteCount + bitCount;
			if (bitIndex > centerPixel - (width/2) && bitIndex < centerPixel + (width / 2)) {
				outputValues[byteCount] |= (1 << (7-bitCount));
			}
		}
	}
	ledcircleSetMutexValue(outputValues);
}