/*
 * Copyright (c) 2024 UARTCommandCenter
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * @file test_uart_handler.c
 * @brief Test suite for the UART handler.
 * 
 * Description:
 * This test suite uses ZTest to verify the functionality of the UART handler.
 * Since the UART handler relies on hardware interrupts and device readiness,
 * not all aspects can be fully tested in a native environment. Still, we can:
 *  - Check that initialization returns expected results.
 *  - Confirm that calling write functions with invalid parameters returns errors.
 *  - Verify the message queue is accessible and can store and retrieve data.
 * 
 * @author Ameed Othman
 * @date 2024-12-19
 */

#include <zephyr/ztest.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <string.h>

#include "uart_handler.h"

LOG_MODULE_REGISTER(test_uart_handler, LOG_LEVEL_INF);

/**
 * @brief Test Initialization of the UART Handler
 *
 * Ensures that the uart_handler_init() function returns 0 on success.
 * In the native_posix environment, actual UART hardware may not exist,
 * but if the device tree and configuration are correct, this should still pass.
 */
ZTEST(uart_handler, test_uart_init)
{
	int ret = uart_handler_init();
	zassert_true(ret == 0, "UART initialization failed with code %d", ret);
}

/**
 * @brief Test uart_handler_write_string() with valid and invalid inputs
 *
 * Checks that passing a valid string returns success and passing a NULL pointer
 * returns an error code. Since we are not verifying actual transmission, we
 * only expect correct handling of parameters.
 */
ZTEST(uart_handler, test_uart_write_string)
{
	/* Test valid string */
	const char *test_str = "Test message";
	int ret = uart_handler_write_string(test_str);
	zassert_true(ret == 0, "Expected success when writing a valid string");

	/* Test NULL pointer */
	ret = uart_handler_write_string(NULL);
	zassert_true(ret == -EINVAL, "Expected -EINVAL when passing NULL");
}

/**
 * @brief Test the UART message queue behavior
 *
 * Although we can’t simulate interrupts here, we can verify that the queue
 * is defined, accessible, and can store/retrieve data. Normally, data is placed
 * into this queue by the ISR, but for testing, we can manually inject a line.
 */
ZTEST(uart_handler, test_uart_msgq_behavior)
{
	char test_line_in[] = "Hello, UART Queue!";
	char test_line_out[64];

	/* Try getting a line before anything is put in. This should timeout. */
	k_timeout_t timeout = K_MSEC(10);
	int ret = k_msgq_get(&uart_msgq, test_line_out, timeout);
	zassert_true(ret == -EAGAIN, "Expected -EAGAIN when queue is empty");

	/* Put a line into the queue manually (simulating ISR behavior) */
	ret = k_msgq_put(&uart_msgq, test_line_in, K_NO_WAIT);
	zassert_true(ret == 0, "Failed to put a message into the queue");

	/* Retrieve the line back */
	memset(test_line_out, 0, sizeof(test_line_out));
	ret = k_msgq_get(&uart_msgq, test_line_out, K_NO_WAIT);
	zassert_true(ret == 0, "Failed to retrieve a message from the queue");
	zassert_true(strcmp(test_line_in, test_line_out) == 0,
				 "Retrieved line does not match the inserted line");
}

/* 
 * Test suite definition: Groups all tests above into a single suite.
 */
ZTEST_SUITE(uart_handler, NULL, NULL, NULL, NULL, NULL);
