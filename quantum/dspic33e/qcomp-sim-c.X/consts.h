/**
 * @file consts.h
 *
 * @brief header file for (global) constants
 */

#ifndef CONSTS_H
#define	CONSTS_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#define NUM_QUBITS 4
#define STATE_LENGTH 16 // 2^NUM_QUBITS

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

