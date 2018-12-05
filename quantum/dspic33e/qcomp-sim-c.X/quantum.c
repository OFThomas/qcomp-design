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
    return (1 << k);
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
    if(a[0] < 0.0) {
        // Both real and imag part negative
        if(a[1] < 0.0) return 2;
        // Real part negative, imaginary part non-negative
        else return 1; 
        // Real part non-negative, imaginary part negative
    } else if(a[1] < 0.0) return 3;
    // Both real and imaginary parts non-negative
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
 * @brief New function to perform single qubit gates
 * @param op the unitary to perform
 * @param k the index of the qubit to modify
 * @param state the state vector
 * 
 * The function computes a single qubit operation acting on @param state.
 * It sets the bit position associated with the qubit to zero, and then 
 * generates all possible indices in the other bit positions. These 
 * correspond to the ZERO states. The ONE states are obtained by changing
 * the kth bit from zero to one.
 * 
 */
void single_qubit_op_new(const Complex op[2][2], int k, Complex state[]) {
	int bit = (1 << k); // The bit position corresponding to the kth qubit
	int high_incr = (bit << 1); 
	// Increment through the indices above bit
	for(int i=0; i<STATE_LENGTH; i+=high_incr) {
		// Increment through the indices less than bit
		for(int j=0; j<bit; j++) {
			// 2x2 matrix multiplication on the zero (i+j)
			// and one (i+j+bit) indices
			mat_mul(op, state, i+j, i+j+bit);
		}
	}
}


/**
 * @brief Efficient controlled qubit operation
 * @param op the operation (ctrl-op is performed)
 * @param ctrl the index of the ctrl qubit
 * @param targ the index of the targ qubit
 * @param state the state vector
 * 
 * This function is implemented similarly to the single qubit case above.
 * Now there are three ranges of indices to increment through, separated by
 * the two qubit indices. 
 * 
 */
void controlled_qubit_op_new(const Complex op[2][2], int ctrl, int targ, Complex state[]) {
    int small_bit, large_bit;
    if(ctrl > targ) {
        small_bit = (1 << targ);
        large_bit = (1 << ctrl);
    } else {
        small_bit = (1 << ctrl);
        large_bit = (1 << targ);
    }
    int mid_incr = (small_bit << 1);
    int high_incr = (large_bit << 1);
    int targ_bit = (1 << targ);

	// Increment through the indices above largest bit (ctrl or targ)
	for(int i=0; i<STATE_LENGTH; i+=high_incr) {
		// Increment through the middle set of bits
		for(int j=0; j<large_bit; j+=mid_incr) {
            // Increment through the low set of bits
            for(int k=0; j<small_bit; j++) {
                // 2x2 matrix multiplication on the zero (i+j+k)
                // and one (i+j+k+targ_bit) indices. 
                mat_mul(op, state, i+j+k, i+j+k+targ_bit);
            }
		}
	}
}

/// Old controlled qubit operations
void controlled_qubit_op(const Complex op[2][2], int ctrl, int targ, Complex state[]) {
    int root_max = pow2(targ); // Declared outside the loop
    /// \bug this needs to be a long int for >16 qubits
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


