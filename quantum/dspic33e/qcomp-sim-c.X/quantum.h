/*
 * File: quantum.h
 * 
 * Description: Header file containing all the matrix arithmetic for 
 * simulating a single qubit.
 * 
 */


#ifndef QUANTUM_H
#define	QUANTUM_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "p33EP512MU810.h"
#include "xc.h"
  
    // Basis states
    typedef enum {ZERO, ONE, PLUS, MINUS} State;
    
    // Basic fractional time
    typedef signed _Fract Q15; 
    
    // Complex type
    typedef Q15 Complex[2];
    
    // Matrix4 type
    typedef Q15 Matrix4[4][4];
    typedef Q15 CMatrix4[4][4][2];
  
    // Matrix2 type
    typedef Q15 Matrix2[2][2];
    typedef Q15 CMatrix2[2][2][2];

    // Vector type
    typedef Q15 Vector[2];
    typedef Q15 CVector[2][2];

    // Create complex X, Y, Z and H
    void make_ops(Matrix2 X, Matrix2 Z, Matrix2 H);
    void make_ops_cmplx(CMatrix2 X, CMatrix2 Y, CMatrix2 Z, CMatrix2 H);
    
    // Initialise a real state vector
    void init_state(Vector V, State s);
    
    // Initialise a complex state vector
    void init_state_cmplx(CVector V, State s);
  
    // 2x2 matrix multiplication
    void mat_mul(Matrix2 M, Vector V);

    // Add a global phase to make first amplitude positive
    void fix_phase(Vector V);
    
    // Clean the state: return the closest state out of
    // 0>, |1>, |+> and |->
    void clean_state(Vector V);
    
    // Show the qubit state on the LEDs
    void show_state(Vector V);
    
#ifdef	__cplusplus
}
#endif

#endif	/* QUANTUM_H */

