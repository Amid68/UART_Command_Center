/*
 * Copyright (c) 2024 UARTCommandCenter
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * @file command_lights.c
 * @brief Lights command logic.
 *
 * Description:
 * ------------
 * This file provides the logic for executing lights-related commands.
 * It translates action_id values into corresponding calls to lights_control.c
 * functions:
 *   action_id = 0: Turn ON
 *   action_id = 1: Turn OFF
 *   action_id = 2: Increase Brightness
 *   action_id = 3: Decrease Brightness
 *
 * If a valid action_id is given, the appropriate lights_control function is
 * called. Depending on success or failure, a message is printed to the user.
 * If the action_id is invalid, an error message is displayed.
 *
 * This implementation ensures that commands_core.c and menu_actions_execute()
 * can successfully route lights commands to actual functionality.
 *
 * Author: Ameed Othman
 * Date: 2024-12-20
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include "command_lights.h"
#include "lights_control.h"
#include "uart_handler.h"

LOG_MODULE_REGISTER(command_lights, LOG_LEVEL_INF);

void command_lights_execute(int action_id)
{
	LOG_INF("command_lights_execute called with action_id=%d", action_id);

	int ret;
	switch (action_id) {
	case 0: // Turn ON
		ret = lights_control_turn_on();
		if (ret == 0) {
			uart_handler_write_string("Lights turned ON.\r\n");
			LOG_INF("Lights turned ON successfully.");
		} else {
			uart_handler_write_string("Failed to turn lights ON.\r\n");
			LOG_ERR("Failed to turn lights ON, error code %d", ret);
		}
		break;
	case 1: // Turn OFF
		ret = lights_control_turn_off();
		if (ret == 0) {
			uart_handler_write_string("Lights turned OFF.\r\n");
			LOG_INF("Lights turned OFF successfully.");
		} else {
			uart_handler_write_string("Failed to turn lights OFF.\r\n");
			LOG_ERR("Failed to turn lights OFF, error code %d", ret);
		}
		break;
	case 2: // Increase Brightness
		ret = lights_control_increase_brightness();
		if (ret == 0) {
			uart_handler_write_string("Brightness increased.\r\n");
			LOG_INF("Brightness increased successfully.");
		} else {
			uart_handler_write_string("Failed to increase brightness.\r\n");
			LOG_ERR("Failed to increase brightness, error code %d", ret);
		}
		break;
	case 3: // Decrease Brightness
		ret = lights_control_decrease_brightness();
		if (ret == 0) {
			uart_handler_write_string("Brightness decreased.\r\n");
			LOG_INF("Brightness decreased successfully.");
		} else {
			uart_handler_write_string("Failed to decrease brightness.\r\n");
			LOG_ERR("Failed to decrease brightness, error code %d", ret);
		}
		break;
	default:
		uart_handler_write_string("Invalid lights action.\r\n");
		LOG_WRN("Invalid action_id=%d provided to command_lights_execute", action_id);
		break;
	}
}
