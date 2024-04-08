#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include "lcd.h"

// GPIO definitions 
static const struct gpio_dt_spec DP 		= GPIO_DT_SPEC_GET(DT_ALIAS(d13), gpios);
static const struct gpio_dt_spec bcdB 		= GPIO_DT_SPEC_GET(DT_ALIAS(d14), gpios);
static const struct gpio_dt_spec bcdC 		= GPIO_DT_SPEC_GET(DT_ALIAS(d15), gpios);
static const struct gpio_dt_spec bcdD 		= GPIO_DT_SPEC_GET(DT_ALIAS(d151), gpios);
static const struct gpio_dt_spec bcdA 		= GPIO_DT_SPEC_GET(DT_ALIAS(d141), gpios);
static const struct gpio_dt_spec MuxDig1 	= GPIO_DT_SPEC_GET(DT_ALIAS(d131), gpios);
static const struct gpio_dt_spec MuxDig2 	= GPIO_DT_SPEC_GET(DT_ALIAS(d121), gpios);
static const struct gpio_dt_spec MuxDig3 	= GPIO_DT_SPEC_GET(DT_ALIAS(d111), gpios);
static const struct gpio_dt_spec MuxDig4 	= GPIO_DT_SPEC_GET(DT_ALIAS(d101), gpios);
#define HIGH			1
#define LOW				0

int _initializeGPIOSegment()
{
	//Checks if gpio is available
	if (!gpio_is_ready_dt(&bcdA) 	&& !gpio_is_ready_dt(&bcdB) 	&&
		!gpio_is_ready_dt(&bcdC) 	&& !gpio_is_ready_dt(&bcdD) 	&&
		!gpio_is_ready_dt(&DP) 		&& !gpio_is_ready_dt(&MuxDig1) 	&&
		!gpio_is_ready_dt(&MuxDig2) && !gpio_is_ready_dt(&MuxDig3)	&& 
		!gpio_is_ready_dt(&MuxDig4)) 
	{
		return 1;
	}
	//configures the gpio
	int ret = 0;
	ret += gpio_pin_configure_dt(&bcdA, GPIO_OUTPUT_ACTIVE);
	ret += gpio_pin_configure_dt(&bcdB, GPIO_OUTPUT_ACTIVE);
	ret += gpio_pin_configure_dt(&bcdC, GPIO_OUTPUT_ACTIVE);
	ret += gpio_pin_configure_dt(&bcdD, GPIO_OUTPUT_ACTIVE);
	ret += gpio_pin_configure_dt(&DP, GPIO_OUTPUT_ACTIVE);
	ret += gpio_pin_configure_dt(&MuxDig1, GPIO_OUTPUT_ACTIVE);
	ret += gpio_pin_configure_dt(&MuxDig2, GPIO_OUTPUT_ACTIVE);
	ret += gpio_pin_configure_dt(&MuxDig3, GPIO_OUTPUT_ACTIVE);
	ret += gpio_pin_configure_dt(&MuxDig4, GPIO_OUTPUT_ACTIVE);
	//return when gpio is configured incorrectly
	if (ret < 0) 
	{
		return 1;
	}

	gpio_pin_set_dt(&bcdA,LOW);
	gpio_pin_set_dt(&bcdB,LOW);
	gpio_pin_set_dt(&bcdC,LOW);
	gpio_pin_set_dt(&bcdD,LOW);
	gpio_pin_set_dt(&DP,LOW);
	gpio_pin_set_dt(&MuxDig1,LOW);
	gpio_pin_set_dt(&MuxDig2,LOW);
	gpio_pin_set_dt(&MuxDig3,LOW);
	gpio_pin_set_dt(&MuxDig4,LOW);
	k_sleep(K_MSEC(5));

	return 0;
}



int main(void)
{
	lcd_initialize();
	lcd_enable();
	char str[20];
	for (size_t i = 10; i > 0; i--)
	{
		sprintf(str, "counting: %d", i);
		lcd_writeData(str);
		k_sleep(K_MSEC(1000));
	}
	lcd_writeData("counting: 0     done with test");
	k_sleep(K_MSEC(1000));
	lcd_disable();
	
	_initializeGPIOSegment();

		int a = 0;
	while (true)
	{		
		if(a == 4)
		{
			a =0;
		}
	
		switch (a)
		{
		case 0:
		gpio_pin_set_dt(&bcdA,HIGH);
		gpio_pin_set_dt(&bcdB,LOW);
		gpio_pin_set_dt(&bcdC,LOW);
		gpio_pin_set_dt(&bcdD,LOW);
		gpio_pin_set_dt(&DP,LOW);
		gpio_pin_set_dt(&MuxDig1,HIGH);
		gpio_pin_set_dt(&MuxDig2,LOW);
		gpio_pin_set_dt(&MuxDig3,LOW);
		gpio_pin_set_dt(&MuxDig4,LOW);
			break;
		case 1:
		gpio_pin_set_dt(&bcdA,HIGH);
		gpio_pin_set_dt(&bcdB,HIGH);
		gpio_pin_set_dt(&bcdC,LOW);
		gpio_pin_set_dt(&bcdD,LOW);
		gpio_pin_set_dt(&DP,LOW);
		gpio_pin_set_dt(&MuxDig1,LOW);
		gpio_pin_set_dt(&MuxDig2,HIGH);
		gpio_pin_set_dt(&MuxDig3,LOW);
		gpio_pin_set_dt(&MuxDig4,LOW);
			break;
		case 2:
		gpio_pin_set_dt(&bcdA,LOW);
		gpio_pin_set_dt(&bcdB,LOW);
		gpio_pin_set_dt(&bcdC,HIGH);
		gpio_pin_set_dt(&bcdD,LOW);
		gpio_pin_set_dt(&DP,LOW);
		gpio_pin_set_dt(&MuxDig1,LOW);
		gpio_pin_set_dt(&MuxDig2,LOW);
		gpio_pin_set_dt(&MuxDig3,HIGH);
		gpio_pin_set_dt(&MuxDig4,LOW);
			break;
		case 3:
		gpio_pin_set_dt(&bcdA,HIGH);
		gpio_pin_set_dt(&bcdB,LOW);
		gpio_pin_set_dt(&bcdC,HIGH);
		gpio_pin_set_dt(&bcdD,LOW);
		gpio_pin_set_dt(&DP,LOW); 
		gpio_pin_set_dt(&MuxDig1,LOW);
		gpio_pin_set_dt(&MuxDig2,LOW);
		gpio_pin_set_dt(&MuxDig3,LOW);
		gpio_pin_set_dt(&MuxDig4,HIGH);
			break;
		default:
		gpio_pin_set_dt(&MuxDig1,LOW);
		gpio_pin_set_dt(&MuxDig2,LOW);
		gpio_pin_set_dt(&MuxDig3,LOW);
		gpio_pin_set_dt(&MuxDig4,LOW);
			break;
		}
		k_sleep(K_USEC(1));
		a++;
	}
	
	return 0;
}
