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
 * This file manages the top-level and secondary menus for the application. 
 * Initially, the user is presented with a main menu. When selecting "[1] Control Lights," 
 * the application now displays a sub-menu to choose specific lights actions:
 *   [1] Turn ON
 *   [2] Turn OFF
 *   [3] Increase Brightness
 *   [4] Decrease Brightness
 *   [0] Return to Main Menu
 *
 * By introducing a sub-menu, we give the user finer control over lights 
 * operations. This logic continues to rely on `menu_display_show_main_menu()` 
 * for the main menu and introduces a local function for the lights menu. User 
 * input from the lights sub-menu is processed similarly to the main menu, 
 * calling `menu_actions_execute()` with different action_ids for each lights action.
 *
 * Author: Ameed Othman
 * Date: 2024-12-19
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <string.h>

#include "app_config.h"
#include "menu.h"
#include "menu_display.h"
#include "menu_actions.h"
#include "uart_handler.h"
#include "commands.h"

LOG_MODULE_REGISTER(menu_core, LOG_LEVEL_INF);

static const int menu_input_max_len = 32;

static char input_buffer[32];
static bool run = true;

/**
 * @brief Display the lights sub-menu options to the user.
 *
 * This function prints a secondary menu specifically for lights control. 
 * Users can choose different actions for the lights here.
 */
static void menu_core_display_lights_menu(void)
{
	menu_display_message("Lights Control Menu:");
	menu_display_message("[1] Turn ON");
	menu_display_message("[2] Turn OFF");
	menu_display_message("[3] Increase Brightness");
	menu_display_message("[4] Decrease Brightness");
	menu_display_message("[0] Return to Main Menu");
	menu_display_message("Enter your choice:");
}

/**
 * @brief Process the user's input from the lights sub-menu.
 *
 * This function takes the user input string from the lights menu and 
 * translates it into the appropriate action_id for lights commands. 
 * Afterwards, it calls `menu_actions_execute(1, action_id)` to perform 
 * the chosen operation.
 *
 * @param input A null-terminated string containing the user's choice.
 * @return true if we should continue the lights sub-menu loop, false if the user requests to return to main menu.
 */
static bool menu_core_handle_lights_input(const char *input)
{
	if (strcmp(input, "1") == 0) {
		uart_handler_write_string("Turning lights ON...\r\n");
		menu_actions_execute(1, 0);  // action_id=0: Turn ON
	} else if (strcmp(input, "2") == 0) {
		uart_handler_write_string("Turning lights OFF...\r\n");
		menu_actions_execute(1, 1);  // action_id=1: Turn OFF
	} else if (strcmp(input, "3") == 0) {
		uart_handler_write_string("Increasing brightness...\r\n");
		menu_actions_execute(1, 2);  // action_id=2: Increase Brightness
	} else if (strcmp(input, "4") == 0) {
		uart_handler_write_string("Decreasing brightness...\r\n");
		menu_actions_execute(1, 3);  // action_id=3: Decrease Brightness
	} else if (strcmp(input, "0") == 0) {
		uart_handler_write_string("Returning to main menu...\r\n");
		return false;  // Go back to main menu
	} else {
		menu_display_error("Invalid choice. Please try again.");
	}

	return true; // Continue lights sub-menu loop
}

/**
 * @brief Run the lights sub-menu loop.
 *
 * Displays the lights menu, waits for input, and processes user requests 
 * until the user chooses to return to the main menu.
 */
static void menu_core_run_lights_menu(void)
{
	while (run) {
		menu_core_display_lights_menu();

		memset(input_buffer, 0, sizeof(input_buffer));
		int ret = k_msgq_get(&uart_msgq, input_buffer, K_FOREVER);
		if (ret == 0) {
			run = menu_core_handle_lights_input(input_buffer);
		} else {
			menu_display_error("Failed to read input.");
		}

		k_sleep(K_MSEC(10));
	}
}

/**
 * @brief Display the main menu to the user.
 *
 * This function delegates to menu_display_show_main_menu() to print the
 * main menu options.
 */
static void menu_core_display_main_menu(void)
{
	menu_display_show_main_menu();
}

/**
 * @brief Process the user's input from the main menu.
 *
 * If the user selects "[1] Control Lights," we now run the lights sub-menu 
 * instead of directly executing a single action. Other menu choices 
 * function as before.
 *
 * @param input A null-terminated string containing the user's choice.
 * @return true if we should continue the main menu loop, false if the user requests exit.
 */
static bool menu_core_handle_input(const char *input)
{
	if (strcmp(input, "1") == 0) {
		uart_handler_write_string("Lights control selected.\r\n");
		menu_core_run_lights_menu(); // Enter the lights sub-menu
		run = true;
		LOG_INF("Returned from lights sub-menu, now resuming main menu loop...");
	} else if (strcmp(input, "2") == 0) {
		uart_handler_write_string("Sensor readings selected.\r\n");
		menu_actions_execute(2, 0);  // Example: Sensor action
	} else if (strcmp(input, "3") == 0) {
		uart_handler_write_string("System configuration selected.\r\n");
		menu_actions_execute(3, 0);  // Example: System config action
	} else if (strcmp(input, "4") == 0) {
		uart_handler_write_string("Diagnostics and logs selected.\r\n");
		menu_actions_execute(4, 0);  // Example: Diagnostics action
	} else if (strcmp(input, "0") == 0) {
		uart_handler_write_string("Exiting menu.\r\n");
		return false;  // Stop the main menu loop
	} else {
		menu_display_error("Invalid choice. Please try again.");
	}

	return true; // Continue main menu loop
}

/**
 * @brief Public function to start the main menu loop.
 *
 * Continuously displays the main menu, waits for user input, and 
 * processes user choices until '0' is chosen. If the user picks 
 * "[1] Control Lights," we temporarily run the lights sub-menu 
 * before returning here.
 */
void menu_core_run(void)
{
	char input_buffer[menu_input_max_len];

	LOG_INF("Starting main menu loop");

	bool run = true;
	while (run) {
		menu_core_display_main_menu();

		memset(input_buffer, 0, sizeof(input_buffer));
		int ret = k_msgq_get(&uart_msgq, input_buffer, K_FOREVER);
		if (ret == 0) {
			run = menu_core_handle_input(input_buffer);
		} else {
			menu_display_error("Failed to read input.");
		}

		k_sleep(K_MSEC(10));
	}

	LOG_INF("Exiting main menu loop");
}
