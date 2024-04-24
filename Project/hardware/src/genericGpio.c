#include "genericGpio.h"

/** 
 * @brief Configures the buttonsinsmallmatrix.
 * 
 * Checks if the GPIO is available and configures the GPIO for its purpose
 * 
 * @return Returns a 0 on succes and a 1 on error.
 */ 
bool buttons4x4Config()
{
	//check if gpio is available
	uint8_t ret = 0;
	uint8_t amount = 16;
	for (uint8_t i = 0; i < amount; i++)
	{
		ret += gpio_is_ready_dt(&buttons4x4[i]);
	}
	if(ret != amount)
	{
		return 1;
	}
	ret = 0;
	for (uint8_t i = 0; i < amount; i++)
	{
		ret += gpio_pin_configure_dt(&buttons4x4[i],GPIO_INPUT);
	}
	if (ret != 0) {
		return 1;
	}
	return 0;
}

/** 
 * @brief Configures the switches.
 * 
 * Checks if the GPIO is available and configures the GPIO for its purpose
 * 
 * @return Returns a 0 on succes and a 1 on error.
 */ 
bool switchesConfig()
{
	//check if gpio is available
	uint8_t ret = 0;
	uint8_t amount = 5;
	for (uint8_t i = 0; i < amount; i++)
	{
		ret += gpio_is_ready_dt(&switchon[i]);
	}
	for (uint8_t i = 0; i < amount; i++)
	{
		ret += gpio_is_ready_dt(&switchoff[i]);
	}
	if(ret != (amount*2))
	{
		return 1;
	}
	ret = 0;
	for (uint8_t i = 0; i < amount; i++)
	{
		ret += gpio_pin_configure_dt(&switchon[i],GPIO_INPUT);
	}
	for (uint8_t i = 0; i < amount; i++)
	{
		ret += gpio_pin_configure_dt(&switchoff[i],GPIO_INPUT);
	}
	if (ret != 0) {
		return 1;
	}
	
	return 0;
}

/** 
 * @brief Configures the abcbuttons.
 * 
 * Checks if the GPIO is available and configures the GPIO for its purpose
 * 
 * @return Returns a 0 on succes and a 1 on error.
 */ 
bool abcbuttonsConfig()
{
	//check if gpio is available
	uint8_t ret = 0;
	uint8_t amount = 3;
	for (uint8_t i = 0; i < amount; i++)
	{
		ret += gpio_is_ready_dt(&abcbutton[i]);
	}
	if(ret != amount)
	{
		return 1;
	}
	ret = 0;
	for (uint8_t i = 0; i < amount; i++)
	{
		ret += gpio_pin_configure_dt(&abcbutton[i],GPIO_INPUT);
	}

	return 0;
}

/** 
 * @brief Configures the abcleds.
 * 
 * Checks if the GPIO is available and configures the GPIO for its purpose
 * 
 * @return Returns a 0 on succes and a 1 on error.
 */ 
bool abcledsConfig()
{
	//check if gpio is available
	uint8_t ret = 0;
	uint8_t amount = 3;
	for (uint8_t i = 0; i < amount; i++)
	{
		ret += gpio_is_ready_dt(&abcleds[i]);
	}
	if(ret != amount)
	{
		return 1;
	}
	ret = 0;

	for (uint8_t i = 0; i < amount; i++)
	{
		ret += gpio_pin_configure_dt(&abcleds[i], GPIO_OUTPUT_ACTIVE);
	}

	if (ret != 0) 
	{
		return 1;
	}
	return 0;
}

/** 
 * @brief Configures the button en led for start.
 * 
 * Checks if the GPIO is available and configures the GPIO for its purpose
 * 
 * @return Returns a 0 on succes and a 1 on error.
 */ 
bool startbuttonledConfig()
{
	//Checks if gpio is available
	if (!gpio_is_ready_dt(&startbutton) && !gpio_is_ready_dt(&startled)) 
	{
		return 1;
	}
	//configures the gpio
	uint8_t ret = 0;
	ret += gpio_pin_configure_dt(&startled, GPIO_OUTPUT_ACTIVE);
	ret += gpio_pin_configure_dt(&startbutton, GPIO_INPUT);
	//return when gpio is configured incorrectly
	if (ret != 0) 
	{
		return 1;
	}
	return 0;
}

/** 
 * @brief Initializes the buttonsinsmallmatrix
 * 
 * Sets all pins from floating to 1
 * 
 * @return Returns a 0 on succes and a 1 on error.
 */ 
uint8_t buttons4x4Init()
{
	uint8_t ret = 0;
	uint8_t amount = 16;
	for (uint8_t i = 0; i < amount; i++)
	{
		ret += gpio_pin_configure_dt(&buttons4x4[i], GPIO_PULL_UP);
	}
	if (ret != 0) 
	{
		return 1;
	}
	return 0;
}

/** 
 * @brief Initializes the switches
 * 
 * Sets all pins from floating to 1
 * 
 * @return Returns a 0 on succes and a 1 on error.
 */ 
uint8_t switchesInit()
{
	uint8_t ret = 0;
	uint8_t amount = 5;
	for (uint8_t i = 0; i < amount; i++)
	{
		ret += gpio_pin_configure_dt(&switchon[i], GPIO_PULL_UP);
	}
	for (uint8_t i = 0; i < amount; i++)
	{
		ret += gpio_pin_configure_dt(&switchoff[i], GPIO_PULL_UP);
	}
	if (ret != 0) 
	{
		return 1;
	}
	return 0;
}

/** 
 * @brief Initializes the abc buttons
 * 
 * Sets all pins from floating to 1
 * 
 * @return Returns a 0 on succes and a 1 on error.
 */ 
uint8_t abcbuttonsInit()
{
	uint8_t ret = 0;
	uint8_t amount = 3;
	for (uint8_t i = 0; i < amount; i++)
	{
		ret += gpio_pin_configure_dt(&abcbutton[i], GPIO_PULL_UP);
	}
	if (ret != 0) 
	{
		return 1;
	}
	return 0;
}

/** 
 * @brief Initializes the abc leds
 * 
 * Sets all pins from floating to 0
 * 
 * @return Returns a 0 on succes and a 1 on error.
 */ 

uint8_t abcledsInit()
{
	uint8_t ret = 0;
	uint8_t amount = 3;
	for (uint8_t i = 0; i < amount; i++)
	{
		ret += gpio_pin_set_dt(&abcleds[i],LOW);
	}
	if (ret != 0) 
	{
		return 1;
	}
	return 0;
}


/** 
 * @brief Initializes the buttonsinsmallmatrix
 * 
 * Sets button pin from floating to 1
 * Sets led pin from floating to 0
 * 
 * @return Returns a 0 on succes and a 1 on error.
 */ 
uint8_t startbuttonledInit()
{
	uint8_t ret = 0;
	ret += gpio_pin_configure_dt(&startbutton, GPIO_PULL_UP);
	ret += gpio_pin_set_dt(&startled,LOW);
	if (ret != 0) 
	{
		return 1;
	}
	return 0;
}
