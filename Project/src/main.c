// /*
//  * Copyright (c) 2012-2014 Wind River Systems, Inc.
//  *
//  * SPDX-License-Identifier: Apache-2.0
//  */

// #include <stdio.h>

// int main(void)
// {
//      printf("Hello World! %s\n", CONFIG_BOARD);
//      return 0;
// }

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

/* STEP 2 - Define stack size and scheduling priority used by each thread */
#define STACKSIZE 1024

#define THREAD0_PRIORITY 7
#define THREAD1_PRIORITY 7

void thread0(void)
{
	int cnt = 0;
	while (1) {
		printk("Hello, I am thread0\t%d\n", cnt);
		k_msleep(1000);
		cnt++;
	}
}

void thread1(void)
{
	int cnt = 0;
	while (1) {
		printk("Hello, I am thread1\t%d\n", cnt);
		k_msleep(3000);
		cnt++;
	}
}

// Define the threads
K_THREAD_DEFINE(thread0_id, STACKSIZE, thread0, NULL, NULL, NULL, THREAD0_PRIORITY, 0, 0);
K_THREAD_DEFINE(thread1_id, STACKSIZE, thread1, NULL, NULL, NULL, THREAD1_PRIORITY, 0, 0);