/// @file consts.c 
/// @brief contains all (global) constants)
/// @authors J Scott, O Thomas
/// @date Nov 2018

#include "consts.h"

/// @param rX sqrt X gate
/// ( 0.5+0.5i  0.5-0.5i )
/// ( 0.5-0.5i  0.5+0.5i )
const Complex rX[2][2] = {{{0.5, 0.5},{0.5, -0.5}},
                          {{0.5, -0.5},{0.5, 0.5}}};

/// @param rXT Adjoint of rX
const Complex rXT[2][2] = {{{0.5, -0.5},{0.5, 0.5}},
                          {{0.5, 0.5},{0.5, -0.5}}};

/// @param X pauli X gate
const Complex X[2][2] = {{{0.0, 0.0},{ONE_Q15, 0.0}},
                         {{ONE_Q15, 0.0},{0.0, 0.0}}};

/// @param Y Pauli y gate
const Complex Y[2][2] = {{{0.0, 0.0}, {0.0, -1.0}},
                         {{0.0, ONE_Q15}, {0.0, 0.0}}};

/// @param Z Pauli z gate
const Complex Z[2][2] = {{{ONE_Q15, 0.0}, {0.0, 0.0}},
                         {{0.0, 0.0}, {-1.0, 0.0}}};

/// @param H Hadamard gate
const Complex H[2][2] = {{{0.7071067812, 0.0}, {0.7071067812, 0.0}},
                         {{0.7071067812, 0.0}, {-0.7071067812, 0.0}}};

