/*
 * File: spi.c
 * 
 * Description: Functions for communicating with serial devices
 *
 */

#include "spi.h"

// Set up serial peripheral interface
int setup_spi(void) {
    
    // --- Pin mappings and codes ---
    // J10:41 = J1:91 = uC:70 = RPI74 (PPS code: 0100 1010)
    // J10:44 = J1:93 = uC:9 = RPI52 (PPS code: 0011 0100)
    // J10:47 = J1:101 = uC:34 = RPI42 (PPS code: 0010 1010)
    // J10:43 = J1:95 = uC:72 = RP64 (PPS reg: RPOR0_L)
    // J10:46 = J1:97 = uC:69 = RPI73 (PPS code: 0100 1001)
    // J10:7 = J1:13 = uC:3 = RP85 (PPS reg: RPOR6_L)
    // J10:5 = J1:7 = uC:5 = RP87 (PPS reg: RPOR6_H)
    // J10:55 = J1:117 = uC:10 = RP118 (PPS reg: RPOR13_H)
    
    // --- Pin mappings for SPI module ---
    // SPI 1 Clock Out (SCK1) PPS code: 000110 (0x06)
    // SPI 1 Data Out (SDO1) PPS code: 000101 (0x05)
    // SPI 1 Slave Select PPS code: 000111
    
    // TODO -- should clear the ANSEL registers here to
    // It's probably not strictly necessary (already clear)
    
    // Unlock pin remappings
    __builtin_write_OSCCONL(OSCCON & ~(1<<6));
    
    // Configure the SPI pins
    RPOR0bits.RP64R = 0x06; // Put SCK1 on J10:43
    RPOR13bits.RP118R = 0x05; // Put SDO1 on J10:55
    
    // Lock pin remappings
    __builtin_write_OSCCONL(OSCCON | (1<<6));
    
     
            
    return 0;
}
