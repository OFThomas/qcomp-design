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

    /// Set up LEDs and buttons on port D 
    int setup_io(void);
    
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
    
    /// Set LEDs flashing indefinitely
    void start_strobe();
    
    /// @brief Set an LED strobing
    /// @param color
    /// @param state
    void set_strobe(int color, int state);
    
    /// @brief Toggle LED strobe
    /// @param color
    void toggle_strobe(int color);
    
    /// @brief Set an led on the display driver
    /// @param data
    int set_external_led(int data);
    
    /// @brief 2 LED DRIVERS
    /// @param data_1 1 driver
    /// @param data_2 2nd Driver
    int set_external_led_2(int data_1, int data_2);

#ifdef	__cplusplus
}
#endif

#endif	/* IO_H */

