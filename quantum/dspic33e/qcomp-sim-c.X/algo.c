/**
 * @file algo.c
 * @brief Contains quantum algorithms to be run
 *
 */

#include "io.h"
#include "quantum.h"
#include "algo.h"
#include "time.h"
#include <math.h>


/// @brief 
void gate(Complex op[2][2], int qubit, Complex state[], int num_qubits){
    /// does 2x2 operator on state vector
    /// displays the average state of the qubit by tracing over all 
    /// waits to let the user see the state (LEDs)
    single_qubit_op(op, qubit, state, num_qubits);
    qubit_display(state, num_qubits);
    delay();
}

void two_gate(Complex op[2][2], int ctrl, int targ, Complex state[], int num_qubits){
    /// does controlled 2x2 operator 
    controlled_qubit_op(op, ctrl, targ, state, num_qubits);
    qubit_display(state, num_qubits);
    delay();
}

/// repetition code
//
