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
    
    // Flash LEDs
    //flash_all(5);

    // Simulate one qubit
    //one_qubit();

    // Run matrix multiplication test
    //mat_mul_test();

    // Run complex matrix multiplication test
    //mat_mul_test_cmplx();
    
    // Simulate one complex qubit
    //one_qubit_cmplx();
    
    //start_strobe((1 << red) + (1 << green) + (1 << amber));
    
    //dim_leds();
    
    // Test multi LED strobing
    //multi_led_strobe();
    
    // Test the SPI
    //while(1==1) {
    //    send_byte(0xFF);
    //}

    while(1){
    
    int out1 = 0;
    int out2 = 0;
    while(1){
        // Bring SH low momentarily
        LATD &= ~(1 << SH); /// SH pin
        unsigned long int n = 0;
        while(n < 5) /// @todo How long should this be? 
            n++;
        LATD |= (1 << SH); // Set SH pin again
    
       // unsigned long int i = 0;
       // while(i < 20) /// @todo How long should this be? 
       //     i++;
        
        // LATD &= ~(1 << CLK_INH); /// CLK_INH low pin

        out1 = read_byte_spi_3();
        out2 = read_byte_spi_3();
        
        //set_external_led(out1);
        //set_external_led(out2);
        
        
        write_display_driver(
                led_color_int(
                    0,
                    (out1 == 1) , 
                    (out1 == 2), 
                    (out1 == 4))
                );
        
        
        int b = led_color_int(0, 
                (out1 & 1), 
                ((out1 >> 1) & 1), 
                ((out1 >> 2) & 1));
        
        int a = 0;
        a += b; // Put a breakpoint here
        
      //  if((out1 != 0 ) || (out2 != 0)){
       //     read_byte_spi_3();
       // }
        
       // LATD |= (1 << CLK_INH);
    }
        
    //while(1) send_byte_spi_1(0);
    
    /** @breif Example use of RGB LEDs
     */
    _Fract R = 0.6; // Colours
    _Fract G = 0.2;
    _Fract B = 0.1;
    int led_index = 2; // LED number
    // Set RGB LED number 2
    set_external_led(led_index, R, G, B);

    
    /**
     * @brief Loop to cycle through LEDs 0 - 15
     *
     */
    
    unsigned int counter = 0;
    int step = 0;
    int flag = 0;
    int j = 0;
    unsigned long int m = 0;
    while(j <= 36) {
        while(m < 10000) m++;
        m = 0;
        write_display_driver(counter);
        if(flag==0){
            step=4<<8;
            flag++;
        }
        else if(flag==1){
            step=32<<8;
            flag++;
        }
        else if(flag==2){
            step=4;
            flag++;
        }
        else if(flag==3){
            step=32;
            flag=0;
        }
        counter += step;
        if(counter == (0xFCFC+(4<<8))){ 
            counter = 0;
            flag = 0;
            step = 0;
        }
        j++;
    }
    }

        // LATD |= (1 << SH); // Set SH pin
       // LATD |= (1 << CLK_INH); // Set CLK pin

    while(1==1);
    return 0;
}
