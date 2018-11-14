/**
 * @file quantum.c
 * 
 * @brief Description: Contains matrix and vector arithmetic for simulating 
 * one qubit.
 * 
 */

#include "io.h"                             
#include "quantum.h"

// Complex addition
void cadd(Complex a, Complex b, Complex result) {
    result[0] = a[0] + b[0];
    result[1] = a[1] + b[1];
}

// Complex multiplication
void cmul(Complex a, Complex b, Complex result) {
    result[0] = a[0] * b[0] - a[1] * b[1];
    result[1] = a[0] * b[1] + a[1] * b[0]; 
}

// Create complex X, Y, Z and H
void make_ops(Complex X[2][2], Complex Y[2][2], 
        Complex Z[2][2], Complex H[2][2]) {
    // Assume the elements are all equal to zero
    X[0][1][0] = 0.9999694824; // X
    X[1][0][0] = 0.9999694824;
    
    Y[0][1][1] = -1.0; // Y
    Y[1][0][1] = 0.9999694824;
    
    Z[0][0][0] = 0.9999694824; // Z
    Z[1][1][0] = -1.0;

    H[0][0][0] = 0.7071067812; // H
    H[0][1][0] = 0.7071067812;
    H[1][0][0] = 0.7071067812;
    H[1][1][0] = -0.7071067812;
}

/// Initialise state to the vacuum (zero apart from the first position)
/// Specify the dimension -- of the matrix, i.e. 2^(number of qubits)
void zero_state(Complex state[], int Qnum) {
    int N = pow(2, Qnum);
    for (int i = 0; i < N; i++) {

        // Loop over the real and imaginary parts
        for (int j = 0; j < 2; j++) {
            state[i][j] = 0.0;
        }
    }
    /// @note oh the clarity! 
    state[0][0] = ONE_Q15;
}

// 2x2 complex matrix multiplication
void mat_mul(Complex M[2][2], Complex V[], int i, int j) {
    Complex a, b, c, d;
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
 * @brief Display the state amplitudes on LEDs
 * @param state Pass in the state vector
 * @param N The total number of qubits
 * 
 * @note Currently the function only displays superpositions using the
 * red and blue colors.
 * 
 * The routine works by adding up the squares of the amplitudes corresponding
 * to each state of a given qubit. Suppose there are three qubits. Then the
 * state vector is given by
 * 
 *      index     binary   amplitude 
 *      ----------------------------- 
 *        0       0 0 0       a_0
 *        1       0 0 1       a_1 
 *        2       0 1 0       a_2
 *        3       0 1 1       a_3
 *        4       1 0 0       a_4
 *        5       1 0 1       a_5
 *        6       1 1 0       a_6
 *        7       1 1 1       a_7
 *      -----------------------------
 *      Qubit:    2 1 0
 * 
 * Consider qubit 2. The value of the ZERO state is formed by adding up all
 * the amplitudes corresponding to its ZERO state. That is, indices 0, 1, 2 
 * and 3. The ONE state is obtained by adding up the other indices: 4, 5, 6 and 
 * 7. 
 * 
 * So the amplitudes for qubit 2 are
 * 
 * ZERO: (a_0)^2 + (a_1)^2 + (a_2)^2 + (a_3)^2
 * ONE:  (a_4)^2 + (a_5)^2 + (a_6)^2 + (a_7)^2
 * 
 * Corresponding to the following indices:
 * 
 * ZERO: 0+0, 1+0, 2+0, 3+0
 * ONE:  4+0, 5+0, 6+0, 7+0
 * 
 * For qubit 1 the indices are:
 * 
 * ZERO: 0+0, 0+4, 1+0, 1+4
 * ONE:  2+0, 2+4, 3+0, 3+4
 * 
 * And for qubit 0 the indices are:
 * 
 * ZERO: 0+0, 0+2, 0+4, 0+6
 * ONE:  1+0, 1+2, 1+4, 1+6
 * 
 * The examples above are supposed to show the general pattern. For N qubits,
 * qubit number k, the ZERO and ONE states are given by summing all the square
 * amplitudes corresponding to the following indices:
 * 
 * ZERO: n + (2^(k+1) * j), where n = 0, 1, ..., 2^k - 1 
 *                          and   j = 0, 1, ..., 2^(N-k-2)
 * 
 * ONE:  n + (2^(k+1) * j), where n = 2^k, 2^k + 1, ..., 2^(k+1) - 1  
 *                          and   j = 0, 1, ..., 2^(N-k-2)
 * 
 * The amplitudes are obtained by summing over both n and j. Notice that there
 * is an edge condition when k = N-1. There, j apparently ranges from 0 to -1.
 * In this case, the only value of j is 0. The condition arises because of the
 * way that 2^(N-k-2) is obtained (i.e. such that multiplying it by 2^(k+1) 
 * gives 2^(N-1).) However, if k = N-1, then 2^(k+1) = 2^N already, so it must
 * be multiplied by 2^(-1). The key point is that the second term should not
 * ever equal 2^N, so j should stop at 0. 
 * 
 * The above indices can be expressed as the sum of a ROOT and a STEP as 
 * follows:
 * 
 * index = ROOT + STEP
 * 
 * where ROOT ranges from 0 to 2^k-1. This corresponds to the n values that
 * give rise to ZERO. The indices for ONE can be obtained by adding 2^k to root.
 * The STEP = j is a multiple of 2^(k+1) starting from zero that does not equal 
 * or exceed 2^N. ROOT can be realised using the following for loop:
 * 
 * for(int root = 0; root \< 2^k; root ++) {
 *      ...
 *      // ZERO index
 *      root;
 *      // ONE index
 *      root + 2^k; 
 * } 
 * 
 * Then the STEP component can be realised as 
 * 
 * for(int step = 0; step \< 2^N; step += 2^(k+1)) {
 *      // Add the following to root...
 *      step;
 * }
 * 
 * 
 */
void qubit_display(Complex state[], int N) {
    /// Loop over all qubits 0, 1, 2, ... N-1
    for (int k = 0; k < N; k ++) {
        Q15 zero_amp = 0;
        Q15 one_amp = 0;
        /// ROOT loop
        for(int root = 0; root < pow(2,k); root ++) {
            /// STEP loop
            for(int step = 0; step < pow(2,N); step += pow(2,k+1)) {
                /// Zeros are at the index root + step
                zero_amp += pow(state[root + step][0],2);
                /// Ones are at the index root + 2^k + step
                one_amp += pow(state[root + (int)pow(2,k) + step][0],2);
            }
        }
        /// update leds for each qubits average zer0 and one amps
        set_external_led(k, zero_amp, 0, one_amp);
    }
}

/** apply operator
 * @param state state vector containing amplitudes 
 * @param qubit qubit number to apply 2x2 matrix to
 * @param Qnum total number of qubits in the state
 * @param op 2x2 operator to be applied
 */
void single_qubit_op(Complex op[2][2], int qubit, Complex state[], int Qnum) {

    // do row 1 of op onto all pairs of state vectors
    // e.g. qubit 0, pairs of values are
    // 000, 001
    // then
    // 010, 011
    // etc.
    //
    // op   ( a b ) * ( 000 ) = (temp1)
    //      ( c d )   ( 001 )   (temp2)
    // temp1 = a(000) + b(001) -> new (000) val
    // temp2 = c(000) + d(001) -> new (001) val

    // qubit zero entries are adjacent 2^0
    // qubit 1 entries are 2^1 apart etc...

    int index;
    int n_max = pow(2, qubit); // loop over n, 2^(current qubit)
    int j_max = pow(2, Qnum - qubit - 1); /// 2^(total qbits -1 - current) 
    /// Loop here for each contribution to the zero and one amplitude
    for (int n = 0; n < n_max; n++) {
        /// loop over j
        for (int j = 0; j < j_max; j++) {
            /// n + j * 2^(k+1)
            index = n + (j * pow(2, qubit + 1));
            mat_mul(op, state, index, index + n_max);
        }
    }
}
