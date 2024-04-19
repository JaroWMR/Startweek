#include <zephyr/drivers/gpio.h>
#include <stdio.h>
#include <zephyr/kernel.h>

// GPIO definitions 
static const struct gpio_dt_spec buttonMatrixShiftDataIn 			= GPIO_DT_SPEC_GET(DT_ALIAS(shiftdatain4x4), gpios);
static const struct gpio_dt_spec buttonMatrixShiftOutputEnable 		= GPIO_DT_SPEC_GET(DT_ALIAS(shiftoutputenable4x4), gpios);
static const struct gpio_dt_spec buttonMatrixShiftClock 				= GPIO_DT_SPEC_GET(DT_ALIAS(shiftclock4x4 ), gpios);
static const struct gpio_dt_spec buttonMatrixMuxA 					= GPIO_DT_SPEC_GET(DT_ALIAS(muxa4x4), gpios);
static const struct gpio_dt_spec buttonMatrixMuxB 					= GPIO_DT_SPEC_GET(DT_ALIAS(muxb4x4), gpios);


#define HIGH			1
#define LOW				0

#define LEDSINROW		4
#define ROWS 			4

bool buttonMatrixConfig();
int8_t buttonMatrixInit();
int8_t buttonMatrixSetLeds(int8_t data[4]);