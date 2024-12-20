/*
 * Copyright (c) 2024
 *
 * SPDX-License-Identifier: Apache-2.0
 * 
 * @file command_sensors.c
 * @brief Sensor-related command logic.
 *
 * Description:
 * ------------
 * This file implements sensor-related command logic, bridging between the commands
 * subsystem and the sensor readings driver. It defines `command_sensors_execute()`,
 * which is called from `commands_core.c` when a command in the sensors category
 * (category=2) is executed.
 *
 * The design approach:
 * - Professional: Uses descriptive comments, structured logging, and robust error handling.
 * - Maintainable: Easy to add new sensor commands by extending the switch-case.
 * - Comprehensive: Provides clear user feedback via `uart_handler_write_string()`
 *   and logs all actions and errors for easier debugging.
 *
 * Examples of actions:
 *   action_id=0: Read temperature
 *   action_id=1: Read humidity
 * Additional actions can be added as needed. If an action_id is unrecognized, it
 * logs a warning and informs the user that the command is invalid.
 *
 * @author Ameed Othman
 * @date 2024-12-21
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <stdio.h>
#include "command_sensors.h"
#include "sensor_readings.h"
#include "uart_handler.h"

LOG_MODULE_REGISTER(command_sensors, LOG_LEVEL_INF);

/**
 * @brief Execute a sensors-related command.
 *
 * This function is called by `commands_core_execute()` whenever a command in the
 * sensors category is requested. It identifies the requested action (based on 
 * action_id), retrieves the appropriate sensor reading, and prints the result via UART.
 *
 * @param action_id The specific sensor action to execute (e.g., 0=temperature, 1=humidity).
 */
void command_sensors_execute(int action_id)
{
	LOG_INF("command_sensors_execute called with action_id=%d", action_id);

	int ret;
	char buf[64];

	switch (action_id) {
	case 0:
		/* Example: Read a temperature sensor value */
		ret = sensor_readings_get_temperature();
		if (ret >= 0) {
			snprintf(buf, sizeof(buf), "Temperature: %d C\r\n", ret);
			uart_handler_write_string(buf);
			LOG_INF("Temperature read successfully: %d C", ret);
		} else {
			uart_handler_write_string("Failed to read temperature.\r\n");
			LOG_ERR("Failed to read temperature, error code=%d", ret);
		}
		break;

	case 1:
		/* Example: Read a humidity sensor value */
		ret = sensor_readings_get_humidity();
		if (ret >= 0) {
			snprintf(buf, sizeof(buf), "Humidity: %d%%\r\n", ret);
			uart_handler_write_string(buf);
			LOG_INF("Humidity read successfully: %d%%", ret);
		} else {
			uart_handler_write_string("Failed to read humidity.\r\n");
			LOG_ERR("Failed to read humidity, error code=%d", ret);
		}
		break;

	default:
		/* Invalid action_id */
		uart_handler_write_string("Invalid sensors command.\r\n");
		LOG_WRN("Invalid sensors action_id=%d provided to command_sensors_execute", action_id);
		break;
	}
}
