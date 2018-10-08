/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
* File Name: sys-debug-options.inc
*
* Type: OPTION
*
* Title: System debugging options
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

; --------------------------------------------------------------------
; Call Counter
; --------------------------------------------------------------------
; The code can count the number of times the application changes the
; program execution. Uncomment the statement below to switch on the
; counter. The program will break when the routine reaches the counter
; limit (defined below).
; --------------------------- STATEMENT ------------------------------
.global SYS.OPTION.DEBUG.SwitchOnCounter
.equiv  SYS.OPTION.DEBUG.EventCounterLimit, 3
; --------------------------------------------------------------------



