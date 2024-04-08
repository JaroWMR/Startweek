#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/printk.h>
#include <inttypes.h>

#define SLEEP_TIME_MS	1
#define Switches
#define Buttons
#define Matrix
#define Rotary

#define d2	DT_ALIAS(d2)
#define d3	DT_ALIAS(d3)
#define d4	DT_ALIAS(d4)
#define d5	DT_ALIAS(d5)
#define d6	DT_ALIAS(d6)
#define d7	DT_ALIAS(d7)
#define d8	DT_ALIAS(d8)
#define d9	DT_ALIAS(d9)
#define d10	DT_ALIAS(d10)
#define d11	DT_ALIAS(d11)
static const struct gpio_dt_spec d2 = GPIO_DT_SPEC_GET_OR(d2, gpios,
							      {0});

bool temp;

void gpio_init()
{
	int ret;
	if (!gpio_is_ready_dt(&d2)) {
		printk("Error: button device %s is not ready\n",
		       d2.port->name);
		return 0;
	}

	ret = gpio_pin_configure_dt(&d2, GPIO_INPUT);
	if (ret != 0) {
		printk("Error %d: failed to configure %s pin %d\n",
		       ret, d2.port->name, d2.pin);
		return 0;
	}
}


int main(void)
{
	

	gpio_init();
	printk("Begin Test\n");
	
	while(true)
	{
		#ifdef Switches
			temp = gpio_pin_get(d2.port, d2.pin);
			printk("sw5 off: %d\n",temp);
			temp = gpio_pin_get(d2.port, d2.pin);
			printk("sw5  on: %d\n",temp);
		#else
			printf("Macro 'test' is not defined\n");
		#endif
		

		k_busy_wait(1000000);
	}

	return 0;
}
