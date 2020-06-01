/*
 * timer.h
 *
 *  Created on: Jul 3, 2018
 *      Author: browt
 */

#ifndef TIMER_H_
#define TIMER_H_

/*
 *  Initializes Timer_A0
 *      SMCLK
 *      Total CLK divider = 8
 *      Up Count Mode
 *      Initial Period 65000 cycles
 *
 */
void init_Motor_Timer(void);

/*
 *  Converts value from Character to int
 *
 */
void motor_change_speed(int value);

/*
 *  Logic to detect motor Change direction input from UART
 *  Sets FLAG if true
 *
 */
void motor_change_direction(char value);

/*
 * The actual motor stepping sequence
 * Toggling the coils
 *
 */
void motor_step_change(void);

/*
 *  Toggles the Compare Capture Interrupt FLAG
 *  Set the step size based on 'RPM' & increments TimerA0 CCR
 *  IF a cycle is complete
 */
void TA0_0_IRQHandler(void);

#endif /* TIMER_H_ */
