/**
 * @file algo.h
 *
 * @brief header file for algorithms
 */
#define NUM_QUBITS 4
#define STATE_LENGTH 16 // 2^NUM_QUBITS


void gate(Complex op[2][2], int qubit, Complex state[], int num_qubits);
