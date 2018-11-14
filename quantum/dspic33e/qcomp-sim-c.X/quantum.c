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
void zero_state(Complex state[], int N) {
    for (int i = 0; i < N; i++) {
        // Loop over the real and imaginary parts
        for (int j = 0; j < 2; j++) {
            state[i][j] = 0.0;
        }
    }
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
 */
void qubit_display(Complex state[], int N) {
    Q15 zero_amp;
    Q15 one_amp;
    int index;
    int n_max;
    int j_max;

    /// qubit 0, 1, 2, ... N-1
    for (int i = 0; i < N; i++) {
        zero_amp = 0;
        one_amp = 0;
        /// loop over n, 2^(current qubit)
        n_max = pow(2, i);

        /// Loop here for each contribution to the zero and one amplitude
        for (int n = 0; n < n_max; n++) {
            /// 2^(total qbits - 1 - current) 
            j_max = pow(2, N-1-i);

            /// loop over j
            for (int j = 0; j < j_max; j++) {
                /// n + j * 2^(i+1)
                index = n + (pow(2, i + 1) * j);
                /// zeros n 
                zero_amp += pow(state[index][0],2);
                /// ones index are always n+1 for zero amps 
                one_amp += pow(state[index + n_max][0],2);
            }
        }
        /// update leds for each qubits average zer0 and one amps
        set_external_led(i, zero_amp, 0, one_amp);
    }
}

/** apply operator
 * @param state state vector containing amplitudes 
 * @param k qubit number to apply 2x2 matrix to
 * @param N total number of qubits in the state
 * @param op 2x2 operator to be applied
 */
void single_qubit_op(Complex state[], int k, int N, Complex op[2][2]) {

    //Q15 temp1 = 0;
    //Q15 temp2 = 0;

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

    int n_max;
    int j_max;
    int index;

    // qubit zero entries are adjacent 2^0
    // qubit 1 entries are 2^1 apart etc...

    /// loop over n, 2^(current qubit)
    n_max = pow(2, k);

    /// Loop here for each contribution to the zero and one amplitude
    for (int n = 0; n < n_max; n++) {
        /// 2^(total qbits - 1 - current) 
        j_max = pow(2, N - 1 - k);

        /// loop over j
        for (int j = 0; j < j_max; j++) {
            /// n + j * 2^(k+1)
            index = n + (j * pow(2, k + 1));
            mat_mul(op, state, index, index + n_max);
        }
    }
}
