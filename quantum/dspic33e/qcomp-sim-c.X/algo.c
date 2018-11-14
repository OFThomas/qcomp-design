/**
 * @file algo.c
 * @brief Contains quantum algorithms to be run
 *
 */

#include "io.h"
#include "quantum.h"
#include "algo.h"

/// initialise to vacuum
void qubit_test_3(CVector3 state){
for(int i=0; i<=8; i++){
    for(int j=0; j<=2; j++){
        state[i][j] = 0.0;
    }
}
state[0][0] = 0.9999694824;
}

/// get state amplitudes
void qubit_display_3(CVector3 state){
    Q15 zero_amp;
    Q15 one_amp;
    int index;
    int n_max;
    int j_max;

    // qubit 0 to 2
    for(int i=0; i <= 2; i++){
        zero_amp = 0;
        one_amp = 0;
        /// loop over n, 2^(current qubit)
        n_max = pow(2,i);
        
        for(int n = 0; n < n_max; n++){
            /// 2^(total qbits - current) 
            j_max = pow(2,2-i);
    
            /// loop over j
            for(int j=0; j <= j_max; j++){
                /// n + j * 2^(i+1)
                index = n + (pow(2,i+1) * j);
                /// zeros n 
                zero_amp += sq(state[index][0]);    
                /// ones index are always n+1 for zero amps 
                one_amp += sq(state[index + 1][0]);
            }
        }
    /// update leds for each qubits average zer0 and one amps
    set_external_led(i, zero_amp, one_amp, 0);
    }
}

/// apply operator
/// @param qubit qubit number to apply 2x2 matrix to
/// @param op 2x2 operator 
/// @param state state vector containing amplitudes 
void qubit_op_3(int qubit, CMatrix2 op, CVector3 state) {

Q15 temp1 = 0;
Q15 temp2 = 0;

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

int i;
int j;

i=0;
j=1;

mat_mul_cmplx(op, state, i, j);

}


/// repetition code
//
