#ifndef GYROCOMPASS_H
#define GYROCOMPASS_H

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>

//defines
#define gyro_sample_freq 119 //see lsm6dso kconfig of zephyr for acceptable values. Note this is for accelometer and gyro

// Function prototypes
uint8_t magnetometer_init(void);
uint8_t magnetometer_exit(void);
uint8_t magnetometer_get_heading(double *aHeading);

uint8_t gyroscope_init(void);
uint8_t gyroscope_exit(void);
uint8_t gyroscope_get_acceleration(double *aAcceleration);
uint8_t gyroscope_get_gyro(double *aGyro);

#endif /* GYROCOMPASS_H */
