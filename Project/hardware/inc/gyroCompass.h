#ifndef GYROCOMPASS_H
#define GYROCOMPASS_H

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>

//defines
#define gyro_sample_freq 119 //see lsm6dso kconfig of zephyr for acceptable values. Note this is for accelometer and gyro
#define magno_sample_freq 155 //see lis3mdl kconfig of zephyr for acceptable values.
#define magno_sample_freq_off 0.625 //see lis3mdl kconfig of zephyr for acceptable values.


// static uint8_t gyroCompass_i_divide(int16_t iy, int16_t ix, int16_t *result);
// static uint8_t gyroCompass_i_hundred_atan_deg(int16_t iy, int16_t ix, int16_t *result);
// static uint8_t gyroCompass_i_hundred_atan2_deg(int16_t iy, int16_t ix, int16_t *result);
// static uint8_t gyroCompass_i_trig(int16_t ix, int16_t iy, int16_t *result);
// static uint8_t gyroCompass_i_ecompass(int16_t iBpx, int16_t iBpy, int16_t iBpz,
//                               int16_t iGpx, int16_t iGpy, int16_t iGpz, int16_t *angle);
// static uint8_t gyroCompass_multiply_matrices(int16_t result[3], const int16_t matrix1[3][3], const int16_t matrix2[3]);
// static uint8_t gyroCompass_rotate_data(int16_t aMagnoData[3], int16_t aAccelData[3]);

uint8_t magnetometer_set_sampling_freq(int aFreq);
uint8_t magnetometer_init(void);
uint8_t magnetometer_exit(void);
uint8_t magnetometer_get_magneto(int16_t *aMagneto);

uint8_t gyroscope_set_sampling_freq(int aFreq);
uint8_t gyroscope_init(void);
uint8_t gyroscope_exit(void);
uint8_t gyroscope_get_acceleration(int16_t aAcceleration[3]);
uint8_t gyroscope_get_gyro(float aGyro[3]);

uint8_t gyroCompass_get_heading(double *aHeading);

#endif /* GYROCOMPASS_H */
