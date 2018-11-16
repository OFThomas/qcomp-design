/**
 * @file algo.h
 *
 * @brief header file for algorithms
 */
#define NUM_QUBITS 4
#define STATE_LENGTH 16 // 2^NUM_QUBITS

/// perform single qubit gate 
void gate(Complex op[2][2], int qubit, Complex state[], int num_qubits);

/// perform controlled single qubit gate 
void two_gate(Complex op[2][2], int ctrl, int targ, Complex state[], int num_qubits);

/// swap using 3 cNots
void swap(int q1, int q2, Complex state[], int num_qubits);