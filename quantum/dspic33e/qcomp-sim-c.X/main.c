/**
 * @file main.c
 * @author J R Scott
 * @date 8/11/18
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

    ///
    /// @brief Reading button state
    /// 
    /// The button states are written into an array of type BUTTON_ARRAY
    /// whose 
    ///
    extern int buttons[16]; /// Global variable for button state
    read_external_buttons(); /// Update the buttons variable
    if(buttons[0] == 1)
        ; /// Do something if button 0 has been pressed...

    //Complex X[2][2], Y[2][2], Z[2][2], H[2][2];
    //make_ops(X, Y, Z, H);
   
    Complex state[STATE_LENGTH]; // Make a 3 qubit state vector of length 8 
   
    
    /// Start of the PROGRAM!
     while(1){
    
         
         T6CONbits.TON = 0;
        // nice to see
        swap_test(state);
        
        
         zero_state(state); // Set the state to the vacuum
         //single_qubit_op(H, 0, state, NUM_QUBITS);
         
         //controlled_qubit_op(X, 0, 1, state, NUM_QUBITS);

         //qubit_display(state, NUM_QUBITS);
         
         gate(H,0,state);
         two_gate(X, 0, 1, state);
         
         delay();
         
         // start the clock
         T6CONbits.TON = 1;
         display_cycle(state);
         
         /*
         reset_cycle();
         RGB colors[NUM_QUBITS];
         
         for(int i=0; i<NUM_QUBITS; i++){
         colors[i].R = 0.0;
         colors[i].G = 0.0;
         colors[i].B = 0.0;
         }
         
         colors[0].R = 0.5;
         add_to_cycle(colors,10);
         colors[0].R = 0.0;
         colors[0].B = 0.5;
         add_to_cycle(colors,10);
        */
           
        while(1);
    

     }
    while(1); ///< @note Really important!
    return 0;
}
