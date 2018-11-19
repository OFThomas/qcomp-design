/**
 * @file algo.c
 * @brief Contains quantum algorithms to be run
 *
 */

#include "algo.h"

/// @brief 
void gate(const Complex op[2][2], int qubit, Complex state[], int num_qubits){
    /// does 2x2 operator on state vector
    /// displays the average state of the qubit by tracing over all 
    /// waits to let the user see the state (LEDs)
    single_qubit_op(op, qubit, state, num_qubits);
    display_average(state, num_qubits);
}

void two_gate(const Complex op[2][2], int ctrl, int targ, Complex state[], int num_qubits){
    /// does controlled 2x2 operator 
    controlled_qubit_op(op, ctrl, targ, state, num_qubits);
    display_average(state, num_qubits);
}

void swap(int q1, int q2, Complex state[], int num_qubits){
   
    controlled_qubit_op(X, q1, q2, state, num_qubits);
    controlled_qubit_op(X, q2, q1, state, num_qubits);
    controlled_qubit_op(X, q1, q2, state, num_qubits);
    display_average(state, num_qubits);
}

void swap_test(Complex state[]){

    zero_state(state, NUM_QUBITS); // Set the state to the vacuum
    display_average(state, NUM_QUBITS); // Display the state for four qubits
    delay();
     
    gate(H, 0, state, NUM_QUBITS);
    gate(X, 2, state, NUM_QUBITS);

    /// swap for ever!
    while(1){
    swap(0,1, state, NUM_QUBITS);
    swap(1,2, state, NUM_QUBITS);
    swap(2,3, state, NUM_QUBITS);
    swap(3,0, state, NUM_QUBITS);
    }
}
///
void toffoli_gate(Complex state[]){



}

/// repetition code
/// @todo this
