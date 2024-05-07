#include "gyroCompass.h"
#include <stdio.h>
#include <zephyr/kernel.h>
#include <math.h>

#define M_PIL 3.141592653589793238462643383279502884L
#define X_OFFSET 11.5 / 100
#define Y_OFFSET -49.63 / 100

static const struct device *magnetometer;
static bool is_initialized = false;

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

    is_initialized = true;
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
    if (!is_initialized)
    {
        return 1;
    }

#ifdef CONFIG_LIS3MDL_TRIGGER
    sensor_trigger_set(magnetometer, NULL, NULL); // Unset the trigger
#endif
    is_initialized = false;

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

    if (!is_initialized)
    {
        printf("Magnetometer not initialized\n");
        return 1;
    }else{
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

static int set_sampling_freq(const struct device *dev)
{
	int ret = 0;
	struct sensor_value odr_attr;

	/* set accel/gyro sampling frequency to 12.5 Hz */
	odr_attr.val1 = 12.5;
	odr_attr.val2 = 0;

	ret = sensor_attr_set(dev, SENSOR_CHAN_ACCEL_XYZ,
			SENSOR_ATTR_SAMPLING_FREQUENCY, &odr_attr);
	if (ret != 0) {
		printf("Cannot set sampling frequency for accelerometer.\n");
		return ret;
	}

	ret = sensor_attr_set(dev, SENSOR_CHAN_GYRO_XYZ,
			SENSOR_ATTR_SAMPLING_FREQUENCY, &odr_attr);
	if (ret != 0) {
		printf("Cannot set sampling frequency for gyro.\n");
		return ret;
	}

	return 0;
}

static void fetch_and_display(const struct device *dev)
{
	struct sensor_value gyro_xyz[3];
	double gyro_double[3];
	static int trig_cnt;

	trig_cnt++;

	/* lsm6dso accel */
	sensor_sample_fetch_chan(dev, SENSOR_CHAN_ACCEL_XYZ);
	sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, &gyro_xyz);

	    for (int i = 0; i < 3; i++)
    {
        gyro_double[i] = sensor_value_to_double(&gyro_xyz[i]);
    }

	printf("accel x:%f ms/2 y:%f ms/2 z:%f ms/2\n",
			gyro_double[0], gyro_double[1], gyro_double[2]);

	/* lsm6dso gyro */
	sensor_sample_fetch_chan(dev, SENSOR_CHAN_GYRO_XYZ);
	sensor_channel_get(dev, SENSOR_CHAN_GYRO_XYZ, &gyro_xyz);

		    for (int i = 0; i < 3; i++)
    {
        gyro_double[i] = sensor_value_to_double(&gyro_xyz[i]);
    }

	printf("gyro x:%f rad/s y:%f rad/s z:%f rad/s\n",
			gyro_double[0], gyro_double[1], gyro_double[2]);

	printf("trig_cnt:%d\n\n", trig_cnt);
}

static void test_polling_mode(const struct device *dev)
{
	if (set_sampling_freq(dev) != 0) {
		return;
	}

	while (1) {
		fetch_and_display(dev);
		k_sleep(K_MSEC(1000));
	}
}

uint8_t gyro_print_data(void){
	const struct device *const dev = DEVICE_DT_GET(DT_ALIAS(gyroscope));

	if (!device_is_ready(dev)) {
		printk("%s: device not ready.\n", dev->name);
		return 0;
	}else{
		printk("%s: device ready.\n", dev->name);
	}

	printf("Testing LSM6DSO sensor in polling mode.\n\n");
	test_polling_mode(dev);	

	return 0;
}
