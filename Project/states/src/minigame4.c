#include "minigame4.h"

K_TIMER_DEFINE(secTimerMg4, NULL, NULL);

char *mg4Threads[mg4ThreadCount] = {"startbtn", "buzzers", "abcbtn"};

void getMg4Threads(char ***names, unsigned *amount) {
	*names = mg4Threads;
	*amount = mg4ThreadCount;
}

char questions[AMOUNT_QUESTIONS][MAX_SIZE] = {
	"Hoe oud is Siem?",
	"Hoe groot is    Siem?",
	"Waar werkt Siem?",
	"Waar woont Siem?"
};

char answers[AMOUNT_QUESTIONS][AMOUNT_ANSWERS][MAX_SIZE] = {
	{"3 jaar", "8 jaar", "14 minuten"},
	{"77cm", "108cm", "135cm"},
	{"Supermarkt", "Dierentuin", "School"},
	{"Limburg", "Gelderland", "Brabant"}
};

const int correctAnswer[AMOUNT_QUESTIONS] = {
	1,
	2,
	0,
	0,
};


int playMg4() {
	uint32_t score = 1000;
	uint8_t *abcBtn;
	bool showQuestion = true;
	bool correct = false;
	bool buttonReleased = true;
	lcdEnable();
	lcdClear();
	lcdStringWrite("    Minigame          Quiz      ");
	k_timer_start(&secTimerMg4, K_MSEC(1000), K_NO_WAIT);
	while (!(k_timer_status_get(&secTimerMg4) > 0)){}	
	for (uint8_t questionIndex = 0; questionIndex < AMOUNT_QUESTIONS; questionIndex++)
	{
		correct = false;
		while (!correct)
		{
			//check if score is 0
			if(score == 0)
			{
				questionIndex = AMOUNT_QUESTIONS;
				break;
			}
			//show output
			if (showQuestion)
			{
				showQuestion = false;
				lcdStringWrite(questions[questionIndex]);
				k_timer_start(&secTimerMg4, K_MSEC(1000), K_NO_WAIT);
				while (!(k_timer_status_get(&secTimerMg4) > 0)){}

				for (uint8_t answersIndex = 0; answersIndex < AMOUNT_ANSWERS; answersIndex++)
				{
					lcdStringWrite(answers[questionIndex][answersIndex]);
					k_timer_start(&secTimerMg4, K_MSEC(1000), K_NO_WAIT);
					while (!(k_timer_status_get(&secTimerMg4) > 0)){}
				}
				
				lcdStringWrite("    Antwoord      A , B of C    ");
				
			}

			//check for input
			abcBtn = abcbtnGetMutexValue();
			if(abcBtn[2] && abcBtn[1] && abcBtn[0])
			{
				buttonReleased = true;
			}
			if ((!abcBtn[2] || !abcBtn[1] || !abcBtn[0]) && buttonReleased)
			{
				buttonReleased = false;
				switch (correctAnswer[questionIndex])
				{
				case 0:
					if (!abcBtn[0])
					{
						lcdStringWrite("Correct!");
						correct = true;
					}
					else
					{
						lcdStringWrite("Incorrect!");
						score -= 100;
					}
					break;
				case 1:
					if (!abcBtn[1])
					{
						lcdStringWrite("Correct!");
						correct = true;
					}
					else
					{
						lcdStringWrite("Incorrect!");
						score -= 100;
					}
					break;
				case 2:
					if (!abcBtn[2])
					{
						lcdStringWrite("Correct!");
						correct = true;
					}
					else
					{
						lcdStringWrite("Incorrect!");
						score -= 100;
					}
					break;
				default:
					break;
				}
				k_timer_start(&secTimerMg4, K_MSEC(1000), K_NO_WAIT);
				while (!(k_timer_status_get(&secTimerMg4) > 0)){}	
				showQuestion = true;
			}
		}
	}
	lcdDisable();
	lcdClear();
	return score;
}