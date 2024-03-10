#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/gpio.h>

// The stack size 
#define STACKSIZE 1024
// Sleep time for the blinking thread
#define SLEEP_TIME_MS 1000
// The devicetree node identifier for the "led0" alias
#define LED0_NODE DT_ALIAS(led0)

/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

#define TCORE_PRIORITY 7
#define TLED_PRIORITY 7

void tcore(void) // Core thread
{
	int cnt = 0;
	while (1) {
		printk("Hello, I am thread0\t%d\n", cnt);
		k_msleep(540);
		cnt++;
	}
}

void tled(void) // led thread
{
	int ret;
	int led_time = 30;

	if (!gpio_is_ready_dt(&led)) {
		return 0;
	}

	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		printk("Failed to configure LED gpio!\n");
	}

	while(1) {
		printk("Starting LED cycle\n");
		for (int i = 1; i <= 10; i++) {
			pwm(i,led_time);
		}
		for (int i = 9; i > 1; i--) {
			pwm(i,led_time);
		}
	}
}

void pwm(int brightness, int duration) {
	// brightness may be a value from 0 (off) to 10 (max brightness)
	// duration in [ms], must be a multiple of 10 ms
	for (int i = 0; i < duration/10; i++) {
		gpio_pin_set_dt(&led, 1);
		k_msleep(brightness);
		gpio_pin_set_dt(&led, 0);
		k_msleep(10-brightness);
	}

}

// Define the threads
K_THREAD_DEFINE(tcore_id, STACKSIZE, tcore, NULL, NULL, NULL, TCORE_PRIORITY, 0, 0);
K_THREAD_DEFINE(tled_id, STACKSIZE, tled, NULL, NULL, NULL, TLED_PRIORITY, 0, 0);