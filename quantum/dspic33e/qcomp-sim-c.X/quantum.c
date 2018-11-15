/**
 * @file quantum.c
 * 
 * @brief Description: Contains matrix and vector arithmetic for simulating 
 * one qubit.
 * 
 */

#include "io.h"                             
#include "quantum.h"
#include "time.h"

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
    
    /// @note IMPLICIT NONE!!!
    for(int i=0; i < 2; i++){
        for(int j=0; j<2; j++){
            for(int k=0; k<2; k++){
                X[i][j][k] = 0.0;
                Y[i][j][k] = 0.0;
                Z[i][j][k] = 0.0;
                H[i][j][k] = 0.0;
            }
        }
    }  
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

void make_ops_4(Complex CNOT[4][4], Complex CPHASE[4][4], Complex SWAP[4][4]){

    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            for(int k=0; k<2; k++){
                CNOT[i][j][k] = 0.0;
                CPHASE[i][j][k] = 0.0;
                SWAP[i][j][k] = 0.0;
            }
        }
    }
    /// [][] = row, col
    // [2][3] is (0 0 0 0)
    //           (0 0 0 0)
    //           (0 0 0 1)
    //           (0 0 0 0)
    CNOT[0][0][0] = ONE_Q15;
    CNOT[1][1][0] = ONE_Q15;
    CNOT[2][3][0] = ONE_Q15;
    CNOT[3][2][0] = ONE_Q15;

    CPHASE[0][0][0] = ONE_Q15;
    CPHASE[1][1][0] = ONE_Q15;
    CPHASE[2][2][0] = ONE_Q15;
    CPHASE[3][3][0] = -1.0;

    SWAP[0][0][0] = ONE_Q15;
    SWAP[1][2][0] = ONE_Q15;
    SWAP[2][1][0] = ONE_Q15;
    SWAP[3][3][0] = ONE_Q15;
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

/// 4x4 matrix 
void mat_mul_4(Complex M[4][4], Complex V[], int i, int j, int k, int l){

    /// store results of each row multiplication 
    Complex temp_in[4];
    Complex temp_out[4];
   
    // initialise temp vars to zero 
    for(int q=0; q<4; q++){
        for(int r=0; r<2; r++){
            temp_in[q][r] = 0.0;
            temp_out[q][r] = 0.0;
        }
    }
    // ( a b c d )  * ( p )
    // ( e f g h )    ( q )
    // ( h i j k )    ( r ) 
    // ( l m n o )    ( s )

    // row 0,1,2,3 
    for(int row=0; row< 4; row++){
        /// 4 cols of V (i,h,k,l)
        cmul(M[row][0], V[i], temp_in[0]);
        cmul(M[row][1], V[j], temp_in[1]);
        cmul(M[row][2], V[k], temp_in[2]);
        cmul(M[row][3], V[l], temp_in[3]);
        
        /// add the 4 terms together and put into temp for that row
        for(int n=0; n<4; n++){
            cadd(temp_in[n], temp_out[row], temp_out[row]);
        }   
    }
    // loop over real and complex parts write sums of rows into V elements
    for(int c=0; c<2; c++){
        V[i][c] = temp_out[0][c];
        V[j][c] = temp_out[1][c];
        V[k][c] = temp_out[2][c];
        V[l][c] = temp_out[3][c];
    }
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
    /// Loop over all qubits k = 0, 1, 2, ... N-1
    for (int k = 0; k < N; k ++) {
        Q15 zero_amp = 0, one_amp = 0;
        /// ROOT loop: starts at 0, increases in steps of 1
        for(int root = 0; root < pow(2,k); root ++) {
            /// STEP loop: starts at 0, increases in steps of 2^(k+1)
            for(int step = 0; step < pow(2,N); step += pow(2,k+1)) {
                /// Zeros are at the index root + step
                zero_amp += pow(state[root + step][0],2);
                /// Ones are at the index root + 2^k + step
                one_amp += pow(state[root + (int)pow(2,k) + step][0],2);
            }
        }
        /// update leds for each qubits average zero and one amps
        set_external_led(k, 0,zero_amp, one_amp);
    }
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
 * 
 *      root + 2^k + step
 * 
 * corresponds to the ONE entry.
 * 
 */
void single_qubit_op(Complex op[2][2], int k, Complex state[], int N) {
    /// ROOT loop: starts at 0, increases in steps of 1
    for (int root = 0; root < pow(2, k); root++) {
        /// STEP loop: starts at 0, increases in steps of 2^(k+1)
        for (int step = 0; step < pow(2, N); step += pow(2, k+1)) {
            /// First index is ZERO, second index is ONE
            mat_mul(op, state, root + step, root + (int) pow(2, k) + step);
        }
    }
}

/// selective 2 qubit op function 
///    00 01 10 11
/// 00( 1  0  0  0   )
/// 01( 0  1  0  0   )
/// 10( 0  0 u00 u01 )
/// 11( 0  0 u10 u11 )
/// checks that the control qubit is |1> then does 2x2 unitary on remaining state vector
// elements
void controlled_qubit_op(Complex op[2][2], int ctrl, int targ, Complex state[], int N) {
    /// ROOT loop: starts at 0, increases in steps of 1
    for (int root = 0; root < pow(2, targ); root++) {
        /// STEP loop: starts at 0, increases in steps of 2^(k+1)
        for (int step = 0; step < pow(2, N); step += pow(2, targ+1)) {
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
            if( (((root+step) & (1 << ctrl)) && ((root+step+(int) pow(2,targ)) & (1 << ctrl))) == 1){
                mat_mul(op, state, root + step, root + (int) pow(2, targ) + step);
            }
        }
    }
}

/// @todo state sorting function.
/// searches the state vector for the largest amplitude states. Then returns the RGB
/// values for each of them to be displayed 
int sort_states(Complex state[], int num_qubits){
    // number of elements in state vect
    int N;
    N = pow(2,num_qubits); 
    // output
    int out_state[16];
    int count = 0;
    long int counter1;
    // max amp
    Q15 max_amp[16];

    // initialise output to 0    
    for(int m=0; m<N; m++){
        max_amp[m] = 0.0;
        out_state[m] = 0;
    }
    // Sorting happens?
    for(int j=0; j<N; j++){
        if(max_amp[count] <= state[j][0]){
            // update new maximum val
            max_amp[count+1] = state[j][0];
            // save pos of maximal val
            out_state[count+1] = j;
            count++;
        }

    }
    /// now have arrays of out_state and amplitudes of size count
    /// need to decode int of the out_state to binary to find each quits state

    while(1){
    // loop over all max vals of the state 
    for(int l=1; l<=count; l++){
        /// display the states of three qubits
        for(int k=0; k<4; k++){
<<<<<<< HEAD
            int zero_amp=0;
            int one_amp=0;
            
            one_amp=val_of_pos_bit(out_state[l],k);
            zero_amp=1-one_amp;
            if(one_amp > 0) zero_amp =0;
=======
            int one_amp=(out_state[l] & (1 << k));
            int zero_amp=1-one_amp;
>>>>>>> a0ab09ab3a321c9212a7ba0c55c3247569da879f
            set_external_led(k, 0,zero_amp, one_amp);
            
        }

        // let the user see
        /// @todo wait for John to fix the timer so the microprocessor isn't locked out
        // while displaying state 
        counter1 = 0;
        while(counter1 <= 1000000){
        counter1++;
        }
    }
    }

return 0;
<<<<<<< HEAD
}
=======
}

/// int to binary function
/// takes an int and a bit position
/// returns 1 or 0 in the bit position of the int
int val_of_pos_bit(int input, int pos){
    // i.e. the is a 1 in the 'pos' bit
    if( ((input) & (1 << pos)) == 1){
    return 1;
    }
    //int c=0;
   // while(c <2) c++;
    //else return 0;
   
}


>>>>>>> 3ae2680ca7c44fd8ba04fb642b5bee370240c9df
