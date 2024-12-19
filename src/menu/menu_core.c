/*
 * Copyright (c) 2024 UARTCommandCenter
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * @file menu_core.c
 * @brief Core functionality of the application's menu system.
 * 
 * Description:
 * ------------
 * This file provides the core functionality of the application's menu system.
 * It displays a main menu over UART, waits for user input, and dispatches
 * selected actions to the appropriate command handlers or other menu modules.
 *
 * The menu system uses the UART handler to interact with the user via a
 * line-based interface. It:
 *   - Prints a main menu of options.
 *   - Waits for the user to enter a choice and press enter.
 *   - Routes the user's choice to corresponding handling functions.
 *
 * This design ensures that the menu logic is centralized, making it easier
 * to maintain, extend, and integrate with other parts of the application,
 * such as commands, sensors, and lights control.
 * 
 * Author: Ameed Othman
 * Date: 2024-12-19
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <string.h>

#include "app_config.h"
#include "menu.h"
#include "uart_handler.h"
#include "commands.h"

LOG_MODULE_REGISTER(menu_core, LOG_LEVEL_INF);

static const int menu_input_max_len = 32;

/**
 * @brief Display the main menu to the user.
 *
 * This function now delegates to menu_display_show_main_menu() to print the
 * main menu options. By centralizing display logic in menu_display.c, we 
 * maintain cleaner and more maintainable code.
 */
static void menu_core_display_main_menu(void)
{
	menu_display_show_main_menu();
}

/**
 * @brief Process the user's input from the main menu.
 *
 * This function takes the user input string and decides what to do next.
 * If the input is not recognized, it now uses menu_display_error() to
 * display the error message rather than hard-coding it.
 *
 * @param input A null-terminated string containing the user's choice.
 * @return true if we should continue the menu loop, false if the user requests exit.
 */
static bool menu_core_handle_input(const char *input)
{
	if (strcmp(input, "1") == 0) {
		uart_handler_write_string("Lights control selected.\r\n");
		menu_actions_execute(1, 0);  /* Example: Lights action */
	} else if (strcmp(input, "2") == 0) {
		uart_handler_write_string("Sensor readings selected.\r\n");
		menu_actions_execute(2, 0);  /* Example: Sensor action */
	} else if (strcmp(input, "3") == 0) {
		uart_handler_write_string("System configuration selected.\r\n");
		menu_actions_execute(3, 0);  /* Example: System config action */
	} else if (strcmp(input, "4") == 0) {
		uart_handler_write_string("Diagnostics and logs selected.\r\n");
		menu_actions_execute(4, 0);  /* Example: Diagnostics action */
	} else if (strcmp(input, "0") == 0) {
		uart_handler_write_string("Exiting menu.\r\n");
		return false;  /* Stop the menu loop */
	} else {
		menu_display_error("Invalid choice. Please try again.");
	}

	return true; /* Continue the menu loop */
}

/**
 * @brief Public function to start the main menu loop.
 *
 * This function:
 *   - Continuously displays the main menu.
 *   - Waits for user input via the UART message queue.
 *   - Processes the user's choice until '0' is chosen.
 */
void menu_core_run(void)
{
	char input_buffer[menu_input_max_len];

	LOG_INF("Starting main menu loop");

	bool run = true;
	while (run) {
		menu_core_display_main_menu();

		/* Block until we get a line of input from the user */
		memset(input_buffer, 0, sizeof(input_buffer));
		int ret = k_msgq_get(&uart_msgq, input_buffer, K_FOREVER);
		if (ret == 0) {
			/* Process the user's input */
			run = menu_core_handle_input(input_buffer);
		} else {
			menu_display_error("Failed to read input.");
		}

		/* Optional: small sleep to prevent CPU lock, if desired */
		k_sleep(K_MSEC(10));
	}

	LOG_INF("Exiting main menu loop");
}
