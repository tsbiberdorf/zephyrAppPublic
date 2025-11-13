/*
 * Copyright (c) 2025
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include "gpioHandler.h"
#include "shellCommands.h"
#include "main.h"

int main(void)
{
	int ret;

	/* Initialize GPIO handler */
	ret = gpioHandlerInit();
	if (ret < 0) {
		return -1;
	}

	/* Initialize shell commands */
	shellCommandsInit();

	/* Print application banner */
	printk("\n");
	printk("========================================\n");
	printk("Nucleo-L476RG LED Control with Shell %d.%d.%d.%d\n",
	       MAJOR_VERSION, MINOR_VERSION, PATCH_VERSION, BUILD_VERSION);
	printk("========================================\n");
	printk("\nAvailable commands:\n");
	printk("  led on          - Turn LED on\n");
	printk("  led off         - Turn LED off\n");
	printk("  led flash [ms]  - Flash LED (optional period in ms)\n");
	printk("  led status      - Show current LED status\n");
	printk("\nDefault mode: Flashing at %d ms\n", gpioHandlerGetFlashPeriod());
	printk("========================================\n\n");

	/* Main thread can sleep - LED control is handled by the dedicated thread */
	while (1) {
		k_sleep(K_FOREVER);
	}

	return 0;
}
