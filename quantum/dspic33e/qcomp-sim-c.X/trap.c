/**
 * @file trap.c
 * 
 * @brief Description: Catch all the hardware traps and exceptions
 * @authors J Scott, O Thomas
 * @date Dec 2018
 *
 */

#include "xc.h"

void trap_enable(void) { 
}

void __attribute__((__interrupt__, no_auto_psv)) _OscillatorFail(void) {
    while(1);
}

void __attribute__((__interrupt__, no_auto_psv)) _AddressError(void) {
    while(1);
}

void __attribute__((__interrupt__, no_auto_psv)) _HardTrapError(void) {
    while(1);
}

void __attribute__((__interrupt__, no_auto_psv)) _StackError(void) {
    while(1);
}

void __attribute__((__interrupt__, no_auto_psv)) _MathError(void) {
    while(1);
}

void __attribute__((__interrupt__, no_auto_psv)) _DMACError(void) {
    while(1);
}

void __attribute__((__interrupt__, no_auto_psv)) _SoftTrapError(void) {
    while(1);
}
