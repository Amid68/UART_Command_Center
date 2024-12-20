# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(UART_Command_Center)

target_sources(app PRIVATE src/main.c
            src/uart_handler.c
            src/menu/menu_core.c
            src/menu/menu_actions.c
            src/menu/menu_display.c
            src/commands/commands_core.c
            src/drivers/lights_control.c
            src/commands/command_lights.c
)
target_include_directories(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
