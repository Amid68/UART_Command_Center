cmake_minimum_required(VERSION 3.20.0)

# Locate the Zephyr environment
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(uart_command_center_tests)

# Add all test source files
# Adjust the list below to include the tests you want to run
target_sources(app PRIVATE
    test_uart_handler.c
    test_commands.c
    test_lights_control.c
    test_menu.c
    test_sensors.c
    test_utils.c
)

# Ensure the test application can access the project's include files
target_include_directories(app PRIVATE ../include)

# If additional dependencies or libraries are needed for testing,
# link them here. For example:
# target_link_libraries(app PRIVATE some_dependency_lib)

# Note:
# - Building and running tests can be done with commands like:
#   `west build -b native_posix tests/ -t run`
#   This runs the tests in a host environment.
# - For hardware tests:
#   `west build -b <your_board> tests/ && west flash && west run`
