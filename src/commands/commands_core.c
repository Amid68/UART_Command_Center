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
 * by the menu system. It ties together the various command implementations 
 * (e.g., lights, sensors, system configuration, diagnostics) into a unified 
 * API. By doing this, the menu system and other components can trigger 
 * high-level commands without needing to know the underlying implementation details.
 * 
 * @author Ameed Othman
 * @date 2024-12-19
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include "commands.h"         // Public interface for commands (if defined)
#include "uart_handler.h"     // For sending feedback if needed

LOG_MODULE_REGISTER(commands_core, LOG_LEVEL_INF);

/**
 * @brief Execute a command for lights control.
 *
 * This function acts as a bridge to the lights command implementation.
 * For now, it may just print a placeholder message. In the future, 
 * it can call functions from "command_lights.c" to perform real actions.
 *
 * @param action_id Identifies which lights action to execute.
 */
static void commands_core_execute_lights(int action_id)
{
	uart_handler_write_string("Executing lights command (placeholder).\r\n");
	LOG_INF("Lights command executed (ID: %d)", action_id);
	/* Future: Integrate with command_lights.c functions */
}

/**
 * @brief Execute a command for sensor operations.
 *
 * This function will route sensor-related requests to the appropriate 
 * sensor reading or processing logic.
 *
 * @param action_id Identifies which sensor action to execute.
 */
static void commands_core_execute_sensors(int action_id)
{
	uart_handler_write_string("Executing sensor command (placeholder).\r\n");
	LOG_INF("Sensor command executed (ID: %d)", action_id);
	/* Future: Integrate with command_sensors.c functions */
}

/**
 * @brief Execute a system configuration command.
 *
 * System commands might adjust configuration parameters, calibrate 
 * devices, or change operation modes.
 *
 * @param action_id Identifies which system config action to execute.
 */
static void commands_core_execute_system(int action_id)
{
	uart_handler_write_string("Executing system config command (placeholder).\r\n");
	LOG_INF("System config command executed (ID: %d)", action_id);
	/* Future: Integrate with command_system.c functions */
}

/**
 * @brief Execute a diagnostic or logging command.
 *
 * Diagnostics commands might show logs, memory usage, or other system 
 * stats. This function will route those requests accordingly.
 *
 * @param action_id Identifies which diagnostic action to execute.
 */
static void commands_core_execute_diagnostics(int action_id)
{
	uart_handler_write_string("Executing diagnostics command (placeholder).\r\n");
	LOG_INF("Diagnostics command executed (ID: %d)", action_id);
	/* Future: Integrate with diagnostic or logger functions directly */
}

/**
 * @brief Public API to execute a command based on a given category and action ID.
 *
 * This function provides a unified entry point for the menu system (or other 
 * parts of the application) to request that a certain command be executed.
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
