/**
 * @file algo.c
 * @brief Contains quantum algorithms to be run
 * @authors J Scott, O Thomas
 * @date Nov 2018
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

void swap_test(Complex state[]) {

    zero_state(state); // Set the state to the vacuum
    display_average(state); // Display the state for four qubits
    delay();

    gate(H, 0, state);
    gate(X, 2, state);

    /// swap for ever!
    while (1) {
        swap(0, 1, state);
        swap(1, 2, state);
        swap(2, 3, state);
        swap(3, 0, state);
    }
}

/** @todo Fancy non-blocking Interrupt routine
{
   if(no button) return;
   
   pause and do display cycling();
 * 
 * Make this a low priority interrupt so that everything else can interrupt 
 * it.
 * 
 * Do stuff for a while
 * 
 * return when you're done.
    
}
 * 
 * */


/// @brief Toffoli gate 
/// \verbatim
/// -o--   ------o-----o--o-----
/// -|--   ------|-----|--|-----
/// -o-- = ---o--X--o--X--|-----
/// -|--   ---|-----|-----|-----
/// -X--   --rX----rX*---rX-----
///          a   b  c  d  e
/// \endverbatim
/// q1 ctrl 1
/// q2 ctrl 2
/// q3 target
void toffoli_gate(int q1, int q2, int q3, Complex state[]){

    controlled_qubit_op(rX, q2, q3, state);     /// a
    controlled_qubit_op(X, q1, q2, state);      /// b 
    controlled_qubit_op(rXT, q2, q3, state);    /// c
    controlled_qubit_op(X, q1, q2, state);      /// d
    controlled_qubit_op(rX, q1, q3, state);     /// e
    display_average(state);
}

void toffoli_test(Complex state[]){

    while(1){
    zero_state(state);
    display_average(state);

    gate(H, 0, state);
    toffoli_gate(0,1,3, state);

    gate(H, 0, state);
    gate(H, 1, state);
    toffoli_gate(0,1,3,state);

    gate(H,0,state);
    toffoli_gate(0,1,3,state);
    }

}

/// repetition code
/// @todo this
