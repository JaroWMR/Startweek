#ifndef BUTTON_MATRIX_H
#define BUTTON_MATRIX_H

#include <zephyr/drivers/gpio.h>
#include <stdio.h>
#include <zephyr/kernel.h>

// GPIO definitions 
static const struct gpio_dt_spec buttonMatrixShiftDataIn 			= GPIO_DT_SPEC_GET(DT_ALIAS(shiftdatain4x4), gpios);
static const struct gpio_dt_spec buttonMatrixShiftOutputEnable 		= GPIO_DT_SPEC_GET(DT_ALIAS(shiftoutputenable4x4), gpios);
static const struct gpio_dt_spec buttonMatrixShiftClock 				= GPIO_DT_SPEC_GET(DT_ALIAS(shiftclock4x4 ), gpios);
static const struct gpio_dt_spec buttonMatrixMuxA 					= GPIO_DT_SPEC_GET(DT_ALIAS(muxa4x4), gpios);
static const struct gpio_dt_spec buttonMatrixMuxB 					= GPIO_DT_SPEC_GET(DT_ALIAS(muxb4x4), gpios);

#define BUTTONMATRIXLEDSINROW		4
#define BUTTONMATRIXROWS 			4

#ifndef HIGH
#define HIGH 1
#endif

#ifndef LOW
#define LOW 0
#endif

bool buttonMatrixConfig();
int8_t buttonMatrixInit();
int8_t buttonMatrixSet(int8_t data[BUTTONMATRIXROWS]);

#endif // BUTTON_MATRIX_H