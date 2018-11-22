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
    display_average(state);
    
    /// Test single qubit gates
    
    /** In this test the qubit buttons (0 - 3) will be used to select a qubit 
     * and the function buttons (4 - 6) will be used to perform an operation
     * on the selected qubit (X, Z or H).
     * 
     * The loop is made of two parts. The first waits for a qubit to be
     * selected and the second chooses a single qubit operation for that
     * qubit. Once the gate has been pressed the operation is immediately
     * executed and the loop repeats.
     * 
     */
    int select_qubit = -1;
    int select_op = -1;
    while (1) {
        while (1) { /// Wait for qubit to be selected
            // Read all the button state
            read_external_buttons();

            // Check whether a qubit has been selected
            for (int n = 0; n < NUM_QUBITS; n++) {
                if (read_qubit_btn(n) == 1) {
                    select_qubit = n;
                }
            }
            
            /// Check whether a qubit button was pressed
            if(select_qubit != -1) {
                break; /// Move onto the next part of the loop
            }
        } /// End of qubit select 

        while (1) { /// Wait for a qubit operation to be selected
            // Read all the button state
            read_external_buttons();

            // Check whether a qubit has been selected
            for (int n = 0; n < 4; n++) {
                if (read_func_btn(n) == 1) {
                    select_op = n;
                }
            }

            /// Check whether a qubit button was pressed
            if (select_op != -1) {
                break; /// Move onto the operation
            }

        } /// End of operation select
        
        /// Perform the single qubit gate
        switch(select_op) {
            case 0: 
                single_qubit_op(X, select_qubit, state);
                display_average(state);
                break;
            case 1:
                single_qubit_op(Z, select_qubit, state);
                display_average(state);
                break;
            case 2:
                single_qubit_op(H, select_qubit, state);
                display_average(state);
                break;
            default:
                break; ///Do nothing   
                
        } /// End of switch
        
        /// Reset the qubit and operation select variables
        select_qubit = -1;
        select_op = -1;
        
    }

    while (1); ///< @note Really important!
    return 0;
}
