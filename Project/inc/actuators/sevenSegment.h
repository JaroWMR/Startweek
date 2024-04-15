#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <string.h>

// GPIO definitions 
static const struct gpio_dt_spec sevenSegmentBcdA 		= GPIO_DT_SPEC_GET(DT_ALIAS(bcda7segment), gpios);
static const struct gpio_dt_spec sevenSegmentBcdB 		= GPIO_DT_SPEC_GET(DT_ALIAS(bcdb7segment), gpios);
static const struct gpio_dt_spec sevenSegmentBcdC 		= GPIO_DT_SPEC_GET(DT_ALIAS(bcdc7segment), gpios);
static const struct gpio_dt_spec sevenSegmentBcdD 		= GPIO_DT_SPEC_GET(DT_ALIAS(bcdd7segment), gpios);
static const struct gpio_dt_spec sevenSegmentDP 		= GPIO_DT_SPEC_GET(DT_ALIAS(decimalpoint7segment), gpios);
static const struct gpio_dt_spec sevenSegmentMuxDig1 	= GPIO_DT_SPEC_GET(DT_ALIAS(muxdig17segment), gpios);
static const struct gpio_dt_spec sevenSegmentMuxDig2 	= GPIO_DT_SPEC_GET(DT_ALIAS(muxdig27segment), gpios);
static const struct gpio_dt_spec sevenSegmentMuxDig3 	= GPIO_DT_SPEC_GET(DT_ALIAS(muxdig37segment), gpios);
static const struct gpio_dt_spec sevenSegmentMuxDig4 	= GPIO_DT_SPEC_GET(DT_ALIAS(muxdig47segment), gpios);


#define HIGH			1
#define LOW				0

bool sevenSegmentConfig();
uint8_t sevenSegmentInit();
void sevenSegmentOneSegment(char value,bool dp);
uint8_t sevenSegmentSet(char input[4],uint8_t dpPosition);