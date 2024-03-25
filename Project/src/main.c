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

int main(void)
{
    static const struct device *gyro = DEVICE_DT_GET(DT_NODELABEL(gyro));

    if (!device_is_ready(gyro)) {
        printf("Could not get LIS3MDL device\n");
        return 0;
    }

    struct sensor_value x_axis, y_axis, z_axis;

    while (1) {
        if (sensor_sample_fetch(gyro) < 0) {
            printf("Sensor sample fetch failed\n");
            return 0;
        }

        if (sensor_channel_get(gyro, SENSOR_CHAN_MAGN_X, &x_axis) < 0 ||
            sensor_channel_get(gyro, SENSOR_CHAN_MAGN_Y, &y_axis) < 0 ||
            sensor_channel_get(gyro, SENSOR_CHAN_MAGN_Z, &z_axis) < 0) {
            printf("Could not read sensor channels\n");
            return 0;
        }

        printf("X: %f, Y: %f, Z: %f\n",
               sensor_value_to_double(&x_axis),
               sensor_value_to_double(&y_axis),
               sensor_value_to_double(&z_axis));

        k_sleep(K_MSEC(1000)); // Sleep for 1 second
    }

	return 1;
}
