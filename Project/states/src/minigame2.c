#include "minigame2.h"
#include <zephyr/kernel.h>
#include <stdio.h>
#include "threads.h"
#include "buttonMatrix.h"
#include <zephyr/random/random.h>

K_TIMER_DEFINE(secTimer, NULL, NULL);


char *mg2Threads[mg2ThreadCount] = {"startbtn", "btnmatrix_in", "btnmatrix_out", "buzzers"};

void getMg2Threads(char ***names, unsigned *amount) {
	*names = mg2Threads;
	*amount = mg2ThreadCount;
}

void generateSequence(uint8_t *sequence,uint8_t size,uint8_t bits) {
	for (uint8_t i = 0; i < size; i++)	{
			uint8_t randomValue = 0;
			sys_rand_get(&randomValue, sizeof(randomValue));
			randomValue = randomValue & (bits);
			sequence[i] = randomValue;
			printf("radnomvalue: %d\n",randomValue);
			printf("random = sequence[%d]: %d\n",i,sequence[i]);
		}
}

void showButton(uint8_t btn)
{
	uint8_t data[4] = {0b00000000,0b00000000,0b00000000,0b00000000};
	switch (btn)
	{
	case 3:
	data[0] = 0b00001000;
		break;
	case 2:
	data[0] = 0b00000001;
		break;
	case 1:
	data[3] = 0b00001000;
		break;
	case 0:
	data[3] = 0b00000001;
		break;
	default:
		//should not reach
		break;
	}	
	btnmatrix_outSetMutexValue(data);
}

void showLevel(uint8_t level,uint8_t sequence[8]){
	for (uint8_t i = 0; i < level+1; i++)
	{
		showButton(sequence[i]);
		printf("sequence[%d]: %d\n",i,sequence[i]);
		k_timer_start(&secTimer, K_MSEC(1000), K_NO_WAIT);
		while (!(k_timer_status_get(&secTimer) > 0)){}	
		showButton(5);
		k_timer_start(&secTimer, K_MSEC(1000), K_NO_WAIT);
		while (!(k_timer_status_get(&secTimer) > 0)){}	
	}
	
	
	
}


int playMg2() {
	uint32_t score = 1000;
	uint8_t *btnValues;
	uint8_t sequence[8] = {0,0,0,0,0,0,0,0};
	uint8_t level = 0;
	uint8_t btnLevel = 0;
	bool genValue = false;
	while (true)
	{
		if(genValue == false)
		{
			level = 0;
			generateSequence(sequence,8,0b11);
			genValue = true;
		}
		k_timer_start(&secTimer, K_MSEC(1000), K_NO_WAIT);
		while (!(k_timer_status_get(&secTimer) > 0)){}	
		showLevel(level,sequence);
		btnLevel = 0;
		bool buttonDetected = false;
		while(genValue == true)
		{
			if(btnLevel == level+1)
			{
				level++;
				break;
			}
			btnValues = btnmatrix_inGetMutexValue();
			if(btnValues[15] && btnValues[12] && btnValues[3] && btnValues[0])
			{
				buttonDetected = false;
			}
			if((!btnValues[15] || !btnValues[12] || !btnValues[3] || !btnValues[0]) && buttonDetected == false)
			{
				buttonDetected = true;
				for (int i = 0; i < 16; i++)
				{
					printf("button once %d: %d\n",i,btnValues[i]);
				}
				printf("sequence before %d \n",sequence[btnLevel]);
				switch (sequence[btnLevel])
				{
				case 3:
					if(btnValues[15] && btnValues[12] && btnValues[3] && !btnValues[0])
					{
						btnLevel++;
						printf("3 correct \n");
						showButton(3);
					}
					else
					{
						genValue = false;
						score -= 100;
						printf("3 incorrect \n");
					}
					break;
				case 2:
					if(btnValues[15] && btnValues[12] && !btnValues[3] && btnValues[0])
					{
						btnLevel++;
						printf("2 correct \n");
						showButton(2);
					}
					else
					{
						genValue = false;
						score -= 100;
						printf("2 incorrect \n");
					}
					break;
				case 1:
					if(btnValues[15] && !btnValues[12] && btnValues[3] && btnValues[0])
					{
						btnLevel++;
						printf("1 correct \n");
						showButton(1);
					}
					else
					{
						genValue = false;
						score -= 100;
						printf("1 incorrect \n");
					}
					break;
				case 0:
					if(!btnValues[15] && btnValues[12] && btnValues[3] && btnValues[0])
					{
						btnLevel++;
						printf("0 correct \n");
						showButton(0);
					}
					else
					{
						genValue = false;
						score -= 100;
						printf("0 incorrect \n");
					}
					break;
				}	
			}
		}
		if(level == 8 || score == 0)
		{
			break;
		}
	}
	// 	if(score == 0 || level == 8)
	// 	{
	// 		printf("end\n");
	// 		break;
	// 	}
	// 	if(!genValue)
	// 	{
	// 		for (uint8_t i = 0; i < 8; i++)
	// 		{
	// 			uint8_t randomValue = 0;
	// 			sys_rand_get(&randomValue, sizeof(randomValue));
	// 			randomValue = randomValue & (0b11);
	// 			printf("randomValue %d,\n",randomValue);
	// 			sequence[i] = randomValue;
	// 		}
	// 		genValue = true;
	// 		level = 0;
	// 	}
	// 	uint8_t data[4] = {0b00000000,0b00000000,0b00000000,0b00000000};
	// 	switch (sequence[level])
	// 	{
	// 	case 3:
	// 	data[0] = 0b00001000;
	// 		break;
	// 	case 2:
	// 	data[0] = 0b00000001;
	// 		break;
	// 	case 1:
	// 	data[4] = 0b00001000;
	// 		break;
	// 	case 0:
	// 	data[4] = 0b00000001;
	// 		break;
	// 	default:
	// 		//should not reach
	// 		break;
	// 	}	
	// 	btnmatrix_outSetMutexValue(data);		
	// 	btnValues = btnmatrix_inGetMutexValue();
	// 	if(!btnValues[15] || !btnValues[11] || !btnValues[3] || !btnValues[0])
	// 	{
	// 		switch (sequence[level])
	// 		{
	// 		case 3:
	// 			if(!btnValues[15] && btnValues[11] && btnValues[3] && btnValues[0])
	// 			{
	// 				level++;
	// 				printf("correct \n");
	// 			}
	// 			else
	// 			{
	// 				genValue = false;
	// 				score -= 100;
	// 				printf("incorrect \n");
	// 			}
	// 			break;
	// 		case 2:
	// 			if(btnValues[15] && !btnValues[11] && btnValues[3] && btnValues[0])
	// 			{
	// 				level++;
	// 				printf("correct \n");
	// 			}
	// 			else
	// 			{
	// 				genValue = false;
	// 				score -= 100;
	// 				printf("incorrect \n");
	// 			}
	// 			break;
	// 		case 1:
	// 			if(btnValues[15] && btnValues[11] && !btnValues[3] && btnValues[0])
	// 			{
	// 				level++;
	// 				printf("correct \n");
	// 			}
	// 			else
	// 			{
	// 				genValue = false;
	// 				score -= 100;
	// 				printf("incorrect \n");
	// 			}
	// 			break;
	// 		case 0:
	// 			if(btnValues[15] && btnValues[11] && btnValues[3] && !btnValues[0])
	// 			{
	// 				level++;
	// 				printf("correct \n");
	// 			}
	// 			else
	// 			{
	// 				genValue = false;
	// 				score -= 100;
	// 				printf("incorrect \n");
	// 			}
	// 			break;
	// 		default:
	// 			//should not reach
	// 			break;
	// 		}	
	// 	}
	// }
	return (int)score;
}
