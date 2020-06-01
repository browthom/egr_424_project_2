/*
 * auxiliary.h
 *
 *  Created on: Jul 3, 2018
 *      Author: browt
 */

#ifndef AUXILIARY_H_
#define AUXILIARY_H_

/*
 *  Function to disable Watch Dog Timer
 *
 */
void stop_watchdog_timer(void);

/*
 *  Enable Global Interrupts
 *  Enable TimerA and UART Interrupts in the NVIC
 *  Assigned priorities to the Interrupts
 */
void enable_interrupts(void);

/*
 *  Enabled P4.5, P4.7, P5.4 and P5.5 as Outputs
 *
 */
void init_motor_outputs(void);


#endif /* AUXILIARY_H_ */
