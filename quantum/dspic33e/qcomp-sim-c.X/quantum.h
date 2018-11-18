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


#include "p33EP512MU810.h"
#include "xc.h"
#include <math.h>
#include <stdbool.h>
#include "consts.h" 
    /// Basis states
    typedef enum {ZERO, ONE, PLUS, MINUS, iPLUS, iMINUS} State;

    /// Initialise state to the vacuum (zero apart from the first position)
    /// Specify the dimension -- of the matrix, i.e. 2^(number of qubits)
    /// @param state complex state vector 
    /// @param Qnum int total number of qubits 
    void zero_state(Complex state[], int Qnum);
  
    /// 2x2 complex matrix multiplication
    /// @param M complex matrix
    /// @param V complex vector
    /// @param i integer first element of state vector
    /// @param j integer second element of state vector
    /// @todo Because of the way the array types work (you can't pass a 
    /// multidimensional array of unknown size) we will also need a function
    /// for 4x4 matrix multiplication.
    void mat_mul(Complex M[2][2], Complex V[], int i, int j);

     /** apply operator
     * @param state state vector containing amplitudes 
     * @param qubit qubit number to apply 2x2 matrix to
     * @param Qnum total number of qubits in the state
     * @param op 2x2 operator to be applied
     */
    void single_qubit_op(Complex op[2][2], int qubit, Complex state[], int Qnum);
    
    /// apply controlled 2x2 op
    /// @param op single qubit unitary 2x2
    /// @param ctrl control qubit number (0,1,..,n-1)
    /// @param targ target qubit number (0,1,...,n-1)
    /// @param state complex state vector
    /// @param N total number of qubits 
    void controlled_qubit_op(Complex op[2][2], int ctrl, int targ, Complex state[], int N);
    
    /// abs function
    Q15 absolute(Complex x);
    
#ifdef	__cplusplus
}
#endif

#endif	/* QUANTUM_H */

