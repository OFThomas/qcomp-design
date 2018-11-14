/**
 * @file algo.c
 * @brief Contains quantum algorithms to be run
 *
 */

#include "io.h"
#include "quantum.h"
#include "algo.h"
#include <math.h>




/// apply operator
/// @param qubit qubit number to apply 2x2 matrix to
/// @param op 2x2 operator 
/// @param state state vector containing amplitudes 
void qubit_op_3(int qubit, CMatrix2 op, CVector3 state) {

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
n_max = pow(2, qubit);

    /// Loop here for each contribution to the zero and one amplitude
    for (int n = 0; n < n_max; n++) {
        /// 2^(total qbits - current) 
        j_max = pow(2, 2-qubit);

        /// loop over j
        for (int j = 0; j < j_max; j++) {
            /// n + j * 2^(i+1)
            index = n + (pow(2, qubit + 1) * j);
            /// zeros n 
            //zero_amp += pow(state[index][0],2);
            /// ones index are always n+1 for zero amps 
            //one_amp += pow(state[index + n_max][0],2);
            mat_mul_cmplx(op, state, index, index + n_max);
        }
    }
}


/// repetition code
//
