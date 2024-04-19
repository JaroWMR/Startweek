#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

// GPIO definitions 
static const struct gpio_dt_spec ledMatrixShiftDataIn 			= GPIO_DT_SPEC_GET(DT_ALIAS(shiftdatain16x16), gpios);
static const struct gpio_dt_spec ledMatrixShiftOutputEnable 	= GPIO_DT_SPEC_GET(DT_ALIAS(shiftenable16x16), gpios);
static const struct gpio_dt_spec ledMatrixShiftClock 			= GPIO_DT_SPEC_GET(DT_ALIAS(shiftclock16x16 ), gpios);
static const struct gpio_dt_spec ledMatrixMuxA 					= GPIO_DT_SPEC_GET(DT_ALIAS(muxa16x16), gpios);
static const struct gpio_dt_spec ledMatrixMuxB 					= GPIO_DT_SPEC_GET(DT_ALIAS(muxb16x16), gpios);
static const struct gpio_dt_spec ledMatrixMuxC 					= GPIO_DT_SPEC_GET(DT_ALIAS(muxc16x16), gpios);
static const struct gpio_dt_spec ledMatrixMuxD 					= GPIO_DT_SPEC_GET(DT_ALIAS(muxd16x16), gpios);


#define HIGH			1
#define LOW				0

#define LEDSINROW		16
#define ROWS 			16

bool ledMatrixConfig();
int8_t ledMatrixInit();
int8_t ledMatrixSetLeds(int16_t data[16]);