#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/gpio.h>

// The stack size 
#define STACKSIZE 1024
// Sleep time for the blinking thread
#define SLEEP_TIME_MS 1000
// The devicetree node identifier for the "led0" alias
#define LED0_NODE DT_ALIAS(led0)

/*
 * Get button configuration from the devicetree sw0 alias. This is mandatory.
 */
#define SW0_NODE	DT_ALIAS(sw0)
#if !DT_NODE_HAS_STATUS(SW0_NODE, okay)
#error "Unsupported board: sw0 devicetree alias is not defined"
#endif
static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET_OR(SW0_NODE, gpios, {0});
static struct gpio_callback button_cb_data;

/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

#define TBUTTON_PRIORITY 7
#define TCORE_PRIORITY 7
#define TLED_PRIORITY 7

int ledFunction = 1; //0=fade, 1=step

void button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
	printk("Button pressed at %" PRIu32 "\n", k_cycle_get_32());
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

void ledFade() {
	int led_time = 30; // ms
	printk("Starting LED fade\n");
	for (int i = 1; i <= 10; i++) {
		pwm(i,led_time);
	}
	for (int i = 9; i > 1; i--) {
		pwm(i,led_time);
	}
}

void ledStep() {
	int led_time = 400; // ms
	printk("Starting LED step\n");
	pwm(3,led_time);
	pwm(6,led_time);
	pwm(10,led_time);
}

void tbutton(void) { // button thread
	int ret;

	if (!gpio_is_ready_dt(&button)) {
		printk("Error: button device %s is not ready\n", button.port->name);
	}

	ret = gpio_pin_configure_dt(&button, GPIO_INPUT);
	if (ret != 0) {
		printk("Error %d: failed to configure %s pin %d\n", ret, button.port->name, button.pin);
	}

	ret = gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_TO_ACTIVE);
	if (ret != 0) {
		printk("Error %d: failed to configure interrupt on %s pin %d\n", ret, button.port->name, button.pin);
	}

	gpio_init_callback(&button_cb_data, button_pressed, BIT(button.pin));
	gpio_add_callback(button.port, &button_cb_data);
	printk("Set up button at %s pin %d\n", button.port->name, button.pin);
}

void tcore(void) // Core thread
{
	int cnt = 0;
	while (1) {
		printk("Hello, I am thread0\t%d\n", cnt);
		ledFunction = gpio_pin_get_dt(&button);
		k_msleep(540);
		cnt++;
	}
}

void tled(void) // led thread
{
	if (!gpio_is_ready_dt(&led)) {
		printk("LED gpio is not ready!\n");
	}

	if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE) < 0) {
		printk("Failed to configure LED gpio!\n");
	}

	while(1) {
		if(ledFunction == 0) {
			ledStep();
		}
		else if (ledFunction == 1) {
			ledFade();
		}
		else {
			printk("Error: invalid value for ledFunction: %d\n", ledFunction);
		}
	}
}

// Define the threads
K_THREAD_DEFINE(tbutton_id, STACKSIZE, tbutton, NULL, NULL, NULL, TBUTTON_PRIORITY, 0,0);
K_THREAD_DEFINE(tcore_id, STACKSIZE, tcore, NULL, NULL, NULL, TCORE_PRIORITY, 0, 0);
K_THREAD_DEFINE(tled_id, STACKSIZE, tled, NULL, NULL, NULL, TLED_PRIORITY, 0, 0);