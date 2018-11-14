/**
 * @file algo.h
 *
 * @brief header file for algorithms
 */

void qubit_test_3(CVector3 state); 

/// @param qubit qubit number to apply 2x2 matrix to
/// @param op 2x2 operator 
/// @param state state vector containing amplitudes 
void qubit_op_3(int qubit, CMatrix2 op, CVector3 state);

void qubit_display_3(CVector3 state);


