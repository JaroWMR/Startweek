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

//define btn
static const struct gpio_dt_spec btn1 = GPIO_DT_SPEC_GET_OR(DT_ALIAS(r1), gpios,{0});
static const struct gpio_dt_spec btn2 = GPIO_DT_SPEC_GET_OR(DT_ALIAS(r2), gpios,{0});
static const struct gpio_dt_spec btn3 = GPIO_DT_SPEC_GET_OR(DT_ALIAS(r3), gpios,{0});
static const struct gpio_dt_spec btn4 = GPIO_DT_SPEC_GET_OR(DT_ALIAS(r4), gpios,{0});
static const struct gpio_dt_spec btn5 = GPIO_DT_SPEC_GET_OR(DT_ALIAS(r5), gpios,{0});
static const struct gpio_dt_spec btn6 = GPIO_DT_SPEC_GET_OR(DT_ALIAS(r6), gpios,{0});
static const struct gpio_dt_spec btn7 = GPIO_DT_SPEC_GET_OR(DT_ALIAS(r7), gpios,{0});
static const struct gpio_dt_spec btn8 = GPIO_DT_SPEC_GET_OR(DT_ALIAS(r8), gpios,{0});
static const struct gpio_dt_spec btn9 = GPIO_DT_SPEC_GET_OR(DT_ALIAS(r9), gpios,{0});
static const struct gpio_dt_spec btn10 = GPIO_DT_SPEC_GET_OR(DT_ALIAS(r10), gpios,{0});
static const struct gpio_dt_spec btn11 = GPIO_DT_SPEC_GET_OR(DT_ALIAS(r11), gpios,{0});
static const struct gpio_dt_spec btn12 = GPIO_DT_SPEC_GET_OR(DT_ALIAS(r12), gpios,{0});
static const struct gpio_dt_spec btn13 = GPIO_DT_SPEC_GET_OR(DT_ALIAS(r13), gpios,{0});
static const struct gpio_dt_spec btn14 = GPIO_DT_SPEC_GET_OR(DT_ALIAS(r14), gpios,{0});
static const struct gpio_dt_spec btn15 = GPIO_DT_SPEC_GET_OR(DT_ALIAS(r15), gpios,{0});
static const struct gpio_dt_spec btn16 = GPIO_DT_SPEC_GET_OR(DT_ALIAS(r16), gpios,{0});
//define switches
static const struct gpio_dt_spec swtichon1 = GPIO_DT_SPEC_GET_OR(DT_ALIAS(r17), gpios,{0});
static const struct gpio_dt_spec swtichoff1 = GPIO_DT_SPEC_GET_OR(DT_ALIAS(r18), gpios,{0});
static const struct gpio_dt_spec swtichon2 = GPIO_DT_SPEC_GET_OR(DT_ALIAS(r19), gpios,{0});
static const struct gpio_dt_spec swtichoff2 = GPIO_DT_SPEC_GET_OR(DT_ALIAS(r20), gpios,{0});
static const struct gpio_dt_spec swtichon3 = GPIO_DT_SPEC_GET_OR(DT_ALIAS(r21), gpios,{0});
static const struct gpio_dt_spec swtichoff3 = GPIO_DT_SPEC_GET_OR(DT_ALIAS(r22), gpios,{0});
static const struct gpio_dt_spec swtichon4 = GPIO_DT_SPEC_GET_OR(DT_ALIAS(r23), gpios,{0});
static const struct gpio_dt_spec swtichoff4 = GPIO_DT_SPEC_GET_OR(DT_ALIAS(r24), gpios,{0});
static const struct gpio_dt_spec swtichon5 = GPIO_DT_SPEC_GET_OR(DT_ALIAS(r25), gpios,{0});
static const struct gpio_dt_spec swtichoff5 = GPIO_DT_SPEC_GET_OR(DT_ALIAS(r26), gpios,{0});
//define leds
static const struct gpio_dt_spec MUXa = GPIO_DT_SPEC_GET_OR(DT_ALIAS(r27), gpios,{0});
static const struct gpio_dt_spec MUXb = GPIO_DT_SPEC_GET_OR(DT_ALIAS(r28), gpios,{0});
static const struct gpio_dt_spec shiftdatain = GPIO_DT_SPEC_GET_OR(DT_ALIAS(r29), gpios,{0});
static const struct gpio_dt_spec outputenable = GPIO_DT_SPEC_GET_OR(DT_ALIAS(r30), gpios,{0});
static const struct gpio_dt_spec shiftclock = GPIO_DT_SPEC_GET_OR(DT_ALIAS(r31), gpios,{0});
//define rotary switch
static const struct gpio_dt_spec RotarySwitch = GPIO_DT_SPEC_GET_OR(DT_ALIAS(r32), gpios,{0});
								  
								  

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
