/*
 * File: quantum.c
 * 
 * Description: Contains matrix and vector arithmetic for simulating 
 * one qubit.
 * 
 */

#include "io.h"
#include "quantum.h"

// 2x2 matrix multiplication
Vector mat_mul(Matrix M, Vector V) {
  Vector W = {0, 0}; // To store the output
  W.a1 = M.a11 * V.a1 + M.a12 * V.a2;
  W.a2 = M.a21 * V.a1 + M.a22 * V.a2;
  return W;
}

// Add a global phase to make first amplitude positive
Vector fix_phase(Vector V) {
  Vector W = V; 
  signed _Fract phase = -1.0;
  if (V.a1 < 0.0) {
    W.a1 = V.a1 * phase;
    W.a2 = V.a2 * phase;
  }
  return W;
}

// Clean the state: return the closest state out of |0>, |1>, |+> and |->
Vector clean_state(Vector V) {
    Vector W;
    if (V.a1 > 0.99) {
      W.a1 = 0.9999694824; // The |0> state
      W.a2 = 0.0;
    }
    else if ((V.a2 > 0.99) || (V.a2 < -0.99)) {
      W.a1 = 0.0; // The |1> state
      W.a2 = 0.9999694824;
    }
    else if ((0.70 < V.a1) && (V.a1 < 0.71)) {
      if (V.a2 > 0.0){
        W.a1 = 0.7071067812; // The |+> state
        W.a2 = 0.7071067812;
      }
      else {
        W.a1 = 0.7071067812; // The |-> state
        W.a2 = -0.7071067812;
      }
    }
    return W;
}

// Show the qubit state on the LEDs
void show_state(Vector V) {
    // Turn all the LEDs off
    leds_off();
    // Show current qubit state on LEDs
    if (V.a1 > 0.99)
        set_led(red, on); // The |0> state
    else if ((V.a2 > 0.99) || (V.a2 < -0.99))
        set_led(green, on); // The |1> state
    else if ((0.70 < V.a1) && (V.a1 < 0.71)) {
        set_led(red, on); // The |+> or |-> state
        set_led(green, on);
        // Now add the sign bit
        if (V.a2 < 0.0)
            set_led(amber, on);
    } else
        set_led(red, on);
}