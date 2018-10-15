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
Matrix2 make_X() {
    Matrix2 X = {0.0, -1.0, -1.0, 0.0};
    return X;
}

Matrix2 make_H() {
    Matrix2 H = {0.7071067812, 0.7071067812, 0.7071067812, -0.7071067812};
    return H;
}

Matrix2 make_Z() {
    Matrix2 Z = {0.9999694824, 0.0, 0.0, -1.0};
    return Z;
}

// 2x2 matrix multiplication
Vector mat_mul(Matrix2 M, Vector V) {
  Vector W = {0, 0}; // To store the output
  W[0] = M[0]1 * V[0] + M[0]2 * V[1];
  W[1] = M[1]1 * V[0] + M[1]2 * V[1];
  return W;
}

// Add a global phase to make first amplitude positive
Vector fix_phase(Vector V) {
  Vector W = V; 
  signed _Fract phase = -1.0;
  if (V[0] < 0.0) {
    W[0] = V[0] * phase;
    W[1] = V[1] * phase;
  }
  return W;
}

// Clean the state: return the closest state out of |0>, |1>, |+> and |->
Vector clean_state(Vector V) {
    Vector W;
    if (V[0] > 0.99) {
      W[0] = 0.9999694824; // The |0> state
      W[1] = 0.0;
    }
    else if ((V[1] > 0.99) || (V[1] < -0.99)) {
      W[0] = 0.0; // The |1> state
      W[1] = 0.9999694824;
    }
    else if ((0.70 < V[0]) && (V[0] < 0.71)) {
      if (V[1] > 0.0){
        W[0] = 0.7071067812; // The |+> state
        W[1] = 0.7071067812;
      }
      else {
        W[0] = 0.7071067812; // The |-> state
        W[1] = -0.7071067812;
      }
    }
    return W;
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