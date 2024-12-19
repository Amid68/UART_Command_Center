/*
 * Copyright (c) 2024 UARTCommandCenter
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * @file uart_handler.c
 * @brief UART handler implementation.
 * 
 * Description:
 * This file provides UART handling functionality, including initialization,
 * interrupt-driven data reception, and utility functions to send strings
 * over UART. The UART handler uses a message queue to store incoming lines,
 * making them available for higher-level logic such as command parsing
 * and menu navigation.
 * 
 * @author Ameed Othman
 * @date 2024-12-19
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/logging/log.h>
#include <string.h>

#include "app_config.h"
#include "uart_handler.h"

/* 
 * LOG_MODULE_REGISTER allows runtime control of log level.
 * Make sure CONFIG_LOG is enabled in prj.conf.
 */
LOG_MODULE_REGISTER(uart_handler, LOG_LEVEL_INF);

/* Fetch the UART device from the chosen node */
#define UART_DEVICE_NODE DT_CHOSEN(zephyr_shell_uart)
static const struct device *uart_dev = DEVICE_DT_GET(UART_DEVICE_NODE);

/* 
 * Buffer for incoming characters before a newline is received.
 * MSG_SIZE and related constants could be defined in app_config.h
 * for consistency and flexibility.
 */
#ifndef UART_MSG_SIZE
#define UART_MSG_SIZE 64
#endif

#ifndef UART_MSGQ_LEN
#define UART_MSGQ_LEN 10
#endif

/* Message queue to hold complete lines received via UART */
K_MSGQ_DEFINE(uart_msgq, UART_MSG_SIZE, UART_MSGQ_LEN, 4);

/* Internal receive buffer and position index */
static char rx_buf[UART_MSG_SIZE];
static size_t rx_buf_pos = 0;

/* Forward declaration of the interrupt callback */
static void uart_irq_handler(const struct device *dev, void *user_data);

/**
 * @brief Initialize the UART subsystem.
 *
 * This function configures the UART device for interrupt-driven reception and
 * sets up the ISR callback. It must be called before using any other UART handler
 * functions.
 *
 * @return 0 if initialization is successful, or a negative error code otherwise.
 */
int uart_handler_init(void)
{
	if (!device_is_ready(uart_dev)) {
		LOG_ERR("UART device not ready");
		return -ENODEV;
	}

	int ret = uart_irq_callback_user_data_set(uart_dev, uart_irq_handler, NULL);
	if (ret < 0) {
		LOG_ERR("Failed to set UART callback (err %d)", ret);
		return ret;
	}

	uart_irq_rx_enable(uart_dev);
	LOG_INF("UART initialized and RX interrupt enabled");

	return 0;
}

/**
 * @brief Write a string to the UART output.
 *
 * This function sends a null-terminated string via UART using polling mode.
 * It should be used sparingly for short messages (e.g., prompts, logs).
 *
 * @param str A null-terminated string to send.
 * @return 0 on success, or a negative error code on failure.
 */
int uart_handler_write_string(const char *str)
{
	if (!uart_dev || !str) {
		return -EINVAL;
	}

	size_t len = strlen(str);
	for (size_t i = 0; i < len; i++) {
		uart_poll_out(uart_dev, str[i]);
	}

	return 0;
}

/**
 * @brief Attempt to read a complete line from the UART message queue.
 *
 * This is not a required function, but it demonstrates how higher-level modules
 * might interface with the UART handler. It is not declared in the header since
 * it’s optional. Higher-level code may use k_msgq_get() directly on uart_msgq.
 *
 * @param buffer A pointer to a buffer to store the line.
 * @param timeout Kernel timeout for waiting on a complete line.
 * @return 0 on success, or -EAGAIN if no data is available before timeout.
 */
static int uart_handler_read_line(char *buffer, k_timeout_t timeout)
{
	if (!buffer) {
		return -EINVAL;
	}

	return k_msgq_get(&uart_msgq, buffer, timeout);
}

/* 
 * UART interrupt callback:
 * Reads characters from the UART hardware until FIFO is empty.
 * If a newline is encountered, the accumulated line is pushed onto the message queue.
 * Characters beyond the buffer size are dropped to prevent overflow.
 */
static void uart_irq_handler(const struct device *dev, void *user_data)
{
	ARG_UNUSED(user_data);

	if (!uart_irq_update(uart_dev)) {
		return;
	}

	if (!uart_irq_rx_ready(uart_dev)) {
		return;
	}

	uint8_t c;
	while (uart_fifo_read(uart_dev, &c, 1) == 1) {
		/* Check for end-of-line */
		if ((c == '\n' || c == '\r') && rx_buf_pos > 0) {
			rx_buf[rx_buf_pos] = '\0';
			int ret = k_msgq_put(&uart_msgq, rx_buf, K_NO_WAIT);
			if (ret < 0) {
				LOG_WRN("UART message queue full. Dropping input line.");
			}
			rx_buf_pos = 0;
		} else if (rx_buf_pos < (UART_MSG_SIZE - 1)) {
			rx_buf[rx_buf_pos++] = (char)c;
		} else {
			/* Buffer full, drop any additional characters until a newline arrives */
			/* Optional: could log this event if needed */
		}
	}
}

/* End of uart_handler.c */
