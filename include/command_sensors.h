/**
* @file command_sensors.h
* @brief Sensors command interface.
*
* Description:
* ------------
* This header defines the public interface for executing sensors-related commands.
* It is included by `commands_core.c` and other modules that need to trigger
* sensors actions by calling `command_sensors_execute()`.
*
* By abstracting sensors commands here, we keep code modular, maintainable, and
* easy to expand with additional sensor operations in the future.
*
* Typical Actions:
* ----------------
*  action_id=0: Read temperature
*  action_id=1: Read humidity
* Additional actions can be easily added as needed.
*
* @author Ameed Othman
* @date 2024-12-21
*/

#ifndef COMMAND_SENSORS_H__
#define COMMAND_SENSORS_H__

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * @brief Execute a sensors command.
     *
     * Called by the commands subsystem when a command in the sensors category
     * (category=2) is requested. The action_id parameter determines which sensor
     * reading to perform or which sensor-related action to execute.
     *
     * @param action_id The sensors command action (e.g., 0=read temperature, 1=read humidity).
     */
    void command_sensors_execute(int action_id);

#ifdef __cplusplus
}
#endif

#endif /* COMMAND_SENSORS_H__ */
