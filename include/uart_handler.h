/*
 * Copyright (c) 2024 UARTCommandCenter
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * @file uart_handler.h
 * @brief UART handler public interface.
 * 
 * Description:
 * This header provides the public interface for UART handling functionality.
 * The UART handler is responsible for:
 *   - Initializing the UART interface with interrupt-driven reception.
 *   - Providing a message queue from which complete input lines can be retrieved.
 *   - Offering utility functions to write strings to the UART output.
 *
 * Other modules can interact with the UART via these functions, enabling line-based
 * command parsing and interactive menus over UART.
 * 
 * @author Ameed Othman
 * @date 2024-12-19
 */

#ifndef UART_HANDLER_H__
#define UART_HANDLER_H__

#include <zephyr/kernel.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize the UART subsystem.
 *
 * This function configures the UART device for interrupt-driven reception,
 * sets the IRQ callback, and enables RX interrupts. It should be called
 * once during startup before using any other UART handler functions.
 *
 * @return 0 on success, or a negative error code on failure.
 */
int uart_handler_init(void);

/**
 * @brief Write a null-terminated string to the UART output.
 *
 * Sends the given string over UART using polling mode. This function is
 * best used for relatively short messages, such as prompts, logging messages,
 * or status updates. For large amounts of data, consider implementing a
 * buffered or DMA-based solution.
 *
 * @param str A null-terminated string to send.
 * @return 0 on success, or a negative error code on invalid parameters.
 */
int uart_handler_write_string(const char *str);

/**
 * @brief Externally accessible message queue for retrieved UART lines.
 *
 * The UART handler pushes complete lines (terminated by newline or carriage return)
 * into this queue. Other application components (e.g., menu systems or command
 * parsers) can retrieve lines non-blocking or blocking using `k_msgq_get()`.
 *
 * Usage example:
 * @code
 *   char line_buffer[LINE_SIZE];
 *   if (k_msgq_get(&uart_msgq, line_buffer, K_FOREVER) == 0) {
 *       // Process the retrieved line
 *   }
 * @endcode
 *
 * Note:
 * The size of the queue elements and the number of messages, as well as the line buffer size,
 * should be defined in app_config.h or uart_handler.c as per design.
 */
extern struct k_msgq uart_msgq;

#ifdef __cplusplus
}
#endif

#endif /* UART_HANDLER_H__ */
