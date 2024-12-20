/*
 * Copyright (c) 2024
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * @file test_commands.c
 * @brief Test suite for the commands subsystem.
 *
 * Description:
 * ------------
 * This file uses the ZTest framework to verify the behavior of the commands
 * subsystem, including `commands_core_execute()` and related command category
 * handlers (e.g., lights, sensors, system, diagnostics).
 *
 * The tests focus on:
 *  - Ensuring valid commands execute without error.
 *  - Confirming that invalid action IDs or categories are handled gracefully.
 *  - Verifying that each category’s commands route through `commands_core`
 *    correctly.
 *
 * @author Ameed Othman
 * @date 2024-12-20
 */

#include <zephyr/ztest.h>
#include <zephyr/kernel.h>

#include "commands.h"

/* 
 * Optionally, consider adding extern variables or mock functions here if
 * you have implemented them in your source code for testing purposes. For example:
 *
 * extern int lights_command_count;
 * extern int sensors_command_count;
 * ...
 *
 * Reset them in a test setup function to ensure each test runs in a clean state.
 */

/* Test setup fixture: runs once before each test suite */
static void *test_commands_setup(void)
{
    /* If you have global counters or states, reset them here:
     * lights_command_count = 0; 
     * sensors_command_count = 0;
     * ...
     */
    return NULL;
}

/* Test teardown fixture: runs once after each test suite */
static void test_commands_teardown(void *fixture)
{
    /* Perform any cleanup if necessary. */
}

/* 
 * Test valid lights commands: 
 * Category = 1 (Lights)
 * Action IDs:
 *   0: Turn ON
 *   1: Turn OFF
 *   2: Increase Brightness
 *   3: Decrease Brightness
 */
ZTEST(commands, test_lights_valid_actions)
{
    /* Turn ON */
    zassert_true(commands_core_execute(1, 0) == 0, "Failed to execute lights ON");

    /* Turn OFF */
    zassert_true(commands_core_execute(1, 1) == 0, "Failed to execute lights OFF");

    /* Increase Brightness */
    zassert_true(commands_core_execute(1, 2) == 0, "Failed to execute lights Increase Brightness");

    /* Decrease Brightness */
    zassert_true(commands_core_execute(1, 3) == 0, "Failed to execute lights Decrease Brightness");
}

/* Test invalid lights action IDs */
ZTEST(commands, test_lights_invalid_actions)
{
    /* Action ID = 99 is invalid */
    zassert_true(commands_core_execute(1, 99) == 0,
                 "Should handle invalid lights action gracefully (no crash, maybe a warning).");
}

/* 
 * Test sensors commands: 
 * Category = 2 (Sensors)
 * Assume action_id=0 is a valid action (e.g., read a sensor), 
 * and action_id=99 is invalid.
 */
ZTEST(commands, test_sensors)
{
    zassert_true(commands_core_execute(2, 0) == 0, "Failed to execute a valid sensors action");
    zassert_true(commands_core_execute(2, 99) == 0, "Failed to handle invalid sensors action gracefully");
}

/* 
 * Test system configuration commands: 
 * Category = 3 (System)
 * Assume action_id=0 is valid and action_id=99 is invalid.
 */
ZTEST(commands, test_system)
{
    zassert_true(commands_core_execute(3, 0) == 0, "Failed to execute a valid system config action");
    zassert_true(commands_core_execute(3, 99) == 0, "Failed to handle invalid system action gracefully");
}

/* 
 * Test diagnostics/logs commands:
 * Category = 4 (Diagnostics)
 * Assume action_id=0 is valid and action_id=99 is invalid.
 */
ZTEST(commands, test_diagnostics)
{
    zassert_true(commands_core_execute(4, 0) == 0, "Failed to execute a valid diagnostics action");
    zassert_true(commands_core_execute(4, 99) == 0, "Failed to handle invalid diagnostics action gracefully");
}

/* 
 * Test unknown categories:
 * Category = 99 doesn't exist, should be handled without crashing.
 */
ZTEST(commands, test_unknown_category)
{
    zassert_true(commands_core_execute(99, 0) == 0,
                 "Should handle unknown category gracefully without a crash");
}

/* 
 * Test Suite Definition:
 * Groups all command-related tests into a single suite.
 */
ZTEST_SUITE(commands, NULL, test_commands_setup, NULL, NULL, test_commands_teardown);
