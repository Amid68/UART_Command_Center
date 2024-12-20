/**
 * @file command_lights.h
 * @brief Lights command interface.
 * 
 * Description:
 * ------------
 * This header provides the function prototype for executing lights-related 
 * commands. It is included by commands_core.c to integrate lights functionality 
 * with the top-level command execution framework.
 * 
 * By separating lights commands here, we keep code modular, maintainable, and 
 * easy to expand with additional lights operations in the future.
 * 
 * Author: Ameed Othman
 * Date: 2024-12-20
 */

#ifndef COMMAND_LIGHTS_H__
#define COMMAND_LIGHTS_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Execute a lights-specific command.
 *
 * @param action_id The ID of the lights action to execute.
 *                  Example: 0=Turn ON, 1=Turn OFF, etc.
 */
void command_lights_execute(int action_id);

#ifdef __cplusplus
}
#endif

#endif /* COMMAND_LIGHTS_H__ */
