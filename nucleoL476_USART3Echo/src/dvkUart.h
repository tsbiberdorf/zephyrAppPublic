/*
 * Copyright (c) 2025
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef DVK_UART_H
#define DVK_UART_H

#include <zephyr/kernel.h>
#include <zephyr/device.h>

/**
 * @brief Initialize the UART handler for USART3
 * @return 0 on success, negative error code on failure
 */
int dvkUartInit(void);

/**
 * @brief UART echo task entry point (called by Zephyr)
 * This task handles receiving and echoing data on USART3
 */
void uartEchoTask(void);

/**
 * @brief Send a string over UART
 * @param str The null-terminated string to send
 * @return 0 on success, negative error code on failure
 */
int dvkUartSendString(const char *str);

#endif /* DVK_UART_H */
