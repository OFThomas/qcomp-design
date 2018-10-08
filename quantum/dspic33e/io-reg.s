/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
* File Name: io-reg.s
*
* Type: DATA
*
* Title: IO Registers
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

.include "p33EP512MU810.inc"
    
; ===================== REGISTER DECLARATIONS ========================
    
.global IO.REG.button_state
    
; --------------------------------------------------------------------
; Push button state
; --------------------------------------------------------------------
.section IO.REG.BUTTON_STATE, data, near
IO.REG.button_state:        .word 0x0000
; Stores the current push button state




