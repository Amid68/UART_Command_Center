/**
 * @file menu_actions.h
 * @brief Menu action execution.
 * 
 * Description:
 * ------------
 * 
 * This file handles the execution of menu-related actions triggered by user input 
 * from the menu system. It acts as a dispatch center that translates user choices 
 * into meaningful operations, such as toggling lights, reading sensors, or 
 * adjusting system configurations.
 * 
 * @author Ameed Othman
 * @date 2024-12-20
 */

#ifndef MENU_ACTIONS_H__
#define MENU_ACTIONS_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Execute a menu action based on a given category and action ID.
 *
 * This function is the interface from the menu system to the command execution layer.
 * It takes a category (e.g., lights=1, sensors=2, etc.) and an action_id, and then
 * delegates to the appropriate command logic via commands_core_execute().
 *
 * @param category The category of action (1=Lights, 2=Sensors, 3=System, 4=Diagnostics).
 * @param action_id Specific action ID within that category.
 */
void menu_actions_execute(int category, int action_id);

#ifdef __cplusplus
}
#endif

#endif /* MENU_ACTIONS_H__ */
