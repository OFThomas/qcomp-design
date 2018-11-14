/**
 * @file quantum.c
 * 
 * @brief Description: Contains matrix and vector arithmetic for simulating 
 * one qubit.
 * 
 */

#include "io.h"                             
#include "quantum.h"

// Complex addition
void cadd(Complex a, Complex b, Complex result) {
    result[0] = a[0] + b[0];
    result[1] = a[1] + b[1];
}

// Complex multiplication
void cmul(Complex a, Complex b, Complex result) {
    result[0] = a[0] * b[0] - a[1] * b[1];
    result[1] = a[0] * b[1] + a[1] * b[0]; 
}

// Create complex X, Y, Z and H
void make_ops_cmplx(CMatrix2 X, CMatrix2 Y, CMatrix2 Z, CMatrix2 H) {
    // Assume the elements are all equal to zero
    X[0][1][0] = 0.9999694824; // X
    X[1][0][0] = 0.9999694824;
    
    Y[0][1][1] = -1.0; // Y
    Y[1][0][1] = 0.9999694824;
    
    Z[0][0][0] = 0.9999694824; // Z
    Z[1][1][0] = -1.0;

    H[0][0][0] = 0.7071067812; // H
    H[0][1][0] = 0.7071067812;
    H[1][0][0] = 0.7071067812;
    H[1][1][0] = -0.7071067812;
}

// Initialise complex state vector
int init_state_cmplx(CVector V, State s) {
    switch(s) {
        case ZERO:
            V[0][0] = 0.9999694824;
            V[0][1] = 0.0;
            V[1][0] = 0.0;
            V[1][1] = 0.0;
            break;
        case ONE:
            V[0][0] = 0.0;
            V[0][1] = 0.0;
            V[1][0] = 0.9999694824;
            V[1][1] = 0.0;
            break;
        case PLUS:
            V[0][0] = 0.7071067812;
            V[0][1] = 0.0;
            V[1][0] = 0.7071067812;
            V[1][1] = 0.0;
            break;
        case MINUS:
            V[0][0] = 0.7071067812;
            V[0][1] = 0.0;
            V[1][0] = -0.7071067812;
            V[1][1] = 0.0;
            break;
        case iPLUS:
            V[0][0] = 0.7071067812;
            V[0][1] = 0.0;
            V[1][0] = 0.0;
            V[1][1] = 0.7071067812;
            break;
        case iMINUS:
            V[0][0] = 0.7071067812;
            V[0][1] = 0.0;
            V[1][0] = 0.0;
            V[1][1] = -0.7071067812;
            break;
    }
    return 0;
}

// 2x2 complex matrix multiplication
void mat_mul_cmplx(CMatrix2 M, CVector V, int i, int j) {
    Complex a, b, c, d;
    cmul(M[0][0],V[i],a); 
    cmul(M[0][1],V[j],b);
    cadd(a,b,c);
    cmul(M[1][0],V[i],a);
    cmul(M[1][1],V[j],b);
    cadd(a,b,d);
    V[i][0] = c[0];
    V[i][1] = c[1];
    V[j][0] = d[0];
    V[j][1] = d[1];
}

// Add a global phase to make first complex amplitude positive
// This only works for certain states (zero, one, plus, minus, etc.)
void fix_phase_cmplx(CVector V) {
    CMatrix2 phase_90 = {{{0}}};
    phase_90[0][0][1] = 0.9999694824;
    phase_90[1][1][1] = 0.9999694824;   
    CMatrix2 phase_270 = {{{0}}};
    phase_270[0][0][1] = -1.0;
    phase_270[1][1][1] = -1.0;    
    CMatrix2 phase_180 = {{{0}}};
    phase_180[0][0][0] = -1.0;
    phase_180[1][1][0] = -1.0;
    if (V[0][0] < -0.1) {
        mat_mul_cmplx(phase_180, V, 0, 1);
    } else if (V[0][1] < -0.1) {
        mat_mul_cmplx(phase_90, V, 0, 1);
    } else if (V[0][1] > 0.1) {
        mat_mul_cmplx(phase_270, V, 0, 1);
    }
}

// Clean the state: return the closest state out of 
// |0>, |1>, |+> , |->, |D> and |A>
void clean_state_cmplx(CVector V) {
    if (V[0][0] > 0.99) {
        init_state_cmplx(V, ZERO);
        // add abs?
    } else if ((V[1][0] > 0.99) || (V[1][0] < -0.99)) {
        init_state_cmplx(V, ONE);
    } else if ((V[1][1] > 0.99) || (V[1][1] < -0.99)) {
        init_state_cmplx(V, ONE);
    } else if ((0.70 < V[0][0]) && (V[0][0] < 0.71)) {
        if (V[1][0] > 0.1) {
            init_state_cmplx(V, PLUS);
        } else if (V[1][0] < -0.1) {
            init_state_cmplx(V, MINUS);
        } else if (V[0][1] > 0.1) {
            init_state_cmplx(V, iPLUS);
        } else {
            init_state_cmplx(V, iMINUS);
        }
    }
}

// Show the qubit state on the LEDs
void show_state_cmplx(CVector V) {
    // Turn all the LEDs off
    leds_off();
    // Show current state of qubit on the LEDs
    if (V[0][0] > 0.99) {
        set_led(red, on); // The |0> state
    } else if ((V[1][0] > 0.99) || (V[1][0] < -0.99)) {
        set_led(green, on); // The |1> state
    } else if ((V[1][1] > 0.99) || (V[1][1] < -0.99)) {
        set_led(green, on); // The |1> state
    } else if ((0.70 < V[0][0]) && (V[0][0] < 0.71)) {
        set_led(red, on); // A superposition state
        set_led(green, on);
        if (V[1][0] > 0.1) {
            // Plus
            set_led(amber, off);
        } else if (V[1][0] < -0.1) {
            // Minus
            set_led(amber, on);
        } else if (V[0][1] > 0.1) {
            // iPlus
           flash_led(amber,10);
        } else {
           flash_led(amber,5);        
      }
    }
}

// squaring function
Q15 sq(Q15 num){
    return num*num;
}

// power function
/// computes a^b
int pow(int a, int b){
    int val = 1;
    for(int i=0; i<=b; i++){
    val = val * a;
    }
    return val;
}



