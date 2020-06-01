#include "msp.h"
/*
 * timer.c
 *
 *  Created on: Jul 3, 2018
 *      Author: browt
 */
int cycle_stage = 0;
int speed_level = 0;
float RPM = 0.0;
int step_size = 65500;
char direction = 'r'; // 0-> clockwise; 1-> counterclockwise
int change_direction = 0; // Boolean for changing direction

void init_Motor_Timer(void) {
    TIMER_A0->CCTL[0] = CCIE;           // TACCR0 interrupt enabled
    TIMER_A0->CCR[0] = 65000;
    // Clock Divider
    // 3,000,000 / (2 * 4) = 375,000 counts per second
    // 375,000 (counts per second) / (counts) = Hz
    //
    //
    // (1 / 4096) * (375,000 / RPM) * 60 = Counts
    // (1/4096) * (375,000 / Counts) * 60 = RPM
    TIMER_A0->CTL = TASSEL1 |           // SMCLK,
                    ID0 |               // Divide by 2 Clock Divider
                    MC1;                // Up mode: counts up to CCR
    TIMER_A0->EX0 = TAIDEX1 | TAIDEX0;  // Divide by 4 Clock Divider
}

void motor_change_speed(char value) {
    speed_level = value-48;
}

void motor_change_direction(char value) {
    if ((value == 'r' && direction == 'l') ||
        (value == 'l' && direction == 'r')) {
        change_direction = 1; // clockwise
    }
}

void motor_step_change() {
    // One -> P4.5
    // Two -> P4.7
    // Three -> P5.4
    // Four -> P5.5
    if (cycle_stage == 0) {
        P4->OUT = BIT5;
        P5->OUT = 0;
    } else if (cycle_stage == 1) {
        P4->OUT = BIT5 | BIT7;
        P5->OUT = 0;
    } else if (cycle_stage == 2) {
        P4->OUT = BIT7;
        P5->OUT = 0;
    } else if (cycle_stage == 3) {
        P4->OUT = BIT7;
        P5->OUT = BIT4;
    } else if (cycle_stage == 4) {
        P4->OUT = 0;
        P5->OUT = BIT4;
    } else if (cycle_stage == 5) {
        P4->OUT = 0;
        P5->OUT = BIT4 | BIT5;
    } else if (cycle_stage == 6) {
        P4->OUT = 0;
        P5->OUT = BIT5;
    } else if (cycle_stage == 7) {
        P4->OUT = BIT5;
        P5->OUT = BIT5;
    }
}

void TA0_0_IRQHandler(void) {
    // Clear Interrupt Flag
    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;

    // (1 / 4096) * (375,000 / RPM) * 60 = Counts
    // Set the step size based on 'RPM'
    step_size = (int) (5493.0 / RPM);

    // Once step size has gotten really large,
    // then leave it at 65,500
    // This occurs right before '0 RPM'
    // - Also make sure that there's no overflow (step_size < 0)
    // - Make sure that step_size is set to 65,500 when RPM == 0.0
    if (step_size >= 65500 || step_size < 0 || RPM == 0.0) {
        step_size = 65500;

        if (change_direction == 1) {
            // This is to let us know that the change in direction
            // can occur now since '0 RPM' has been reached.
            change_direction = 0;
            // Now change direction
            direction = (direction == 'r') ? 'l' : 'r';
        }
    }

    // Add Offset to TACCR0 (Setup Interval)
    TIMER_A0->CCR[0] += step_size;

        // If 65,500 hasn't been reached (0 RPM)
    if (step_size != 65500 ||
        // If we need to accelerate out of a stop position
        (step_size == 65500 && speed_level != 0)) {

        // Change Cycle Stage of Half-Step Motor Sequence
        if (direction == 'l') {
            cycle_stage = cycle_stage > 0 ? cycle_stage - 1 : 7;
        } else if (direction == 'r') {
            cycle_stage = cycle_stage < 7 ? cycle_stage + 1 : 0;
        }

        // If Cycle complete, then change speed
        if (cycle_stage == 7) {
            // Execute if the direction is not going to be changed
            if ((RPM < speed_level*3 && change_direction == 0)) {
                RPM += 0.1;
            // Execute if the direction is not going to be changed and speed needs to be decremented
            // or the direction needs to be changed.
            // If the direction is going to be changed, a decrement in speed
            // needs to occur first.
            } else if ((RPM > speed_level*3 && change_direction == 0) ||
                       change_direction == 1) {
                RPM -= 0.1;
            }
        }
    }
}
