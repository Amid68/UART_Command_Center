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

LOG_MODULE_REGISTER(menu_actions, LOG_LEVEL_INF);

/**
 * @brief Public API to execute a menu action based on a given category and ID.
 *
 * All specific actions are now handled via commands_core_execute(), which will
 * route requests to the correct command handler (e.g., command_lights_execute()).
 *
 */
void menu_actions_execute(int category, int action_id)
{
	LOG_INF("menu_actions_execute: category=%d, action_id=%d", category, action_id);

	// Directly call commands_core_execute, no placeholders here.
	commands_core_execute(category, action_id);
}
