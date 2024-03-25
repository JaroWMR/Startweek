/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>

/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */

// #ifdef CONFIG_LIS3MDL_TRIGGER
// static int gyro_trig_cnt;

// static void gyro_trigger_handler(const struct device *dev,
// 									const struct sensor_trigger *trig)
// {
// 	sensor_sample_fetch_chan(dev, trig->chan);
// 	gyro_trig_cnt++;
// }
// #endif

// int main(void)
// {
// 	struct sensor_value magn_xyz[3];
// 	static const struct device *gyro = DEVICE_DT_GET_ONE(st_lis3mdl_magn);

// #if defined(CONFIG_LIS3MDL_TRIGGER)
// 	struct sensor_trigger trig;
// 	int cnt = 1;
// #endif

// 	if (!device_is_ready(gyro))
// 	{
// 		printf("Could not get LIS3MDL device\n");
// 		return 0;
// 	}

// #ifdef CONFIG_LIS3MDL_TRIGGER
// 	trig.type = SENSOR_TRIG_DATA_READY;
// 	trig.chan = SENSOR_CHAN_MAGN_XYZ;
// 	sensor_trigger_set(gyro, &trig, gyro_trigger_handler);
// #endif

// 	while (1)
// 	{
// #ifndef CONFIG_LIS3MDL_TRIGGER
// 		if (sensor_sample_fetch(gyro) < 0)
// 		{
// 			printf("LIS3MDL Sensor sample update error\n");
// 			return 0;
// 		}
// #endif

// 		sensor_channel_get(gyro, SENSOR_CHAN_MAGN_XYZ, magn_xyz);

// 		printf(
// 			"LIS3MDL: Magnetic field (gauss): x: %.1f, y: %.1f, z: %.1f\n",
// 			sensor_value_to_double(&magn_xyz[0]),
// 			sensor_value_to_double(&magn_xyz[1]),
// 			sensor_value_to_double(&magn_xyz[2]));

// #if defined(CONFIG_LIS3MDL_TRIGGER)
// 		printk("%d:: lis3mdl trig %d\n", cnt, gyro_trig_cnt);
// 		cnt++;
// #endif

// 		k_sleep(K_MSEC(100)); // Sleep for 1 second
// 	}

// 	return 1;
// }

int main(void)
{
    static const struct device *gyro = DEVICE_DT_GET_ONE(st_lis3mdl_magn);

    if (!device_is_ready(gyro)) {
        printk("Could not get LIS3MDL device\n");
        return 0;
    }

    struct sensor_value x_axis, y_axis, z_axis;

    while (1) {
        if (sensor_sample_fetch(gyro) < 0) {
            printk("Sensor sample fetch failed\n");
            return 0;
        }

        if (sensor_channel_get(gyro, SENSOR_CHAN_MAGN_X, &x_axis) < 0 ||
            sensor_channel_get(gyro, SENSOR_CHAN_MAGN_Y, &y_axis) < 0 ||
            sensor_channel_get(gyro, SENSOR_CHAN_MAGN_Z, &z_axis) < 0) {
            printk("Could not read sensor channels\n");
            return 0;
        }

        printk("X: %f, Y: %f, Z: %f\n",
               sensor_value_to_double(&x_axis),
               sensor_value_to_double(&y_axis),
               sensor_value_to_double(&z_axis));

        k_sleep(K_MSEC(100)); // Sleep for 1 second
    }

	return 1;
}