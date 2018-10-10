/*
 * File:   main.c
 *
 */

#include "p33EP512MU810.h"
#include "xc.h"

#define red 0
#define amber 1
#define green 2

#define off 0
#define on 1

#define sw1 6
#define sw2 7
#define sw3 13

int set_led(int color, int state) {
    if (state == on) 
        LATD |= (1 << color);
    else 
        LATD &= ~(1 << color);
    return 0;
}

int read_sw(int sw) {
    if ((sw != sw1) && (sw != sw2) && (sw != sw3)) {
        return -1;
    } else {
        // How well do you know C:
        return (((PORTD & (1 << sw)) >> sw) ^ 0x0001);
    }
}

int main(void) {
    
    // Set up the input/output
    ANSELD = 0x0000; // Set port D as digital
    TRISD = 0x20C0; // Set liens 0,1,2 as output; 6,7,13 as input 
    
    while (1 == 1) {
        
        // Simple IO test
        //set_led(red, read_sw(sw1));
        //set_led(amber, read_sw(sw2));
        //set_led(green, read_sw(sw3));
        
        // Define H
        signed _Fract H[2][2] = {{0.707, 0.707},
                                 {0.707, -0.707}};
        
        // Define input state vector
        signed _Fract V[2] = {0.707, -0.707};

        // Define output state vector
        signed _Fract W[2] = {0, 0};
        
        // Multiply H by V
        W[0] = H[0][0] * V[0] + H[0][1] * V[1];
        W[1] = H[1][0] * V[0] + H[1][1] * V[1];
        
        // Light LEDs to show output
        if ((W[0] > 0.95) && (W[1] < 0.05)) 
            set_led(green, on);
        else 
            set_led(amber, on);
        
    }
    return 0;
}
