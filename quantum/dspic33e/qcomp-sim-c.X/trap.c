/**
 * @file trap.c
 * 
 * @brief Description: Catch all the hardware traps and exceptions
 * @authors J Scott, O Thomas
 * @date Dec 2018
 *
 */

/**
 * @brief Ocsillator fail
 * 
 * This trap occurs when 
 */
void __attribute__((__interrupt__)) _OscillatorFail(void) {
    while(1);
}

void __attribute__((__interrupt__)) _AddressError(void) {
    while(1);
}

void __attribute__((__interrupt__)) _HardTrapError(void) {
    while(1);
}

void __attribute__((__interrupt__)) _StackError(void) {
    while(1);
}

void __attribute__((__interrupt__)) _MathError(void) {
    while(1);
}

void __attribute__((__interrupt__)) _DMACError(void) {
    while(1);
}

void __attribute__((__interrupt__)) _SoftTrapError(void) {
    while(1);
}
