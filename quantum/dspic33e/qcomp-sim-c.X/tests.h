/*
 * File: tests.h
 * 
 * Description: Header file containing all the tests we performed
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

    // Simulating one qubit. Buttons apply H, X and Z and LEDs display the
    // state of the qubit.
    void one_qubit();
    
#ifdef	__cplusplus
}
#endif

#endif	/* TESTS_H */

