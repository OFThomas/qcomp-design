/*
 * File: time.c
 * 
 * Description: Functions to control the on chip timers
 * 
 */

#include "time.h"

// Run this before using the timer functions
// Use timers 2 and 3 (type A and B) together to form a single 32 bit timer
// 
//  * Timer 2 is the low word
//  * Timer 3 is the high word
//
// The timers are set up using TxCON registers (x = 2, 3). The PRx registers
// contain the period of the timers, and the TMRx registers contain the 
// value of the timer.
//
void setup_timer() {
    T2CON = 0x0000; // Reset the timer control registers
    T3CON = 0x0000;
    // Set up timer 2 in 32 bit mode with timer 3
    // Clock prescaler 1:1, internal oscillator source.
    T2CON = 0x0008;
    // Node need to change anything in T3CON
    // Reset TMR2, TMR3, PR2 and PR3
    TMR2 = 0x0000;
    TMR3 = 0x0000;
    PR2 = 0xFFFF; // Highest possible period
    PR3 = 0xFFFF;
}

// Reset the timer
void reset_timer() {
    // Reset TMR2, TMR3, PR2 and PR3
    TMR2 = 0x0000;
    TMR3 = 0x0000;
}

// Start the timer
void start_timer() {
    T2CONbits.TON = 1; // Turn timer 2 on
}

// Stop the timer 
void stop_timer() {
    T2CONbits.TON = 0; // Turn timer 2 off
}

// Read the timer
unsigned long int read_timer() {
    // The 32 bit timer is read by reading TMR2 and then TMR3HLD. Do not read
    // TMR3 -- the contents of TMR3 are automatically moved to TMR3HLD when
    // TMR2 is read
    unsigned int lsw = TMR2; // 16 bit type
    unsigned long int msw = TMR3HLD; // 32 bit type (ready to shift left)
    unsigned long int count; // 32 type for counter result
    count = (msw << 16) + lsw; // Add the msw to lsw to get the total count
    return count;
}
