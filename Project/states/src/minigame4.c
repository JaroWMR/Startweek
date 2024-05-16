#include "minigame4.h"

K_TIMER_DEFINE(secTimerMg4, NULL, NULL);

char *mg4Threads[mg4ThreadCount] = {"startbtn", "buzzers", "abcbtn"};

void getMg4Threads(char ***names, unsigned *amount) {
	*names = mg4Threads;
	*amount = mg4ThreadCount;
}

const char questions[AMOUNT_QUESTIONS][MAX_SIZE] = {
	"Hoe oud is Siem?",
	"Hoe groot is Siem?",
	"Waar werkt Siem?",
	"Waar woont Siem?"
};

const char answers[AMOUNT_QUESTIONS][AMOUNT_ANSWERS][MAX_SIZE] = {
	{"3 jaar", "8 jaar", "14 minuten"},
	{"77cm", "108cm", "135cm"},
	{"Supermarkt", "Dierentuin", "School"},
	{"Limburg", "Gelderland", "Brabant"}
};

const int correctAnswer[AMOUNT_QUESTIONS] = {
	2,
	3,
	1,
	1,
};


int playMg4() {
	// State loop
	uint8_t *test;
	lcdEnable();
	lcdClear();
	lcdStringWrite("    Minigame          Quiz      ");
	k_timer_start(&secTimerMg4, K_MSEC(1000), K_NO_WAIT);
	while (!(k_timer_status_get(&secTimerMg4) > 0)){}	
	while (true)
	{
		
		test = abcbtnGetMutexValue();
		
	}
	
	lcdDisable();
	return 0;
}