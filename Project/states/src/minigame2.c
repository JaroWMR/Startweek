#include "minigame2.h"
#include <zephyr/kernel.h>
#include <stdio.h>
#include "threads.h"
#include "buttonMatrix.h"
#include <zephyr/random/random.h>

char *mg2Threads[mg2ThreadCount] = {"startbtn", "btnmatrix_in", "btnmatrix_out", "buzzers"};

void getMg2Threads(char ***names, unsigned *amount) {
	*names = mg2Threads;
	*amount = mg2ThreadCount;
}

int playMg2() {
	uint32_t score = 1000;
	uint8_t *btnValues;
	uint8_t sequence[8] = {0,0,0,0,0,0,0,0};
	uint8_t level = 0;
	bool genValue = false;
	while (true)
	{
		if(score == 0 || level == 8)
		{
			printf("end\n");
			break;
		}
		if(!genValue)
		{
			for (uint8_t i = 0; i < 8; i++)
			{
				uint8_t randomValue = 0;
				sys_rand_get(&randomValue, sizeof(randomValue));
				randomValue = randomValue & (0b11);
				printf("randomValue %d,\n",randomValue);
				sequence[i] = randomValue;
			}
			genValue = true;
			level = 0;
		}
		uint8_t data[4] = {0b00000000,0b00000000,0b00000000,0b00000000};
		switch (sequence[level])
		{
		case 3:
		data[0] = 0b00001000;
			break;
		case 2:
		data[0] = 0b00000001;
			break;
		case 1:
		data[4] = 0b00001000;
			break;
		case 0:
		data[4] = 0b00000001;
			break;
		default:
			//should not reach
			break;
		}	
		btnmatrix_outSetMutexValue(data);		
		btnValues = btnmatrix_inGetMutexValue();
		if(!btnValues[15] || !btnValues[11] || !btnValues[3] || !btnValues[0])
		{
			switch (sequence[level])
			{
			case 3:
				if(!btnValues[15] && btnValues[11] && btnValues[3] && btnValues[0])
				{
					level++;
					printf("correct \n");
				}
				else
				{
					genValue = false;
					score -= 100;
					printf("incorrect \n");
				}
				break;
			case 2:
				if(btnValues[15] && !btnValues[11] && btnValues[3] && btnValues[0])
				{
					level++;
					printf("correct \n");
				}
				else
				{
					genValue = false;
					score -= 100;
					printf("incorrect \n");
				}
				break;
			case 1:
				if(btnValues[15] && btnValues[11] && !btnValues[3] && btnValues[0])
				{
					level++;
					printf("correct \n");
				}
				else
				{
					genValue = false;
					score -= 100;
					printf("incorrect \n");
				}
				break;
			case 0:
				if(btnValues[15] && btnValues[11] && btnValues[3] && !btnValues[0])
				{
					level++;
					printf("correct \n");
				}
				else
				{
					genValue = false;
					score -= 100;
					printf("incorrect \n");
				}
				break;
			default:
				//should not reach
				break;
			}	
		}
	}
	return 0;
}