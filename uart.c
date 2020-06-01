#include "msp.h"
#include "uart.h"
/*
 * uart.c
 *
 *  Created on: Jul 3, 2018
 *      Author: browt
 */
char* global_string;

void init_UART(int baud_rate) {

    // UART Pins
    P1->SEL0 |= BIT2 | BIT3;

    // Set D0 bit -> Puts UART into software reset
    EUSCI_A0->CTLW0 |= UCSWRST;

    // Clear bit 0 of UCA0MTLW register
    EUSCI_A0->MCTLW &= ~UCOS16;

    // Set 9600 Baud Rate
    // 3MHz / 9600 = 312.5
    // This must be the case when
    // setting the baud rate: UCSWRST = 1
    int time = 3000000 / baud_rate;
    EUSCI_A0->BRW = time;

    // Select SMCLK
    EUSCI_A0->CTLW0 |= UCSSEL1 | UCSSEL0;
    // Clear Bit 12 for 8-bit data
    EUSCI_A0->CTLW0 &= ~UC7BIT;
    // Clear bit for 1 Stop bit
    EUSCI_A0->CTLW0 &= ~UCSPB;
    // Clear for No Parity
    EUSCI_A0->CTLW0 &= ~UCPEN;

    // Clear D0, thus disabling RESET mode
    EUSCI_A0->CTLW0 &= ~UCSWRST;
    // Clear eUSCI RX interrupt flag
    EUSCI_A0->IFG &= ~UCRXIFG;
    EUSCI_A0->IFG &= ~UCTXIFG;
    // Enable USCI_A0 RX and TX interrupt
    EUSCI_A0->IE = UCRXIE | UCTXIE;
}

void UART_Send(char* string) {
    // Store string into global
    global_string = string;
    // Trigger Interrupt
    EUSCI_A0->IFG |= UCTXIFG;
}

char UART_Receive() {
    return value_entered;
}

void UART_Clear() {
    value_entered = '\0';
}

void EUSCIA0_IRQHandler(void) {


    // If byte has been received
    if (EUSCI_A0->IFG & UCRXIFG) {

        // Retrieve character that was sent
        value_entered = EUSCI_A0->RXBUF;
        // Transmit character
        EUSCI_A0->TXBUF = value_entered;

    }

    // If transmit interrupt flag is set
    // If there are chars left in the string
    if ((EUSCI_A0->IFG & UCTXIFG) && (global_string[0] != '\0')) {
        // Individually transmit each character of the string
        EUSCI_A0->TXBUF = global_string[0];
        // Increment the beginning pointer of the string for next go-around
        global_string++;

    // Clear transmit interrupt flag so that this ISR is not accessed anymore
    // Until a new string is added to 'global_string'
    } else if (global_string[0] == '\0') {
        EUSCI_A0->IFG &= ~UCTXIFG;
    }
}
