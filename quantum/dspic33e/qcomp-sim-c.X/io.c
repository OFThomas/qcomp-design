/*
 * File: io.c
 * 
 * Description: Contains all the functions for reading buttons and writing to
 * LEDs
 *
 */

#include "io.h"

// Set up LEDs and buttons on port D 
int setup_io(void) {
  // Set up the input/output
  ANSELD = 0x0000; // Set port D to digital
  TRISD = 0x20C0; // Set lines 0,1,2 as output; 6,7,13 as input 
  return 0;
}
  
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

// Turn all the LEDs off
void leds_off(void) {
  set_led(green, off);
  set_led(amber, off);
  set_led(red, off);
}