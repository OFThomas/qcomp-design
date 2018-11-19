/**
 * @file algo.c
 * @brief Contains quantum algorithms to be run
 *
 */

#include "algo.h"

/// @brief 
void gate(const Complex op[2][2], int qubit, Complex state[]){
    /// does 2x2 operator on state vector
    /// displays the average state of the qubit by tracing over all 
    /// waits to let the user see the state (LEDs)
    single_qubit_op(op, qubit, state);
    display_average(state);
}

void two_gate(const Complex op[2][2], int ctrl, int targ, Complex state[]){
    /// does controlled 2x2 operator 
    controlled_qubit_op(op, ctrl, targ, state);
    display_average(state);
}

void swap(int q1, int q2, Complex state[]){
   
    controlled_qubit_op(X, q1, q2, state);
    controlled_qubit_op(X, q2, q1, state);
    controlled_qubit_op(X, q1, q2, state);
    display_average(state);
}

void swap_test(Complex state[]){

    zero_state(state); // Set the state to the vacuum
    display_average(state); // Display the state for four qubits
    delay();
     
    gate(H, 0, state);
    gate(X, 2, state);

    /// swap for ever!
    while(1){
    swap(0,1, state);
    swap(1,2, state);
    swap(2,3, state);
    swap(3,0, state);
    }
}
///
void toffoli_gate(Complex state[]){



}

/// repetition code
/// @todo this
