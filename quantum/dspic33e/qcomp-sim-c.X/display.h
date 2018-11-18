/**
 * @file display.h
 * 
 * @brief Description: Header file containing all the functions for displaying the qubits
 * state vector.
 */


#ifndef DISPLAY_H
#define	DISPLAY_H

#ifdef	__cplusplus
extern "C" {
#endif


#include "p33EP512MU810.h"
#include "xc.h"
#include <math.h>
#include <stdbool.h>
#include "consts.h" 
#include "quantum.h"

       /**
     * @brief Display the state amplitudes on LEDs
     * @param state Pass in the state vector
     * @param Qnum The total number of qubits
     * 
     * @note Currently the function only displays superpositions using the
     * red and blue colors.
     * @todo rename to display_average
     */
    void display_average(Complex state[], int Qnum);
    
    /// @brief cycles through the non-zero amplitude states
    void display_cycle(Complex state[], int N);

    /// @brief updates disp_state where the first 'return value of the function'elements
    /// are the nonzero elements of the state vector 'state'
    /// @param state complex state vector in
    /// @param num_qubits int number of qubits in
    /// @param disp_state complex inout vector where the first n entries are the nonzero
    /// elements of 'state'
    /// @return returns the number of elements to look at in disp_state.
    int remove_zero_amp_states(Complex state[], int num_qubits, int disp_state[]);

    /// @todo this
    int sort_states(Complex state[], int num_qubits);

#ifdef	__cplusplus
}
#endif

#endif	/* DISPLAY_H */

