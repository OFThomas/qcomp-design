/*
 * File: quantum.c
 * 
 * Description: Contains matrix and vector arithmetic for simulating 
 * one qubit.
 * 
 */

#include "io.h"
#include "quantum.h"

// Create X, H and Z
void make_ops(Matrix2 X, Matrix2 Z, Matrix2 H) {
    X[0][0] = 0.0;
}

// 2x2 matrix multiplication
void mat_mul(Matrix2 M, Vector V) {
  V[0] = M[0][0] * V[0] + M[0][1] * V[1];
  V[1] = M[1][0] * V[0] + M[1][1] * V[1];
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
