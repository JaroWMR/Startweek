#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

static const struct gpio_dt_spec d2 = GPIO_DT_SPEC_GET(DT_ALIAS(d2), gpios);
static const struct gpio_dt_spec d3 = GPIO_DT_SPEC_GET(DT_ALIAS(d3), gpios);
static const struct gpio_dt_spec d4 = GPIO_DT_SPEC_GET(DT_ALIAS(d4), gpios);
static const struct gpio_dt_spec d5 = GPIO_DT_SPEC_GET(DT_ALIAS(d5), gpios);
static const struct gpio_dt_spec d6 = GPIO_DT_SPEC_GET(DT_ALIAS(d6), gpios);
static const struct gpio_dt_spec d7 = GPIO_DT_SPEC_GET(DT_ALIAS(d7), gpios);
static const struct gpio_dt_spec d8 = GPIO_DT_SPEC_GET(DT_ALIAS(d8), gpios);
static const struct gpio_dt_spec d9 = GPIO_DT_SPEC_GET(DT_ALIAS(d9), gpios);
static const struct gpio_dt_spec d10 = GPIO_DT_SPEC_GET(DT_ALIAS(d10), gpios);
static const struct gpio_dt_spec d11 = GPIO_DT_SPEC_GET(DT_ALIAS(d11), gpios);
static const struct gpio_dt_spec d12 = GPIO_DT_SPEC_GET(DT_ALIAS(d12), gpios);
static const struct gpio_dt_spec d13 = GPIO_DT_SPEC_GET(DT_ALIAS(d13), gpios);
static const struct gpio_dt_spec d14 = GPIO_DT_SPEC_GET(DT_ALIAS(d14), gpios);
static const struct gpio_dt_spec d15 = GPIO_DT_SPEC_GET(DT_ALIAS(d15), gpios);

int main(void)
{
	int ret;
	bool led_state = true;

	if (!gpio_is_ready_dt(&d2)) {
		return 0;
	}

	ret = gpio_pin_configure_dt(&d2, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return 0;
	}

	while (1) {
		ret = gpio_pin_toggle_dt(&d2);
		if (ret < 0) {
			return 0;
		}

		led_state = !led_state;
		printf("LED state: %s\n", led_state ? "ON" : "OFF");
		k_msleep(1000);
	}
	return 0;
}