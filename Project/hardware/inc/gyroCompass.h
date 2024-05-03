#ifndef GYROCOMPASS_H
#define GYROCOMPASS_H

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>

// Function prototypes
uint8_t magnetometer_init(void);
uint8_t magnetometer_exit(void);
uint8_t magnetometer_get_heading(double *aHeading);

#endif /* GYROCOMPASS_H */
