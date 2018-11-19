/// @file display.c 
/// for all the state display functions

#include "display.h"

/**
 * @brief Display the state amplitudes on LEDs
 * @param state Pass in the state vector
 * 
 * @note Currently the function only displays superpositions using the
 * red and blue colors.
 * 
 * The routine works by adding up the squares of the amplitudes corresponding
 * to each state of a given qubit. Suppose there are three qubits. Then the
 * state vector is given by
 * 
 * \verbatim
 *      index     binary   amplitude 
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
 * is an edge condition when k = N-1. There, j acycle_lengthpparently ranges from 0 to -1.
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
 */
void display_average(Complex state[]) {
    ///@todo Bring all constants out of the loops. Don't use pow.
    /// Loop over all qubits k = 0, 1, 2, ... N-1
    for (int k = 0; k < NUM_QUBITS; k ++) {
        /// Compute powers of 2
        int root_max = pow2(k);
        int increment = 2 * root_max;
        Q15 zero_amp = 0, one_amp = 0;
        /// ROOT loop: starts at 0, increases in steps of 1
        for(int root = 0; root < root_max; root ++) {
            /// STEP loop: starts at 0, increases in steps of 2^(k+1)
            for(int step = 0; step < STATE_LENGTH; step += increment) {
                /// Zeros are at the index root + step
                /// @todo Rewrite pow for Q15 
                zero_amp += square_magnitude(state[root + step]);
                /// Ones are at the index root + 2^k + step
                one_amp += square_magnitude(state[root + root_max + step]);
            }
        }
        /// update leds for each qubits average zero and one amps
        set_external_led(k, 0, zero_amp, one_amp);
    }
}

/**
 * @param state The state to display
 * @param N The length of the state vector
 */
void display_cycle(Complex state[]) {
    
    int output[STATE_LENGTH];
    
    /// Filter the state
    int cycle_length = remove_zero_amp_states(state, NUM_QUBITS, output);
    
    /// Allocate RGB array
    RGB colors[cycle_length][NUM_QUBITS];
    
    /// Decode 
    for(int k=0; k < cycle_length; k++) {
        for(int j=0; j < NUM_QUBITS; j++) {
            colors[k][j].R = 0;
            colors[k][j].G = 0;
            colors[k][j].B = 0;
            /// Look at the jth bit
            if((output[k] & (1 << j)) == 0) {
                colors[k][j].G = ONE_Q15;
            } else {
                colors[k][j].B = ONE_Q15;
            }
        }
    }
    /// Reset the cycle
    reset_cycle();

    for (int k = 0; k < cycle_length; k++) {
        /// Each iteration of this loop writes 

        /// Loop here to add stuff
        add_to_cycle(colors[k], NUM_QUBITS);
    }

}

/**
 * 
 * @param state The state vector
 * @param num_qubits The number of qubits in the state vector
 * @return 
 * 
 * This function finds the amplitude of the state vector with the largest
 * magnitude. 
 * 
 */
#define NUM_MAX_AMPS 4 /// Define the number of largest amplitudes to store
int sort_states(Complex state[], int num_qubits){
    /// @todo this function...
    return 0;
}

/// @brief takes state vector, number of qubits and vector to write the nonzero elements
/// of the statevector to.
/// the disp_state elements are the nonzero elements of the state 
/// \verbatim 
/// e.g. state =  (00) = (1/r2) (Bell state) 
///               (01)   ( 0  ) 
///               (10)   ( 0  ) 
///               (11)   (1/r2) 
/// Then displ_state would have 2 elements 
/// disp_state = (0) standing for (00) 
///              (3)              (11) 
/// \endverbatim 
/// @note we have to allocate disp_state to be the size of state, the function returns 
/// count which tells us the first 'count' elements of disp_state to use.
/// In the Bell state example there are 2 values in disp_state, 0 & 3, count is returned
/// as 3 which means take the first count-1 elements (in this case 2) of disp_state which 
/// is 0,1 which is the correct elements
int remove_zero_amp_states(Complex state[], int num_qubits, int disp_state[]) {
    int N = pow(2, num_qubits);
    int count = 0;
    for (int i = 0; i < N; i++) {
        if (absolute(state[i]) > 0.0) {
            disp_state[count] = i;
            count++;
        }
    }
    return count;
}
