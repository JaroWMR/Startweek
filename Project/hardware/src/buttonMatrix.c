#include <stdio.h>
#include <zephyr/kernel.h>
#include "smallMatrix.h"

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
void smallMatrixSendOneBitData(bool ShiftDataValue)
{
	gpio_pin_set_dt(&smallMatrixShiftDataIn,ShiftDataValue);
	gpio_pin_set_dt(&smallMatrixShiftClock,HIGH);
	gpio_pin_set_dt(&smallMatrixShiftClock,LOW);
}

/** 
 * @brief Configures the smallMatrix.
 * 
 * Checks if the GPIO is available and configures the GPIO for its purpose
 * 
 * @return Returns a 0 on succes and a 1 on error.
 */ 
bool smallMatrixConfig()
{
	//Checks if gpio is available
	if (!gpio_is_ready_dt(&smallMatrixShiftDataIn) && !gpio_is_ready_dt(&smallMatrixShiftOutputEnable) &&
		!gpio_is_ready_dt(&smallMatrixShiftClock) && !gpio_is_ready_dt(&smallMatrixMuxA) &&
		!gpio_is_ready_dt(&smallMatrixMuxB)) 
	{
		return 1;
	}
	//configures the gpio
	uint8_t ret = 0;
	ret += gpio_pin_configure_dt(&smallMatrixShiftDataIn, GPIO_OUTPUT_ACTIVE);
	ret += gpio_pin_configure_dt(&smallMatrixShiftOutputEnable, GPIO_OUTPUT_ACTIVE);
	ret += gpio_pin_configure_dt(&smallMatrixShiftClock, GPIO_OUTPUT_ACTIVE);
	ret += gpio_pin_configure_dt(&smallMatrixMuxA, GPIO_OUTPUT_ACTIVE);
	ret += gpio_pin_configure_dt(&smallMatrixMuxB, GPIO_OUTPUT_ACTIVE);
	//return when gpio is configured incorrectly
	if (ret != 0) 
	{
		return 1;
	}
	return 0;
}

/** 
 * @brief Initializes the smallMatrix
 * 
 * Sets all pins from floating to 0
 * Clears the Shift registers by writing 16 times a 0.
 * 
 * @return Returns a 0 on succes and a 1 on error.
 */ 
int8_t smallMatrixInit ()
{
	uint8_t ret = 0;
	ret += gpio_pin_set_dt(&smallMatrixShiftDataIn,LOW);
	ret += gpio_pin_set_dt(&smallMatrixShiftOutputEnable,LOW);
	ret += gpio_pin_set_dt(&smallMatrixShiftClock,LOW);
	ret += gpio_pin_set_dt(&smallMatrixMuxA,LOW);
	ret += gpio_pin_set_dt(&smallMatrixMuxB,LOW);

	if (ret != 0) 
	{
		return 1;
	}
	for (size_t row = 0; row < ROWS; row++)
	{
		for (int i = 0; i < LEDSINROW; i++)
		{
			smallMatrixSendOneBitData(LOW);
		}
		gpio_pin_set_dt(&smallMatrixShiftOutputEnable,HIGH);
		gpio_pin_set_dt(&smallMatrixShiftOutputEnable,LOW);

		gpio_pin_set_dt(&smallMatrixMuxA,(row & 0x1));
		gpio_pin_set_dt(&smallMatrixMuxB,(row & 0x2));
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
int8_t smallMatrixSets(int8_t data[ROWS])
{
	for (size_t row = 0; row < ROWS; row++)
	{
		for (size_t led = 0; led < LEDSINROW; led++)
		{
			if(data[row] & 0x1<<led)
			{
				smallMatrixSendOneBitData(HIGH);
			}
			else
			{
				smallMatrixSendOneBitData(LOW);
			}
		}

		gpio_pin_set_dt(&smallMatrixShiftOutputEnable,HIGH);
		gpio_pin_set_dt(&smallMatrixShiftOutputEnable,LOW);

		gpio_pin_set_dt(&smallMatrixMuxA,(row & 0x1));
		gpio_pin_set_dt(&smallMatrixMuxB,(row & 0x2));

		//TODO: determine this k_sleep delay
		k_sleep(K_USEC(100));
	}
	return 0;
}
