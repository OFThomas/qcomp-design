/*
 * File: tests.c
 * 
 * Description: Contains all the tests we have performed on the micro-
 * controller
 *
 */

#include "tests.h"
#include "io.h"
#include "quantum.h"
#include "time.h"

// Testing the speed of 2^15 2x2 matrix multiplications
void mat_mul_test() {
    
    // Start the timer
    start_timer();
    
    // Define state vector
    // |0> = (1,0)
    // |1> = (0,1)
    Vector V = {0.9999694824, 0.0};
    
    Matrix2 X = make_X();
    
    // Do a matrix multiplication test
    unsigned int n = 0;
    while (n < 32768) {
        V = mat_mul(X, V);
        n++;
    }

    // Read the timer
    unsigned long int time = read_timer();
    
    // Show that the test is finished
    set_led(red, on);

    // wait (add a breakpoint here)
    while(1 == 1);
    
}

// Simulating one qubit. Buttons apply H, X and Z and LEDs display the
// state of the qubit.
void one_qubit() {
    // Define quantum operations
    Matrix2 H = make_H(), X = make_X(), Z = make_Z();

    // Define state vector
    // |0> = (1,0)
    // |1> = (0,1)
    Vector V = {0.9999694824, 0.0};
    
    // Show qubit state
    show_state(V);

    while (1 == 1) {

        // Wait for user to choose an operation
        int btn1 = off, btn2 = off, btn3 = off;
        while ((btn1 == off) && (btn2 == off) && (btn3 == off)) {
            btn1 = read_btn(sw1);
            btn2 = read_btn(sw2);
            btn3 = read_btn(sw3);
        }

        // Apply operation
        if (btn1 == on)
            V = mat_mul(H, V); // Multiply H by V, put result in V
        if (btn2 == on)
            V = mat_mul(X, V); // Multiply X by V, put result in V
        if (btn3 == on)
            V = mat_mul(Z, V); // Multiply Z by V, put result in V

        // Add a global phase to make first amplitude positive
        V = fix_phase(V);

        // Clean state
        V = clean_state(V);

        // Show qubit state
        show_state(V);

        // Wait for all the buttons to be released
        while ((btn1 == on) || (btn2 == on) || (btn3 == on)) {
            btn1 = read_btn(sw1);
            btn2 = read_btn(sw2);
            btn3 = read_btn(sw3);
        }

        // Short delay to stop button bouncing
        int cnt = 0;
        while (cnt < 10000) cnt++;

    }
}