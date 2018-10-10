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

typedef struct {
    signed _Fract a11;
    signed _Fract a12;
    signed _Fract a21;
    signed _Fract a22;
} Matrix;

typedef struct {
    signed _Fract a1;
    signed _Fract a2;
} Vector;

Vector mat_mul(Matrix M, Vector V) {
    Vector W = {0, 0}; // To store the output
    W.a1 = M.a11 * V.a1 + M.a12 * V.a2;
    W.a2 = M.a21 * V.a1 + M.a22 * V.a2;
    return W;
}

int main(void) {

    // Set up the input/output
    ANSELD = 0x0000; // Set port D as digital
    TRISD = 0x20C0; // Set liens 0,1,2 as output; 6,7,13 as input 

    // Define H
    Matrix H = {0.707, 0.707, 0.707, -0.707};
    Matrix X = {0.0, 0.9999, 0.9999, 0.0};

    // Define state vector
    Vector V = {0.0, 0.0};

    while (1 == 1) {
        
        // Simple IO test
        set_led(red, on);
        int cnt = 0;
        while(cnt < 10000) cnt++;
        cnt = 0;
        set_led(red, off);
        while(cnt < 10000) cnt++;
        cnt = 0;
        //set_led(amber, read_sw(sw2));
        //set_led(green, read_sw(sw3));

        
        
        // Multiply H by V
        V = mat_mul(H, V);

        // Light LEDs to show output
        if ((V.a1 > 0.95) && (V.a2 < 0.05))
            set_led(green, on);
        else
            set_led(amber, on);

    }

    return 0;
}
