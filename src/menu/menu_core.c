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
 * @author Ameed Othman
 * @date 2024-12-19
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <string.h>

#include "app_config.h"
#include "menu.h"
#include "uart_handler.h"
#include "commands.h"

/* 
 * LOG_MODULE_REGISTER allows runtime control of log level.
 * Ensure CONFIG_LOG is enabled in prj.conf.
 */
LOG_MODULE_REGISTER(menu_core, LOG_LEVEL_INF);

/* 
 * Menu prompt and options: Adjust these strings as needed.
 * You can store them in a separate header or configuration file if desired.
 */
static const char *main_menu_header =
	"\r\n"
	"--------------------------------------\r\n"
	"        UART Command Center Menu      \r\n"
	"--------------------------------------\r\n"
	"[1] Control Lights\r\n"
	"[2] View Sensor Readings\r\n"
	"[3] System Configuration\r\n"
	"[4] Diagnostics and Logs\r\n"
	"[0] Exit\r\n"
	"Enter your choice: ";

static const int menu_input_max_len = 32;

/**
 * @brief Display the main menu to the user.
 *
 * This function prints the main menu options to the UART console, inviting
 * the user to select one of the listed choices.
 */
static void menu_core_display_main_menu(void)
{
	uart_handler_write_string(main_menu_header);
}

/**
 * @brief Process the user's input from the main menu.
 *
 * This function takes the user input string and decides what to do next:
 *   - If '1' is entered, navigate to lights control commands.
 *   - If '2' is entered, display sensor readings.
 *   - If '3' is entered, go to system configuration menu.
 *   - If '4' is entered, show diagnostics/logs.
 *   - If '0' is entered, exit the menu system.
 *   - Otherwise, print an error message and redisplay the menu.
 *
 * @param input A null-terminated string containing the user's choice.
 * @return true if we should continue the menu loop, false if the user requests exit.
 */
static bool menu_core_handle_input(const char *input)
{
	if (strcmp(input, "1") == 0) {
		uart_handler_write_string("Lights control selected.\r\n");
        /* Test action: Category 1 for Lights, action_id = 0 as a placeholder */
         menu_actions_execute(1, 0);
	} else if (strcmp(input, "2") == 0) {
		uart_handler_write_string("Sensor readings selected.\r\n");
        menu_actions_execute(2, 0); // Category 2 for Sensors, action_id = 0
	} else if (strcmp(input, "3") == 0) {
		uart_handler_write_string("System configuration selected.\r\n");
        menu_actions_execute(3, 0);
	} else if (strcmp(input, "4") == 0) {
		uart_handler_write_string("Diagnostics and logs selected.\r\n");
        menu_actions_execute(4, 0);
	} else if (strcmp(input, "0") == 0) {
		uart_handler_write_string("Exiting menu.\r\n");
		return false;  /* Stop the menu loop */
	} else {
		uart_handler_write_string("Invalid choice. Please try again.\r\n");
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
			uart_handler_write_string("Error: Failed to read input.\r\n");
		}

		/* Optional: small sleep to prevent CPU lock, if desired */
		k_sleep(K_MSEC(10));
	}

	LOG_INF("Exiting main menu loop");
}
