/*
 * File: quantum.c
 * 
 * Description: Contains matrix and vector arithmetic for simulating 
 * one qubit.
 * 
 */

#include "io.h"
#include "quantum.h"

// Complex addition
Complex cadd(Complex a, Complex b) {
    Complex result;
    result.real = a.real + b.real;
    result.imag = a.imag + b.imag;
    return result;
}

// Complex multiplication
Complex cmul(Complex a, Complex b) {
    Complex result;
    result.real = a.real * b.real - a.imag * b.imag;
    result.imag = a.real * b.imag + a.imag * b.real; 
    return result;
}

// Create X, H and Z
void make_ops(Matrix2 X, Matrix2 Z, Matrix2 H) {
    // Assume the elements are all equal to zero
    X[0][1] = 0.9999694824; // X
    X[1][0] = 0.9999694824;
    Z[0][0] = 0.9999694824; // Z
    Z[1][1] = -1.0;
    H[0][0] = 0.7071067812; // H
    H[0][1] = 0.7071067812;
    H[1][0] = 0.7071067812;
    H[1][1] = -0.7071067812;
}

// Initialise state vector
void init_state(Vector V, State s) {
    switch(s) {
        case ZERO:
            V[0] = 0.9999694824;
            V[1] = 0.0;
            break;
        case ONE:
            V[0] = 0.0;
            V[1] = 0.9999694824;
            break;
        case PLUS:
            V[0] = 0.7071067812;
            V[1] = 0.7071067812;
            break;
        case MINUS:
            V[0] = 0.7071067812;
            V[1] = -0.7071067812;
            break;
    }
}

// 2x2 matrix multiplication
void mat_mul(Matrix2 M, Vector V) {
  signed _Fract a = M[0][0] * V[0] + M[0][1] * V[1];
  signed _Fract b = M[1][0] * V[0] + M[1][1] * V[1];
  V[0] = a;
  V[1] = b;
}

// 2x2 complex matrix multiplication
void cmat_mul(CMatrix2 M, CVector V) {
  V[0] = cadd(cmul(M[0][0],V[0]), cmul(M[0][1],V[1]));
  V[1] = cadd(cmul(M[1][0],V[0]), cmul(M[1][1], V[1]));
}

// Add a global phase to make first amplitude positive
void fix_phase(Vector V) {
  signed _Fract phase = -1.0;
  if (V[0] < 0.0) {
    V[0] *= phase;
    V[1] *= phase;
  }
}

// Clean the state: return the closest state out of |0>, |1>, |+> and |->
void clean_state(Vector V) {
    if (V[0] > 0.99) {
      V[0] = 0.9999694824; // The |0> state
      V[1] = 0.0;
    }
    else if ((V[1] > 0.99) || (V[1] < -0.99)) {
      V[0] = 0.0; // The |1> state
      V[1] = 0.9999694824;
    }
    else if ((0.70 < V[0]) && (V[0] < 0.71)) {
      if (V[1] > 0.0){
        V[0] = 0.7071067812; // The |+> state
        V[1] = 0.7071067812;
      }
      else {
        V[0] = 0.7071067812; // The |-> state
        V[1] = -0.7071067812;
      }
    }
}

// Show the qubit state on the LEDs
void show_state(Vector V) {
    // Turn all the LEDs off
    leds_off();
    // Show current qubit state on LEDs
    if (V[0] > 0.99)
        set_led(red, on); // The |0> state
    else if ((V[1] > 0.99) || (V[1] < -0.99))
        set_led(green, on); // The |1> state
    else if ((0.70 < V[0]) && (V[0] < 0.71)) {
        set_led(red, on); // The |+> or |-> state
        set_led(green, on);
        // Now add the sign bit
        if (V[1] < 0.0)
            set_led(amber, on);
    } else
        set_led(red, on);
}
