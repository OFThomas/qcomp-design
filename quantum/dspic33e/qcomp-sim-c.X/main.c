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
VACUUM:zero_state(state);
    display_average(state);
    
    /// Test single qubit gates
    /// @todo fix this menu system
    /// @todo add a button for switching between display average and cycle modes
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
    int select_qubit = -1; ///< @param qubit integer to act on
    int targ =0;            ///< @param target qubit integer for 2-qubit gates
    int select_op = -1;     ///< @param integer used in switch to pick which gate to do
    while (1) {

        // while(qubit == -1) -> read 
        // while(op == -1) -> read
        
        /// after reading buttons see if any qubit is selected
        /// write the qubit number to "select_qubit"
        select_qubit = check_qubit();

        /// Wait for a qubit operation to be selected
        select_op = check_op();
        /// if the '0' button is ever pressed reset to the vacuum state.
        /// \note Nothing wrong here...
        if(select_op == 0) goto VACUUM;

        /// End of operation select
        /// Perform the qubit gates
        switch(select_op) {
            case 0:
                // X
                gate_display(X, select_qubit, state);
                break;
            case 1:
                // Z
                gate_display(Z, select_qubit, state);
                break;
            case 2:
                // H
                gate_display(H, select_qubit, state);
                break;
            case 3:
                // CNOT
                /// wait for target qubit to be selected
                targ = check_qubit();
                two_gate_display(X, select_qubit, targ, state);
                break;
            default:
                break; ///Do nothing   
        } /// End of switch
    }

    while (1); ///< @note Really important!
    return 0;
}
