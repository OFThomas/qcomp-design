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
    void make_ops(Complex X[2][2], Complex Y[2][2], 
            Complex Z[2][2], Complex H[2][2]);
    
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


    /// 4x4 complex matrix multiplication
    /// @param M 4x4 complex matrix
    /// @param V complex state vector
    /// @param i first element of V 
    /// @param j second element of V
    /// @param k third element of V
    /// @param l Fourth element of V
    /// @note this function is never used as we realised that a 2-qubit gate is a reduced
    /// form of a single qubit gate...
    /// @todo remove or make this general? It might not even be needed :(
    void mat_mul_4(Complex M[4][4], Complex V[], int i, int j, int k, int l);
            
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
    

    /**
     * @brief Display the state amplitudes on LEDs
     * @param state Pass in the state vector
     * @param Qnum The total number of qubits
     * 
     * @note Currently the function only displays superpositions using the
     * red and blue colors.
     * @todo rename to display_average
     */
    void qubit_display(Complex state[], int Qnum);


    int sort_states(Complex state[], int num_qubits);

    int val_of_pos_bit(int input, int pos);

    /// @brief updates disp_state where the first 'return value of the function'elements
    /// are the nonzero elements of the state vector 'state'
    /// @param state complex state vector in
    /// @param num_qubits int number of qubits in
    /// @param disp_state complex inout vector where the first n entries are the nonzero
    /// elements of 'state'
    /// @return returns the number of elements to look at in disp_state.
    int remove_zero_amp_states(Complex state[], int num_qubits, int disp_state[]);

#ifdef	__cplusplus
}
#endif

#endif	/* QUANTUM_H */

