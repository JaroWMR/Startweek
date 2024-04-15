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
	gpio_pin_set_dt(&bigMatrixShiftDataIn,ShiftDataValue);
	gpio_pin_set_dt(&bigMatrixShiftClock,HIGH);
	gpio_pin_set_dt(&bigMatrixShiftClock,LOW);
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
	if (!gpio_is_ready_dt(&bigMatrixShiftDataIn) && !gpio_is_ready_dt(&bigMatrixShiftOutputEnable) &&
		!gpio_is_ready_dt(&bigMatrixShiftClock) && !gpio_is_ready_dt(&bigMatrixMuxA) &&
		!gpio_is_ready_dt(&bigMatrixMuxB) && !gpio_is_ready_dt(&bigMatrixMuxC) && !gpio_is_ready_dt(&bigMatrixMuxD)) 
	{
		return 1;
	}
	//configures the gpio
	uint8_t ret = 0;
	ret += gpio_pin_configure_dt(&bigMatrixShiftDataIn, GPIO_OUTPUT_ACTIVE);
	ret += gpio_pin_configure_dt(&bigMatrixShiftOutputEnable, GPIO_OUTPUT_ACTIVE);
	ret += gpio_pin_configure_dt(&bigMatrixShiftClock, GPIO_OUTPUT_ACTIVE);
	ret += gpio_pin_configure_dt(&bigMatrixMuxA, GPIO_OUTPUT_ACTIVE);
	ret += gpio_pin_configure_dt(&bigMatrixMuxB, GPIO_OUTPUT_ACTIVE);
	ret += gpio_pin_configure_dt(&bigMatrixMuxC, GPIO_OUTPUT_ACTIVE);
	ret += gpio_pin_configure_dt(&bigMatrixMuxD, GPIO_OUTPUT_ACTIVE);
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
	ret += gpio_pin_set_dt(&bigMatrixShiftDataIn,LOW);
	ret += gpio_pin_set_dt(&bigMatrixShiftOutputEnable,LOW);
	ret += gpio_pin_set_dt(&bigMatrixShiftClock,LOW);
	ret += gpio_pin_set_dt(&bigMatrixMuxA,LOW);
	ret += gpio_pin_set_dt(&bigMatrixMuxB,LOW);
	ret += gpio_pin_set_dt(&bigMatrixMuxC,LOW);
	ret += gpio_pin_set_dt(&bigMatrixMuxD,LOW);

	if (ret != 0) 
	{
		return 1;
	}
	for (size_t row = 0; row < ROWS; row++)
	{
		for (int i = 0; i < LEDSINROW; i++)
		{
			SendOneBitData(LOW);
		}
		gpio_pin_set_dt(&bigMatrixShiftOutputEnable,HIGH);
		gpio_pin_set_dt(&bigMatrixShiftOutputEnable,LOW);

		gpio_pin_set_dt(&bigMatrixMuxA,(row & 0x1));
		gpio_pin_set_dt(&bigMatrixMuxB,(row & 0x2));
		gpio_pin_set_dt(&bigMatrixMuxC,(row & 0x4));
		gpio_pin_set_dt(&bigMatrixMuxD,(row & 0x8));
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

		gpio_pin_set_dt(&bigMatrixShiftOutputEnable,HIGH);
		gpio_pin_set_dt(&bigMatrixShiftOutputEnable,LOW);

		gpio_pin_set_dt(&bigMatrixMuxA,(row & 0x1));
		gpio_pin_set_dt(&bigMatrixMuxB,(row & 0x2));
		gpio_pin_set_dt(&bigMatrixMuxC,(row & 0x4));
		gpio_pin_set_dt(&bigMatrixMuxD,(row & 0x8));

		//TODO: determine this k_sleep delay
		k_sleep(K_USEC(100));
	}
	return 0;
}
