/*
 * Copyright (c) 2025
 * SPDX-License-Identifier: Apache-2.0
 */

#include "gpioHandler.h"
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

/* The devicetree node identifier for the "led0" alias */
#define LED0_NODE DT_ALIAS(led0)

/*
 * A build error on this line means your board is unsupported.
 * For Nucleo-L476RG, led0 is the green user LED (LD2) on PA5
 */
static const struct gpio_dt_spec ledGpio = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

/* Global LED state */
static enum LedMode currentMode = LED_MODE_FLASHING;
static int flashPeriodMs = 1000;

int gpioHandlerInit(void)
{
	int ret;

	if (!gpio_is_ready_dt(&ledGpio)) {
		printk("Error: LED GPIO device not ready\n");
		return -1;
	}

	ret = gpio_pin_configure_dt(&ledGpio, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		printk("Error: Failed to configure LED GPIO\n");
		return -1;
	}

	printk("GPIO Handler initialized: %s pin %d\n", ledGpio.port->name, ledGpio.pin);
	return 0;
}

void gpioHandlerSetMode(enum LedMode mode)
{
	currentMode = mode;

	/* Immediately set LED state for ON/OFF modes */
	if (mode == LED_MODE_ON) {
		gpio_pin_set_dt(&ledGpio, 1);
	} else if (mode == LED_MODE_OFF) {
		gpio_pin_set_dt(&ledGpio, 0);
	}
}

enum LedMode gpioHandlerGetMode(void)
{
	return currentMode;
}

void gpioHandlerSetFlashPeriod(int periodMs)
{
	flashPeriodMs = periodMs;
}

int gpioHandlerGetFlashPeriod(void)
{
	return flashPeriodMs;
}

/* LED control thread */
void ledControlThread(void)
{
	bool ledState = false;

	while (1) {
		switch (currentMode) {
		case LED_MODE_OFF:
			/* LED is already off, just sleep */
			k_msleep(100);
			break;

		case LED_MODE_ON:
			/* LED is already on, just sleep */
			k_msleep(100);
			break;

		case LED_MODE_FLASHING:
			/* Toggle the LED */
			gpio_pin_toggle_dt(&ledGpio);
			ledState = !ledState;
			k_msleep(flashPeriodMs);
			break;
		}
	}
}

K_THREAD_DEFINE(ledThread, 1024, ledControlThread, NULL, NULL, NULL, 7, 0, 0);