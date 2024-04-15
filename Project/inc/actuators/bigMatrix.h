#include <zephyr/drivers/gpio.h>

// GPIO definitions 
static const struct gpio_dt_spec bigMatrixShiftDataIn 			= GPIO_DT_SPEC_GET(DT_ALIAS(shiftdatain16x16), gpios);
static const struct gpio_dt_spec bigMatrixShiftOutputEnable 	= GPIO_DT_SPEC_GET(DT_ALIAS(shiftenable16x16), gpios);
static const struct gpio_dt_spec bigMatrixShiftClock 			= GPIO_DT_SPEC_GET(DT_ALIAS(shiftclock16x16 ), gpios);
static const struct gpio_dt_spec bigMatrixMuxA 					= GPIO_DT_SPEC_GET(DT_ALIAS(muxa16x16), gpios);
static const struct gpio_dt_spec bigMatrixMuxB 					= GPIO_DT_SPEC_GET(DT_ALIAS(muxb16x16), gpios);
static const struct gpio_dt_spec bigMatrixMuxC 					= GPIO_DT_SPEC_GET(DT_ALIAS(muxc16x16), gpios);
static const struct gpio_dt_spec bigMatrixMuxD 					= GPIO_DT_SPEC_GET(DT_ALIAS(muxd16x16), gpios);


#define HIGH			1
#define LOW				0

#define LEDSINROW		16
#define ROWS 			16

bool bigLedMatrixConfig();
int8_t bigLedMatrixInit();
int8_t bigLedMatrixSetLeds(int16_t data[16]);