#include "gyroCompass.h"
#include <stdio.h>
#include <zephyr/kernel.h>
#include <math.h>

#define M_PIL 3.141592653589793238462643383279502884L
#define X_OFFSET 11.5 / 100
#define Y_OFFSET -49.63 / 100
#define zero 0

static const struct device *magnetometer;
static bool magnetometer_is_init = false;

static const struct device *gyroscope;
static bool gyroscope_is_init = false;

#ifdef CONFIG_LIS3MDL_TRIGGER

/**
 * @brief Handler for LIS3MDL trigger events.
 *
 * @param dev Pointer to the device structure.
 * @param trig Pointer to the sensor trigger structure.
 */
static void magnetometer_trigger_handler(const struct device *dev,
										 const struct sensor_trigger *trig)
{
	sensor_sample_fetch_chan(dev, trig->chan);
}
#endif

/**
 * @brief Initializes the magnetometer sensor.
 *
 * This function initializes the LIS3MDL magnetometer sensor and sets up
 * any necessary trigger for data ready events.
 *
 * @return 0 if successful, otherwise a positive error code:
 *         - 1: Failed to get LIS3MDL device
 *         - 2: Failed to set sensor trigger
 */
uint8_t magnetometer_init(void)
{
	magnetometer = DEVICE_DT_GET(DT_ALIAS(magnetometer));
	if (!device_is_ready(magnetometer))
	{
		printf("Could not get LIS3MDL device\n");
		return 1;
	}

#ifdef CONFIG_LIS3MDL_TRIGGER
	struct sensor_trigger trig;
	trig.type = SENSOR_TRIG_DATA_READY;
	trig.chan = SENSOR_CHAN_MAGN_XYZ;
	if (sensor_trigger_set(magnetometer, &trig, magnetometer_trigger_handler) < 0)
	{
		printf("Failed to set sensor trigger\n");
		return 2;
	}
#endif

	magnetometer_is_init = true;
	return 0;
}

/**
 * @brief Exits the magnetometer sensor.
 *
 * This function stops the magnetometer sensor and releases any resources
 * associated with it.
 *
 * @return 0 if successful, 1 if the sensor was not initialized.
 */
uint8_t magnetometer_exit(void)
{
	if (!magnetometer_is_init)
	{
		return 1;
	}

#ifdef CONFIG_LIS3MDL_TRIGGER
	sensor_trigger_set(magnetometer, NULL, NULL); // Unset the trigger
#endif
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
	else
	{
		printf("Magnetometer initialized\n");
	}

	if (sensor_sample_fetch(magnetometer) < 0)
	{
		printf("LIS3MDL Sensor sample update error\n");
		return 2;
	}

	sensor_channel_get(magnetometer, SENSOR_CHAN_MAGN_XYZ, magn_xyz);

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
 *         - 1: Cannot set sampling frequency for accelerometer
 *         - 2: Cannot set sampling frequency for gyro
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
		return 1;
	}

	error = sensor_attr_set(gyroscope, SENSOR_CHAN_GYRO_XYZ,
							SENSOR_ATTR_SAMPLING_FREQUENCY, &odr_attr);
	if (error != 0)
	{
		printf("Cannot set sampling frequency for gyro.\n");
		return 2;
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
 *         - Errors from gyroscope_set_sampling_freq()
 */
uint8_t gyroscope_init(void)
{
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

	/* lsm6dso accel */
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
	/* lsm6dso gyro */
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