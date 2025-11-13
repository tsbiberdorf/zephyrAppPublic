/*
 * Copyright (c) 2025
 * SPDX-License-Identifier: Apache-2.0
 */

#include "dvkUart.h"
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include <string.h>

/* Get USART3 device from device tree */
#define UART_DEVICE_NODE DT_NODELABEL(usart3)

static const struct device *uartDev = DEVICE_DT_GET(UART_DEVICE_NODE);

/* Ring buffer for received data */
#define UART_RX_BUF_SIZE 256
static uint8_t rxBuffer[UART_RX_BUF_SIZE];
static uint32_t rxHead = 0;
static uint32_t rxTail = 0;

/* Semaphore to signal data received */
K_SEM_DEFINE(rxDataSem, 0, 1);

/**
 * @brief UART interrupt callback
 * Called when UART data is received
 */
static void uartIsrCallback(const struct device *dev, void *userData)
{
	uint8_t c;

	if (!uart_irq_update(uartDev)) {
		return;
	}

	if (!uart_irq_rx_ready(uartDev)) {
		return;
	}

	/* Read all available bytes */
	while (uart_fifo_read(uartDev, &c, 1) == 1) {
		/* Store in ring buffer */
		uint32_t nextHead = (rxHead + 1) % UART_RX_BUF_SIZE;

		if (nextHead != rxTail) {
			rxBuffer[rxHead] = c;
			rxHead = nextHead;

			/* Signal that data is available */
			k_sem_give(&rxDataSem);
		} else {
			/* Buffer overflow - data lost */
			printk("UART RX buffer overflow\n");
		}
	}
}

int dvkUartInit(void)
{
	if (!device_is_ready(uartDev)) {
		printk("Error: UART device %s not ready\n", uartDev->name);
		return -1;
	}

	/* Configure interrupt-driven UART */
	uart_irq_callback_user_data_set(uartDev, uartIsrCallback, NULL);
	uart_irq_rx_enable(uartDev);

	printk("UART Handler initialized: %s\n", uartDev->name);
	printk("USART3 configured on PC3 (RX) and PC4 (TX) at 115200 baud\n");

	return 0;
}

int dvkUartSendString(const char *str)
{
	if (!str) {
		return -1;
	}

	size_t len = strlen(str);
	for (size_t i = 0; i < len; i++) {
		uart_poll_out(uartDev, str[i]);
	}

	return 0;
}

/**
 * @brief Get a character from the receive buffer
 * @param c Pointer to store the received character
 * @return 0 on success, -1 if buffer is empty
 */
static int uartGetChar(uint8_t *c)
{
	if (rxTail == rxHead) {
		return -1;  /* Buffer empty */
	}

	*c = rxBuffer[rxTail];
	rxTail = (rxTail + 1) % UART_RX_BUF_SIZE;

	return 0;
}

/* UART echo task */
void uartEchoTask(void)
{
	uint8_t c;
	char welcomeMsg[] = "\r\n=== USART3 Echo Ready ===\r\n";
	char prompt[] = "Type characters to echo them back...\r\n";

	/* Send welcome message */
	dvkUartSendString(welcomeMsg);
	dvkUartSendString(prompt);

	while (1) {
		/* Wait for data to be available */
		k_sem_take(&rxDataSem, K_FOREVER);

		/* Process all available characters */
		while (uartGetChar(&c) == 0) {
			/* Echo the character back */
			uart_poll_out(uartDev, c);

			/* Handle special characters */
			if (c == '\r') {
				/* Echo newline after carriage return */
				uart_poll_out(uartDev, '\n');
			}

			/* Future expansion: Parse commands here
			 * For now, just echo. In the future, you can:
			 * - Accumulate characters into a command buffer
			 * - Parse on '\r' or '\n'
			 * - Execute commands and send responses
			 */
		}
	}
}

/* Define the UART task thread */
K_THREAD_DEFINE(uartThread, 2048, uartEchoTask, NULL, NULL, NULL, 7, 0, 0);
