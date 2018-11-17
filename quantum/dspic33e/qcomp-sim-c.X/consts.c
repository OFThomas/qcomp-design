/// @file consts.c 
/// @brief contains all (global) constants)

#include "consts.h"

/// @param rX sqrt X gate
/// \verbatim 
///( 0.5+0.5i  0.5-0.5i )
///( 0.5-0.5i  0.5+0.5i )
/// \endverbatim
Complex rX[2][2] = {{ {0.5, 0.5}, {0.5, -0.5} }, 
                    { {0.5, -0.5}, {0.5, 0.5} }};

/// @param X pauli X gate
Complex X[2][2] = {{ {0.0, 0.0}, {ONE_Q15, 0,0} },
                    {{ONE_Q15, 0.0},{0.0, 0.0}} };

/// @param Y Pauli y gate
Complex Y[2][2] = {{ {0.0, 0.0}, {0.0, -1,0} },
                   { {0.0, ONE_Q15}, {0.0, 0.0} }};

/// @param Z Pauli z gate
Complex Z[2][2] = {{ {ONE_Q15, 0.0}, {0.0, 0,0} },
                   { {0.0, 0.0}, {-1.0, 0.0} }};

/// @param H Hadamard gate
Complex H[2][2] = {{ {0.7071067812, 0.0}, {0.7071067812, 0,0} },
                   { {0.7071067812, 0.0}, {-0.7071067812, 0.0} }};

