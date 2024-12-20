/*
 * Copyright (c) 2024 UARTCommandCenter
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * @file menu_actions.c
 * @brief Menu action execution.
 * 
 * Description:
 * ------------
 * This file handles the execution of menu-related actions triggered by user input
 * from the menu system. It acts as a dispatch center that translates user choices
 * into meaningful operations, such as toggling lights, reading sensors, or 
 * adjusting system configurations.
 *
 * The design goal is to keep "menu_actions.c" focused on the "what" (i.e., 
 * which action to perform) rather than the "how" (implementation details). 
 * The "how" is delegated to commands or drivers accessed through well-defined 
 * interfaces.
 * 
 * @author Ameed Othman
 * @date 2024-12-19
 * 
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include "commands.h"
#include "uart_handler.h"

/*
 * Register a logging module for menu actions. 
 * Adjust the log level or name as appropriate for debugging.
 */
LOG_MODULE_REGISTER(menu_actions, LOG_LEVEL_INF);

/**
 * @brief Execute an action related to lights control.
 *
 * For example, this could toggle lights on or off, set brightness, or 
 * perform more sophisticated lighting patterns. At this point, we may 
 * just print a placeholder message or call a stub in "command_lights.c".
 *
 * @param command_id An identifier for the specific lights command, if needed.
 */
static void menu_actions_lights_control(int command_id)
{
	/* Placeholder: In the future, call a function from command_lights.c */
	uart_handler_write_string("Lights action triggered (placeholder).\r\n");
	LOG_INF("Lights control action executed (ID: %d)", command_id);
}

/**
 * @brief Execute a sensor-related action, such as reading a sensor value 
 *        and printing it to UART.
 *
 * @param sensor_type A code indicating which sensor to read (e.g., temperature, humidity).
 */
static void menu_actions_show_sensor_readings(int sensor_type)
{
	/* Placeholder: In the future, call functions from command_sensors.c or sensor_readings.c */
	uart_handler_write_string("Sensor readings action triggered (placeholder).\r\n");
	LOG_INF("Sensor readings displayed (Type: %d)", sensor_type);
}

/**
 * @brief Execute a system configuration action, e.g., changing a parameter,
 *        updating a mode, or performing calibration.
 *
 * @param config_id An identifier for the configuration action.
 */
static void menu_actions_system_configuration(int config_id)
{
	/* Placeholder: In the future, integrate with command_system.c */
	uart_handler_write_string("System configuration action triggered (placeholder).\r\n");
	LOG_INF("System configuration action executed (ID: %d)", config_id);
}

/**
 * @brief Execute a diagnostics/log action, such as showing logs, system status,
 *        or memory usage.
 *
 * @param diag_id An identifier to specify what type of diagnostics to perform.
 */
static void menu_actions_diagnostics(int diag_id)
{
	/* Placeholder: In the future, integrate with logging or diagnostics commands */
	uart_handler_write_string("Diagnostics/logs action triggered (placeholder).\r\n");
	LOG_INF("Diagnostics/log action executed (ID: %d)", diag_id);
}

/**
 * @brief Public API to execute a menu action based on a given category and ID.
 *
 * The category could represent which submenu (lights, sensors, config, diagnostics)
 * and the action_id could map to a specific operation within that category. This 
 * approach makes the menu system modular and easy to extend.
 *
 * @param category Category of action (e.g., lights=1, sensors=2, config=3, diag=4).
 * @param action_id Specific action ID within the category.
 */
void menu_actions_execute(int category, int action_id)
{
	LOG_INF("menu_actions_execute: category=%d, action_id=%d", category, action_id);
	commands_core_execute(category, action_id);
	/*
	 * After calling commands_core_execute(), you will see the placeholders
	 * defined in commands_core.c triggered. This confirms that the command
	 * execution path is working end-to-end.
	 */
}