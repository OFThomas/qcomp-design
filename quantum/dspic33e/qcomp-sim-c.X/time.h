/**
 * @file time.h
 * 
 * @brief Description: Header file containing all the timing functions
  * @authors J Scott, O Thomas
 * @date Nov 2018
* 
 */

#ifndef TIME_H
#define	TIME_H

#ifdef	__cplusplus
extern "C" {
#endif
    
//#include "xc.h"
#include "spi.h"

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
    
    void delay();

#ifdef	__cplusplus
}
#endif

#endif	/* TIME_H */

