#ifndef MINIGAME4_H
#define MINIGAME4_H

#include <zephyr/kernel.h>
#include <stdio.h>
#include "threads.h"
#include "lcd.h"

#define mg4ThreadCount 3

void getMg4Threads(char ***, unsigned *);

int playMg4();
#define AMOUNT_QUESTIONS 5
#define AMOUNT_ANSWERS 3
#define MAX_SIZE 32

#endif // MINIGAME4_H