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
 */
void display_average(Complex state[]) {
    /// @todo Bring all constants out of the loops. Don't use pow.
    /// @bug there is a phase bug when cycling the gates 
    /// Loop over all qubits k = 0, 1, 2, ... N-1
    for (int k = 0; k < NUM_QUBITS; k ++) {
        /// Compute powers of 2
        int root_max = pow2(k);
        int increment = 2 * root_max;
        Q15 zero_amp = 0, one_amp = 0;
        Q15 phase = 0;
        Q15 tmp1 = 0;
        Q15 tmp2 = 0;
        int c = 0;
        /// ROOT loop: starts at 0, increases in steps of 1
        for(int root = 0; root < root_max; root ++) {
            /// STEP loop: starts at 0, increases in steps of 2^(k+1)
            for(int step = 0; step < STATE_LENGTH; step += increment) {

                /// @bug The problem is with the sign function, which will
                /// not distinguish between (e.g.) 1 and i. I think all the 
                /// problems stem from that kind of error.
                /// c = sign(state[root + step]) - sign(state[root + root_max + step]);
                
                
                /// Compute two temporary variables to check real and imaj signs
                tmp1 = state[root+step][0]*state[root + root_max + step][0];
                tmp2 = state[root+step][1]*state[root + root_max + step][1];
                c = 0;
                /// Set c = 1 if there is a phase difference in either r or i
                
                if(tmp1 < -0.01) c = 1;
                if(tmp2 < -0.01) c = 1;
                /*
                // Deal with zero cases
                if (state[root + step][0] == 0.0 ||
                        state[root + root_max + step][0] == 0.0) {
                    if (state[root + step][0] < -0.001 ||
                            state[root + root_max + step][0] < -0.01) {
                        c = 1; /// Treat 0 and -ve as a difference in phase
                    }
                }
                if (state[root + step][1] == 0.0 ||
                        state[root + root_max + step][1] == 0.0) {
                    if (state[root + step][1] < -0.001 ||
                            state[root + root_max + step][1] < -0.01) {
                        c = 1; /// Treat 0 and -ve as a difference in phase
                    }
                }
                */
                ///if(c==1 || c==3 || c==2)
                if(c==1) phase += FULL_PHASE;
                
                /// Zeros are at the index root + step
                /// @todo Rewrite pow for Q15 
                zero_amp += square_magnitude(state[root + step]);
                /// Ones are at the index root + 2^k + step
                one_amp += square_magnitude(state[root + root_max + step]);
                

            }
        }
        /// write phase
        /// update leds for each qubits average zero and one amps
        one_amp = 0.2 * one_amp;
        zero_amp = 0.2 * zero_amp;
        if (phase != 0.0) phase = 0.99;
        else phase = 0.0;
        set_external_led(k, phase, one_amp, zero_amp);
    }
}

/**
 * @param state The state to display
 * @param N The length of the state vector
 */
void display_cycle(Complex state[]) {
    
    int output[STATE_LENGTH];
    
    /// Filter the state
    int cycle_length = remove_zero_amp_states(state, output);
    
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
int remove_zero_amp_states(Complex state[], int disp_state[]) {
    int count = 0;
    for (int i = 0; i < STATE_LENGTH; i++) {
        if (absolute(state[i]) > 0.0) {
            disp_state[count] = i;
            count++;
        }
    }
    return count;
}
