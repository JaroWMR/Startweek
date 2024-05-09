/*
 * Copyright (c) 2023 Trackunit Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "statemachine.h"
#include "threads.h"
#include <stdio.h>

int main(void)
{
	startStatemachine();
	printf("A thing\n");
	return 0;
}
