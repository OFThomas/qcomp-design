/**
 * @file tests.h
 * 
 * @brief Description: Header file containing all the tests we performed
 * 
 */
#ifndef TESTS_H
#define	TESTS_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "p33EP512MU810.h"
#include "xc.h"

    // Testing the speed of 2^15 2x2 matrix multiplications
    void mat_mul_test();
    
    // Testing the speed of 2^15 2x2 real matrix multiplications
    void mat_mul_test_cmplx();

    // Simulating one qubit. Buttons apply H, X and Z and LEDs display the
    // state of the qubit.
    void one_qubit();
    
    // Simulating one qubit with complex amplitudes. Buttons apply H, X, Y and Z 
    // and LEDs display the state of the qubit.
    void one_qubit_cmplx();
    
    // Dimmable LEDs 
    void dim_leds();
    
    // Multi LED strobing test
    void multi_led_strobe();
    
    /// delays for 100,000
    void delay();
#ifdef	__cplusplus
}
#endif

#endif	/* TESTS_H */

