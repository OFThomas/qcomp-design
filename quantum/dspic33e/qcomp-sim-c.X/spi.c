/**
 * @file spi.c
 * 
 * @brief Description: Functions for communicating with serial devices
  * @authors J Scott, O Thomas
 * @date Nov 2018
*
 */

#include "spi.h"

// Set up serial peripheral interface
int setup_spi(void) {
    
    /// @brief Pin mappings
    /// @detail --- Pin mappings and codes --- \n
    /// J10:41 = J1:91 = uC:70 = RPI74 (PPS code: 0100 1010) \n
    /// J10:44 = J1:93 = uC:9 = RPI52 (PPS code: 0011 0100) \n
    /// J10:47 = J1:101 = uC:34 = RPI42 (PPS code: 0010 1010) \n
    /// J10:43 = J1:95 = uC:72 = RP64 (PPS reg: RPOR0_L; code: 0100 0000) \n
    /// J10:46 = J1:97 = uC:69 = RPI73 (PPS code: 0100 1001) \n
    /// J10:7 = J1:13 = uC:3 = RP85 (PPS reg: RPOR6_L; code: 0101 0101) \n
    /// J10:5 = J1:7 = uC:5 = RP87 (PPS reg: RPOR6_H) \n
    /// J10:55 = J1:117 = uC:10 = RP118 (PPS reg: RPOR13_H) \n

    /// @brief --- Pin mappings for SPI 1 module --- \n 
    /// SPI 1 Clock Out (SCK1) PPS code: 000110 (0x06) \n
    /// SPI 1 Data Out (SDO1) PPS code: 000101 (0x05) \n
    /// SPI 1 Slave Select PPS code: 000111 \n
    
    /// @brief --- Pin mappings for SPI 3 module --- \n
    /// SPI 3 Clock Out (SCK3) PPS code: 100000 (0x20) \n
    /// SPI 3 Data Out (SDO3) PPS code: 011111 (0x1F) \n
    /// SPI 3 Slave Select PPS code: 100001 \n
    
    // @todo TODO -- should clear the ANSEL registers here to
    // It's probably not strictly necessary (already clear)
    
    // Unlock pin remappings
    __builtin_write_OSCCONL(OSCCON & ~(1<<6));
    
    /// Configure the SPI 1 pins
    RPOR0bits.RP64R = 0x06; ///< Put SCK1 on J10:43
    RPOR13bits.RP118R = 0x05; ///< Put SDO1 on J10:55
    /// The clock pin also needs to be configured as an input
    RPINR20bits.SCK1R = 0x40; ///< Set SCK1 on J10:43 as input
    
    /// Configure the SPI 3 output pins
    RPOR6bits.RP85R = 0x20; ///< Put SCK3 on J10:7
    RPOR6bits.RP87R = 0x1F; ///< Put SDO3 on J10:5
    // Configure the SPI 3 input pins
    RPINR29bits.SDI3R = 0x34; ///< Put SDI3 on J10:44
    // The clock pin also needs to be configured as an input
    RPINR29bits.SCK3R = 0x55; ///< Set SCK3 on J10:7 as input
    
    // Lock pin remappings
    __builtin_write_OSCCONL(OSCCON | (1<<6));
   
    /** @note
    // SPI 1 clock configuration
    //
    // SCK1 = F_CY / (Primary Prescaler * Secondary Prescaler)
    // 
    // Assuming that F_CY = 50MHz, and the prescalers are 4 and 1,
    // the SPI clock frequency will be 12.5MHz.
    // 
    */
    SPI1CON1bits.PPRE = 0x2; // Primary Prescaler = 4
    SPI1CON1bits.SPRE = 0x7; // Secondary Prescaler = 1
    
    // SPI1CON1 Register Settings
    SPI1CON1bits.DISSCK = 0; // Internal serial clock is enabled
    SPI1CON1bits.DISSDO = 0; // SDOx pin is controlled by the module
    SPI1CON1bits.MODE16 = 0; // Communication is byte-wide (8 bits)
    SPI1CON1bits.MSTEN = 1;  // Master mode enabled
    SPI1CON1bits.SMP = 0;    // Input data is sampled at the middle of 
                             // data output time
    SPI1CON1bits.CKE = 0;    // Serial output data changes on transition from
                             // Idle clock state to active clock state
    SPI1CON1bits.CKP = 0;    // Idle state for clock is a low level;
                             // active state is a high level
    
    // Enable SPI 1 module
    SPI1STATbits.SPIEN = 1;  
    
    // SPI 3 clock configuration
    //
    // SCK3 = F_CY / (Primary Prescaler * Secondary Prescaler)
    // 
    // Assuming that F_CY = 50MHz, and the prescalers are 4 and 1,
    // the SPI clock frequency will be 12.5MHz.
    // 
    SPI3CON1bits.PPRE = 0x1; // Primary Prescaler = 16
    SPI3CON1bits.SPRE = 0x7; // Secondary Prescaler = 1
    
    // SPI3CON1 Register Settings
    SPI3CON1bits.DISSCK = 0; // Internal serial clock is enabled
    SPI3CON1bits.DISSDO = 0; // SDOx pin is controlled by the module
    SPI3CON1bits.MODE16 = 0; // Communication is byte-wide (8 bits)
    SPI3CON1bits.MSTEN = 1;  // Master mode enabled
    SPI3CON1bits.SMP = 0;    // Input data is sampled at the middle of 
                             // data output time
    SPI3CON1bits.CKE = 1;    // Serial output data changes on transition from
                             // Idle clock state to active clock state
    SPI3CON1bits.CKP = 0;    // Idle state for clock is a low level;
                             // active state is a high level
    
    // Enable SPI 3module
    SPI3STATbits.SPIEN = 1;  
    
    return 0;
}

// Send a byte to the SPI 1 peripheral (For display driver)
//
// This function can be improved -- it currently blocks
// execution until the transfer is finished. The correct
// way to do it is to configure the SPI interrupts. 
int send_byte_spi_1(int data) {
    // Check that the transmit buffer is empty
    if(SPI1STATbits.SPITBF == 0) {
        // Write data to the SPI buffer
        SPI1BUF = data;
        // Transmission starts automatically
        // Wait for the operation to finish
        while(SPI1STATbits.SPIRBF != 1)
            ; // Do nothing
        // Read the receive buffer (just to clear the SPIRBF flag)
        int rx = SPI1BUF;
        return rx; // Should be zero -- removes unused variable warning
    } else {
        // Unable to transmit
        return -1;
    }
}

// Send a byte to the SPI 3 peripheral (For button shift register)
//
// This function can be improved -- it currently blocks
// execution until the transfer is finished. The correct
// way to do it is to configure the SPI interrupts. 
int read_byte_spi_3() {
    // Check that the transmit buffer is empty
    if(SPI3STATbits.SPITBF == 0) {
        // Write dummy data to the SPI buffer 
        SPI3BUF = 0;
        // Trans-
        // mission starts automatically
        // Wait for the operation to finish
        while(SPI3STATbits.SPIRBF != 1)
            ; // Do nothing
	if(SPI3STATbits.SPIROV == 1) {
	  return -2; // Overflow
	}
        // Read the receive buffer
        int data = 0;
        data = SPI3BUF;
        return data;
    } else {
        // Unable to start operation
        return -1;
    }
}
