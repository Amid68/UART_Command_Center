/*
 * Copyright (c) 2024 UARTCommandCenter
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * @file menu_display.c
 * @brief Menu display functions.
 * 
 * Description:
 * ------------
 * This file focuses on presenting menu content to the user via UART. It 
 * provides functions to print headers, option lists, and any other 
 * UI-related elements needed for the menu system.
 *
 * By centralizing all display logic here, the menu system remains modular:
 * - menu_core.c: Manages menu logic and user input handling
 * - menu_actions.c: Executes actions based on user choices
 * - menu_display.c: Handles all menu-related visual output
 *
 * Following best practices:
 *  - Functions are clearly named and documented.
 *  - Multi-line comments describe the intent and usage of each function.
 *  - UART output is encapsulated for easy maintenance and potential localization.
 * 
 * @author Ameed Othman
 * @date 2024-12-19
 * 
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <string.h>
#include "uart_handler.h"

/*
 * Register a logging module for menu display to track display-related events.
 * Adjust the verbosity as needed for debugging or silence logs if it's stable.
 */
LOG_MODULE_REGISTER(menu_display, LOG_LEVEL_INF);

/* 
 * A header template for the main menu or other menus.
 * You may consider storing various templates or styles here.
 */
static const char *main_menu_header =
	"\r\n"
	"--------------------------------------\r\n"
	"      UART Command Center Menu\r\n"
	"--------------------------------------\r\n";

/**
 * @brief Print the main menu options to the user.
 *
 * This function displays a predefined list of menu items. You can easily 
 * modify this to accept parameters or to build menus dynamically in the future.
 */
void menu_display_show_main_menu(void)
{
	uart_handler_write_string(main_menu_header);
	uart_handler_write_string("[1] Control Lights\r\n");
	uart_handler_write_string("[2] View Sensor Readings\r\n");
	uart_handler_write_string("[3] System Configuration\r\n");
	uart_handler_write_string("[4] Diagnostics and Logs\r\n");
	uart_handler_write_string("[0] Exit\r\n");
	uart_handler_write_string("Enter your choice: ");
}

/**
 * @brief Print a generic message to the user via UART.
 *
 * This utility function can be used by other parts of the menu system 
 * to present simple prompts, instructions, or feedback messages.
 *
 * @param msg A null-terminated string containing the message to print.
 */
void menu_display_message(const char *msg)
{
	if (!msg) {
		LOG_WRN("Tried to display a NULL message.");
		return;
	}

	uart_handler_write_string(msg);
	uart_handler_write_string("\r\n");
	LOG_INF("Displayed message: %s", msg);
}

/**
 * @brief Print an error message indicating invalid input or a similar condition.
 *
 * Having a dedicated function for errors allows you to style errors differently,
 * or potentially log them at a different log level.
 *
 * @param err_msg A null-terminated string describing the error condition.
 */
void menu_display_error(const char *err_msg)
{
	if (!err_msg) {
		err_msg = "Unknown error.";
	}
	uart_handler_write_string("Error: ");
	uart_handler_write_string(err_msg);
	uart_handler_write_string("\r\n");
	LOG_WRN("Displayed error message: %s", err_msg);
}
