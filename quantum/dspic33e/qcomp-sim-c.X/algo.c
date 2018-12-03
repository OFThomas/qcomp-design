/**
 * @file algo.c
 * @brief Contains quantum algorithms to be run
 * @authors J Scott, O Thomas
 * @date Nov 2018
 */
#include "algo.h"

/// gate routine
/// \todo not sure if the breaks are needed here, I don't think they are.
void op_routine(int select_qubit, int select_op, Complex state[]){
    int targ=0;
    switch(select_op) {
        case 0:
            // X
            gate_display(Z, select_qubit, state);
            break;
        case 1:
            // Z
            gate_display(X, select_qubit, state);
            break;
        case 2:
            // H
            gate_display(H, select_qubit, state);
            break;
        case 3:
            // CNOT
            /// wait for target qubit to be selected
            targ = check_qubit();
            two_gate_display(X, select_qubit, targ, state);
            break;
        case 4:
            // repetition_code example
            repetition_code(select_qubit, state);
            break;
        default:
            break; ///Do nothing   
    } /// End of switch
}

// Check whether a qubit has been selected
int check_qubit(){
    int c = 0;
    // clear previous val of select_qubit
    int select_qubit=-1;
    /// \bug this probably shouldn't be an infinite loop.
    /// the counter lets the loop exit after some time to check if the 
    /// 'reset' button is pressed 
    while((select_qubit == -1) && (c<10000000)){
        // Read all the button state
        read_external_buttons();
        c++;
        // check if any of the qubits are selected
        for (int n = 0; n < NUM_QUBITS; n++) {
            if (read_qubit_btn(n) == 1) {
                select_qubit = n;
            }
        }
    }
return select_qubit;
} /// End of qubit select 


// Check whether a qubit has been selected
int check_op(){
    int c = 0;
    int select_op=-1;
    /// \todo this is a temp fix to avoid getting stuck waiting for a user input.
    while( (select_op == -1) && (c<10000000)){
        read_external_buttons();
        c++;
        for (int n = 0; n < 4; n++) {
            if (read_func_btn(n) == 1) {
                select_op = n;
            }
        }
    }
return select_op;
}

/// @brief single qubit gate 
void gate(const Complex op[2][2], int qubit, Complex state[]){
    /// does 2x2 operator on state vector
    single_qubit_op(op, qubit, state);
}

/// @brief single qubit gate with display  
void gate_display(const Complex op[2][2], int qubit, Complex state[]){
    /// does 2x2 operator on state vector
    /// displays the average state of the qubit by tracing over all 
    /// waits to let the user see the state (LEDs)
    single_qubit_op(op, qubit, state);
    display_average(state);
    delay();
}

/// @brief two-qubit gate 
void two_gate(const Complex op[2][2], int ctrl, int targ, Complex state[]){
    /// does controlled 2x2 operator 
    controlled_qubit_op(op, ctrl, targ, state);
}

/// @brief two-qubit gate with display
void two_gate_display(const Complex op[2][2], int ctrl, int targ, Complex state[]){
    /// does controlled 2x2 operator 
    /// displays the state 
    /// waits to let the user see the state 
    controlled_qubit_op(op, ctrl, targ, state);
    display_average(state);
    delay();
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

    
    gate(X, 0, state);
    gate(H, 0, state);
    gate(X, 2, state);

    /// swap for ever!
    while (1) {
        swap(0, 1, state);
        delay();
        swap(1, 2, state);
        delay();
        swap(2, 3, state);
        delay();
        swap(3, 0, state);
        delay();
    }
}

/// QFT
/// \verbatim
/// H Rz Rz --------
/// ---o--|---H Rz---
/// ------o------o-H-
/// \endverbatim

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

    controlled_qubit_op(rX, q2, q3, state);     ///< a
    controlled_qubit_op(X, q1, q2, state);      ///< b 
    controlled_qubit_op(rXT, q2, q3, state);    ///< c
    controlled_qubit_op(X, q1, q2, state);      ///< d
    controlled_qubit_op(rX, q1, q3, state);     ///< e
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
/// \verbatim
///       a  b     c     d  e  f 
/// |q2> ----X--|-----|-----X--o--
///          |  |     |     |  |
/// |q1> -X--|--|  U  |--X--|--o--
///       |  |  |     |  |  |  |
/// |q0> -o--o--|-----|--o--o--X-- |q0>
/// \endverbatim
void repetition_code(int q0, Complex state[]){
    int q1;
    int q2;
    
    /// check position of 'q0' in the state, if the first qubit 
    /// then q1, q2 are the next 2. e.g. q0=0 -> q1=1, q2=2
    if(q0 == 0){
        q1=1;
        q2=2;
    }
    /// check if q0 is the last qubit then wrap so q1 is (q0-1)
    /// q2 is (q0-2)
    else if (q0 == (NUM_QUBITS-1)){
        q1=q0-1;
        q2=q0-2;
    }
    /// else have q0 in the middle of q1 & q0
    else {
        q1=q0-1;
        q2=q0+1;
    }
    /// do the gates, without displaying the intermediate steps?
    two_gate_display(X,q0,q1, state);   /// step a
    two_gate_display(X,q0,q2, state);   /// step b
    
    /// now up to U step c
    /// as a test I have hardcoded in only 2 X's corresponding to an error on q2
    /// @todo generalise this, we should either have pseudo-random errors
    /// or the user should be able to choose which gates to do here
    gate_display(X,q0,state);
    gate_display(X,q1, state);

    /// decoding steps d-f
    two_gate_display(X,q0, q1, state);  /// step d
    two_gate_display(X,q0, q2, state);  /// step e
    toffoli_gate(q1, q2, q0, state);    /// step f
/// done!
}
