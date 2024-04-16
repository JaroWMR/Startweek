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



static const struct gpio_dt_spec buttons[16] = {
	//GPIO_DT_SPEC_GET_OR(DT_ALIAS(r0), gpios, {0}),
    //GPIO_DT_SPEC_GET_OR(DT_ALIAS(r1), gpios, {0}),
    GPIO_DT_SPEC_GET_OR(DT_ALIAS(r2), gpios, {0}),
    GPIO_DT_SPEC_GET_OR(DT_ALIAS(r3), gpios, {0}),
    GPIO_DT_SPEC_GET_OR(DT_ALIAS(r4), gpios, {0}),
    GPIO_DT_SPEC_GET_OR(DT_ALIAS(r5), gpios, {0}),
    GPIO_DT_SPEC_GET_OR(DT_ALIAS(r6), gpios, {0}),
    GPIO_DT_SPEC_GET_OR(DT_ALIAS(r7), gpios, {0}),
    GPIO_DT_SPEC_GET_OR(DT_ALIAS(r8), gpios, {0}),
    GPIO_DT_SPEC_GET_OR(DT_ALIAS(r9), gpios, {0}),
    GPIO_DT_SPEC_GET_OR(DT_ALIAS(r10), gpios, {0}),
    GPIO_DT_SPEC_GET_OR(DT_ALIAS(r11), gpios, {0}),
    GPIO_DT_SPEC_GET_OR(DT_ALIAS(r12), gpios, {0}),
    GPIO_DT_SPEC_GET_OR(DT_ALIAS(r13), gpios, {0}),
    GPIO_DT_SPEC_GET_OR(DT_ALIAS(r14), gpios, {0}),
    GPIO_DT_SPEC_GET_OR(DT_ALIAS(r15), gpios, {0})
};
//define switches
static const struct gpio_dt_spec switchon[5] = {
    GPIO_DT_SPEC_GET_OR(DT_ALIAS(r16), gpios, {0}),
    GPIO_DT_SPEC_GET_OR(DT_ALIAS(r18), gpios, {0}),
    GPIO_DT_SPEC_GET_OR(DT_ALIAS(r20), gpios, {0}),
    GPIO_DT_SPEC_GET_OR(DT_ALIAS(r22), gpios, {0}),
    GPIO_DT_SPEC_GET_OR(DT_ALIAS(r24), gpios, {0}),
};

static const struct gpio_dt_spec switchoff[5] = {
    GPIO_DT_SPEC_GET_OR(DT_ALIAS(r17), gpios, {0}),
    GPIO_DT_SPEC_GET_OR(DT_ALIAS(r19), gpios, {0}),
    GPIO_DT_SPEC_GET_OR(DT_ALIAS(r21), gpios, {0}),
    GPIO_DT_SPEC_GET_OR(DT_ALIAS(r23), gpios, {0}),
    GPIO_DT_SPEC_GET_OR(DT_ALIAS(r25), gpios, {0}),
};

//define leds
static const struct gpio_dt_spec MUXa = GPIO_DT_SPEC_GET_OR(DT_ALIAS(r27), gpios,{0});
static const struct gpio_dt_spec MUXb = GPIO_DT_SPEC_GET_OR(DT_ALIAS(r26), gpios,{0});
static const struct gpio_dt_spec shiftdatain = GPIO_DT_SPEC_GET_OR(DT_ALIAS(r28), gpios,{0});
static const struct gpio_dt_spec outputenable = GPIO_DT_SPEC_GET_OR(DT_ALIAS(r29), gpios,{0});
static const struct gpio_dt_spec shiftclock = GPIO_DT_SPEC_GET_OR(DT_ALIAS(r30), gpios,{0});
//define rotary switch
//static const struct gpio_dt_spec RotarySwitch = GPIO_DT_SPEC_GET_OR(DT_ALIAS(r32), gpios,{0});
								  

#define HIGH 1
#define LOW 0

bool btnState[16] = {false};

bool switchState[5] = {false};


bool temp1;
bool temp2;

void gpio_init()
{
	int ret = 0;	
	for (uint8_t i = 0; i < 14; i++)
	{
		ret += gpio_pin_configure_dt(&buttons[i],GPIO_INPUT);
	}

	for (uint8_t i = 0; i < 5; i++)
	{
		ret += gpio_pin_configure_dt(&switchoff[i],GPIO_INPUT);
	}

	for (uint8_t i = 0; i < 5; i++)
	{
		ret += gpio_pin_configure_dt(&switchon[i],GPIO_INPUT);
	}


	ret += gpio_pin_configure_dt(&MUXa, GPIO_OUTPUT_ACTIVE);
	ret += gpio_pin_configure_dt(&MUXb, GPIO_OUTPUT_ACTIVE);
	ret += gpio_pin_configure_dt(&shiftdatain, GPIO_OUTPUT_ACTIVE);
	ret += gpio_pin_configure_dt(&outputenable, GPIO_OUTPUT_ACTIVE);
	ret += gpio_pin_configure_dt(&shiftclock, GPIO_OUTPUT_ACTIVE);


	if (ret != 0) {
		printk("Error");
		while(true){}
	}
}

void SendOneBitData(bool ShiftDataValue)
{
	gpio_pin_set_dt(&shiftdatain,ShiftDataValue);
	k_busy_wait(10000);
	gpio_pin_set_dt(&shiftclock,HIGH);
	k_busy_wait(10000);
	gpio_pin_set_dt(&shiftclock,LOW);
}

int count = 0;
void LedMatrix()
{
	
	if(count == 4)
	{
		count = 0;
	}

	for (size_t i = 0; i < 4; i++)
	{
		SendOneBitData(HIGH);
	}
	
	gpio_pin_set_dt(&outputenable,HIGH);
	gpio_pin_set_dt(&outputenable,LOW);
	volatile bool siem = (count & 0b1);
	gpio_pin_set_dt(&MUXa,(count & 0b1));
	gpio_pin_set_dt(&MUXb,(count & 0b10));

	count++;
	k_busy_wait(1000);
	
}



int main(void){
	

	gpio_init();
	gpio_pin_set_dt(&shiftdatain,LOW);
	gpio_pin_set_dt(&outputenable,LOW);
	gpio_pin_set_dt(&shiftclock,LOW);
	gpio_pin_set_dt(&MUXa,HIGH);
	gpio_pin_set_dt(&MUXb,HIGH);
	k_sleep(K_MSEC(10));

	
	printf("Begin Test\n");
	
	while(true)
	{
		for (int i = 0; i < 14; i++)
		{
			temp1 = gpio_pin_get(buttons[i].port, buttons[i].pin);
			if (temp1 != btnState[i]) {
				printf("btn%d: %d\n", i, temp1);
				btnState[i] = temp1;
			}
		}

		for (int i = 0; i < 5; i++)
		{
			temp1 = gpio_pin_get(switchon[i].port, switchon[i].pin);
			temp2 = gpio_pin_get(switchoff[i].port, switchoff[i].pin);
			if (temp1 != switchState[i]) {
				printf("btn%d: %d\n", i, temp1);
				switchState[i] = (bool)temp1;
			}
		}

		if (switchState[0] == false && switchState[1] == true && switchState[2] == false && switchState[3] == true && switchState[4] == false)
		{
			LedMatrix();
		}
	}
	return 0;
}
