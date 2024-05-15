#include "gyroCompass.h"
#include <stdio.h>
#include <zephyr/kernel.h>
#include <math.h>

#define M_PIL 3.141592653589793238462643383279502884L
#define X_OFFSET 0 //11.5 / 100
#define Y_OFFSET 0 //-49.63 / 100
#define zero 0

static const struct device *magnetometer;
static bool magnetometer_is_init = false;

static const struct device *gyroscope;
static bool gyroscope_is_init = false;

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
uint8_t magnetometer_get_heading(double *aHeading)
{
	struct sensor_value magn_xyz[3];
	double magn_xyz_double[3];

	if (!magnetometer_is_init)
	{
		printf("Magnetometer not initialized\n");
		return 1;
	}

	sensor_sample_fetch_chan(magnetometer, SENSOR_CHAN_MAGN_XYZ);
	sensor_channel_get(magnetometer, SENSOR_CHAN_MAGN_XYZ, &magn_xyz);

	for (int i = 0; i < 3; i++)
	{
		magn_xyz_double[i] = sensor_value_to_double(&magn_xyz[i]);
	}

	magn_xyz_double[0] -= X_OFFSET;
	magn_xyz_double[1] -= Y_OFFSET;

	double direction;
	if (magn_xyz_double[0] == 0)
	{
		direction = (magn_xyz_double[1] < 0) ? 90 : 0;
	}
	else
	{
		direction = atan2(magn_xyz_double[1], magn_xyz_double[0]) * 180 / M_PIL;
	}

	if (direction < 0)
	{
		direction += 360;
	}
	else if (direction > 360)
	{
		direction -= 360;
	}

	*aHeading = direction;
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
uint8_t gyroscope_get_acceleration(double *aAcceleration)
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
		aAcceleration[i] = sensor_value_to_double(&gyro_xyz[i]);
	}

	printf("accel x:%f ms/2 y:%f ms/2 z:%f ms/2\n",
		   aAcceleration[0], aAcceleration[1], aAcceleration[2]);

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
uint8_t gyroscope_get_gyro(double *aGyro)
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
		aGyro[i] = sensor_value_to_double(&gyro_xyz[i]);
	}

	printf("gyro x:%f rad/s y:%f rad/s z:%f rad/s\n",
		   aGyro[0], aGyro[1], aGyro[2]);

	return 0;
}