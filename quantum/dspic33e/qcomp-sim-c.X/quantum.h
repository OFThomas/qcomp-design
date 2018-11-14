/**
 * @file quantum.h
 * 
 * @brief Description: Header file containing all the matrix arithmetic for 
 * simulating a single qubit.
 */


#ifndef QUANTUM_H
#define	QUANTUM_H

#ifdef	__cplusplus
extern "C" {
#endif

#define ONE_Q15 0.9999694824

#include "p33EP512MU810.h"
#include "xc.h"
#include <math.h>
  
    /// Basis states
    typedef enum {ZERO, ONE, PLUS, MINUS, iPLUS, iMINUS} State;
    
    /// Basic fractional time
    typedef signed _Fract Q15; 
    
    /// Complex type
    typedef Q15 Complex[2];

    /// Create complex X, Y, Z and H
    /// @param X Pauli X c-Matrix
    /// @param Z Pauli Z c-matrix
    /// @param H Hadamard c-matrix
    /// @param Y Pauli Y c-matrix
    void make_ops_cmplx(Complex X[2][2], Complex Y[2][2], 
            Complex Z[2][2], Complex H[2][2]);
    
    /// Initialise state to the vacuum (zero apart from the first position)
    /// Specify the dimension -- of the matrix, i.e. 2^(number of qubits)
    void zero_state(Complex state[], int N);
  
    /// 2x2 complex matrix multiplication
    /// @param M complex matrix
    /// @param V complex vector
    /// @param i integer first element of state vector
    /// @param j integer second element of state vector
    void mat_mul_cmplx(Complex M[2][2], Complex V[], int i, int j);

    /**
     * @brief Display the state amplitudes on LEDs
     * @param state Pass in the state vector
     * @param N The total number of qubits
     * 
     * @note Currently the function only displays superpositions using the
     * red and blue colors.
     */
    void qubit_display(Complex state[], int N);

#ifdef	__cplusplus
}
#endif

#endif	/* QUANTUM_H */

