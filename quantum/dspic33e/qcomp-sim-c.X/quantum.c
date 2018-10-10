/*
 * File: quantum.c
 * 
 * Description: Contains matrix and vector arithmetic for simulating 
 * one qubit.
 * 
 */

#include "quantum.h"

// 2x2 matrix multiplication
Vector mat_mul(Matrix M, Vector V) {
  Vector W = {0, 0}; // To store the output
  W.a1 = M.a11 * V.a1 + M.a12 * V.a2;
  W.a2 = M.a21 * V.a1 + M.a22 * V.a2;
  return W;
}

// Add a global phase to make first amplitude positive
int fix_phase(Vector *V) {
  signed _Fract phase = -1.0;
  if ((*V).a1 < 0.0) {
    (*V).a1 *= phase;
    (*V).a2 *= phase;
  }
  return 0;
}