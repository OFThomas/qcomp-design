/**
 * @file tests.c
 * 
 * @brief Description: Contains all the tests we have performed on the micro-
 * controller
 *
 */

#include "tests.h"
#include "io.h"
#include "quantum.h"
#include "time.h"
#include "algo.h"

void swap_test(Complex state[]){
    Complex X[2][2], Y[2][2], Z[2][2], H[2][2];
    make_ops(X, Y, Z, H);

    zero_state(state, NUM_QUBITS); // Set the state to the vacuum
    qubit_display(state, NUM_QUBITS); // Display the state for four qubits
    delay();
     
    gate(H, 0, state, NUM_QUBITS);
    gate(X, 1, state, NUM_QUBITS);

    /// swap for ever!
    while(1){
    swap(0,1, state, NUM_QUBITS);
    swap(1,2, state, NUM_QUBITS);
    swap(2,0, state, NUM_QUBITS);
    }
}

/**
// Testing the speed of 2^15 2x2 real matrix multiplications
void mat_mul_test() {
    
    // Define state vector
    // |0> = (1,0)
    // |1> = (0,1)
  Vector V;
  init_state(V, ZERO);
    
    Matrix2 X = {{0}}, Z = {{0}}, H = {{0}};
    make_ops(X, Z, H);
    
    // Start the timer
    start_timer();
    
    // Do a matrix multiplication test
    unsigned int n = 0;
    while (n < 32768) {
        mat_mul(X, V);
        n++;
    }

    // Read the timer
    //unsigned long int time = read_timer();
    
    // Show that the test is finished
    set_led(red, on);

    // wait (add a breakpoint here)
    while(1 == 1);
    
}
*/

// Dimmable LEDs
// ** NOTE: This won't work any more with the T4/T5 interrupt routine **
void dim_leds() {
    // Setup initial duty period
    unsigned int duty = 0x0100;
    
    // loop 
    while(1 == 1) {
        //
        
        // Read two buttons
        int btn1 = off, btn3 = off;
        btn1 = read_btn(sw1);
        btn3 = read_btn(sw3);
        // Alter duty cycle
        if (btn1 == on && duty < 0x1CF2) {
            duty++;
        }
        if (btn3 == on && duty > 0x10) {
            duty--;
        }
        
        // Reset TMR4, TMR5
        TMR4 = 0x0000;
        TMR5 = 0x0000;
        // Set the duty period
        PR4 = duty;
        // Turn on LEDs
        LATD |= 0x0007;
        // Turn timer 4 on
        T4CONbits.TON = 1;
        
        
        // Delay of total period
        unsigned long int cnt = 0; // 32 bit int
        while (cnt < 500) cnt++;
    }
    
}

