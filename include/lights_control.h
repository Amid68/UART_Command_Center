/*
 * Copyright (c) 2024 UARTCommandCenter
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * @file lights_control.h
 * @brief Public interface for controlling lights.
 * 
 * Description:
 * ------------
 * This header defines the public interface for the lights control driver.
 * It provides function prototypes for turning lights on/off, adjusting 
 * brightness levels, and querying their current state.
 *
 * @author Ameed Othman
 * @date 2024-12-20
 */

#ifndef LIGHTS_CONTROL_H__
#define LIGHTS_CONTROL_H__

#include <stdbool.h>
#include <zephyr/kernel.h>  /* For return types like int */

/* C++ support */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize the lights control subsystem.
 *
 * Prepares any necessary hardware interfaces (e.g., GPIO, PWM) or internal state
 * related to controlling the lights. If no hardware initialization is needed, 
 * this may simply set up default values.
 *
 * @return 0 on success, or a negative error code if initialization fails.
 */
int lights_control_init(void);

/**
 * @brief Turn the lights ON.
 *
 * Activates whatever hardware mechanism is required to illuminate the lights.
 * In a simple LED scenario, this might set a GPIO pin high or enable a PWM output.
 *
 * @return 0 on success, or a negative error code if the operation fails.
 */
int lights_control_turn_on(void);

/**
 * @brief Turn the lights OFF.
 *
 * Deactivates the lights. For LEDs, this might mean setting a GPIO pin low or 
 * disabling a PWM output channel.
 *
 * @return 0 on success, or a negative error code if the operation fails.
 */
int lights_control_turn_off(void);

/**
 * @brief Increase the brightness level of the lights.
 *
 * Adjusts the lights to a higher brightness setting, if supported. For a PWM-based
 * LED system, this might increase the duty cycle. The implementation should 
 * ensure brightness does not exceed the maximum allowed level (e.g., 100%).
 *
 * @return 0 on success, or a negative error code if the operation fails.
 */
int lights_control_increase_brightness(void);

/**
 * @brief Decrease the brightness level of the lights.
 *
 * Adjusts the lights to a lower brightness setting. The implementation should 
 * ensure brightness does not go below zero. If the lights are already at minimum 
 * brightness, this operation can either do nothing or log a message indicating 
 * no further reduction is possible.
 *
 * @return 0 on success, or a negative error code if the operation fails.
 */
int lights_control_decrease_brightness(void);

/**
 * @brief Retrieve the current lights state.
 *
 * Queries whether the lights are currently ON or OFF, and what the current 
 * brightness level is. Brightness is typically represented as a percentage (0-100%).
 *
 * @param state Pointer to a bool that will receive the ON/OFF state. True = ON, False = OFF.
 * @param level Pointer to an int that will receive the brightness level (e.g., 0-100).
 *
 * @return 0 on success, or a negative error code if invalid parameters are provided 
 * or the operation fails for some reason.
 */
int lights_control_get_state(bool *state, int *level);

#ifdef __cplusplus
}
#endif

#endif /* LIGHTS_CONTROL_H__ */
