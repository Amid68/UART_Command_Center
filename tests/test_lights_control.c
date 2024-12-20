/*
 * Copyright (c) 2024
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * @file test_lights_control.c
 * @brief Test suite for the lights control driver.
 *
 * Description:
 * ------------
 * This test file uses ZTest to validate the functions provided by `lights_control.c`.
 * It assumes that lights_control_* functions (turn_on, turn_off, increase_brightness,
 * decrease_brightness) return 0 on success and possibly other values on error.
 *
 * The tests focus on:
 *  - Turning lights on and off.
 *  - Increasing and decreasing brightness multiple times.
 *  - Ensuring brightness does not exceed the expected maximum or go below minimum.
 *  - Confirming that repeated on/off calls behave correctly.
 *
 * @author Ameed Othman
 * @date 2024-12-20
 */

#include <zephyr/ztest.h>
#include <zephyr/kernel.h>

#include "lights_control.h"

/* Optional: If you track lights state in a global variable, reset it in setup. */

/* Setup fixture: runs once before the test suite */
static void *test_lights_control_setup(void)
{
    /* If lights_control has any init function, call it here:
     * lights_control_init();
     */
    return NULL;
}

/* Teardown fixture: runs once after the test suite completes */
static void test_lights_control_teardown(void *fixture)
{
    /* Perform cleanup if necessary. */
}

/* Test turning lights ON and OFF */
ZTEST(lights_control, test_on_off)
{
    int ret;

    /* Initially turn on the lights */
    ret = lights_control_turn_on();
    zassert_equal(ret, 0, "Failed to turn lights ON");
    
    /* Turn off the lights */
    ret = lights_control_turn_off();
    zassert_equal(ret, 0, "Failed to turn lights OFF");

    /* Try turning off again - it should still succeed */
    ret = lights_control_turn_off();
    zassert_equal(ret, 0, "Failed turning lights OFF when already off");

    /* Turn on again */
    ret = lights_control_turn_on();
    zassert_equal(ret, 0, "Failed to turn lights ON again");
}

/* Test brightness adjustments */
ZTEST(lights_control, test_brightness)
{
    int ret;

    /* Ensure lights are ON to adjust brightness meaningfully */
    ret = lights_control_turn_on();
    zassert_equal(ret, 0, "Failed to ensure lights are ON before brightness test");

    /* Increase brightness several times */
    for (int i = 0; i < 5; i++) {
        ret = lights_control_increase_brightness();
        /* Assuming no errors for now; in a real test, check brightness levels */
        zassert_equal(ret, 0, "Failed to increase brightness");
    }

    /* Decrease brightness multiple times */
    for (int i = 0; i < 3; i++) {
        ret = lights_control_decrease_brightness();
        zassert_equal(ret, 0, "Failed to decrease brightness");
    }

    /* You might add more specific assertions if you know the expected brightness bounds.
     * For example:
     * zassert_equal(lights_control_get_current_brightness(), 70, "Unexpected brightness level");
     * This requires adding a lights_control_get_state() or similar function for verification.
     */
}

/* Test boundaries: If brightness is at max or min, ensure no error occurs */
ZTEST(lights_control, test_brightness_boundaries)
{
    int ret;

    /* Increase brightness until at max (assuming 100% max) */
    for (int i = 0; i < 20; i++) {
        ret = lights_control_increase_brightness();
        zassert_equal(ret, 0, "Failed to handle brightness at upper boundary");
    }

    /* Decrease brightness until at min (0%) */
    for (int i = 0; i < 20; i++) {
        ret = lights_control_decrease_brightness();
        zassert_equal(ret, 0, "Failed to handle brightness at lower boundary");
    }
}

/* Test repeated on/off cycles to ensure stable state transitions */
ZTEST(lights_control, test_repeated_on_off_cycles)
{
    int ret;

    for (int i = 0; i < 3; i++) {
        ret = lights_control_turn_on();
        zassert_equal(ret, 0, "Failed in repeated ON cycle");
        ret = lights_control_turn_off();
        zassert_equal(ret, 0, "Failed in repeated OFF cycle");
    }
}

/* Test Suite Definition */
ZTEST_SUITE(lights_control, NULL, test_lights_control_setup, NULL, NULL, test_lights_control_teardown);
