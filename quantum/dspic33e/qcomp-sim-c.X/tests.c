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
    unsigned long int time = read_timer();
    
    // Show that the test is finished
    set_led(red, on);

    // wait (add a breakpoint here)
    while(1 == 1);
    
}

// Testing the speed of 2^15 2x2 complex matrix multiplications
void mat_mul_test_cmplx() {
    
    // Define state vector
    // |0> = (1,0)
    // |1> = (0,1)
    CVector V;
    init_state_cmplx(V, ZERO);
    
    CMatrix2 X = {{{0}}}, Y = {{{0}}}, Z = {{{0}}}, H = {{{0}}};
    make_ops_cmplx(X, Y, Z, H);
    
    // Start the timer
    start_timer();
    
    // Do a complex matrix multiplication test
    unsigned int n = 0;
    while (n < 32768) {
        mat_mul_cmplx(X, V);
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
    Matrix2 X = {{0}}, Z = {{0}}, H = {{0}};
    make_ops(X, Z, H);

    // Define state vector
    // |0> = (1,0)
    // |1> = (0,1)
    Vector V;
    init_state(V, ZERO);
    
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
            mat_mul(H, V); // Multiply H by V, put result in V
        if (btn2 == on)
            mat_mul(X, V); // Multiply X by V, put result in V
        if (btn3 == on)
            mat_mul(Z, V); // Multiply Z by V, put result in V

        // Add a global phase to make first amplitude positive
        fix_phase(V);

        // Clean state
        clean_state(V);

        // Show qubit state
        show_state(V);

        // Wait for all the buttons to be released
        while ((btn1 == on) || (btn2 == on) || (btn3 == on)) {
            btn1 = read_btn(sw1);
            btn2 = read_btn(sw2);
            btn3 = read_btn(sw3);
        }

        // Short delay to stop button bouncing
        unsigned long int cnt = 0; // 32 bit int
        while (cnt < 100000) cnt++;

    }
}

// Simulating one qubit with complex amplitudes. Buttons apply H, X, Y and Z 
// and LEDs display the state of the qubit.
void one_qubit_cmplx() {
    
    // Define quantum operations
    CMatrix2 X = {{{0}}}, Y = {{{0}}}, Z = {{{0}}}, H = {{{0}}};
    make_ops_cmplx(X, Y, Z, H);

    // Define state vector
    // |0> = (1,0)
    // |1> = (0,1)
    CVector V;
    init_state_cmplx(V, ZERO);
    
    // Show qubit state
    show_state_cmplx(V);

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
            mat_mul_cmplx(H, V); // Multiply H by V, put result in V
        if (btn2 == on)
            mat_mul_cmplx(X, V); // Multiply X by V, put result in V
        if (btn3 == on)
            mat_mul_cmplx(Z, V); // Multiply Z by V, put result in V

        // Add a global phase to make first amplitude positive
        fix_phase_cmplx(V);

        // Clean state
        clean_state_cmplx(V);

        // Show qubit state
        show_state_cmplx(V);

        // Wait for all the buttons to be released
        while ((btn1 == on) || (btn2 == on) || (btn3 == on)) {
            btn1 = read_btn(sw1);
            btn2 = read_btn(sw2);
            btn3 = read_btn(sw3);
        }

        // Short delay to stop button bouncing
        unsigned long int cnt = 0; // 32 bit int
        while (cnt < 100000) cnt++;

    }
}

// Dimmable LEDs 
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