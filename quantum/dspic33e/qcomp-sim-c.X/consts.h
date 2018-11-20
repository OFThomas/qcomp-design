/**
 * @file consts.h
 *
 * @brief header file for (global) constants
 * @authors J Scott, O Thomas
 * @date Nov 2018
 */

#ifndef CONSTS_H
#define	CONSTS_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include <stdbool.h>
#include <stdlib.h>

#define NUM_QUBITS 4
#define STATE_LENGTH 16 // 2^NUM_QUBITS

       // ONE_Q15 / 2^(N-1) 
#define FULL_PHASE 0.124996185
        // half of full phase
#define HALF_PHASE 0.062498093

/// The number of external LEDs  
#define LED_NUM 4 

#define ONE_Q15 0.9999694824
    
/// Basic fractional time
typedef signed _Fract Q15; 
    
/// Complex type
typedef Q15 Complex[2];

/// @param rX is square root of X
extern const Complex rX[2][2];

/// @param rXT Adjoint of rX
extern const Complex rXT[2][2];

/// @param X pauli X gate
extern const Complex X[2][2];

/// @param Y Pauli y gate
extern const Complex Y[2][2];

/// @param Z Pauli z gate
extern const Complex Z[2][2];

/// @param H Hadamard gate
extern const Complex H[2][2];

#ifdef	__cplusplus
}
#endif

#endif	/* CONSTS_H */

