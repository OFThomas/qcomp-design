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

/// @param rX is square root of X
/// \verbatim 
///( 0.5+0.5i  0.5-0.5i )
///( 0.5-0.5i  0.5+0.5i )
/// \endverbatim
Complex rX[2][2] = {{ {0.5, 0.5}, {0.5, -0.5} }, 
                    { {0.5, -0.5}, {0.5, 0.5} }};

/// @param X pauli X gate
Complex X[2][2] = {{ {0.0, 0.0}, {ONE_Q15, 0,0} },
                   { {ONE_Q15, 0.0}, {0.0, 0.0} }};

/// @param Y Pauli y gate
Complex Y[2][2] = {{ {0.0, 0.0}, {0.0, -1,0} },
                   { {0.0, ONE_Q15}, {0.0, 0.0} }};

/// @param Z Pauli z gate
Complex Z[2][2] = {{ {ONE_Q15, 0.0}, {0.0, 0,0} },
                   { {0.0, 0.0}, {-1.0, 0.0} }};

/// @param H Hadamard gate
Complex H[2][2] = {{ {0.7071067812, 0.0}, {0.7071067812, 0,0} },
                   { {0.7071067812, 0.0}, {-0.7071067812, 0.0} }};

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

void swap(int q1, int q2, Complex state[], int num_qubits){
    Complex X[2][2], Y[2][2], Z[2][2], H[2][2];
    make_ops(X, Y, Z, H);
    
    controlled_qubit_op(X, q1, q2, state, num_qubits);
    controlled_qubit_op(X, q2, q1, state, num_qubits);
    controlled_qubit_op(X, q1, q2, state, num_qubits);
    qubit_display(state, num_qubits);
    delay();
}

///
void toffoli_gate(Complex state[]){



}

/// repetition code
/// @todo this
