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
    // J10:43 = J1:95 = uC:72 = RP64 (PPS reg: RPOR0_L; code: 0100 0000)
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
    
    // The clock pin also needs to be configured as an input
    RPINR20bits.SCK1R = 0x40; // Set SCK1 on J10:43 as input
    
    // Lock pin remappings
    __builtin_write_OSCCONL(OSCCON | (1<<6));
    
    // SPI clock configuration
    //
    // SCK1 = F_CY / (Primary Prescaler * Secondary Prescaler)
    // 
    // Assuming that F_CY = 50MHz, and the prescalers are 4 and 1,
    // the SPI clock frequency will be 12.5MHz.
    // 
    SPI1CON1bits.PPRE = 0x2; // Primary Prescaler = 4
    SPI1CON1bits.SPRE = 0x7; // Secondary Prescaler = 1
    
    // SPI1CON1 Register Settings
    SPI1CON1bits.DISSCK = 0; // Internal serial clock is enabled
    SPI1CON1bits.DISSDO = 0; // SDOx pin is controlled by the module
    SPI1CON1bits.MODE16 = 1; // Communication is byte-wide (8 bits)
    SPI1CON1bits.MSTEN = 1;  // Master mode enabled
    SPI1CON1bits.SMP = 0;    // Input data is sampled at the middle of 
                             // data output time
    SPI1CON1bits.CKE = 0;    // Serial output data changes on transition from
                             // Idle clock state to active clock state
    SPI1CON1bits.CKP = 0;    // Idle state for clock is a low level;
                             // active state is a high level
    
    // Enable SPI module
    SPI1STATbits.SPIEN = 1;  
    
    return 0;
}

// Send a byte to the SPI peripheral
int send_byte(int data) {
    // Check that the transmit buffer is empty
    if(SPI1STATbits.SPITBF == 0) {
        // Write data to the SPI buffer
        SPI1BUF = data;
        // Transmission starts automatically
        return 0;
    } else {
        // Unable to transmit
        return -1;
    }
}