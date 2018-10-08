/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
* File Name: io.inc
*
* Type: External Declaration
*
* Title: Subroutine and register declarations 
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

; =================== SUBROUTINE DECLARATIONS ==========================

.extern IO.SUB.CONFIG
    
.extern IO.SUB.green_on
.extern IO.SUB.green_off
.extern IO.SUB.amber_on
.extern IO.SUB.amber_off
.extern IO.SUB.red_on
.extern IO.SUB.red_off
    
.extern IO.SUB.green_flash
.extern IO.SUB.amber_flash
.extern IO.SUB.red_flash
.extern IO.SUB.amber_and_green_flash
    
.extern IO.SUB.UPDATE_BUTTON_STATE 
    
; ===================== REGISTER DECLARATIONS ========================
    
.extern IO.REG.button_state
    