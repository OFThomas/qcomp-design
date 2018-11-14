/**
 * @file algo.c
 * @brief Contains quantum algorithms to be run
 *
 */

#include "io.h"
#include "quantum.h"
#include "algo.h"

void 3qubit_test(CVector3 state){
/// initialise to vacuum
for(int i=0; i<=8; i++){
    for(int j=0; j<=2; j++){
        state[i][j] = 0.0;
    }
}
state[0][0] = 0.9999694824;
}

/// apply operator 

/// repetition code
//
