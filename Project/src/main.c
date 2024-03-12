#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <string.h>

// GPIO definitions 
static const struct gpio_dt_spec ShiftDataIn 		= GPIO_DT_SPEC_GET(DT_ALIAS(d2), gpios);
static const struct gpio_dt_spec ShiftOutputEnable 	= GPIO_DT_SPEC_GET(DT_ALIAS(d3), gpios);
static const struct gpio_dt_spec ShiftClock 		= GPIO_DT_SPEC_GET(DT_ALIAS(d4), gpios);
static const struct gpio_dt_spec MuxA 				= GPIO_DT_SPEC_GET(DT_ALIAS(d5), gpios);
static const struct gpio_dt_spec MuxB 				= GPIO_DT_SPEC_GET(DT_ALIAS(d6), gpios);
static const struct gpio_dt_spec MuxC 				= GPIO_DT_SPEC_GET(DT_ALIAS(d7), gpios);
static const struct gpio_dt_spec MuxD 				= GPIO_DT_SPEC_GET(DT_ALIAS(d8), gpios);
#define HIGH			1
#define LOW				0

int _initializeGPIO()
{
	//Checks if gpio is available
	if (!gpio_is_ready_dt(&ShiftDataIn) && !gpio_is_ready_dt(&ShiftOutputEnable) &&
		!gpio_is_ready_dt(&ShiftClock) && !gpio_is_ready_dt(&MuxA) &&
		!gpio_is_ready_dt(&MuxB) && !gpio_is_ready_dt(&MuxC) && !gpio_is_ready_dt(&MuxD)) 
	{
		return 1;
	}
	//configures the gpio
	int ret = 0;
	ret += gpio_pin_configure_dt(&ShiftDataIn, GPIO_OUTPUT_ACTIVE);
	ret += gpio_pin_configure_dt(&ShiftOutputEnable, GPIO_OUTPUT_ACTIVE);
	ret += gpio_pin_configure_dt(&ShiftClock, GPIO_OUTPUT_ACTIVE);
	ret += gpio_pin_configure_dt(&MuxA, GPIO_OUTPUT_ACTIVE);
	ret += gpio_pin_configure_dt(&MuxB, GPIO_OUTPUT_ACTIVE);
	ret += gpio_pin_configure_dt(&MuxC, GPIO_OUTPUT_ACTIVE);
	ret += gpio_pin_configure_dt(&MuxD, GPIO_OUTPUT_ACTIVE);
	//return when gpio is configured incorrectly
	if (ret < 0) 
	{
		return 1;
	}
	return 0;
}

void SendOneBitData(bool ShiftDataValue)
{
	gpio_pin_set_dt(&ShiftDataIn,ShiftDataValue);
	gpio_pin_set_dt(&ShiftClock,HIGH);
	gpio_pin_set_dt(&ShiftClock,LOW);
}

void LedMatrix8by8()
{
	

	gpio_pin_set_dt(&ShiftDataIn,LOW);
	gpio_pin_set_dt(&ShiftOutputEnable,LOW);
	gpio_pin_set_dt(&ShiftClock,LOW);
	gpio_pin_set_dt(&MuxA,LOW);
	gpio_pin_set_dt(&MuxB,LOW);
	gpio_pin_set_dt(&MuxC,LOW);
	k_sleep(K_MSEC(10));
	int count = 0;
	while (true)
	{
		if(count == 8)
		{
			count = 0;
		}

		for (size_t i = 0; i < 4; i++)
		{
			SendOneBitData(HIGH);
			SendOneBitData(HIGH);
			SendOneBitData(LOW);
			SendOneBitData(LOW);
		}
		
		gpio_pin_set_dt(&ShiftOutputEnable,HIGH);
		gpio_pin_set_dt(&ShiftOutputEnable,LOW);

		gpio_pin_set_dt(&MuxA,(count & 0x1));
		gpio_pin_set_dt(&MuxB,(count & 0x2));
		gpio_pin_set_dt(&MuxC,(count & 0x4));

		count++;
		k_sleep(K_MSEC(1));
	}
}


int LedMatrix16by16()
{
	

	gpio_pin_set_dt(&ShiftDataIn,LOW);
	gpio_pin_set_dt(&ShiftOutputEnable,LOW);
	gpio_pin_set_dt(&ShiftClock,LOW);
	gpio_pin_set_dt(&MuxA,LOW);
	gpio_pin_set_dt(&MuxB,LOW);
	gpio_pin_set_dt(&MuxC,LOW);
	gpio_pin_set_dt(&MuxD,LOW);
	k_sleep(K_MSEC(10));
	int count = 0;
	while (true)
	{
		if(count == 16)
		{
			count = 0;
		}

		
		SendOneBitData(HIGH);
		SendOneBitData(HIGH);
		SendOneBitData(HIGH);
		SendOneBitData(HIGH);
		SendOneBitData(HIGH);
		SendOneBitData(HIGH);
		SendOneBitData(HIGH);
		SendOneBitData(HIGH);
		SendOneBitData(HIGH);
		SendOneBitData(HIGH);
		SendOneBitData(HIGH);
		SendOneBitData(HIGH);
		SendOneBitData(HIGH);
		SendOneBitData(HIGH);
		SendOneBitData(HIGH);
		SendOneBitData(HIGH);
		
		gpio_pin_set_dt(&ShiftOutputEnable,HIGH);
		gpio_pin_set_dt(&ShiftOutputEnable,LOW);

		gpio_pin_set_dt(&MuxA,(count & 0x1));
		gpio_pin_set_dt(&MuxB,(count & 0x2));
		gpio_pin_set_dt(&MuxC,(count & 0x4));
		gpio_pin_set_dt(&MuxD,(count & 0x8));

		count++;
		k_sleep(K_USEC(300));
	}
	return 0;
}

int main(void)
{
	_initializeGPIO();
	LedMatrix16by16();

	

	return 0;
}
