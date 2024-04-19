#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

// GPIO definitions 
static const struct gpio_dt_spec circleMatrixShiftDataIn 			= GPIO_DT_SPEC_GET(DT_ALIAS(shiftdataincircle), gpios);
static const struct gpio_dt_spec circleMatrixShiftOutputEnable 		= GPIO_DT_SPEC_GET(DT_ALIAS(shiftoutputenablecircle), gpios);
static const struct gpio_dt_spec circleMatrixShiftClock 			= GPIO_DT_SPEC_GET(DT_ALIAS(shiftclockcircle ), gpios);
static const struct gpio_dt_spec circleMatrixMuxA 					= GPIO_DT_SPEC_GET(DT_ALIAS(muxacircle), gpios);
static const struct gpio_dt_spec circleMatrixMuxB 					= GPIO_DT_SPEC_GET(DT_ALIAS(muxbcircle), gpios);
static const struct gpio_dt_spec circleMatrixMuxC 					= GPIO_DT_SPEC_GET(DT_ALIAS(muxccircle), gpios);


#define HIGH			1
#define LOW				0

#define LEDSINROW		8
#define ROWS 			8

bool circleMatrixConfig();
int8_t circleMatrixInit();
int8_t circleMatrixSetLeds(int8_t data[8]);