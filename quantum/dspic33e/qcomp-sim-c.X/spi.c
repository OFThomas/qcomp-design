/*
 * File: spi.c
 * 
 * Description: Contains all the functions for reading buttons and writing to
 * LEDs
 *
 */

#include "spi.h"

// Set up serial peripheral interface
int setup_spi(void) {
    
    // Setup the SPI pins
    // J10:41 = J1:91 = uC:70 = RPI74 (PPS code: 0100 1010)
    // J10:44 = J1:93 = uC:9 = RPI52 (PPS code: 011 0100)
    // J10:47 = J1:101 = uC:34 = RPI42 (PPS code: )
    // J10:43 = J1:95 = uC:72 = RP64
    // J10:46 = J1:97 = uC:69 = RPI73
    // J10:7 = J1:13 = uC:3 = RP85
    // J10:5 = J1:7 = uC:5 = RP87
    // J10:55 = J1:117 = uC:10 = RP118
    
    RPOR1 
    
    // Configure the SPI registers
    
    
    return 0;
}
