/*
 * File:   main.c
 *
 * Description: A minimal example of fixed precision 2x2 matrix multiplication 
 * for applying operations to a single qubit. The only operations included
 * are H, X and Z so that everything is real (this can be extended later).
 *
 * Compile command: make
 *
 * Notes: You need the microchip xc16 compilers which
 * are freely available from https://www.microchip.com/mplab/compilers 
 *
 */

#include "p33EP512MU810.h"
#include "xc.h"

// Locations of LEDs and buttons on Port D
#define red 0
#define amber 1
#define green 2

#define sw1 6
#define sw2 7
#define sw3 13

#define off 0
#define on 1

// Turn a particular LED on or off
int set_led(int color, int state) {
  if (state == on)
    LATD |= (1 << color);
  else
    LATD &= ~(1 << color);
  return 0;
}

// Read the state of a push button
int read_btn(int btn) {
  if ((btn != sw1) && (btn != sw2) && (btn != sw3)) {
    return -1;
  } else {
    // How well do you know C
    return (((PORTD & (1 << btn)) >> btn) ^ 0x0001);
  }
}

// Matrix type
typedef struct {
  signed _Fract a11;
  signed _Fract a12;
  signed _Fract a21;
  signed _Fract a22;
} Matrix;

// Vector type
typedef struct {
  signed _Fract a1;
  signed _Fract a2;
} Vector;

// 2x2 matrix multiplication
Vector mat_mul(Matrix M, Vector V) {
  Vector W = {0, 0}; // To store the output
  W.a1 = M.a11 * V.a1 + M.a12 * V.a2;
  W.a2 = M.a21 * V.a1 + M.a22 * V.a2;
  return W;
}

int main(void) {

  // Set up the input/output
  ANSELD = 0x0000; // Set port D to digital
  TRISD = 0x20C0; // Set lines 0,1,2 as output; 6,7,13 as input 

  // Define H
  Matrix H = {0.707, 0.707, 0.707, -0.707};
  Matrix X = {0.0, 0.9999, 0.9999, 0.0};
  Matrix Z = {0.9999, 0.0, 0.0, -1.0};
  
  // Define state vector
  Vector V = {0.0, 0.0};

  while (1 == 1) {
        
    // Wait for user to choose operation
    int btn1, btn2, btn3 = off;
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
    
    // Light LEDs to show output
    if ((V.a1 > 0.95) && (V.a2 < 0.05))
      set_led(green, on);
    else
      set_led(amber, on);

    // Wait for all the buttons to be released
    while ((btn1 == on) || (btn2 == on) || (btn3 == on))
      // Do nothing
      ;
    
    // Short delay to stop button bouncing
    int cnt = 0;
    while(cnt < 100000) cnt++;
    cnt = 0;
        

  }

  return 0;
}
