/*
 * File:   main.c
 *
 * Description: Contains an example of fixed precision 2x2 matrix multiplication 
 * for applying operations to a single qubit. The only operations included
 * are H, X and Z so that everything is real (this can be extended later).
 * 
 * All the functions have now been moved into separate files. io.h and io.c 
 * contain functions for reading and controlling the buttons and LEDs, and
 * quantum.h/quantum.c contain the matrix arithmetic for simulating one qubit.
 *
 * Compile command: make (on linux). But if you want to program the micro-
 * controller too or if you're using windows you're better of downloading
 * and installing MPLAB-X https://www.microchip.com/mplab/mplab-x-ide.
 *
 * Notes: You also need the microchip xc16 compilers which
 * are available from https://www.microchip.com/mplab/compilers 
 *
 */

#include "p33EP512MU810.h"
#include "xc.h"

#include "config.h"
#include "time.h"
#include "io.h"
#include "quantum.h"
#include "tests.h"
#include "time.h"
#include "spi.h"

int main(void) {

    // Change clock speed
    setup_clock();
    
    // Setup input/output (LEDs and buttons)
    setup_io();

    // Setup the timer
    setup_timer();
    
    // Setup SPI interface
    setup_spi();
    
    // Flash LEDs
    //flash_all(5);

    // Simulate one qubit
    //one_qubit();

    // Run matrix multiplication test
    //mat_mul_test();

    // Run complex matrix multiplication test
    //mat_mul_test_cmplx();
    
    // Simulate one complex qubit
    //one_qubit_cmplx();
    
    //start_strobe((1 << red) + (1 << green) + (1 << amber));
    
    //dim_leds();
    
    // Test multi LED strobing
    //multi_led_strobe();
    
    // Test the SPI
    //while(1==1) {
    //    send_byte(0xFF);
    //}

    int counter = 0;
    unsigned long int m = 0;
    while(1 == 1) {
        while(m < 1000000) m++;
        m = 0;
        set_external_led(counter);
        counter++;
        if(counter == 16) 
            counter = 0;
    }
    
    while(1==1);
    return 0;
}
