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

#define THREAD0_PRIORITY 7
#define THREAD1_PRIORITY 7

void thread0(void) // printing thread
{
	int cnt = 0;
	while (1) {
		printk("Hello, I am thread0\t%d\n", cnt);
		k_msleep(100);
		cnt++;
	}
}

void thread1(void) // led thread
{
	int ret;
	bool led_state = true;

	if (!gpio_is_ready_dt(&led)) {
		return 0;
	}

	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return 0;
	}

	pwm(8,0);

	while (1) {
		//ret = gpio_pin_toggle_dt(&led);
		ret = gpio_pin_set_dt(&led, led_state);
		
		if (ret < 0) {
			return 0;
		}

		printf("LED state: %s\n", led_state ? "ON" : "OFF");
		led_state = !led_state;
		k_msleep(SLEEP_TIME_MS);
	}
}

void pwm(int brightness, int duration) {
	// brightness may be a value from 0 (off) to 10 (max brightness)
	
	for (int i = 0; i < 200; i++) {
		gpio_pin_set_dt(&led, 1);
		k_msleep(brightness);
		gpio_pin_set_dt(&led, 0);
		k_msleep(10-brightness);
	}

}

// Define the threads
K_THREAD_DEFINE(thread0_id, STACKSIZE, thread0, NULL, NULL, NULL, THREAD0_PRIORITY, 0, 0);
K_THREAD_DEFINE(thread1_id, STACKSIZE, thread1, NULL, NULL, NULL, THREAD1_PRIORITY, 0, 0);