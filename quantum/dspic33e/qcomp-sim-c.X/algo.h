/**
 * @file algo.h
 *
 * @brief header file for algorithms
 * @authors J Scott, O Thomas
 * @date Nov 2018
 */

#ifndef ALG_H
#define	ALG_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "quantum.h"
#include "display.h"


/// functions for performing gate routines, takes qubit & button ints
int op_routine(int select_op, Complex state[]);


/// function returns the integer for the label of which qubit is selected
/// @returns int select_qubit (-1 if no qubit is selected)
/// \bug problem with sampling, will cause the program to hang while waiting
/// for qubit input
int check_qubit();

/// function returns integer label used in switch statement in main
/// \bug same as above^
int check_op();

/// perform single qubit gate 
void gate(const Complex op[2][2], int qubit, Complex state[]);

/// perform controlled single qubit gate 
void two_gate(const Complex op[2][2], int ctrl, int targ, Complex state[]);

/// Display gates!!!
void gate_display(const Complex op[2][2], int qubit, Complex state[]);
void two_gate_display(const Complex op[2][2], int ctrl, int targ, Complex state[]);

    
/// swap using 3 cNots
void swap(int q1, int q2, Complex state[]);

/// from tests.c
void swap_test(Complex state[]);

/// Toffoli gate
void toffoli_gate(int q1, int q2, int q3, Complex state[]);
    
void toffoli_test(Complex state[]);

/// added repetition_code for bit flip errors, currently only shows a fixed
/// error which is a failed X on one of the ancillas. 
/// todo.
void repetition_code(int q0, Complex state[]);


#ifdef	__cplusplus
}
#endif

#endif	/* ALG_H */

