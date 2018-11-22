/**
 * @file main.c
 * @authors J Scott, O Thomas
 * @date Nov 2018
 *
 * @brief The main function
 *
 * @detail Contains an example of fixed precision 2x2 matrix multiplication 
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
 * @note You also need the microchip xc16 compilers which
 * are available from https://www.microchip.com/mplab/compilers 
 *
 */

#include "config.h"
#include "time.h"
#include "algo.h"
#include "display.h"

int main(void) {

    // Change clock speed
    setup_clock();
    
    // Setup input/output (LEDs and buttons)
    setup_io();

    // Setup the timer
    setup_timer();
    
    // Setup SPI interface
    setup_spi();
    
    // Setup the external LEDs
    setup_external_leds();
    
    // Setup the external buttons
    setup_external_buttons();

    Complex state[STATE_LENGTH]; // Make a 3 qubit state vector of length 


    // set to vacuum
    zero_state(state);
    /// button reading test. 
    while (1) {
        // loop over all 4 qubits 
        read_external_buttons();
        // Read the qubit buttons
        for (int n = 0; n < NUM_QUBITS; n++) {
            if (read_qubit_btn(n) == 1) set_external_led(n, 0.0, 0.0, 0.9);
            else set_external_led(n, 0.0, 0.0, 0.0);
        }
        for (int n = 0; n < NUM_BTNS - NUM_QUBITS - 1; n++) {
            if (read_func_btn(n) == 1) set_external_led(n, 0.0, 0.9, 0);
            /// Workaround below to prevent turning 4th LED off 
            else if(n != 4) set_external_led(n, 0.0, 0.0, 0.0);
        }
    }

    //swap_test(state);

    while (1); ///< @note Really important!
    return 0;
}
