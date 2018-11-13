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

/** @brief Contains the button states
 * 
 * Each entry in the array is either 1 if the button is 
 * pressed or 0 if not. The array is accessed globally using
 * `extern buttons;' in a *.c file. Read buttons array us updated
 * by calling read_external_buttons
 */
int buttons[16];

/// @brief Takes led number & RGB -> returns integer for sending via SPI to set the LED
/// @param device input LED number to change
/// @param R red value between 0 & 1 
/// @param G green value between 0 & 1
/// @param B blue value between 0 & 1
/// @return Returns int to be sent to LED Driver 
int led_color_int(int device, int R, int G, int B) {

    int color = 0;
    int led_output;
    
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

/// The LED array 
LED led[LED_NUM]; 

#define DISPLAY_CHIP_NUM 2
/// Display buffer to be written to display driver
int display_buf[DISPLAY_CHIP_NUM] = {0};

/** @brief Counter for the interrupt service routine _T5Interrupt
 * 
 * These variables are for keeping track of the interrupt based LED pulsing.
 * The type is _Fract because it is easier to directly compare two _Fracts
 * than attempt multiplication of integers and _Fracts (which isn't 
 * supported) The limit is not 1 because _Fract types do not go up to 1.
 * 
 * It's probably a good idea to make sure the isr_res counter doesn't overflow
 * (by ensuring that isr_res + isr_limit does not exceed 0.999..., the max 
 * value of unsigned _Fract).
 */
unsigned _Fract isr_counter = 0; /// Counter value
unsigned _Fract isr_res = 0.01; /// Counter resolution
const unsigned _Fract isr_limit = 0.95; /// The max value for isr_counter

/** @brief Interrupt service routine for timer 4
 * 
 * Interrupt service routines are automatically called by the microcontroller
 * when an event occurs. In this case, _T5Interrupt is called when the 32 bit
 * timer formed from T4 and T5 reaches its preset period. The silly name and
 * sill attributes are so that the compiler can correctly map the function in 
 * the microcontroller memory. More details of interrupts and interrupt vectors 
 * can be found in the compiler manual and the dsPIC33E datasheet.
 * 
 * The job of this routine is to control the modulated brightnesses of the 
 * RBG LEDs. This routine is set to be called periodically with a very long
 * period on the time scale of microcontroller operations, but very fast
 * in comparison to what the eye can see. For example, once every 100us.
 * 
 * Each time the routine is called, it increments counters corresponding to
 * RGB line of every LED. Once these counters reach thresholds that have been 
 * set globally in another function, the interrupt routine turns off the 
 * corresponding LED line. Once the isr counter has reached a different (fixed)
 * threshold value, the whole routine resets. In this way the LEDs are turned
 * on and off repeatedly with adjustable duty cycles.
 */
void __attribute__((__interrupt__, no_auto_psv)) _T5Interrupt(void) {
    extern LED led[LED_NUM];
    int rgb_update[3] = {1,1,1};
    /// Loop over all the LEDs (the index i). 
    for(int i = 0; i < LED_NUM; i++) {
        // Reset the tmp variable rgb_update to 1 (all LEDs ON)
        for(int m=0; m<3; m++) rgb_update[m] = 1;
        // Check if the RGB lines need to change
        /// when counter is reached turn led OFF rgb_update=0
        if(led[i].n_R > led[i].N_R) rgb_update[0] = 0; 
        else led[i].n_R += isr_res; /// Increment the LED RGB counter
        if(led[i].n_G > led[i].N_G) rgb_update[1] = 0;
        else led[i].n_G += isr_res; /// Increment the LED RGB counter
        if(led[i].n_B > led[i].N_B) rgb_update[2] = 0;
        else led[i].n_B += isr_res; /// Increment the LED RGB counter
        // Update the display buffer
        update_display_buffer(i, rgb_update[0], rgb_update[1], rgb_update[2]);    
    }
    // Write the display buffer data to the display drivers
    write_display_driver();
    
    // Add an increment to the ISR counter
    isr_counter += isr_res; 
  
    // Check if counter has reached the limit
    if(isr_counter > isr_limit) {
        isr_counter = 0; /// Reset the counter
        /// @todo turn on all the LEDs back on
        for (int i = 0; i < LED_NUM; i++){
            update_display_buffer(i, 1, 1, 1);
            write_display_driver();
            /// Reset all the counters
            led[i].n_R = led[i].n_G = led[i].n_B = 0;
        }
    }
    
    // Reset the timer
    TMR4 = 0x0000;
    TMR5 = 0x0000;
    // Clear Timer4 interrupt flag
    IFS1bits.T5IF = 0;
}

/// @brief Set external variable RGB LEDs
void setup_external_leds(void) {
    /// Setup up external LED lines
    extern LED led[LED_NUM];

    /// Initialise LED lines
    led[0].R[1] = 4; led[0].R[0] = 0;
    led[0].G[1] = 2; led[0].G[0] = 0;
    led[0].B[1] = 3; led[0].B[0] = 0;
    
    led[1].R[1] = 5; led[1].R[0] = 0;
    led[1].G[1] = 6; led[1].G[0] = 0;
    led[1].B[1] = 7; led[1].B[0] = 0;

    led[2].R[1] = 2; led[2].R[0] = 1;
    led[2].G[1] = 3; led[2].G[0] = 1;
    led[2].B[1] = 4; led[2].B[0] = 1;
    
    led[3].R[1] = 5; led[3].R[0] = 1;
    led[3].G[1] = 6; led[3].G[0] = 1;
    led[3].B[1] = 7; led[3].B[0] = 1;
    
    /// Initialise parameters to zero
    for (int i = 0; i < LED_NUM; i++) {
        set_external_led(i, 0, 0, 0); // Set brightnesses to zero
        led[i].n_R = led[i].n_G = led[i].n_B = 0; // Set counters to zero
    }
    
    /// Initialise display buffer to zero
    for (int i = 0; i < DISPLAY_CHIP_NUM; i++)
        display_buf[i] = 0;
    
    // Reset TMR4, TMR5
    TMR4 = 0x0000;
    TMR5 = 0x0000;
    // Set flashing period
    PR4 = 0x0000;
    PR5 = 0x0008;
    
    // Turn timer 4 on
    T4CONbits.TON = 1;
}

/// @brief Stop LEDs flashing
void stop_external_leds(void) {
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
 * 
 * @param index LED number to modify
 * @param R Intended value of the R led
 * @param G Intended value of the G led
 * @param B Intended value of the B led
 * @return 0 if successful
 * 
 * Could this get any worse!
 * 
 * This function is supposed to make the display writing process 
 * more efficient. It updates a global display buffer which is 
 * written periodically to the led display drivers. Instead of the display
 * driver function re-reading the desired state of all the LED lines every 
 * time it is called, this function can be used to update only the lines
 * that have changed.
 * 
 * There are quite a few potential bugs in here, mainly array out of bounds
 * if the DISPLAY_CHIP_NUM is not set correctly or the LED RGB lines are 
 * wrong. (Or if there are just bugs.)
 */
int update_display_buffer(int index, int R, int G, int B) {
    // Global variables
    extern int display_buf[DISPLAY_CHIP_NUM]; /// @todo hmmm...
    extern LED led[LED_NUM]; // @todo hmmmmmm! ...

    if(R==0) display_buf[led[index].R[0]] &= ~(1 << led[index].R[1]);
    else display_buf[led[index].R[0]] |= (1 << led[index].R[1]);
    
    
    if(G==0) display_buf[led[index].G[0]] &= ~(1 << led[index].G[1]);
    else display_buf[led[index].G[0]] |= (1 << led[index].G[1]);
    
    if(B==0) display_buf[led[index].B[0]] &= ~(1 << led[index].B[1]);
    else display_buf[led[index].B[0]] |= (1 << led[index].B[1]);
    
    return 0;
}

/** 
 * @brief Turn on an LED via the external display driver
 *
 * On power on, the chip (TLC591x) is in normal mode which means that
 * the clocked bytes sent to the chip set which LEDs are on and which 
 * are off (as opposed to setting the current of the LEDs)
 *
 * To write to the device, use the SPI module to write a byte to the
 * SDI 1 pin on the chip. Then momentarily set the LE(ED1) pin to latch
 * the data onto the output register. Finally, bring the OE(ED2) pin low
 * to enable the current sinking to turn on the LEDs. See the timing diagram 
 * on page 17 of the datasheet for details.  
 *
 * LE(ED1) and OE(ED2) will be on Port D 
 */
int write_display_driver(void) {
    /// Global variables
    extern int display_buf[DISPLAY_CHIP_NUM]; /// @todo hmmm...
    // Write the display buffer to the device using SPI
    for (int n = 0; n < DISPLAY_CHIP_NUM; n++) 
        send_byte_spi_1(display_buf[n]);
    
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
 *
 * The mode switch for the TLC591x chip is a bit tricky because it 
 * involves synchronising the control lines LE(ED1) and OE(ED2) on Port D 
 * with the SPI 1 clock. To initiate a mode switch, OE(ED2) must be brought 
 * low for one clock cycle, and then the value of LE(ED1) two clock cycles
 * later determines the new mode. See the diagrams on page 19 of the
 * datasheet
 *
 * So long as the timing is not strict, we can probably implement the
 * mode switch by starting a non-blocking transfer of 1 byte to the device
 * (which starts the SPI 1 clock), followed by clearing OE(ED2) momentarily 
 * and then setting the value of LE(ED1) as required. So long as those 
 * two things happen before the SPI 1 clock finishes the procedure will
 * probably work. (The reason is the lack of max timing parameters on page
 * 9 for the setup and hold time for ED1 and ED2, which can therefore 
 * presumably be longer than one clock cycle.)
 * @param mode 
 * @todo mode switcher for LED Driver
 */
int TLC591x_mode_switch(int mode) {
    return 0;
}

/**
 * @brief Updates color properties of global led array
 * @param led_index
 * @param R red value between 0 & 1 
 * @param G green value between 0 & 1
 * @param B blue value between 0 & 1
 * @return 0 if successful, -1 otherwise
 * 
 * Use the function to set the RGB level of an LED. The LED is chosen
 * using the @param led_index. The @param R, @param G and @param B are
 * numbers between 0 and 1 (not including 1) indicating the amount of 
 * each color. The function returns 0 if successful and -1 otherwise.   
 */
int set_external_led(int index, 
        unsigned _Fract R, 
        unsigned _Fract G,
        unsigned _Fract B) {
    extern LED led[LED_NUM];
    led[index].N_R = R;
    led[index].N_G = G;
    led[index].N_B = B;
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
#define BTN_CHIP_NUM 2
int read_external_buttons(void) {
    // Bring SH low momentarily
    LATD &= ~(1 << SH); /// SH pin
    unsigned long int n = 0;
    while (n < 5) /// @todo How long should this be? 
        n++;
    LATD |= (1 << SH); // Set SH pin again

    // Read the button states
    int btn_byte = 0;
    // Loop over the number of chips
    for(int r = 0; r < BTN_CHIP_NUM; r++) {
        btn_byte = read_byte_spi_3();
        // loop over the bits in the byte
        for(int s = 0; s < 8; s++) {
            buttons[s] = ((btn_byte >> s) & 1); // Update the button array
        }
    }
    return 0;
    /// @todo button remappings...
}

/**
 * @brief Loop to cycle through LEDs 0 - 15
 *
 */
int led_cycle_test(void) {
    unsigned int counter = 0;
    int step = 0;
    int flag = 0;
    int j = 0;
    unsigned long int m = 0;
    while (j <= 36) {
        while (m < 10000) m++;
        m = 0;
        /// @todo This won't work now:
        /// write_display_driver(counter);
        if (flag == 0) {
            step = 4 << 8;
            flag++;
        } else if (flag == 1) {
            step = 32 << 8;
            flag++;
        } else if (flag == 2) {
            step = 4;
            flag++;
        } else if (flag == 3) {
            step = 32;
            flag = 0;
        }
        counter += step;
        if (counter == (0xFCFC + (4 << 8))) {
            counter = 0;
            flag = 0;
            step = 0;
        }
        j++;
    }
    return 0;
}