#include <stdio.h>
#include <zephyr/kernel.h>
#include "bigMatrix.h"

/** 
 * @brief Shifts one bit of data in the shift register. 
 * 
 * Shifts one bit of data in the shift register by setting the correct value on the shiftdatain16x16 pin.
 * After setting the value the Clock is toggled so the bit is set in the register.
 * 
 * @param[in] ShiftDataValue – Value of pin when shifting data 
 * 
 * @return No return
 */ 
void SendOneBitData(bool ShiftDataValue)
{
	gpio_pin_set_dt(&ShiftDataIn,ShiftDataValue);
	gpio_pin_set_dt(&ShiftClock,HIGH);
	gpio_pin_set_dt(&ShiftClock,LOW);
}

/** 
 * @brief Configures the bigLedMatrix.
 * 
 * Checks if the GPIO is available and configures the GPIO for its purpose
 * 
 * @return Returns a 0 on succes and a 1 on error.
 */ 
bool bigLedMatrixConfig()
{
	//Checks if gpio is available
	if (!gpio_is_ready_dt(&ShiftDataIn) && !gpio_is_ready_dt(&ShiftOutputEnable) &&
		!gpio_is_ready_dt(&ShiftClock) && !gpio_is_ready_dt(&MuxA) &&
		!gpio_is_ready_dt(&MuxB) && !gpio_is_ready_dt(&MuxC) && !gpio_is_ready_dt(&MuxD)) 
	{
		return 1;
	}
	//configures the gpio
	uint8_t ret = 0;
	ret += gpio_pin_configure_dt(&ShiftDataIn, GPIO_OUTPUT_ACTIVE);
	ret += gpio_pin_configure_dt(&ShiftOutputEnable, GPIO_OUTPUT_ACTIVE);
	ret += gpio_pin_configure_dt(&ShiftClock, GPIO_OUTPUT_ACTIVE);
	ret += gpio_pin_configure_dt(&MuxA, GPIO_OUTPUT_ACTIVE);
	ret += gpio_pin_configure_dt(&MuxB, GPIO_OUTPUT_ACTIVE);
	ret += gpio_pin_configure_dt(&MuxC, GPIO_OUTPUT_ACTIVE);
	ret += gpio_pin_configure_dt(&MuxD, GPIO_OUTPUT_ACTIVE);
	//return when gpio is configured incorrectly
	if (ret != 0) 
	{
		return 1;
	}
	return 0;
}

/** 
 * @brief Initializes the bigLedMatrix
 * 
 * Sets all pins from floating to 0
 * Clears the Shift registers by writing 16 times a 0.
 * 
 * @return Returns a 0 on succes and a 1 on error.
 */ 
int8_t bigLedMatrixInit ()
{
	uint8_t ret = 0;
	ret += gpio_pin_set_dt(&ShiftDataIn,LOW);
	ret += gpio_pin_set_dt(&ShiftOutputEnable,LOW);
	ret += gpio_pin_set_dt(&ShiftClock,LOW);
	ret += gpio_pin_set_dt(&MuxA,LOW);
	ret += gpio_pin_set_dt(&MuxB,LOW);
	ret += gpio_pin_set_dt(&MuxC,LOW);
	ret += gpio_pin_set_dt(&MuxD,LOW);

	if (ret != 0) 
	{
		return 1;
	}

	for (int i = 0; i < LEDSINROW; i++)
	{
		SendOneBitData(LOW);
	}

	return 0;
}

/** 
 * @brief Writes data to the whole matrix
 * 
 * By reading the values from the data variable each bit is defined and will be turned on in te correct sequence.
 * Each row is shifted into a registers and after each row the correct Mux is enabled to select the correct row.
 * 
 * @param[in] data – Array of int16_t with the size of ROWS. Every bit is equal to one led in the matrix.
 * 
 * @return Returns 0 if Configuration and initialization have been performs.
 * Returns a 1 if the Configuration has not been perfomed.
 * Returns a 2 if the Initialiazation has not been perfomed.
 * Returns a 3 if the Configuration and the Initialiazation has not been perfomed.
 */ 
int8_t bigLedMatrixSetLeds(int16_t data[ROWS])
{
	for (size_t row = 0; row < ROWS; row++)
	{
		for (size_t led = 0; led < LEDSINROW; led++)
		{
			if(data[row] & 0x1<<led)
			{
				SendOneBitData(HIGH);
			}
			else
			{
				SendOneBitData(LOW);
			}
		}

		gpio_pin_set_dt(&ShiftOutputEnable,HIGH);
		gpio_pin_set_dt(&ShiftOutputEnable,LOW);

		gpio_pin_set_dt(&MuxA,(row & 0x1));
		gpio_pin_set_dt(&MuxB,(row & 0x2));
		gpio_pin_set_dt(&MuxC,(row & 0x4));
		gpio_pin_set_dt(&MuxD,(row & 0x8));

		//TODO: determine this k_sleep delay
		k_sleep(K_USEC(100));
	}
	return 0;
}
