/**
 * @file time.h
 * 
 * @brief Description: Header file containing all the timing functions
 * 
 */

#ifndef TIME_H
#define	TIME_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "p33EP512MU810.h"
#include "xc.h"

    // Use this routine to set up the instruction cycle clock
    void setup_clock();
    
    // Run this before using the timer functions
    void setup_timer();
    
    // Reset the timer
    void reset_timer();
    
    // Start the timer
    void start_timer();
    
    // Stop the timer 
    void stop_timer();
    
    // Read the timer
    unsigned long int read_timer();
    

#ifdef	__cplusplus
}
#endif

#endif	/* TIME_H */

