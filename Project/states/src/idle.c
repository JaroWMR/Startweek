#include "idle.h"

unsigned idleThreadCount = 1;
char *idleThreads[1] = {"gps"};

void getIdleThreads(char ***names, unsigned *amount) {
	*names = idleThreads;
	*amount = idleThreadCount;
}