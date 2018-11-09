
#include "stdio.h"

/// @brief Takes led number & RGB -> returns integer for sending via SPI to set the LED
/// @param device input LED number to change
/// @param R red value between 0 & 1 
/// @param G green value between 0 & 1
/// @param B blue value between 0 & 1
/// @return Returns int to be sent to LED Driver 
int led_color_int(int device, int R, int G, int B){

    int color;
    int led_output;

    color = 0;
   
    /// convention RGB -> 000
    color = (R*4) + (G*2) + B;
    printf( "The color is, %d\n", color);

    /// Each LED takes 3 lines, assumes there are no gaps between LED channels
    /// "device" goes between 0 to 2^n -1
    led_output = (color << (device*3));
    printf("led output is, %d\n", led_output);
    
    return led_output;
}

int main(void){
    /// for device 1, R=1, G=1, B=1 -> white LED integer
    led_color_int(1, 1, 1, 1);
    return 0;
}
