/**
 * @file quantum.c
 * 
 * @brief Description: Contains matrix and vector arithmetic for simulating 
 * one qubit.
  * @authors J Scott, O Thomas
 * @date Nov 2018
* @todo split into a complex math and operator files 
 */

#include "quantum.h"

/**
 * @brief A simple function to compute integer powers of 2
 * @param k The exponent of 2 to compute
 * @return 2^k
 * 
 */
int pow2(int k) {
    int result = 1;
    for(int n = 0; n < k; n++)
        result *= 2; /// Multiply by 2
    return result;
}

/// 
/// \verbatim
///        Im
//          |
//     1    |   0
//          |
//  ----------------- Re
//          |
//     2    |   3
//          |
/// \endverbatim
int sign(Complex a){
    //real part
    /// if real negative and im neg return -1
    /// if real negative and im pos return -0.5
    if(a[0] < 0.0){
        if(a[1] < 0.0) return 2;
        else return 1;
    }
    /// else if real pos and im negative return -0.5
    else if(a[1] < 0.0) return 3;
    /// else if both pos return 0
    else return 0;
}


// Complex addition
void cadd(const Complex a, const Complex b, Complex result) {
    result[0] = a[0] + b[0];
    result[1] = a[1] + b[1];
}

// Complex multiplication
void cmul(const Complex a, const Complex b, Complex result) {
    result[0] = a[0] * b[0] - a[1] * b[1];
    result[1] = a[0] * b[1] + a[1] * b[0]; 
}

/**
 * @param x A complex number to find the absolute value of
 * @return The absolute value
 */
Q15 absolute(Complex x) {
    Complex y; // Conjugate of x
    y[0] = x[0];
    y[1] = -x[1];
    Complex z;
    cmul(x, y, z);
    /// @todo Check that the complex part is small
    return z[0]; // Return real part
}

/**
 * @brief Compute the magnitude squared of a complex number
 * 
 * @param x The input complex number x
 * @return The value of |x|^2
 * 
 * @todo Maybe we should inline this 
 */
Q15 square_magnitude(Complex x) {
    return x[0] * x[0] + x[1] * x[1];
}

/// Initialise state to the vacuum (zero apart from the first position)
/// Specify the dimension -- of the matrix, i.e. 2^(number of qubits)
void zero_state(Complex state[]) {
    for (int i = 0; i < STATE_LENGTH; i++) {
        // Loop over the real and imaginary parts
        for (int j = 0; j < 2; j++) {
            state[i][j] = 0.0;
        }
    }
    /// @note oh the clarity! 
    state[0][0] = ONE_Q15;
}


/**
 * @brief This is an old version of the mat_mul function
 * 
 * @param M A 2x2 complex matrix
 * @param V A Nx1 complex vector
 * @param i The first index to pick from the vector V
 * @param j The second index to pick from the vector V
 * 
 * The function uses cadd and cmul
 * 
 */
void mat_mul_old(const Complex M[2][2], Complex V[], int i, int j) {
    static Complex a, b, c, d; ///@todo Should these be outside the function?
    cmul(M[0][0],V[i],a); 
    cmul(M[0][1],V[j],b);
    cadd(a,b,c);
    cmul(M[1][0],V[i],a);
    cmul(M[1][1],V[j],b);
    cadd(a,b,d);
    V[i][0] = c[0];
    V[i][1] = c[1];
    V[j][0] = d[0];
    V[j][1] = d[1];
}

/**
 * @brief This version uses inlined cadd and cmul
 * 
 * @param M A 2x2 complex matrix
 * @param V A Nx1 complex vector
 * @param i The first index to pick from the vector V
 * @param j The second index to pick from the vector V
 * 
 */
void mat_mul(const Complex M[2][2], Complex V[], int i, int j) {

    /// @todo Is static enough? Or should we declare outside the function?
    static Q15 a, b, c, d;
    
    /// @todo Should we use for loops? Or is it better not to..?
    
    // Manual complex matrix multiplication for first element of vector
    a = M[0][0][0] * V[i][0] - M[0][0][1] * V[i][1] + 
            M[0][1][0] * V[j][0] - M[0][1][1] * V[j][1]; // Real part
    b = M[0][0][0] * V[i][1] + M[0][0][1] * V[i][0] + 
            M[0][1][0] * V[j][1] + M[0][1][1] * V[j][0]; // Imag part
    
    // Manual complex matrix multiplication for second element of vector
    c = M[1][0][0] * V[i][0] - M[1][0][1] * V[i][1] + 
            M[1][1][0] * V[j][0] - M[1][1][1] * V[j][1]; // Real part
    d = M[1][0][0] * V[i][1] + M[1][0][1] * V[i][0] + 
            M[1][1][0] * V[j][1] + M[1][1][1] * V[j][0]; // Imag part
    
    /// This is necessary because the previous computations use V
    V[i][0] = a;
    V[i][1] = b;
    V[j][0] = c;
    V[j][1] = d;
    
    // Get me out of here
    return;
}

/** apply operator
 * @param state state vector containing amplitudes 
 * @param qubit qubit number to apply 2x2 matrix to
 * @param N total number of qubits in the state
 * @param op 2x2 operator to be applied
 * 
 * This routine applies a single qubit gate to the state vector @param state.
 * Consider the three qubit case, with amplitudes shown in the table below:
 *
 *   \verbatim
 *     index     binary   amplitude 
 *      ----------------------------- 
 *        0       0 0 0       a0
 *        1       0 0 1       a1 
 *        2       0 1 0       a2
 *        3       0 1 1       a3
 *        4       1 0 0       a4
 *        5       1 0 1       a5
 *        6       1 1 0       a6
 *        7       1 1 1       a7
 *      -----------------------------
 *      Qubit:    2 1 0
 * \endverbatim
 * 
 * If a single qubit operation is applied to qubit 2, then the 2x2 matrix 
 * must be applied to all pairs of (0,1) in the first column, with the numbers
 * in the other columns fixed. In other words, the following indices are paired:
 * 
 *       (0+0) (1+0) (2+0) (3+0)
 *       (4+0) (5+0) (6+0) (7+0)
 * 
 * where the top line corresponds to the ZERO amplitude and the bottom row
 * corresponds to the ONE amplitude. 
 * 
 * Similarly, for qubit 1 the pairings are:
 * 
 *       (0+0) (0+4) (1+0) (1+4)
 *       (2+0) (2+4) (3+0) (3+4)
 * 
 * And for qubit 0 the pairings are:
 * 
 *       (0+0) (0+2) (0+4) (0+6)
 *       (1+0) (1+2) (1+4) (1+6)
 * 
 * These numbers are exactly the same as the previous function, which means
 * the same nested loops can be used to perform operation. Now the index
 * 
 *      root + step 
 * 
 * refers to the ZERO amplitude (the first element in the column vector to
 * be multiplied by the 2x2 matrix), and the index
 * Complex state[], int N
 *      root + 2^k + step
 * 
 * corresponds to the ONE entry.
 * 
 */
void single_qubit_op(const Complex op[2][2], int k, Complex state[]) {
    int root_max = pow2(k); // Declared outside the loop
    int increment = 2 * root_max;
    /// ROOT loop: starts at 0, increases in steps of 1
    for (int root = 0; root < root_max; root++) {
        /// STEP loop: starts at 0, increases in steps of 2^(k+1)
        for (int step = 0; step < STATE_LENGTH; step += increment) {
            /// First index is ZERO, second index is ONE
            /// @todo Should we inline mat_mul here?
            mat_mul(op, state, root + step, root + root_max + step);
        }
    }
}

/**
 * selective 2 qubit op function 

 *  checks that the control qubit is |1> then does 2x2 unitary on remaining state vector
 *  elements
 * 
 * This routine implements a controlled unitary gate. Controlled unitaries can
 * be expressed as single qubit unitaries that are conditionally applied if 
 * the control qubit state (ctrl) is 1. Otherwise no operation is performed.
 * 
 * The following example is for the three qubit case. Suppose the following
 * operation is performed. 
 * 
 *  \verbatim
 *      00 01 10 11
 *  00 ( 1  0  0  0   )
 *  01 ( 0  1  0  0   )
 *  10 ( 0  0 u00 u01 )
 *  11 ( 0  0 u10 u11 )
 *  \endverbatim
 * 
 * The first qubit is the control (ctrl) and the second qubit is the target 
 * (targ). If the control is 0 the identity operation is performed. If the
 * control qubit is 1, then a unitary U (the second block above) is performed.
 * 
 * For three qubits, the state vector is shown below: 
 *
 *   \verbatim
 *     index     binary   amplitude 
 *      ----------------------------- 
 *        0       0 0 0       a0
 *        1       0 0 1       a1 
 *        2       0 1 0       a2
 *        3       0 1 1       a3
 *        4       1 0 0       a4
 *        5       1 0 1       a5
 *        6       1 1 0       a6
 *        7       1 1 1       a7
 *      -----------------------------
 *      Qubit:    2 1 0
 * \endverbatim
 * 
 * Suppose the controlled unitary is to be performed between qubits 0 and 1,
 * with the control qubit on 0. Suppose the controlled gate is a CNOT, so that
 * the 2x2 matrices involved are I and X. X and I are performed on the 
 * following (vertical) pairs of indices
 * 
 *             I               X
 * i:     (0+0) (0+4)     (1+0) (1+4)           (ctrl = 0, targ = 1)
 * j:     (0+2) (0+6)     (1+2) (1+6)           
 * 
 * If the control and target are reversed (ctrl on 1), then the pairings of the
 * indices are
 *
 *           I                 X
 * i:     (0+0) (0+4)     (2+0) (2+4)           (ctrl = 1, targ = 0)
 * j:     (0+1) (0+5)     (2+1) (2+5)
 * 
 * For control and target qubits on 0 and 2 the indices are
 * 
 *             I               X
 * i:     (0+0) (0+2)     (1+0) (1+2)           (ctrl = 0, targ = 2)
 * j:     (0+4) (0+6)     (1+4) (1+6)           
 * 
 * If the control and target are reversed (ctrl on 2), then the pairings of the
 * indices are
 *
 *             I               X
 * i:     (0+0) (0+2)     (4+0) (4+4)           (ctrl = 2, targ = 0)
 * j:     (0+1) (0+3)     (4+1) (4+5)
 * 
 * Finally, if the control and target are 1 and 2, then
 * 
 *             I               X
 * i:     (0+0) (0+1)     (2+0) (2+1)           (ctrl = 1, targ = 2)
 * j:     (0+4) (0+5)     (2+4) (2+5)           
 * 
 * If the control and target are reversed (ctrl on 2), then the pairings of the
 * indices are
 *
 *             I               X
 * i:     (0+0) (0+1)     (4+0) (4+1)           (ctrl = 2, targ = 1)
 * j:     (0+2) (0+3)     (4+2) (4+3)
 * 
 * The pattern in the general case is as follows. Firstly, similarly to the 
 * single qubit case, the index required can be expressed as the sum of a root
 * and another contribution. In this case, the root depends only on the ctrl
 * qubit number:
 * 
 * root = x * 2^ctrl
 * 
 * where x is the state of the ctrl qubit (either 1 or 0). This will determine
 * whether I or (in the case of CNOT) X is applied. That the root only depends
 * on the ctrl qubit number is due to the interpretation of root -- it is the
 * base index of all the ctrl states of a particular value. For example, 
 * whatever the qubit number, the starting index of the zero ctrl state is 
 * always zero. Then, the first occurance of a 1 in the ctrl qubit depends 
 * on the ctrl qubit number, and is just a power of 2 into the state vector.
 * 
 * The other contributions to the index depend on the the target qubit number 
 * (targ). The offset between indices of the same operation (either I or X) are 
 * separated by 
 * 
 * sep = 2^targ
 * 
 * The logic for this is similar to the case for ctrl: the way to get from a 
 * 0 in the target to a 1 in the target is to add 2^targ to the index in the
 * state vector.
 * 
 * Finally, there is the offset due to moving from the 0 to 1 state within a 
 * particular operation (I or X). This depends on both the values of the ctrl 
 * and targ qubit numbers as follows:
 * 
 * offset = 2^(N-ctrl-targ) * y
 * 
 * where N is the number of qubits (3 in the above case). Here, y is either 
 * zero or one, and enumerates the operations that must be performed In other 
 * words, the index is given by the following expression
 * 
 * i:   root + offset       = x*2^ctrl + y*2^(N-ctrl-targ)
 * j:   root + sep + offset = x*2^ctrl + 2^targ + y*2^(N-ctrl-targ)
 * 
 * where x is the value of the ctrl qubit (do X when x is 1, I when x is zero)
 * and y ranges from 0 to 2^(N-1) where N is the number of qubits. Since it is
 * only necessary to do the non-trivial unitary, x is always 1.
 * 
 */
void controlled_qubit_op_new(const Complex op[2][2], int ctrl, int targ, Complex state[]) {
    ///@todo Replace pow2 with left rotations
    int root = pow2(ctrl); // Base of indices
    int sep = pow2(targ); // Separation between 0 and 1 target positions
    int increment = pow2(NUM_QUBITS - ctrl - targ); // Increment between 
                                                    // adjacent mat muls
    int step_max = STATE_LENGTH - sep - root; // Limit to step
    // Perform the matrix multiplications
    for (int step = 0; step < step_max; step += increment) {
        int a = 0;
        a++;
        mat_mul(op, state, root + step, root + sep + step);
    }
}

/// Old controlled qubit operations
void controlled_qubit_op(const Complex op[2][2], int ctrl, int targ, Complex state[]) {
    int root_max = pow2(targ); // Declared outside the loop
    int increment = 2 * root_max;
    /// ROOT loop: starts at 0, increases in steps of 1
    for (int root = 0; root < root_max; root++) {
        /// STEP loop: starts at 0, increases in steps of 2^(k+1)
        for (int step = 0; step < STATE_LENGTH; step += increment) {
            /// First index is ZERO, second index is ONE
            /// @note for 2 qubit case check if the index in the ctrl qubit 
            /// is a 1 then apply the 2x2 unitary else do nothing
            ///
            /// @note sorry.
            /// this checks for the first element of the state vector i.e. the target 
            /// qubits |0> and checks that the state vector element is one which the 
            /// control qubit has a |1> state -> (root + step)
            ///
            /// The second element of the state vector to take is then the first
            /// +2^(target qubit number). This also needs to be checked that the control
            /// qubit is in the |1>. 
            /// @todo This expression can probably be simplified or broken over lines.
            /// The condition for the if statement is that root+step and
            /// root + step + root_max contain 1 in the ctrl-th bit. 
            if( (((root+step) & (1 << ctrl)) && 
                    
                    ((root+step+root_max) & (1 << ctrl))) == 1){
                mat_mul(op, state, root + step, root + root_max + step);
            }
        }
    }
}


