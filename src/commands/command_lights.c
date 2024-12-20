/*
 * Copyright (c) 2024 UARTCommandCenter
 *
 * SPDX-License-Identifier: Apache-2.0
 * 
 * @file command_lights.c
 * @brief High-level lights control commands.
 *
 * Description:
 * ------------
 * This file implements high-level lights control commands, providing a 
 * clean interface for `commands_core_execute()` calls that target lights.
 *
 * @author Ameed Othman
 * @date 2024-12-20
 * 
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include "lights_control.h"   // Low-level driver interface for controlling lights
#include "uart_handler.h"

LOG_MODULE_REGISTER(command_lights, LOG_LEVEL_INF);

/* 
 * Example actions:
 * action_id = 0: Toggle Lights ON
 * action_id = 1: Toggle Lights OFF
 * action_id = 2: Increase Brightness (if supported)
 * action_id = 3: Decrease Brightness (if supported)
 */

/**
 * @brief Execute a lights command given an action_id.
 *
 * @param action_id Identifies which lights command to execute.
 *                  Use a defined scheme (e.g., 0=ON, 1=OFF, etc.).
 */
static void command_lights_execute_action(int action_id)
{
	switch (action_id) {
	case 0:
		if (lights_control_turn_on() == 0) {
			uart_handler_write_string("Lights turned ON.\r\n");
			LOG_INF("Lights are ON");
		} else {
			uart_handler_write_string("Failed to turn lights ON.\r\n");
			LOG_ERR("Failed to execute lights ON command");
		}
		break;
	case 1:
		if (lights_control_turn_off() == 0) {
			uart_handler_write_string("Lights turned OFF.\r\n");
			LOG_INF("Lights are OFF");
		} else {
			uart_handler_write_string("Failed to turn lights OFF.\r\n");
			LOG_ERR("Failed to execute lights OFF command");
		}
		break;
	case 2:
		if (lights_control_increase_brightness() == 0) {
			uart_handler_write_string("Brightness increased.\r\n");
			LOG_INF("Brightness level raised");
		} else {
			uart_handler_write_string("Failed to increase brightness.\r\n");
			LOG_ERR("Failed to increase brightness");
		}
		break;
	case 3:
		if (lights_control_decrease_brightness() == 0) {
			uart_handler_write_string("Brightness decreased.\r\n");
			LOG_INF("Brightness level lowered");
		} else {
			uart_handler_write_string("Failed to decrease brightness.\r\n");
			LOG_ERR("Failed to decrease brightness");
		}
		break;
	default:
		uart_handler_write_string("Invalid lights command.\r\n");
		LOG_WRN("Invalid action_id for lights: %d", action_id);
		break;
	}
}

/**
 * @brief Public function called by commands_core to execute lights commands.
 *
 * This function serves as an entry point for the lights command category. 
 * It ensures that high-level commands from commands_core_execute() are 
 * translated into appropriate driver calls. If you expand lights functionality, 
 * add more cases in the switch statement above.
 *
 * @param action_id Action identifier passed down from commands_core.
 */
void command_lights_execute(int action_id)
{
	LOG_INF("command_lights_execute: action_id=%d", action_id);
	command_lights_execute_action(action_id);
}
