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

/// perform single qubit gate 
void gate(const Complex op[2][2], int qubit, Complex state[]);

/// perform controlled single qubit gate 
void two_gate(const Complex op[2][2], int ctrl, int targ, Complex state[]);

/// swap using 3 cNots
void swap(int q1, int q2, Complex state[]);

/// from tests.c
void swap_test(Complex state[]);

/// Toffoli gate
void toffoli_gate(int q1, int q2, int q3, Complex state[]);
    
void toffoli_test(Complex state[]);
#ifdef	__cplusplus
}
#endif

#endif	/* ALG_H */

