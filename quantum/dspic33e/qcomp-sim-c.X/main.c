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

#include "io.h"
#include "quantum.h"
#include "tests.h"

int main(void) {

    // Setup input/output (LEDs and buttons)
    setup_io();

    // Flash LEDs
    flash_all(5);

    // Simulate one qubit
    //one_qubit();

    // Run matrix multiplication test
    mat_mul_test();
    
    return 0;
}
