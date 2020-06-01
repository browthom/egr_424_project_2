/*
 * uart.h
 *
 *  Created on: Jul 3, 2018
 *      Author: browt
 */

#ifndef UART_H_
#define UART_H_

//Global Variable used to interface w/ UART Buffer
char value_entered;

/*
 * Enables SysTick Timer
 *      period of 0x20000
 * Function to initialize UART Communication
 *      Baud Rate 9600
 *      SMCLK
 *      1 Stop Bit / No Parity
 *      Enables Interrupts for Recieve & Transmit
 *
 */
void init_UART(int baud_rate);

/*
 *  returns value_entered
 *
 */
char UART_Receive(void);

/*
 *   Sets value_entered = '\0'
 *
 */
void UART_Clear(void);

/*
 *  Store string into global variable
 *  Enables Transmit Interrupt
 *
 */
void UART_Send(char* string);


#endif /* UART_H_ */
