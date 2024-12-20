/*
 * Copyright (c) 2024 UARTCommandCenter
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * @file commands_core.c
 * @brief Centralized command execution.
 * 
 * Description:
 * ------------
 * This file provides a centralized interface for executing commands requested 
 * by the menu system. It ties together various command implementations 
 * (e.g., lights, sensors, system configuration, diagnostics) into a unified 
 * API. By doing this, the menu system and other components can trigger 
 * high-level commands without needing to know the underlying implementation details.
 *
 * Changes:
 * --------
 * - Removed placeholder messages for lights commands.
 * - Integrated `command_lights_execute()` for the lights category to leverage real logic.
 * - For sensors, system, and diagnostics, we now print a "Not implemented yet" message 
 *   instead of generic placeholders, making it clear that these features are pending.
 * 
 * With these changes, selecting a lights option should now route through 
 * `command_lights_execute()` and subsequently `lights_control.c`, displaying 
 * the messages defined there.
 * 
 * Author: Ameed Othman
 * Date: 2024-12-19
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include "commands.h"
#include "uart_handler.h"
#include "command_lights.h"  // Ensure this header provides `command_lights_execute()` prototype

LOG_MODULE_REGISTER(commands_core, LOG_LEVEL_INF);

/**
 * @brief Execute a command for lights control.
 *
 * Now we directly call `command_lights_execute()` instead of printing placeholders.
 *
 * @param action_id Identifies which lights action to execute.
 */
static void commands_core_execute_lights(int action_id)
{
	LOG_INF("commands_core_execute_lights: action_id=%d", action_id);
	command_lights_execute(action_id);
}

/**
 * @brief Execute a command for sensor operations.
 *
 * Since sensor commands are not yet implemented, we clearly state that 
 * this feature is pending rather than using placeholders.
 *
 * @param action_id Identifies which sensor action to execute.
 */
static void commands_core_execute_sensors(int action_id)
{
	uart_handler_write_string("Sensor commands not implemented yet.\r\n");
	LOG_WRN("Sensor command requested but not implemented. action_id=%d", action_id);
}

/**
 * @brief Execute a system configuration command.
 *
 * Clearly indicate that system configuration commands are not yet implemented.
 *
 * @param action_id Identifies which system config action to execute.
 */
static void commands_core_execute_system(int action_id)
{
	uart_handler_write_string("System configuration commands not implemented yet.\r\n");
	LOG_WRN("System config command requested but not implemented. action_id=%d", action_id);
}

/**
 * @brief Execute a diagnostic or logging command.
 *
 * Clearly indicate that diagnostic commands are not yet implemented.
 *
 * @param action_id Identifies which diagnostic action to execute.
 */
static void commands_core_execute_diagnostics(int action_id)
{
	uart_handler_write_string("Diagnostics commands not implemented yet.\r\n");
	LOG_WRN("Diagnostics command requested but not implemented. action_id=%d", action_id);
}

/**
 * @brief Public API to execute a command based on a given category and action ID.
 *
 * Routes the given category and action_id to the appropriate handler function.
 * Lights commands are fully integrated, while others are pending implementation.
 *
 * @param category The command category (1=Lights, 2=Sensors, 3=System config, 4=Diagnostics)
 * @param action_id The specific action within that category.
 */
void commands_core_execute(int category, int action_id)
{
	LOG_INF("commands_core_execute: category=%d, action_id=%d", category, action_id);

	switch (category) {
	case 1:
		commands_core_execute_lights(action_id);
		break;
	case 2:
		commands_core_execute_sensors(action_id);
		break;
	case 3:
		commands_core_execute_system(action_id);
		break;
	case 4:
		commands_core_execute_diagnostics(action_id);
		break;
	default:
		uart_handler_write_string("Invalid command category.\r\n");
		LOG_WRN("Unknown command category: %d", category);
		break;
	}
}
