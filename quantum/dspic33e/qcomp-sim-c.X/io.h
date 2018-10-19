/*
 * File: io.h
 * 
 * Description: Header file for input output functions. Include it at the
 * top of any C source file which uses buttons and LEDs. It also defines
 * various constants representing the positions of the buttons and LEDs 
 * on port D.
 *
 */

#ifndef IO_H
#define	IO_H

#ifdef	__cplusplus
extern "C" {
#endif

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
    
    // Global LED strobing state parameter
    typedef struct {
        int strobe_leds; // Bit set the LEDs which are strobing 
        int strobe_state; // Bit zero is the current state (on/off)
    } LED_GLOBAL;

    // Set up LEDs and buttons on port D 
    int setup_io(void);
    
    // Turn a particular LED on or off
    int set_led(int color, int state);

    // Read the state of a push button
    int read_btn(int btn);
    
    // Turn all the LEDs off
    void leds_off(void);
    
    // Flash one LED a number of times
    void flash_led(int color, int number);
    
    // Flash all the LEDs a number of times
    void flash_all(int number);
    
    // Set LEDs flashing indefinitely
    void start_strobe();
    
    // Set an LED strobing
    void set_strobe(int color, int state);
    
    // Toggle LED strobe
    void toggle_strobe(int color);

#ifdef	__cplusplus
}
#endif

#endif	/* IO_H */

