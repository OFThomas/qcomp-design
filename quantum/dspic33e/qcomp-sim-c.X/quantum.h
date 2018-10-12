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

    // to add matrix4

    // Matrix2 type
    typedef struct {
        signed _Fract a11;
        signed _Fract a12;
        signed _Fract a21;
        signed _Fract a22;
    } Matrix2;

    // Vector type
    typedef struct {
        signed _Fract a1;
        signed _Fract a2;
    } Vector;

    // Create X, H and Z
    Matrix2 make_X();
    Matrix2 make_Z();
    Matrix2 make_H();
    
    // 2x2 matrix multiplication
    Vector mat_mul(Matrix2 M, Vector V);

    // Add a global phase to make first amplitude positive
    Vector fix_phase(Vector V);
    
    // Clean the state: return the closest state out of
    // 0>, |1>, |+> and |->
    Vector clean_state(Vector V);
    
    // Show the qubit state on the LEDs
    void show_state(Vector V);
    
#ifdef	__cplusplus
}
#endif

#endif	/* QUANTUM_H */

