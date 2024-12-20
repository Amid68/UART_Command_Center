cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(uart_command_center_tests)

# Add test source files (your test files)
target_sources(app PRIVATE
    test_uart_handler.c
    test_commands.c
    test_lights_control.c
    test_menu.c
    test_sensors.c
    test_utils.c
)

# Add source files from the application that define tested functions
target_sources(app PRIVATE
    ../src/uart_handler.c
    ../src/commands/commands_core.c
    ../src/commands/command_lights.c
    ../src/drivers/lights_control.c
)


target_include_directories(app PRIVATE ../include)
