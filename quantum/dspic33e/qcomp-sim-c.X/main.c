/**
 * @file main.c
 * @author J R Scott
 * @date 8/11/18
 *
 * @brief The main function
 *
 * @detail Description: Contains an example of fixed precision 2x2 matrix multiplication 
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
 * @note You also need the microchip xc16 compilers which
 * are available from https://www.microchip.com/mplab/compilers 
 *
 */

#include "p33EP512MU810.h"
#include "xc.h"

#include "config.h"
#include "time.h"
#include "io.h"
#include "quantum.h"
#include "tests.h"
#include "time.h"
#include "spi.h"

int main(void) {

    // Change clock speed
    setup_clock();
    
    // Setup input/output (LEDs and buttons)
    setup_io();

    // Setup the timer
    setup_timer();
    
    // Setup SPI interface
    setup_spi();
    
    // Setup the external LEDs
    setup_external_leds();

    /**
     * @brief Reading button state
     * 
     * The button states are written into an array of type BUTTON_ARRAY
     * whose 
     */
    extern int buttons[16]; /// Global variable for button state
    read_external_buttons(); /// Update the buttons variable
    if(buttons[0] == 1)
        ; /// Do something if button 0 has been pressed...
    
    
    /** @brief Example use of RGB LEDs -- won't do anything yet
     * 
     * Just pass the values of R, G and B to the function along with the led
     * index (which can just be an integer, like qubit number). The exact 
     * mapping of indices to LED lines in the display driver will be in the 
     * io.h file. 
     */ 
    _Fract R = 1; // Colours
    _Fract G = 0;
    _Fract B = 0;
    int led_index = 1; // LED number
    // Set RGB LED number 2
    //set_external_led(led_index, R, G, B);

    CVector state_v;
    
    init_state_cmplx(state_v, ZERO);
    // led 0, r=0, b=0, g=0
   set_external_led(0, 0, 0, 0);
    
    extern LED led[LED_NUM];
    
        
    while(1==1);

    
    return 0;
}
