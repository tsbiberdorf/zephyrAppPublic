/*
 * Copyright (c) 2025
 * SPDX-License-Identifier: Apache-2.0
 */

#include "shellCommands.h"
#include "gpioHandler.h"
#include <zephyr/shell/shell.h>
#include <stdlib.h>
#include "main.h"

/* Shell command: led on */
static int cmdLedOn(const struct shell *sh, size_t argc, char **argv)
{
	gpioHandlerSetMode(LED_MODE_ON);
	shell_print(sh, "LED set to ON");
	return 0;
}

/* Shell command: led off */
static int cmdLedOff(const struct shell *sh, size_t argc, char **argv)
{
	gpioHandlerSetMode(LED_MODE_OFF);
	shell_print(sh, "LED set to OFF");
	return 0;
}

/* Shell command: led flash [period_ms] */
static int cmdLedFlash(const struct shell *sh, size_t argc, char **argv)
{
	if (argc > 1) {
		int period = atoi(argv[1]);
		if (period < 100 || period > 10000) {
			shell_error(sh, "Period must be between 100 and 10000 ms");
			return -1;
		}
		gpioHandlerSetFlashPeriod(period);
	}

	gpioHandlerSetMode(LED_MODE_FLASHING);
	shell_print(sh, "LED set to FLASHING (period: %d ms)",
		    gpioHandlerGetFlashPeriod());
	return 0;
}

/* Shell command: led status */
static int cmdLedStatus(const struct shell *sh, size_t argc, char **argv)
{
	const char *modeStr;
	enum LedMode mode = gpioHandlerGetMode();

	switch (mode) {
	case LED_MODE_OFF:
		modeStr = "OFF";
		break;
	case LED_MODE_ON:
		modeStr = "ON";
		break;
	case LED_MODE_FLASHING:
		modeStr = "FLASHING";
		break;
	default:
		modeStr = "UNKNOWN";
		break;
	}

	shell_print(sh, "LED Mode: %s", modeStr);
	if (mode == LED_MODE_FLASHING) {
		shell_print(sh, "Flash Period: %d ms", gpioHandlerGetFlashPeriod());
	}

	return 0;
}

/* Register shell commands */
SHELL_STATIC_SUBCMD_SET_CREATE(ledCommands,
	SHELL_CMD(on, NULL, "Turn LED on", cmdLedOn),
	SHELL_CMD(off, NULL, "Turn LED off", cmdLedOff),
	SHELL_CMD(flash, NULL, "Set LED to flashing mode [period_ms]", cmdLedFlash),
	SHELL_CMD(status, NULL, "Show LED status", cmdLedStatus),
	SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(led, &ledCommands, "LED control commands", NULL);

/* Shell command: version */
static int cmdVersion(const struct shell *sh, size_t argc, char **argv)
{
	shell_print(sh, "Version: %d.%d.%d.%d", MAJOR_VERSION, MINOR_VERSION, PATCH_VERSION, BUILD_VERSION);
	return 0;
}

SHELL_CMD_REGISTER(version, NULL, "Code Version", cmdVersion);

void shellCommandsInit(void)
{
	/* Shell commands are automatically registered via SHELL_CMD_REGISTER macro */
	printk("Shell commands registered\n");
}