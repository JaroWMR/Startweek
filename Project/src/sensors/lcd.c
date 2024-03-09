#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <string.h>

// GPIO definitions 
static const struct gpio_dt_spec Enable = GPIO_DT_SPEC_GET(DT_ALIAS(d2), gpios);
static const struct gpio_dt_spec RS 	= GPIO_DT_SPEC_GET(DT_ALIAS(d3), gpios);
static const struct gpio_dt_spec Data0 	= GPIO_DT_SPEC_GET(DT_ALIAS(d4), gpios);
static const struct gpio_dt_spec Data1 	= GPIO_DT_SPEC_GET(DT_ALIAS(d5), gpios);
static const struct gpio_dt_spec Data2 	= GPIO_DT_SPEC_GET(DT_ALIAS(d6), gpios);
static const struct gpio_dt_spec Data3 	= GPIO_DT_SPEC_GET(DT_ALIAS(d7), gpios);
static const struct gpio_dt_spec Data4 	= GPIO_DT_SPEC_GET(DT_ALIAS(d8), gpios);
static const struct gpio_dt_spec Data5 	= GPIO_DT_SPEC_GET(DT_ALIAS(d9), gpios);
static const struct gpio_dt_spec Data6 	= GPIO_DT_SPEC_GET(DT_ALIAS(d10), gpios);
static const struct gpio_dt_spec Data7 	= GPIO_DT_SPEC_GET(DT_ALIAS(d11), gpios);
#define HIGH			1
#define LOW				0

// LCD instructions
#define Clear           0b00000001          // replace all characters with ASCII 'space'
#define Home            0b00000010          // return cursor to first position on first line
#define EntryMode       0b00000110          // shift cursor from left to right on read/write
#define DisplayOff      0b00001000          // turn display off
#define DisplayOn       0b00001100          // display on, cursor off, don't blink character
#define FunctionReset   0b00110000          // reset the LCD
#define FunctionSet8bit 0b00111000          // 8-bit data, 2-line display, 5 x 7 font
#define SetCursor       0b10000000          // set cursor position
#define LineOne     0x00                    // start of line 1
#define LineTwo     0x40                    // start of line 2

// local variable
#define actioninstruction 0
#define actiondata 1
bool lastaction;

void _lcdWrite8bits(uint8_t bits)
{
	gpio_pin_set_dt(&Enable,LOW);
	k_sleep(K_USEC(1));

	gpio_pin_set_dt(&Data0,LOW);
	gpio_pin_set_dt(&Data1,LOW);
	gpio_pin_set_dt(&Data2,LOW);
	gpio_pin_set_dt(&Data3,LOW);
	gpio_pin_set_dt(&Data4,LOW);
	gpio_pin_set_dt(&Data5,LOW);
	gpio_pin_set_dt(&Data6,LOW);
	gpio_pin_set_dt(&Data7,LOW);
	

	if ((bits & BIT(0)) == BIT(0)) {
		gpio_pin_set_dt(&Data0,HIGH);
	}
	if ((bits & BIT(1)) == BIT(1)) {
		gpio_pin_set_dt(&Data1,HIGH);
	}
	if ((bits & BIT(2)) == BIT(2)) {
		gpio_pin_set_dt(&Data2,HIGH);
	}
	if ((bits & BIT(3)) == BIT(3)) {
		gpio_pin_set_dt(&Data3,HIGH);
	}
	if ((bits & BIT(4)) == BIT(4)) {
		gpio_pin_set_dt(&Data4,HIGH);
	}
	if ((bits & BIT(5)) == BIT(5)) {
		gpio_pin_set_dt(&Data5,HIGH);
	}
	if ((bits & BIT(6)) == BIT(6)) {
		gpio_pin_set_dt(&Data6,HIGH);
	}
	if ((bits & BIT(7)) == BIT(7)) {
		gpio_pin_set_dt(&Data7,HIGH);
	}

	gpio_pin_set_dt(&Enable,HIGH);
	k_sleep(K_USEC(1));
	gpio_pin_set_dt(&Enable,LOW);
	k_sleep(K_USEC(1));

}

void _lcdWriteInstruction(uint8_t bits)
{
	if(lastaction == actiondata)
	{
		gpio_pin_set_dt(&RS,LOW);
		k_sleep(K_USEC(1));
	}
	lastaction = actioninstruction;
	_lcdWrite8bits(bits);
}

void _lcdWriteData(uint8_t bits)
{
	if(lastaction == actioninstruction)
	{
		gpio_pin_set_dt(&RS,HIGH);
		k_sleep(K_USEC(1));
	}
	lastaction = actiondata;
	_lcdWrite8bits(bits);
}

void _lcdStringWrite(char *msg)
{
	int i;
	int len = 0;
	uint8_t data;
	
	_lcdWriteInstruction(Clear);
	_lcdWriteInstruction(Home);

	len = strlen(msg);
	if (len <= 16)
	{
		for (i = 0; i < len; i++) {
		data = msg[i];
		_lcdWriteData(data);
		}
	}
	else if (len <= 32) {
		for (i = 0; i < 16; i++) {
		data = msg[i];
		_lcdWriteData(data);
		}	
	
		_lcdWriteInstruction(SetCursor | LineTwo);
		k_sleep(K_USEC(80));

		for (i = 16; i < len; i++) {
		data = msg[i];
		_lcdWriteData(data);
		}	
	}
	else {
		printk("Too long message! len %d %s\n", len, msg);
	}

	
}

int _initializeGPIO()
{
	//Checks if gpio is available
	if (!gpio_is_ready_dt(&Data1) && !gpio_is_ready_dt(&Data2) &&
		!gpio_is_ready_dt(&Data3) && !gpio_is_ready_dt(&Data4) &&
		!gpio_is_ready_dt(&Data5) && !gpio_is_ready_dt(&Data6) &&
		!gpio_is_ready_dt(&Data7) && !gpio_is_ready_dt(&Data0) &&
		!gpio_is_ready_dt(&Enable) && !gpio_is_ready_dt(&RS)) 
	{
		return 1;
	}
	//configures the gpio
	int ret = 0;
	ret += gpio_pin_configure_dt(&Data0, GPIO_OUTPUT_ACTIVE);
	ret += gpio_pin_configure_dt(&Data1, GPIO_OUTPUT_ACTIVE);
	ret += gpio_pin_configure_dt(&Data2, GPIO_OUTPUT_ACTIVE);
	ret += gpio_pin_configure_dt(&Data3, GPIO_OUTPUT_ACTIVE);
	ret += gpio_pin_configure_dt(&Data4, GPIO_OUTPUT_ACTIVE);
	ret += gpio_pin_configure_dt(&Data5, GPIO_OUTPUT_ACTIVE);
	ret += gpio_pin_configure_dt(&Data6, GPIO_OUTPUT_ACTIVE);
	ret += gpio_pin_configure_dt(&Data7, GPIO_OUTPUT_ACTIVE);
	ret += gpio_pin_configure_dt(&Enable, GPIO_OUTPUT_ACTIVE);
	ret += gpio_pin_configure_dt(&RS, GPIO_OUTPUT_ACTIVE);
	//return when gpio is configured incorrectly
	if (ret < 0) 
	{
		return 1;
	}
	return 0;
}

void _initializeLCD()
{
	lastaction = actiondata;
	k_sleep(K_MSEC(50)); // Wait for startup
	
	// Reset LCD
	_lcdWriteInstruction(FunctionReset);
	k_sleep(K_MSEC(10));			/* wait for 10ms */
	
	_lcdWriteInstruction(FunctionReset);
	k_sleep(K_USEC(200));			/* wait for 200us */

	_lcdWriteInstruction(FunctionReset);
	k_sleep(K_USEC(200));			/* wait for 200us */
	// end Reset lcd

	_lcdWriteInstruction(FunctionSet8bit);
	k_sleep(K_USEC(80));			/* wait for 80us */

	_lcdWriteInstruction(DisplayOff);
	k_sleep(K_USEC(80));			/* wait for 80us */
	
	_lcdWriteInstruction(Clear);
	k_sleep(K_USEC(80));			/* wait for 80us */

	_lcdWriteInstruction(EntryMode);
	k_sleep(K_USEC(80));			/* wait for 80us */

	_lcdWriteInstruction(DisplayOn);
	k_sleep(K_USEC(80));			/* wait for 80us */
}

void lcd_initialize()
{
	_initializeGPIO();
	_initializeLCD();
}

void lcd_writeData(char *msg)
{
	_lcdStringWrite(msg);
}

void lcd_enable()
{

}
