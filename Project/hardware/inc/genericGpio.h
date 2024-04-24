#include <zephyr/kernel.h>
#include <stdio.h>
#include <zephyr/drivers/gpio.h>

static const struct gpio_dt_spec buttons4x4[16] = {
	GPIO_DT_SPEC_GET(DT_ALIAS(button14x4), gpios),
	GPIO_DT_SPEC_GET(DT_ALIAS(button24x4), gpios),
	GPIO_DT_SPEC_GET(DT_ALIAS(button34x4), gpios),
	GPIO_DT_SPEC_GET(DT_ALIAS(button44x4), gpios),
	GPIO_DT_SPEC_GET(DT_ALIAS(button54x4), gpios),
	GPIO_DT_SPEC_GET(DT_ALIAS(button64x4), gpios),
	GPIO_DT_SPEC_GET(DT_ALIAS(button74x4), gpios),
	GPIO_DT_SPEC_GET(DT_ALIAS(button84x4), gpios),
	GPIO_DT_SPEC_GET(DT_ALIAS(button94x4), gpios),
	GPIO_DT_SPEC_GET(DT_ALIAS(button104x4), gpios),
	GPIO_DT_SPEC_GET(DT_ALIAS(button114x4), gpios),
	GPIO_DT_SPEC_GET(DT_ALIAS(button124x4), gpios),
	GPIO_DT_SPEC_GET(DT_ALIAS(button134x4), gpios),
	GPIO_DT_SPEC_GET(DT_ALIAS(button144x4), gpios),
	GPIO_DT_SPEC_GET(DT_ALIAS(button154x4), gpios),
	GPIO_DT_SPEC_GET(DT_ALIAS(button164x4), gpios)
};

static const struct gpio_dt_spec switchon[5] = {
	GPIO_DT_SPEC_GET(DT_ALIAS(switches1on), gpios),
	GPIO_DT_SPEC_GET(DT_ALIAS(switches2on), gpios),
	GPIO_DT_SPEC_GET(DT_ALIAS(switches3on), gpios),
	GPIO_DT_SPEC_GET(DT_ALIAS(switches4on), gpios),
	GPIO_DT_SPEC_GET(DT_ALIAS(switches5on), gpios),
};

static const struct gpio_dt_spec switchoff[5] = {
	GPIO_DT_SPEC_GET(DT_ALIAS(switches1off), gpios),
	GPIO_DT_SPEC_GET(DT_ALIAS(switches2off), gpios),
	GPIO_DT_SPEC_GET(DT_ALIAS(switches3off), gpios),
	GPIO_DT_SPEC_GET(DT_ALIAS(switches4off), gpios),
	GPIO_DT_SPEC_GET(DT_ALIAS(switches5off), gpios),
};

static const struct gpio_dt_spec abcbutton[3] = {
	GPIO_DT_SPEC_GET(DT_ALIAS(abcbuttona), gpios),
	GPIO_DT_SPEC_GET(DT_ALIAS(abcbuttonb), gpios),
	GPIO_DT_SPEC_GET(DT_ALIAS(abcbuttonc), gpios),
};

static const struct gpio_dt_spec abcleds[3] = {
	GPIO_DT_SPEC_GET(DT_ALIAS(abcleda), gpios),
	GPIO_DT_SPEC_GET(DT_ALIAS(abcledb), gpios),
	GPIO_DT_SPEC_GET(DT_ALIAS(abcledc), gpios),
};

static const struct gpio_dt_spec startbutton	= GPIO_DT_SPEC_GET(DT_ALIAS(startbutton), gpios);
static const struct gpio_dt_spec startled		= GPIO_DT_SPEC_GET(DT_ALIAS(startled), gpios);

#define HIGH			1
#define LOW				0

bool buttons4x4Config();
bool switchesConfig();
bool abcbuttonsConfig();
bool abcledsConfig();
bool startbuttonledConfig();

uint8_t buttons4x4Init();
uint8_t switchesInit();
uint8_t abcbuttonsInit();
uint8_t abcledsInit();
uint8_t startbuttonledInit();

uint8_t buttons4x4Get(uint8_t selectedbtn);
uint8_t switchesGet(uint8_t selectedswitch);
uint8_t abcbuttonsGet(char selectedbtn);
uint8_t abcledsSet(char selectedled,bool value);
uint8_t startbuttonGet();
uint8_t startledSet(bool value);