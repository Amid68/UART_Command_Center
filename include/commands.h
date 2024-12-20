/**
 * @file commands.h
 * @brief Command execution interface.
 * 
 * @author Ameed Othman
 * @date 2024-12-19
 */

#ifndef COMMANDS_H__
#define COMMANDS_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Execute a command based on category and action_id.
 *
 * @param category Command category (1=Lights, 2=Sensors, 3=System, 4=Diagnostics)
 * @param action_id Specific action within the category.
 */
void commands_core_execute(int category, int action_id);

#ifdef __cplusplus
}
#endif

#endif /* COMMANDS_H__ */
