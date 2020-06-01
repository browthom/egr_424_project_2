#include "msp.h"
/*
 * auxiliary.c
 *
 *  Created on: Jul 3, 2018
 *      Author: browt
 */

void stop_watchdog_timer(void) {
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
}

void enable_interrupts(void) {
    // Enable global interrupt
    __enable_irq();

    // Need to go over this portion:
    // - This was taken from 'msp432p401x_euscia0_uart_01'
    // Enable eUSCIA0 interrupt in NVIC module
    // Enable TimerA interrupt in NVIC module
    NVIC->ISER[0] = (1 << (EUSCIA0_IRQn)) |
                    (1 << (TA0_0_IRQn));

    // Assign priorities to interrupts
    NVIC->IP[TA0_0_IRQn] = 0x01;
    NVIC->IP[EUSCIA0_IRQn] = 0x00;
}

void init_motor_outputs(void) {
    P4->DIR = BIT5 | BIT7;    // P4.5 and P4.7 set as output
    P5->DIR = BIT4 | BIT5;    // P5.4 and P5.5 set as output
}
