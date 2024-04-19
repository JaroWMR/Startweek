#include <zephyr/drivers/gpio.h>

// GPIO definitions 
static const struct gpio_dt_spec smallMatrixShiftDataIn 			= GPIO_DT_SPEC_GET(DT_ALIAS(shiftdatain4x4), gpios);
static const struct gpio_dt_spec smallMatrixShiftOutputEnable 		= GPIO_DT_SPEC_GET(DT_ALIAS(shiftoutputenable4x4), gpios);
static const struct gpio_dt_spec smallMatrixShiftClock 				= GPIO_DT_SPEC_GET(DT_ALIAS(shiftclock4x4 ), gpios);
static const struct gpio_dt_spec smallMatrixMuxA 					= GPIO_DT_SPEC_GET(DT_ALIAS(muxa4x4), gpios);
static const struct gpio_dt_spec smallMatrixMuxB 					= GPIO_DT_SPEC_GET(DT_ALIAS(muxb4x4), gpios);


#define HIGH			1
#define LOW				0

#define LEDSINROW		4
#define ROWS 			4

bool smallMatrixConfig();
int8_t smallMatrixInit();
int8_t smallMatrixSetLeds(int8_t data[4]);