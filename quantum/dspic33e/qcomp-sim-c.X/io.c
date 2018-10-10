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

// Flash LED a number of times
void flash_led(int color, int number) {
    unsigned int m = 0, n = 0; // You need a 16 bit int for this
    while(n < number) {
        set_led(color, on);
        m = 0;
        while(m < 50000) m++;
        set_led(color, off);
        m = 0;
        while(m < 50000) m++;
        n++;
    }
}

// Flash all the LEDs a number of times
void flash_all(int number) {
    unsigned int m = 0, n = 0; // You need a 16 bit int for this
    while(n < number) {
        set_led(red, on);
        set_led(amber, on);
        set_led(green, on);
        m = 0;
        while(m < 40000) m++;
        leds_off();
        m = 0;
        while(m < 40000) m++;
        n++;
    }
}