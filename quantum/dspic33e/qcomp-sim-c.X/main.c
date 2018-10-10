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

int main(void) {
    
  // Setup input/output (LEDs and buttons)
  setup_io();

  // Define quantum operations
  Matrix H = {0.7071067812, 0.7071067812, 0.7071067812, -0.7071067812};
  Matrix X = {0.0, 0.9999, 0.9999, 0.0};
  Matrix Z = {0.9999, 0.0, 0.0, -1.0};
  
  // Define state vector
  Vector V = {0.9999, 0.0};

  while (1 == 1) {
        
    // Wait for user to choose an operation
    int btn1 = off, btn2 = off, btn3 = off;
    while((btn1 == off) && (btn2 == off) && (btn3 == off)) { 
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
    fix_phase(&V);
    
    // Clean state
    if (V.a1 > 0.99) {
      V.a1 = 0.9999; // The |0> state
      V.a2 = 0.0;
    }
    else if ((V.a2 > 0.99) || (V.a2 < -0.99)) {
      V.a1 = 0.0; // The |1> state
      V.a2 = 0.9999;
    }
    else if ((0.70 < V.a1) && (V.a1 < 0.71)) {
      if (V.a2 > 0.0){
        V.a1 = 0.7071067812; // The |+> state
        V.a2 = 0.7071067812;
      }
      else {
        V.a1 = 0.7071067812; // The |-> state
        V.a2 = -0.7071067812;
      }
    }
    else
      set_led(red, on);
    
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
    }
    else
      set_led(red, on);
    
    // Wait for all the buttons to be released
    while ((btn1 == on) || (btn2 == on) || (btn3 == on)) {
      btn1 = read_btn(sw1);
      btn2 = read_btn(sw2);
      btn3 = read_btn(sw3);
    }
    
    // Short delay to stop button bouncing
    int cnt = 0;
    while(cnt < 10000) cnt++;
    
  }

  return 0;
}
