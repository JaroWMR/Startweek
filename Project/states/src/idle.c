#include "idle.h"
#include "gps.h"
#include "gyroCompass.h"
#include "lcd.h"
#include "threads.h"
#include "circleMatrix.h"
#include "locations.h"
#include <zephyr/kernel.h>
#include <stdio.h>

#define REQUIRED_DIST_METERS 30
#define ANGLE_BUFFER_SIZE 10
#define GAMES_AMOUNT 10

unsigned idleThreadCount = 1;
char *idleThreads[1] = {"ledcircle"};
double angleSinBuffer[ANGLE_BUFFER_SIZE];
double angleCosBuffer[ANGLE_BUFFER_SIZE];

int circleMovingAvg(int newValue) {
	double rad = toRadians(newValue);
	double sinVal = sin(rad);
	double cosVal = cos(rad);

	for (int i = ANGLE_BUFFER_SIZE-1; i > 0; i--) {	// Move every entry in the buffer up by one index
		angleSinBuffer[i] = angleSinBuffer[i-1];
		angleCosBuffer[i] = angleCosBuffer[i-1];
	}
	angleSinBuffer[0] = sinVal;						// Insert the new value
	angleCosBuffer[0] = cosVal;

	double sinSum = 0;
	double cosSum = 0;
	for (int i = 0; i < ANGLE_BUFFER_SIZE; i++) {
		sinSum += angleSinBuffer[i];
		cosSum += angleCosBuffer[i];
	}
	double sinAvg = sinSum / ANGLE_BUFFER_SIZE;
	double cosAvg = cosSum / ANGLE_BUFFER_SIZE;
	return toDegrees(atan2(sinAvg,cosAvg));
}

void getIdleThreads(char ***names, unsigned *amount) {
	*names = idleThreads;
	*amount = idleThreadCount;
}

int playIdle() {
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
	int64_t lats[NR_OF_LOCS] = {LAT_LOC_A, LAT_LOC_B, LAT_LOC_C};
	int64_t lons[NR_OF_LOCS] = {LON_LOC_A, LON_LOC_B, LON_LOC_C};
	static int locIndex = 0;
	locIndex = rand() % GAMES_AMOUNT;
	static bool completedGames[GAMES_AMOUNT] = {false, false, false, false, false, false, false, false, false, false};

	// Check if all games have been completed
	for (int i = 0; i < GAMES_AMOUNT; i++) {
		if (completedGames[GAMES_AMOUNT] == false) {
			break;
		}
		printf("All games completed\n");
		return -1;
	}

	while (completedGames[locIndex] == true) {
		locIndex = rand() % 10;	// Keep getting random index until a game which has not been done yet is found
	}
	// How to use the random generator to randomize?

	int distMeters = 20;	// Initialize to a value outside the expected range
	int dir = 0;			// Direction the user must head in
	lcdEnable();
	for (int i = 0; i < 3; i++) {
	while(1) {
	//while(distMeters > REQUIRED_DIST_METERS) {	// Device is too far away from next target
		int64_t currLat = getLatitude();		// Get the current latitude
		int64_t currLon = getLongitude();		// Get the current longitude
		//currLat = 51688805560;
		//currLon = 5285611110;
		//if ( currLat == 0 && currLon == 0) {	// GPS doesn't have lock
		//if (getGnssData().info.fix_status == GNSS_FIX_STATUS_NO_FIX) {
		//	//printf("GPS does not have a lock!\n");
		//	//lcdClear();
		//	lcdStringWrite("No fix 1");
		//	k_msleep(500);
		//	lcdStringWrite("No fix 2");
		//	k_msleep(500);
		//	continue;
		//}
		distMeters = getDistanceMeters(nanoDegToLdDeg(currLon), nanoDegToLdDeg(currLat), nanoDegToLdDeg(lons[i]), nanoDegToLdDeg(lats[i])); // Distance from current position to next location (meters)
		//lcdClear();
		dir = getAngle(nanoDegToLdDeg(currLat), nanoDegToLdDeg(currLon), nanoDegToLdDeg(lats[i]), nanoDegToLdDeg(lons[i]));					// Angle between current location and next location
		//printf("dir: %d\n", dir);
		int compassDir;
		gyroCompass_get_heading(&compassDir);	// Angle of device
		
		char banana[25];
		char distValue[3];
		strcpy(banana, "Distance: ");
		sprintf(distValue, "%d", distMeters);
		strcat(banana,distValue);
		strcat(banana, " angle: ");
		char rotationValue[3];
		sprintf(rotationValue, "%d", compassDir);
		strcat(banana, rotationValue);
		lcdStringWrite(banana);

		compassDir = circleMovingAvg(compassDir);
		
		dir = dir - compassDir;					// Add to get the direction compared to the device
		if (dir < 0 ) {
			dir += 360;
		} else if (dir >= 360) {
			dir -= 360;
		}
		setLedCircleDirWidth(dir, 10);			// Point the user in the correct direction
		if (distMeters < 20) {
			lcdStringWrite("Arrived at location!!");
			k_msleep(4000);
			break;
		}
		k_msleep(1);
	}
	}
	completedGames[locIndex] = true;
	return locIndex;
	//}
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