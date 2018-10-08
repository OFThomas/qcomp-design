/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
* File Name: sys-debug.s
*
* Type: SOURCE
*
* Title: System debugging
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

.include "p33EP512MU810.inc"
    
.text
; --------------------------------------------------------------------
; Application Errors
; --------------------------------------------------------------------

SysAppError1:  BRA     SysAppError1
; This error occurs if the system routine attempts to access an
; unimplemeted entry in the execution jump table
