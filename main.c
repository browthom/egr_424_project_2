// Custom Driver Lib
#include "uart.h"
#include "timer.h"
#include "auxiliary.h"

/**
 * main.c
 */
void main(void) {
    // Stop Watchdog timer
    stop_watchdog_timer();
    // Initialize Motor Outputs
    init_motor_outputs();

    // Initialize UART communication with baud rate
    init_UART(9600);
    // Initialize Timer A
    init_Motor_Timer();



    // Enable UART and Timer A Interrupts
    enable_interrupts();

    UART_Send("\n\rSelect Speed (0,1,2,3,4,5) or Direction (l,r): ");

    while(1) {

        if (UART_Receive() >= '0' && UART_Receive() <= '5') {
            // Change Motor Speed
            motor_change_speed(UART_Receive());
            UART_Send("\n\rSelect Speed (0,1,2,3,4,5) or Direction (l,r): ");
            UART_Clear();

        } else if (UART_Receive() == 'l' ||
                   UART_Receive() == 'r') {

            motor_change_direction(UART_Receive());
            UART_Send("\n\rSelect Speed (0,1,2,3,4,5) or Direction (l,r): ");
            UART_Clear();

        } else if (UART_Receive() != '\0' &&
                   !(UART_Receive() >= '0' && UART_Receive() <= '5') &&
                   !(UART_Receive() == 'l' || UART_Receive() == 'r')) {
            UART_Send("\n\rSelect Speed (0,1,2,3,4,5) or Direction (l,r): ");
            UART_Clear();
        }

        motor_step_change();
    }
}
