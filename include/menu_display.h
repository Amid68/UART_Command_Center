/**
 * @file menu_display.h
 * @brief Menu display functions.
 * 
 * Description:
 * ------------
 * 
 * This file provides functions to display menus and messages to the user via UART. 
 * 
 * @author Ameed Othman
 * @date 2024-12-20
 */

#ifndef MENU_DISPLAY_H__
#define MENU_DISPLAY_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Show the main menu options to the user.
 *
 * This function prints a predefined list of main menu items.
 */
void menu_display_show_main_menu(void);

/**
 * @brief Print a generic message to the user via UART.
 *
 * This function is used by other parts of the menu system to present 
 * simple prompts, instructions, or feedback messages.
 *
 * @param msg A null-terminated string containing the message to print.
 */
void menu_display_message(const char *msg);

/**
 * @brief Print an error message indicating invalid input or similar conditions.
 *
 * A dedicated function for errors allows distinct styling or logging.
 *
 * @param err_msg A null-terminated string describing the error condition.
 */
void menu_display_error(const char *err_msg);

#ifdef __cplusplus
}
#endif

#endif /* MENU_DISPLAY_H__ */
