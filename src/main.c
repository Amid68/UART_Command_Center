#include "uart_handler.h"
#include "menu.h"

int main(void)
{
    int ret = uart_handler_init();
    if (ret < 0) {
        printk("Failed to initialize UART handler\n");
        return 0;
    }

    // Optionally print a welcome message
    uart_handler_write_string("Welcome! Starting the menu...\r\n");

    // Run the main menu loop
    menu_core_run();

    // If the user exits the menu:
    uart_handler_write_string("Menu exited. Shutting down.\r\n");
    return 0;
}
