#include <stdio.h>
#include <zephyr/kernel.h>
#include "lcd.h"

int main(void)
{
	lcd_initialize();
	char str[20];
	for (size_t i = 10; i > 0; i--)
	{
		sprintf(str, "counting: %d", i);
		lcd_writeData(str);
		k_sleep(K_MSEC(1000));
	}
	lcd_writeData("counting: 0     done with test");
	return 0;
}