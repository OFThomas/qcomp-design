/**
 * @file: io.h
 * 
 * @brief Description: Header file for input output functions. Include it at the
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

/// Locations of LEDs and buttons on Port D
#define red 0
#define amber 1
#define green 2

#define sw1 6
#define sw2 7
#define sw3 13

#define off 0
#define on 1
    
/// Control for TLC591x chip on Port D
#define LE 3
#define OE 4
    
/// COntrol lines for SNx4HC165 chip
#define SH 5
#define CLK_INH 8
    
/// The number of external LEDs  
#define LED_NUM 4 
   
    /** @brief pin mappings
    // Pins for LE and OE on port D
    // OE = RD4 = uC:81 = J1:28 = J10:14
    // LE = RD3 = uC:78 = J1:40 = J11:18
    
    // Pins for SH and CLK_INH on port D
    // SH = RD5 = uC:82 = J1:25 = J10:13
    // CLK_INH = RD8 = uC:68 = J1:58 = J11:25
    */

    /// @brief Global LED strobing state parameter
    typedef struct {
        int strobe_leds; ///< Bit set the LEDs which are strobing 
        int strobe_state; ///< Bit zero is the current state (on/off)
    } LED_GLOBAL;
    
    /**
     *  @brief Each LED has the following type
     * 
     * The type holds the information about the position of the RGB lines
     * in the display driver array and also the brightness of the RGB lines.
     * The counters are used by a timer interrupt service routine pulse the
     * RGB LEDs at a specified rate.
     * 
     * The type of the counter is _Fract to facilitate easy comparison with
     * the N_* variables which used the fractional type.
     */
    typedef struct {
        int R_line; /// The line number for red
        int G_line; /// the line number for green
        int B_line; /// The line number for blue
        int R_chip; /// The line number for red
        int G_chip; /// the line number for green
        int B_chip; /// The line number for blue
        unsigned _Fract N_R; /// The R brightness
        unsigned _Fract N_G; /// The G brightness
        unsigned _Fract N_B; /// The B brightness
        unsigned _Fract n_R; /// Counter for R -- do not modify
        unsigned _Fract n_G; /// Counter for G -- do not modify
        unsigned _Fract n_B; /// Counter for B -- do not modify
    } LED;
    
    /// Set up LEDs and buttons on port D 
    int setup_io(void);
    
    /// @brief Set external variable RGB LEDs
    void setup_external_leds();
    
    /// @brief Turn a particular LED on or off
    /// @param color 
    /// @param state
    int set_led(int color, int state);

    /// @brief Read the state of a push button
    /// @param btn
    int read_btn(int btn);
    
    /// @brief Turn all the LEDs off
    void leds_off(void);
    
    /// @brief Flash one LED a number of times
    /// @param color
    /// @param number
    void flash_led(int color, int number);
    
    /// @brief Flash all the LEDs a number of times
    /// @param number
    void flash_all(int number);
    
    /// @brief Set an LED strobing
    /// @param color
    /// @param state
    void set_strobe(int color, int state);
    
    /// @brief Toggle LED strobe
    /// @param color
    void toggle_strobe(int color);

    /**
     * 
     * @param led_index LED number to modify
     * @param R Intended value of the R led
     * @param G Intended value of the G led
     * @param B Intended value of the B led
     * @return 0 if successful
     */
    int update_display_buffer(int led_index, int R, int G, int B);
    
    /** @brief Send a byte to the display driver
     * @param data
     * 
     * Don't use this function to write to LEDs -- use the set_external_led
     * function
     */
    int write_display_driver(int * data);
    
    /**
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
    int set_external_led(int led_index,
            unsigned _Fract R,
            unsigned _Fract G,
            unsigned _Fract B);

    /// @brief Takes led number & RGB -> returns integer for sending via SPI to set the LED
    /// @param device input LED number to change
    /// @param R red value between 0 & 1 
    /// @param G green value between 0 & 1
    /// @param B blue value between 0 & 1
    /// @return Returns int to be sent to LED Driver 
    int led_color_int(int device, int R, int G, int B);

    /**
     * @brief Loop to cycle through LEDs 0 - 15
     *
     */
    int led_cycle_test();
    
    /**
     * @brief Update the buttons array (see declaration above) 
     * 
     */
    int read_external_buttons(); 
    
#ifdef	__cplusplus
}
#endif

#endif	/* IO_H */

