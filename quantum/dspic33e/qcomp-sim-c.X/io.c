/**
 * @file io.c
 * @brief Contains all the functions for reading buttons and writing to LEDs
 * 
 * @author J Scott
 * @date 8/11/18
 *
 */

#include "io.h"
#include "time.h"
#include "spi.h"


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

    /// Each LED takes 3 lines, assumes there are no gaps between LED channels
    /// "device" goes between 0 to 2^n -1
    led_output = (color << (2));
    
    return led_output;
}

/// @brief Set up LEDs and buttons on port D 
int setup_io(void) {
    // Set up the input/output
    ANSELD = 0x0000; // Set port D to digital
    TRISD = 0x20C0; // Set lines 0,1,2 as output; 6,7,13 as input 
    ///< Set port c digital for spi3
    ANSELC = 0x0000; // Set port C to digital
    TRISC = 0x0010; // Set line 4 as input 
    
    // Setup timers for flashing LEDs
    T4CON = 0x0000; // Reset the timer control registers
    T5CON = 0x0000;
    // Set up timer 4 in 32 bit mode with timer 5
    // Clock prescaler 1:1, internal oscillator source.
    T4CON = 0x0008;
    // No need to change anything in T5CON
    // Reset TMR4, TMR5, PR4 and PR5
    TMR4 = 0x0000;
    TMR5 = 0x0000;
    PR4 = 0x0000; // Reset registers
    PR5 = 0x0000;
    // Setup interrupts for timer 5
    IEC1bits.T5IE = 1; // Enable the interrupt
    IFS1bits.T5IF = 0; // Clear the interrupt flag
    /// Set the OE pin high
    LATD |= (1 << OE); /// Set OE(ED2) pin
    /// Set the SH pin high
    LATD |= (1 << SH); /// Set SH pin
    /// set CLK_INH high while buttons are pressed
    LATD |= (1 << CLK_INH);
    return 0;
}

/// @param led_global  Global LED strobing state parameter
LED_GLOBAL led_global = {0};
    
/// @brief Interrupt service routine for timer 4
/// @note I have no idea what this line means...
void __attribute__((__interrupt__, no_auto_psv)) _T5Interrupt(void) {
    // Read the state and change to next state
    led_global.strobe_state ^= 1; // Flip bit zero
    if (led_global.strobe_state == 1) {
        // Turn on LEDs
        LATD |= (led_global.strobe_leds);
    } else {
        // Turn off LEDs
        LATD &= ~(led_global.strobe_leds);
    }

    // Reset the timer
    TMR4 = 0x0000;
    TMR5 = 0x0000;
    // Clear Timer4 interrupt flag
    IFS1bits.T5IF = 0;
}

/// @brief Set LEDs flashing
void start_strobe() {
    // Reset TMR4, TMR5
    TMR4 = 0x0000;
    TMR5 = 0x0000;
    // Set flashing period
    PR4 = 0x0000;
    PR5 = 0x0080;
    // Turn timer 4 on
    T4CONbits.TON = 1;
}

/// @brief Stop LEDs flashing
void stop_strobe() {
    T4CONbits.TON = 0; // Turn timer 4 off
    
}

/// @brief Set an LED strobing
void set_strobe(int color, int state) {
    extern LED_GLOBAL led_global;
    switch(state) {
        case on: // Start the strobing
            LATD &= ~(1 << color);
            led_global.strobe_leds |= (1 << color);
            break;
        case off:
            LATD &= ~(1 << color);
            led_global.strobe_leds &= ~(1 << color);
            break;
    }
}
  
/// @brief Toggle LED strobe
void toggle_strobe(int color) {
    extern LED_GLOBAL led_global;
    LATD &= ~(1 << color);
    led_global.strobe_leds ^= (1 << color);
}

/// @brief Turn a particular LED on or off
  int set_led(int color, int state) {
  if (state == on)
    LATD |= (1 << color);
  else
    LATD &= ~(1 << color);
  return 0;
}

/// @brief Read the state of a push button
  int read_btn(int btn) {
  if ((btn != sw1) && (btn != sw2) && (btn != sw3)) {
    return -1;
  } else {
    /// @note How well do you know C
    return (((PORTD & (1 << btn)) >> btn) ^ 0x0001);
  }
}

/// @brief Turn all the LEDs off
void leds_off(void) {
  set_led(green, off);
  set_led(amber, off);
  set_led(red, off);
}

#define PERIOD 500000
/// @brief Flash LED a number of times
  void flash_led(int color, int number) {
    unsigned long int m = 0, n = 0; // You need 32 bit types for this
    while(n < number) {
        set_led(color, on);
        m = 0;
        while(m < PERIOD) m++;
        set_led(color, off);
        m = 0;
        while(m < PERIOD) m++;
        n++;
    }
}

/// @brief Flash all the LEDs a number of times
  void flash_all(int number) {

    unsigned long int m = 0, n = 0; // You need 32 bit types for this
    while(n < number) {
        set_led(red, on);
        set_led(amber, on);
        set_led(green, on);
        m = 0;
        while(m < PERIOD) m++;
        leds_off();
        m = 0;
        while(m < PERIOD) m++;
        n++;
    }
}

/** 
 * @brief Turn on an LED via the external display driver
//
// On power on, the chip (TLC591x) is in normal mode which means that
// the clocked bytes sent to the chip set which LEDs are on and which 
// are off (as opposed to setting the current of the LEDs)
//
// To write to the device, use the SPI module to write a byte to the
// SDI 1 pin on the chip. Then momentarily set the LE(ED1) pin to latch
// the data onto the output register. Finally, bring the OE(ED2) pin low
// to enable the current sinking to turn on the LEDs. See the timing diagram 
// on page 17 of the datasheet for details.  
//
// LE(ED1) and OE(ED2) will be on Port D 
* @param data the byte to send to LED driver
*/
int set_external_led(int data) {
    // Write data to the device using SPI
    send_byte_spi_1(data);
    // Bring LE high momentarily
    LATD |= (1 << LE); /// Set LE(ED1) pin
    unsigned long int n = 0;
    while(n < 10) /// @todo How long should this be? 
        n++;
    LATD &= ~(1 << LE); // Clear LE(ED1) pin
    
    // Bring the output enable low
    LATD &= ~(1 << OE); // Clear OE(ED2) pin
    
    return 0;
    
}

/** 
 * @brief Switch between normal and special mode
//
// The mode switch for the TLC591x chip is a bit tricky because it 
// involves synchronising the control lines LE(ED1) and OE(ED2) on Port D 
// with the SPI 1 clock. To initiate a mode switch, OE(ED2) must be brought 
// low for one clock cycle, and then the value of LE(ED1) two clock cycles
// later determines the new mode. See the diagrams on page 19 of the
// datasheet
//
// So long as the timing is not strict, we can probably implement the
// mode switch by starting a non-blocking transfer of 1 byte to the device
// (which starts the SPI 1 clock), followed by clearing OE(ED2) momentarily 
// and then setting the value of LE(ED1) as required. So long as those 
// two things happen before the SPI 1 clock finishes the procedure will
// probably work. (The reason is the lack of max timing parameters on page
// 9 for the setup and hold time for ED1 and ED2, which can therefore 
// presumably be longer than one clock cycle.)
* @param mode 
* @todo mode switcher for LED Driver
*/
int TLC591x_mode_switch(int mode) {
    return 0;
}

/** @brief Read external buttons
//
// The external buttons are interfaced to the microcontroller via a shift
// register. Data is shifted in a byte at a time using the SPI 3 module. The
// sequence to read the buttons is as follows:
//
// 1) Momentarily bring SH low to latch button data into the shift registers
// 2) Bring CLK_INH low to enable the clock input on the shift register
// 3) Start the SPI 3 clock and read data in via the SDI 3 line
//
// The control lines SH and CLK_INH are on port D
//
* @todo read buttons
*/
int read_external_buttons() {
    
}
