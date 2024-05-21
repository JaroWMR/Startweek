#include "gyroCompass.h"
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <zephyr/kernel.h>

#define M_PIL 3.141592653589793238462643383279502884L
#define X_OFFSET -28.76 / 100
#define Y_OFFSET 45.67 / 100
#define Z_OFFSET -7.58 / 100
#define zero 0

// rotation matrix for magnetometer
const uint16_t rotation_magno[3][3] = {
	{1, 0, 0},
	{0, -1, 0},
	{0, 0, -1}};

// rotation matrix for accelometer
const uint16_t rotation_accel[3][3] = {
	{0, 1, 0},
	{1, 0, 0},
	{0, 0, -1}};

static const struct device *magnetometer;
static bool magnetometer_is_init = false;

static const struct device *gyroscope;
static bool gyroscope_is_init = false;

/* roll pitch and yaw angles computed by iecompass */
static int32_t iPhi, iThe, iPsi;
/* magnetic field readings corrected for hard iron effects and PCB orientation */
static int16_t iBfx, iBfy, iBfz;

int32_t tmpAngle;					   /* temporary angle*100 deg: range -36000 to 36000 */
static int16_t iLPPsi;				   /* low pass filtered angle*100 deg: range -18000 to 18000 */
static int16_t iLPPhi;				   /* low pass filtered angle*100 deg: range -18000 to 18000 */
static int16_t iLPThe;				   /* low pass filtered angle*100 deg: range -18000 to 18000 */
static uint16_t ANGLE_LPF = 32768 / 5; /* low pass filter: set to 32768 / N for N samples averaging */

#define MINDELTATRIG 1

/* Fifth order polynomial approximation giving 0.05 deg max error */
const int16_t K1 = 5701;
const int16_t K2 = -1645;
const int16_t K3 = 446;

/* Final step size for iDivide */
const uint16_t MINDELTADIV = 1;

/* Function to calculate ir = iy / ix with iy <= ix, and ix, iy both > 0 */
static int16_t iDivide(int16_t iy, int16_t ix)
{
	int16_t itmp;	/* Scratch */
	int16_t ir;		/* Result = iy / ix, range 0. to 1., returned in range 0 to 32767 */
	int16_t idelta; /* Delta on candidate result, dividing each stage by factor of 2 */

	/* Set result r to zero and binary search step to 16384 = 0.5 */
	ir = 0;
	idelta = 16384; /* Set as 2^14 = 0.5 */

	/* To reduce quantization effects, boost ix and iy to the maximum signed 16 bit value */
	while ((ix < 16384) && (iy < 16384))
	{
		ix += ix;
		iy += iy;
	}

	/* Loop over binary sub-division algorithm solving for ir*ix = iy */
	do
	{
		/* Generate new candidate solution for ir and test if we are too high or too low */
		itmp = ir + idelta; /* itmp=ir+delta, the candidate solution */
		itmp = (int16_t)((itmp * ix) >> 15);
		if (itmp <= iy)
			ir += idelta;
		idelta >>= 1; /* Divide by 2 using right shift one bit */
	} while (idelta >= MINDELTADIV); /* Last loop is performed for idelta=MINDELTADIV */

	return ir;
}

/* Calculates 100*atan(iy/ix) range 0 to 9000 for all ix, iy positive in range 0 to 32767 */
static int16_t iHundredAtanDeg(int16_t iy, int16_t ix)
{
	int32_t iAngle; /* Angle in degrees times 100 */
	int16_t iRatio; /* Ratio of iy / ix or vice versa */
	int32_t iTmp;	/* Temporary variable */

	/* Check for pathological cases */
	if ((ix == 0) && (iy == 0))
		return 0;
	if ((ix == 0) && (iy != 0))
		return 9000;

	/* Check for non-pathological cases */
	if (iy <= ix)
		iRatio = iDivide(iy, ix); /* return a fraction in range 0. to 32767 = 0. to 1. */
	else
		iRatio = iDivide(ix, iy); /* return a fraction in range 0. to 32767 = 0. to 1. */

	/* First, third, and fifth order polynomial approximation */
	iAngle = (int32_t)K1 * (int32_t)iRatio;
	iTmp = ((int32_t)iRatio >> 5) * ((int32_t)iRatio >> 5) * ((int32_t)iRatio >> 5);
	iAngle += (iTmp >> 15) * (int32_t)K2;
	iTmp = (iTmp >> 20) * ((int32_t)iRatio >> 5) * ((int32_t)iRatio >> 5);
	iAngle += (iTmp >> 15) * (int32_t)K3;
	iAngle = iAngle >> 15;

	/* Check if above 45 degrees */
	if (iy > ix)
		iAngle = 9000 - iAngle;

	/* For tidiness, limit result to range 0 to 9000 equals 0.0 to 90.0 degrees */
	if (iAngle < 0)
		iAngle = 0;
	if (iAngle > 9000)
		iAngle = 9000;

	return (int16_t)iAngle;
}

/* Calculates 100*atan2(iy/ix)=100*atan2(iy,ix) in degrees for ix, iy in range -32768 to 32767 */
static int16_t iHundredAtan2Deg(int16_t iy, int16_t ix)
{
	int16_t iResult; /* angle in degrees times 100 */

	/* Check for -32768 which is not handled correctly */
	if (ix == -32768)
		ix = -32767;
	if (iy == -32768)
		iy = -32767;

	/* Check for quadrants */
	if ((ix >= 0) && (iy >= 0)) /* Range 0 to 90 degrees */
		iResult = iHundredAtanDeg(iy, ix);
	else if ((ix <= 0) && (iy >= 0)) /* Range 90 to 180 degrees */
		iResult = (int16_t)(18000 - (int16_t)iHundredAtanDeg(iy, (int16_t)-ix));
	else if ((ix <= 0) && (iy <= 0)) /* Range -180 to -90 degrees */
		iResult = (int16_t)((int16_t)-18000 + iHundredAtanDeg((int16_t)-iy, (int16_t)-ix));
	else /* ix >=0 and iy <= 0 giving range -90 to 0 degrees */
		iResult = (int16_t)(-iHundredAtanDeg((int16_t)-iy, ix));

	return iResult;
}

/* function to calculate ir = ix / sqrt(ix*ix+iy*iy) using binary division */
static int16_t iTrig(int16_t ix, int16_t iy)
{
	uint32_t itmp;	 /* scratch */
	uint32_t ixsq;	 /* ix * ix */
	int16_t isignx;	 /* storage for sign of x. algorithm assumes x >= 0 then corrects later */
	uint32_t ihypsq; /* (ix * ix) + (iy * iy) */
	int16_t ir;		 /* result = ix / sqrt(ix*ix+iy*iy) range -1, 1 returned as signed int16 */
	int16_t idelta;	 /* delta on candidate result dividing each stage by factor of 2 */

	/* correct for pathological case: ix==iy==0 */
	if ((ix == 0) && (iy == 0))
	{
		ix = iy = 1;
	}

	/* check for -32768 which is not handled correctly */
	if (ix == -32768)
	{
		ix = -32767;
	}
	if (iy == -32768)
	{
		iy = -32767;
	}

	/* store the sign for later use. algorithm assumes x is positive for convenience */
	isignx = 1;
	if (ix < 0)
	{
		ix = (int16_t)-ix;
		isignx = -1;
	}

	/* for convenience in the boosting set iy to be positive as well as ix */
	iy = (int16_t)abs(iy);

	/* to reduce quantization effects, boost ix and iy but keep below maximum signed 16 bit */
	while ((ix < 16384) && (iy < 16384))
	{
		ix += ix;
		iy += iy;
	}

	/* calculate ix*ix and the hypotenuse squared */
	ixsq = ((int32_t)ix * (int32_t)ix);			 /* ixsq=ix*ix: 0 to 32767^2 = 1073676289 */
	ihypsq = (ixsq + (int32_t)iy * (int32_t)iy); /* ihypsq=(ix*ix+iy*iy) 0 to 2*32767*32767=2147352578 */

	/* set result r to zero and binary search step to 16384 = 0.5 */
	ir = 0;
	idelta = 16384; /* set as 2^14 = 0.5 */

	/* loop over binary sub-division algorithm */
	do
	{
		/* generate new candidate solution for ir and test if we are too high or too low */
		/* itmp=(ir+delta)^2, range 0 to 32767*32767 = 2^30 = 1073676289 */
		itmp = (((int32_t)ir + (int32_t)idelta) * ((int32_t)ir + (int32_t)idelta));
		/* itmp=(ir+delta)^2*(ix*ix+iy*iy), range 0 to 2^31 = 2147221516 */
		itmp = (itmp >> 15) * (ihypsq >> 15);
		if (itmp <= ixsq)
			ir += idelta;
		idelta >>= 1; /* divide by 2 using right shift one bit */
	} while (idelta >= MINDELTATRIG); /* last loop is performed for idelta=MINDELTATRIG */

	/* correct the sign before returning */
	return (int16_t)(ir * isignx);
}

/* Implement a modulo arithmetic exponential low pass filter on the yaw angle */
/*
void implementLPFPsi()
{
	/* Compute the change in angle modulo 360 degrees 
	tmpAngle = (int32_t)iPsi - (int32_t)iLPPsi;
	if (tmpAngle > 18000)
		tmpAngle -= 36000;
	if (tmpAngle < -18000)
		tmpAngle += 36000;
	/* Calculate the new low pass filtered angle 
	tmpAngle = (int32_t)iLPPsi + ((ANGLE_LPF * tmpAngle) >> 30);
	/* Check that the angle remains in -180 to 180 deg bounds 
	if (tmpAngle > 18000)
		tmpAngle -= 36000;
	if (tmpAngle < -18000)
		tmpAngle += 36000;

	// if (tmpAngle > 9000)
	// 	tmpAngle = (Int16)(18000 - tmpAngle);
	// if (tmpAngle < -9000)
	// 	tmpAngle = (Int16)(-18000 - tmpAngle);

	/* Store the correctly bounded low pass filtered angle 
	iLPPsi = (int16_t)tmpAngle;
} */

/* Implement a modulo arithmetic exponential low pass filter on the roll angle */
/*
void implementLPFPhi()
{
	/* Compute the change in angle modulo 360 degrees 
	tmpAngle = (int32_t)iPhi - (int32_t)iLPPhi;
	if (tmpAngle > 18000)
		tmpAngle -= 36000;
	if (tmpAngle < -18000)
		tmpAngle += 36000;
	/* Calculate the new low pass filtered angle 
	tmpAngle = (int32_t)iLPPhi + ((ANGLE_LPF * tmpAngle) >> 5);
	/* Check that the angle remains in -180 to 180 deg bounds 
	if (tmpAngle > 18000)
		tmpAngle -= 36000;
	if (tmpAngle < -18000)
		tmpAngle += 36000;

	/* Store the correctly bounded low pass filtered angle 
	iLPPhi = (int16_t)tmpAngle;
} */

/* Implement a modulo arithmetic exponential low pass filter on the yaw angle */
/*
void implementLPFThe()
{
	// Compute the change in angle modulo 360 degrees 
	tmpAngle = (int32_t)iThe - (int32_t)iLPThe;
	if (tmpAngle > 18000)
		tmpAngle -= 36000;
	if (tmpAngle < -18000)
		tmpAngle += 36000;
	// Calculate the new low pass filtered angle 
	tmpAngle = (int32_t)iLPThe + ((ANGLE_LPF * tmpAngle) >> 5);
	// Check that the angle remains in -90 to 90 deg bounds 
	if (tmpAngle > 9000)
		tmpAngle = (18000 - tmpAngle);
	if (tmpAngle < -9000)
		tmpAngle = (-18000 - tmpAngle);

	// Store the correctly bounded low pass filtered angle 
	iLPThe = (int16_t)tmpAngle;
}
*/

void iecompass(int16_t iBpx, int16_t iBpy, int16_t iBpz,
			   int16_t iGpx, int16_t iGpy, int16_t iGpz, int16_t *angle)
{
	printf("iBpx: %i, iBpy: %i, iBpz: %i\n", iBpx, iBpy, iBpz);
	printf("iGpx: %i, iGpy: %i, iGpz: %i\n", iGpx, iGpy, iGpz);
	/* stack variables */
	/* iBpx, iBpy, iBpz: the three components of the magnetometer sensor */
	/* iGpx, iGpy, iGpz: the three components of the accelerometer sensor */
	/* local variables */
	int16_t iSin, iCos; /* sine and cosine */


	/* calculate current roll angle Phi */
	iPhi = iHundredAtan2Deg(iGpy, iGpz); /* Eq 13 */
	iPhi -= 18000;						 // 180 degree offset
	if (iPhi > 18000)					 // make sure it stays between -180 and 180 degrees
		iPhi -= 36000;
	if (iPhi < -18000)
		iPhi += 36000;
	// implementLPFPhi();
	printf("roll/iPhi = %i\n", iPhi / 100);
	/* calculate sin and cosine of roll angle Phi */
	iSin = iTrig(iGpy, iGpz); /* Eq 13: sin = opposite / hypotenuse */
	iCos = iTrig(iGpz, iGpy); /* Eq 13: cos = adjacent / hypotenuse */
	printf("iSin = %i\niCos = %i\n", iSin, iCos);
	/* de-rotate by roll angle Phi */
	iBfy = (int16_t)((iBpy * iCos - iBpz * iSin) >> 15); /* Eq 19 y component */
	iBpz = (int16_t)((iBpy * iSin + iBpz * iCos) >> 15); /* Bpy*sin(Phi)+Bpz*cos(Phi)*/
	iGpz = (int16_t)((iGpy * iSin + iGpz * iCos) >> 15); /* Eq 15 denominator */
	printf("iBfy: %i, iBpz: %i, iGpz: %i\n", iBfy, iBpz, iGpz);

	/* calculate current pitch angle Theta */
	iThe = iHundredAtan2Deg((int16_t)-iGpx, iGpz); /* Eq 15 */
	// implementLPFThe();
	printf("pitch/iThe = %i\n", iThe / 100);
	/* restrict pitch angle to range -90 to 90 degrees */
	if (iThe > 9000)
		iThe = (int16_t)(18000 - iThe);
	if (iThe < -9000)
		iThe = (int16_t)(-18000 - iThe);
	/* calculate sin and cosine of pitch angle Theta */
	iSin = (int16_t)-iTrig(iGpx, iGpz); /* Eq 15: sin = opposite / hypotenuse */
	iCos = iTrig(iGpz, iGpx);			/* Eq 15: cos = adjacent / hypotenuse */
	/* correct cosine if pitch not in range -90 to 90 degrees */
	if (iCos < 0)
		iCos = (int16_t)-iCos;
	/* de-rotate by pitch angle Theta */
	iBfx = (int16_t)((iBpx * iCos + iBpz * iSin) >> 15);  /* Eq 19: x component */
	iBfz = (int16_t)((-iBpx * iSin + iBpz * iCos) >> 15); /* Eq 19: z component */
	printf("iBfx: %i, iBfz: %i\n", iBfx, iBfz);

	/* calculate current yaw = e-compass angle Psi */
	iPsi = iHundredAtan2Deg((int16_t)-iBfy, iBfx); /* Eq 22 */
	// implementLPFPsi();
	printf("yaw/iPsi = %d\n\n", iPsi / 100);

	*angle = iPsi / 100;
}

// Function to perform matrix multiplication
void multiplyMatrices(int16_t result[3], const int16_t matrix1[3][3], const int16_t matrix2[3])
{
	int i, j;
	for (i = 0; i < 3; ++i)
	{
		result[i] = 0;
		for (j = 0; j < 3; ++j)
		{
			result[i] += matrix1[i][j] * matrix2[j];
		}
	}
}

// Function to rotate the x/y/z axis
uint8_t rotate_data(int16_t aMagnoData[3], int16_t aAccelData[3])
{
	uint16_t tempResult[3];

	// Apply rotations
	multiplyMatrices(tempResult, rotation_magno, aMagnoData);
	for (int i = 0; i < 3; i++)
	{
		aMagnoData[i] = tempResult[i];
	}

	multiplyMatrices(tempResult, rotation_accel, aAccelData);
	for (int i = 0; i < 3; i++)
	{
		aAccelData[i] = tempResult[i];
	}

	return 0;
}

/**
 * @brief Sets the sampling frequency for the gyroscope.
 *
 * This function sets the sampling frequency for the gyroscope sensor.
 *
 * @return 0 if successful, otherwise a positive error code:
 *         - 10: Cannot set sampling frequency for accelerometer
 */
uint8_t magnetometer_set_sampling_freq(int aFreq)
{
	int error = 0;
	struct sensor_value odr_attr;

	odr_attr.val1 = aFreq;
	odr_attr.val2 = 0;

	error = sensor_attr_set(magnetometer, SENSOR_CHAN_MAGN_XYZ,
							SENSOR_ATTR_SAMPLING_FREQUENCY, &odr_attr);
	if (error != 0)
	{
		printf("Cannot set sampling frequency for magnetometer.\n");
		return 10;
	}

	return 0;
}

/**
 * @brief Initializes the magnetometer sensor.
 *
 * This function initializes the LIS3MDL magnetometer sensor and sets up
 * any necessary trigger for data ready events.
 *
 * @return 0 if successful, otherwise a positive error code:
 *         - 1: Failed to get LIS3MDL device
 *         - 2: Magnetometer already initialized
 *         - Errors from magnetometer_set_sampling_freq()
 */
uint8_t magnetometer_init(void)
{
	if (magnetometer_is_init)
	{
		printf("Magnetometer already initialized\n");
		return 2;
	}

	magnetometer = DEVICE_DT_GET(DT_ALIAS(magnetometer));
	if (!device_is_ready(magnetometer))
	{
		printf("Could not get LIS3MDL device\n");
		return 1;
	}

	// int error = magnetometer_set_sampling_freq(magno_sample_freq);
	// if (error != 0)
	// {
	//  return error;
	// }

	magnetometer_is_init = true;
	return 0;
}

/**
 * @brief Exits the magnetometer sensor.
 *
 * This function stops the magnetometer sensor and releases any resources
 * associated with it.
 *
 * @return 0 if successful, otherwise a positive error code:
 *         - 1: Magnetometer not initialized
 *         - Errors from magnetometer_set_sampling_freq()
 */
uint8_t magnetometer_exit(void)
{
	if (!magnetometer_is_init)
	{
		return 1;
	}

	// int error = magnetometer_set_sampling_freq(magno_sample_freq_off);
	// if (error != 0)
	// {
	//  return error;
	// }

	magnetometer_is_init = false;
	return 0;
}

/**
 * @brief Retrieves the heading from the magnetometer sensor.
 *
 * This function fetches the latest magnetometer data, processes it to
 * calculate the heading, and returns the result.
 *
 * @param aHeading[out] Pointer to a variable where the heading will be stored.
 *
 * @return 0 if successful, otherwise a positive error code:
 *         - 1: Magnetometer not initialized
 *         - 2: LIS3MDL Sensor sample update error
 */
uint8_t magnetometer_get_magneto(int16_t *aMagneto)
{
	struct sensor_value magn_xyz[3];

	if (!magnetometer_is_init)
	{
		printf("Magnetometer not initialized\n");
		return 1;
	}

	sensor_sample_fetch_chan(magnetometer, SENSOR_CHAN_MAGN_XYZ);
	sensor_channel_get(magnetometer, SENSOR_CHAN_MAGN_XYZ, &magn_xyz);

	for (int i = 0; i < 3; i++)
	{
		float floatValue = magn_xyz[i].val2;
		aMagneto[i] = floatValue / 1000000 * 32767;
	}

	aMagneto[0] -= X_OFFSET;
	aMagneto[1] -= Y_OFFSET;
	aMagneto[2] -= Z_OFFSET;

	// double direction;
	// if (aMagneto[0] == 0)
	// {
	// 	direction = (aMagneto[1] < 0) ? 90 : 0;
	// }
	// else
	// {
	// 	direction = atan2(aMagneto[1], aMagneto[0]) * 180 / M_PIL;
	// }

	// if (direction < 0)
	// {
	// 	direction += 360;
	// }
	// else if (direction > 360)
	// {
	// 	direction -= 360;
	// }

	// *aHeading = direction;
	return 0;
}

/**
 * @brief Sets the sampling frequency for the gyroscope.
 *
 * This function sets the sampling frequency for the gyroscope sensor.
 *
 * @return 0 if successful, otherwise a positive error code:
 *         - 10: Cannot set sampling frequency for accelerometer
 *         - 20: Cannot set sampling frequency for gyro
 */
uint8_t gyroscope_set_sampling_freq(int aFreq)
{
	int error = 0;
	struct sensor_value odr_attr;

	/* set accel/gyro sampling frequency to 12.5 Hz */
	odr_attr.val1 = aFreq;
	odr_attr.val2 = 0;

	error = sensor_attr_set(gyroscope, SENSOR_CHAN_ACCEL_XYZ,
							SENSOR_ATTR_SAMPLING_FREQUENCY, &odr_attr);
	if (error != 0)
	{
		printf("Cannot set sampling frequency for accelerometer.\n");
		return 10;
	}

	error = sensor_attr_set(gyroscope, SENSOR_CHAN_GYRO_XYZ,
							SENSOR_ATTR_SAMPLING_FREQUENCY, &odr_attr);
	if (error != 0)
	{
		printf("Cannot set sampling frequency for gyro.\n");
		return 20;
	}

	return 0;
}

/**
 * @brief Initializes the gyroscope sensor.
 *
 * This function initializes the gyroscope sensor and sets the sampling frequency.
 *
 * @return 0 if successful, otherwise a positive error code:
 *         - 1: Device not ready
 *         - 2: Gyroscope already initialized
 *         - Errors from gyroscope_set_sampling_freq()
 */
uint8_t gyroscope_init(void)
{
	if (gyroscope_is_init)
	{
		printf("Magnetometer already initialized\n");
		return 2;
	}

	gyroscope = DEVICE_DT_GET(DT_ALIAS(gyroscope));
	int error = 0;

	if (!device_is_ready(gyroscope))
	{
		printk("%s: device not ready.\n", gyroscope->name);
		return 1;
	}

	error = gyroscope_set_sampling_freq(gyro_sample_freq);
	if (error != 0)
	{
		return error;
	}

	gyroscope_is_init = true;
	return 0;
}

/**
 * @brief Exits the gyroscope sensor.
 *
 * This function stops the gyroscope sensor.
 *
 * @return 0 if successful, otherwise a positive error code:
 *         - 1: Device is not initialized
 *         - Errors from gyroscope_set_sampling_freq()
 */
uint8_t gyroscope_exit(void)
{
	if (!gyroscope_is_init)
	{
		printf("Gyroscope not initialized\n");
		return 1;
	}

	int error = gyroscope_set_sampling_freq(zero);
	if (error != 0)
	{
		return error;
	}

	gyroscope_is_init = false;

	return 0;
}

/**
 * @brief Retrieves the acceleration from the gyroscope sensor.
 *
 * This function fetches the latest acceleration data from the gyroscope sensor.
 *
 * @param aAcceleration[out] Pointer to a variable where the acceleration will be stored. Expects a double[3] array.
 *
 * @return 0 if successful, 1 if the gyroscope was not initialized.
 */
uint8_t gyroscope_get_acceleration(int16_t aAcceleration[3])
{
	if (!gyroscope_is_init)
	{
		printf("Gyroscope not initialized\n");
		return 1;
	}

	struct sensor_value gyro_xyz[3];

	sensor_sample_fetch_chan(gyroscope, SENSOR_CHAN_ACCEL_XYZ);
	sensor_channel_get(gyroscope, SENSOR_CHAN_ACCEL_XYZ, &gyro_xyz);

	for (int i = 0; i < 3; i++)
	{
		float floatValue = sensor_value_to_float(&gyro_xyz[i]);
		aAcceleration[i] = floatValue / 19.6 * 32767;
	}

	// printf("accel x:%f ms/2 y:%f ms/2 z:%f ms/2\n",
	// 	   aAcceleration[0], aAcceleration[1], aAcceleration[2]);

	return 0;
}

/**
 * @brief Retrieves the angular velocity from the gyroscope sensor.
 *
 * This function fetches the latest angular velocity data from the gyroscope sensor.
 *
 * @param aGyro[out] Pointer to a variable where the angular velocity will be stored. Expects a double[3] array.
 *
 * @return 0 if successful, 1 if the gyroscope was not initialized.
 */
uint8_t gyroscope_get_gyro(float aGyro[3])
{
	if (!gyroscope_is_init)
	{
		printf("Gyroscope not initialized\n");
		return 1;
	}

	struct sensor_value gyro_xyz[3];
	sensor_sample_fetch_chan(gyroscope, SENSOR_CHAN_GYRO_XYZ);
	sensor_channel_get(gyroscope, SENSOR_CHAN_GYRO_XYZ, &gyro_xyz);

	for (int i = 0; i < 3; i++)
	{
		aGyro[i] = sensor_value_to_float(&gyro_xyz[i]);
	}

	printf("gyro x:%f rad/s y:%f rad/s z:%f rad/s\n",
		   aGyro[0], aGyro[1], aGyro[2]);

	return 0;
}

uint8_t gyroCompass_get_heading(double *aHeading)
{
	int16_t MagnetoValue[3], AccelValue[3];
	int16_t angle;
	magnetometer_get_magneto(MagnetoValue);
	gyroscope_get_acceleration(AccelValue);
	rotate_data(MagnetoValue, AccelValue);

	iecompass(MagnetoValue[0], MagnetoValue[1], MagnetoValue[2], AccelValue[0], AccelValue[1], AccelValue[2], &angle);

	// printf("Angle is: %i\n\n", angle);
	return 0;
}