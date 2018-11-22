/**
 * @file quantum.h
 * 
 * @brief Description: Header file containing all the matrix arithmetic for 
 * simulating a single qubit.
  * @authors J Scott, O Thomas
 * @date Nov 2018
*/


#ifndef QUANTUM_H
#define	QUANTUM_H

#ifdef	__cplusplus
extern "C" {
#endif


#include <math.h>
#include "consts.h" 

    /// Basis states
    typedef enum {ZERO, ONE, PLUS, MINUS, iPLUS, iMINUS} State;

    /// Initialise state to the vacuum (zero apart from the first position)
    /// Specify the dimension -- of the matrix, i.e. 2^(number of qubits)
    /// @param state complex state vector 
    void zero_state(Complex state[]);

    /// returns phase quadrant 
    int sign(Complex a);

    /// 2x2 complex matrix multiplication
    /// @param M complex matrix
    /// @param V complex vector
    /// @param i integer first element of state vector
    /// @param j integer second element of state vector
    /// @todo Because of the way the array types work (you can't pass a 
    /// multidimensional array of unknown size) we will also need a function
    /// for 4x4 matrix multiplication.
    void mat_mul(const Complex M[2][2], Complex V[], int i, int j);

     /** apply operator
     * @param state state vector containing amplitudes 
     * @param qubit qubit number to apply 2x2 matrix to
     * @param op 2x2 operator to be applied
     */
    void single_qubit_op(const Complex op[2][2], int qubit, Complex state[]);
    
    /// apply controlled 2x2 op
    /// @param op single qubit unitary 2x2
    /// @param ctrl control qubit number (0,1,..,n-1)
    /// @param targ target qubit number (0,1,...,n-1)
    /// @param state complex state vector
    void controlled_qubit_op(const Complex op[2][2], int ctrl, int targ, Complex state[]);
    
    /// abs function
    Q15 absolute(Complex x);

    /**
     * @brief A simple function to compute integer powers of 2
     * @param k The exponent of 2 to compute
     * @return 2^k
     * 
     */
    int pow2(int k);

    /**
     * @brief Compute the magnitude squared of a complex number
     * 
     * @param x The input complex number x
     * @return The value of |x|^2
     * 
     * @todo Maybe we should inline this 
     */
    Q15 square_magnitude(Complex x);
    
#ifdef	__cplusplus
}
#endif

#endif	/* QUANTUM_H */

