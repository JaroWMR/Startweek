// #include "statemachine.h"
// #include "threads.h"
// #include <stdio.h>
// #include <zephyr/kernel.h>
// #include <zephyr/device.h>
// #include <zephyr/sys/util.h>
// #include <zephyr/sys/printk.h>
// #include <zephyr/drivers/gpio.h>

// // The stack size of all threads
// #define STACKSIZE 1024

// // Thread priority values (lower value is higher priority)
// #define TMAIN_PRIORITY 1

// void tmain(void) // Core thread
// {
// 	printf("Main\n");
// 	startStatemachine();
// }

// // Define the threads
// K_THREAD_DEFINE(tmain_id, STACKSIZE, tmain, NULL, NULL, NULL, TMAIN_PRIORITY, 0, 0);

/*
 * Copyright (c) 2016 Intel Corporation.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <zephyr/device.h>
#include <zephyr/sys/printk.h>

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>
#include <zephyr/usb/usb_device.h>
#include <zephyr/drivers/uart.h>

BUILD_ASSERT(DT_NODE_HAS_COMPAT(DT_CHOSEN(zephyr_console), zephyr_cdc_acm_uart),
			 "Console device is not ACM CDC UART device");

void main(void)
{
	// Configure USB Serial for Console output
	const struct device *usb_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_console));
	uint32_t dtr = 0;

	if (usb_enable(NULL)) {
		return;
	}

	// Sleep to wait for a terminal connection. To wait until connected, comment out
	// these two lines and uncomment the while below.
	// k_sleep(K_MSEC(2500));
	// uart_line_ctrl_get(usb_dev, UART_LINE_CTRL_DTR, &dtr);

	/* To wait for a Console connection, uncomment to poll if the DTR flag was set to activate USB */
	while (!dtr) {
		uart_line_ctrl_get(usb_dev, UART_LINE_CTRL_DTR, &dtr);
		k_sleep(K_MSEC(100));
	}

	while (1) {
		printk("Hello USB Console...\n");
		k_sleep(K_MSEC(5000));
	}
}
