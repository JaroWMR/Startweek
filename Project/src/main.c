#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/gpio.h>

// The stack size 
#define STACKSIZE 1024

#define THREAD0_PRIORITY 7
#define THREAD1_PRIORITY 7

int brightness = 0;
int fadeAmount = 5;
int fadeDelay = 30;

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)

/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

void thread0(void) // printing thread
{
	int cnt = 0;
	while (1) {
		printk("Hello, I am thread0\t%d\n", cnt);
		k_msleep(1000);
		cnt++;
	}
}

void thread1(void) // led thread
{
	// int cnt = 0;
	// while (1) {
	// 	printk("Hello, I am thread1\t%d\n", cnt);
	// 	k_msleep(3000);
	// 	cnt++;
	// }

	for (int i = 0; i < 255; i+= fadeAmount) {
		//setBrightness(i);
		gpio_pin_toggle_dt(&led);
		printk("LED!\n");
		k_msleep(500);
	}

	// for (int i = 255; i < 0; i-= fadeAmount) {
	// 	setBrightness(i);
	// 	k_msleep(fadeDelay);
	// }
}

void setBrightness(int brightness) {
	float dutyCycle = brightness / 255.0;

	int onTime = dutyCycle * fadeDelay;

	gpio_pin_toggle_dt(&led);
	k_msleep(fadeDelay);

	gpio_pin_toggle_dt(&led);
	k_msleep(fadeDelay-onTime);
}

// Define the threads
K_THREAD_DEFINE(thread0_id, STACKSIZE, thread0, NULL, NULL, NULL, THREAD0_PRIORITY, 0, 0);
K_THREAD_DEFINE(thread1_id, STACKSIZE, thread1, NULL, NULL, NULL, THREAD1_PRIORITY, 0, 0);