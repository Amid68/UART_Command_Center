/*
 * Copyright (c) 2024 UARTCommandCenter
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * @file lights_control.c
 * @brief Low-level driver for controlling lights via PWM.
 *
 * Description:
 * ------------
 * This file implements a driver interface for controlling lights (LEDs) using a
 * PWM peripheral on the STM32F446RE Nucleo. It references the devicetree overlay
 * that enables TIM3 Channel 1 on PA6 for PWM output, but now labeled as `timer3:`.
 *
 * Key points:
 *  - The devicetree overlay uses `timer3: timers@40000400`.
 *  - For brightness, we store an integer (0-100) and compute the duty cycle as
 *      (brightness/100) * 20,000 microseconds.
 *  - The code references `DT_NODELABEL(timer3)` to find the PWM device.
 *
 * Author: Ameed Othman
 * Date: 2024-12-20
 */

#include <zephyr/device.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/dt-bindings/pwm/pwm.h>
#include <zephyr/logging/log.h>
#include "lights_control.h"

/* Logging for debug/tracing. */
LOG_MODULE_REGISTER(lights_control, LOG_LEVEL_INF);

/*
 * We'll store:
 *   - pwm_dev:        Pointer to the PWM device (timer3).
 *   - pwm_period_us:  Period in microseconds (20 kHz = 20,000 µs).
 *   - brightness:     Current brightness (0..100).
 *   - on_state:       Whether the lights are currently ON or OFF.
 */

/*
 * Because our overlay now labels the node as:
 *    timer3: timers@40000400 { ... };
 * we use DT_NODELABEL(timer3) here:
 */
#define PWM_TIMER3_NODE DT_NODELABEL(timers3)

/* 20 kHz period => 20,000 µs */
static const uint32_t pwm_period_us = 20000;
static const struct device *pwm_dev;
static bool on_state = false;
static int brightness = 50;

/**
 * @brief Initialize the lights subsystem by retrieving the PWM device.
 */
int lights_control_init(void)
{
    /* Look up the device pointer for `timer3:` */
    pwm_dev = DEVICE_DT_GET(PWM_TIMER3_NODE);
    if (!device_is_ready(pwm_dev)) {
        LOG_ERR("timer3 not ready for PWM!");
        return -ENODEV;
    }

    on_state = false;
    brightness = 50;
    LOG_INF("lights_control_init: Using timer3 at 20 kHz, default brightness=%d%%", brightness);
    return 0;
}

/**
 * @brief Turn the lights ON at the stored brightness.
 */
int lights_control_turn_on(void)
{
    on_state = true;

    if (brightness == 0) {
        brightness = 50;
        LOG_INF("Brightness was 0%%; setting to default 50%% on turn_on()");
    }

    /* Compute duty cycle in microseconds: (brightness / 100) * 20000 */
    uint32_t duty_us = (brightness * pwm_period_us) / 100U;

    int ret = pwm_set(pwm_dev, 1 /* channel */,
                      PWM_USEC(pwm_period_us),
                      PWM_USEC(duty_us),
                      PWM_POLARITY_NORMAL);
    if (ret < 0) {
        LOG_ERR("Failed to set PWM ON, ret=%d", ret);
        return ret;
    }

    LOG_INF("Lights turned ON. brightness=%d%% => duty=%u us", brightness, duty_us);
    return 0;
}

/**
 * @brief Turn the lights OFF by setting duty cycle = 0.
 */
int lights_control_turn_off(void)
{
    on_state = false;

    int ret = pwm_set(pwm_dev, 1 /* channel */,
                      PWM_USEC(pwm_period_us),
                      PWM_USEC(0),
                      PWM_POLARITY_NORMAL);
    if (ret < 0) {
        LOG_ERR("Failed to set PWM OFF, ret=%d", ret);
        return ret;
    }

    LOG_INF("Lights turned OFF, channel=1");
    return 0;
}

/**
 * @brief Increase brightness by 10% steps up to 100%.
 */
int lights_control_increase_brightness(void)
{
    if (brightness <= 90) {
        brightness += 10;
    } else {
        brightness = 100;
        LOG_INF("Brightness is already at max (100%%).");
    }

    if (!on_state) {
        LOG_INF("Lights OFF; storing brightness=%d%% without enabling LED", brightness);
        return 0;
    }

    uint32_t duty_us = (brightness * pwm_period_us) / 100U;
    int ret = pwm_set(pwm_dev, 1 /* channel */,
                      PWM_USEC(pwm_period_us),
                      PWM_USEC(duty_us),
                      PWM_POLARITY_NORMAL);
    if (ret < 0) {
        LOG_ERR("Failed to increase brightness, ret=%d", ret);
        return ret;
    }

    LOG_INF("Brightness increased to %d%% => duty=%u us", brightness, duty_us);
    return 0;
}

/**
 * @brief Decrease brightness by 10% steps down to 0%.
 */
int lights_control_decrease_brightness(void)
{
    if (brightness >= 10) {
        brightness -= 10;
    } else {
        brightness = 0;
        LOG_INF("Brightness is already at min (0%%).");
    }

    if (!on_state) {
        LOG_INF("Lights OFF; storing brightness=%d%% without enabling LED", brightness);
        return 0;
    }

    uint32_t duty_us = (brightness * pwm_period_us) / 100U;
    int ret = pwm_set(pwm_dev, 1 /* channel */,
                      PWM_USEC(pwm_period_us),
                      PWM_USEC(duty_us),
                      PWM_POLARITY_NORMAL);
    if (ret < 0) {
        LOG_ERR("Failed to decrease brightness, ret=%d", ret);
        return ret;
    }

    LOG_INF("Brightness decreased to %d%% => duty=%u us", brightness, duty_us);
    return 0;
}

/**
 * @brief Retrieve current lights state (ON/OFF) and brightness (0..100).
 */
int lights_control_get_state(bool *state, int *level)
{
    if (!state || !level) {
        LOG_ERR("lights_control_get_state: Invalid pointers");
        return -EINVAL;
    }

    *state = on_state;
    *level = brightness;
    LOG_INF("Queried lights state: on_state=%d, brightness=%d%%", on_state, brightness);
    return 0;
}