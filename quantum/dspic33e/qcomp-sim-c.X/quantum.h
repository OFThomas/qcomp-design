/*
 * File: quantum.h
 * 
 * Description: Header file containing all the matric arithmetic for 
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

    // Matrix type
    typedef struct {
        signed _Fract a11;
        signed _Fract a12;
        signed _Fract a21;
        signed _Fract a22;
    } Matrix;

    // Vector type
    typedef struct {
        signed _Fract a1;
        signed _Fract a2;
    } Vector;

    // 2x2 matrix multiplication
    Vector mat_mul(Matrix M, Vector V);

#ifdef	__cplusplus
}
#endif

#endif	/* QUANTUM_H */

