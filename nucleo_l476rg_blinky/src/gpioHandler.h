/*
 * Copyright (c) 2025
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef GPIO_HANDLER_H
#define GPIO_HANDLER_H

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

/* LED control modes */
enum LedMode {
	LED_MODE_OFF,
	LED_MODE_ON,
	LED_MODE_FLASHING
};

/**
 * @brief Initialize the GPIO LED
 * @return 0 on success, negative error code on failure
 */
int gpioHandlerInit(void);

/**
 * @brief Set the LED mode
 * @param mode The desired LED mode
 */
void gpioHandlerSetMode(enum LedMode mode);

/**
 * @brief Get the current LED mode
 * @return Current LED mode
 */
enum LedMode gpioHandlerGetMode(void);

/**
 * @brief Set the flash period for flashing mode
 * @param periodMs Flash period in milliseconds
 */
void gpioHandlerSetFlashPeriod(int periodMs);

/**
 * @brief Get the current flash period
 * @return Current flash period in milliseconds
 */
int gpioHandlerGetFlashPeriod(void);

/**
 * @brief LED control thread entry point (called by Zephyr)
 */
void ledControlThread(void);

#endif /* GPIO_HANDLER_H */