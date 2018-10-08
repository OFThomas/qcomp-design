/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
* File Name: io.s
*
* Type: SOURCE
*
* Title: Input/Output Configuration
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

.include "p33EP512MU810.inc"
.include "io-const.inc"
    
; ==================== SUBROUTINE DECLARATIONS =======================
.global IO.SUB.CONFIG
    
.global IO.SUB.green_on
.global IO.SUB.green_off
.global IO.SUB.amber_on
.global IO.SUB.amber_off
.global IO.SUB.red_on
.global IO.SUB.red_off
    
.global IO.SUB.green_flash
.global IO.SUB.amber_flash
.global IO.SUB.red_flash
.global IO.SUB.amber_and_green_flash
    
.global IO.SUB.UPDATE_BUTTON_STATE 
    
.text
; =============================== CODE ===============================

; --------------------------------------------------------------------
; IO Configuration
; --------------------------------------------------------------------
; The following routine sets up the relevant lines on port D to
; accept input from the push buttons and control the LEDs on the
; USB starter kit.
; --------------------------------------------------------------------

IO.SUB.CONFIG:

    ; Choose Port D as a digital port
    CLR     ANSELD
    ; Choose LED lines as outputs
    BCLR    TRISD, #red
    BCLR    TRISD, #amber
    BCLR    TRISD, #green
    ; Choose push button lines are inputs
    BSET    TRISD, #SW1
    BSET    TRISD, #SW2
    BSET    TRISD, #SW3
    ; Return to caller
    RETURN

                                                      ; End of routine
; --------------------------------------------------------------------



; --------------------------------------------------------------------
; Notifications
; --------------------------------------------------------------------

IO.SUB.green_on:
    BSET    LATD, #green
    RETURN
IO.SUB.green_off:
    BCLR    LATD, #green
    RETURN
    
IO.SUB.amber_on:
    BSET    LATD, #amber
    RETURN
IO.SUB.amber_off:
    BCLR    LATD, #amber
    RETURN
	
IO.SUB.red_on:
    BSET    LATD, #red
    RETURN
IO.SUB.red_off:
    BCLR    LATD, #red
    RETURN
						      
IO.SUB.green_flash:
    ; Routine to blink green LED three times
    DO      #2, 0f
    BSET    LATD, #green
    DO      #1000, 1f
    REPEAT  #3000
    NOP
1:  NOP
    BCLR    LATD, #green
    DO      #3000, 1f
    REPEAT  #3000
    NOP
1:  NOP
0:  NOP

    ; Return to caller
    RETURN

IO.SUB.amber_flash:

    ; Routine to blink green LED three times
    DO      #2, 0f
    BSET    LATD, #amber
    DO      #1000, 1f
    REPEAT  #3000
    NOP
1:  NOP
    BCLR    LATD, #amber
    DO      #3000, 1f
    REPEAT  #3000
    NOP
1:  NOP
0:  NOP

    ; Return to caller
    RETURN

IO.SUB.red_flash:

    ; Routine to blink green LED three times
    DO      #2, 0f
    BSET    LATD, #red
    DO      #1000, 1f
    REPEAT  #3000
    NOP
1:  NOP
    BCLR    LATD, #red
    DO      #3000, 1f
    REPEAT  #3000
    NOP
1:  NOP
0:  NOP

    ; Return to caller
    RETURN

IO.SUB.amber_and_green_flash:

    ; Routine to blink amber and green LEDs three times
    DO      #2, 0f
    BSET    LATD, #amber
    BSET    LATD, #green
    DO      #1000, 1f
    REPEAT  #3000
    NOP
1:  NOP
    BCLR    LATD, #amber
    BCLR    LATD, #green
    DO      #3000, 1f
    REPEAT  #3000
    NOP
1:  NOP
0:  NOP

    ; Return to caller
    RETURN


                                                      ; End of routine
; --------------------------------------------------------------------



; --------------------------------------------------------------------
; Button state
; --------------------------------------------------------------------
; Call before reading the IO.REG.button_state register.
						      
IO.SUB.UPDATE_BUTTON_STATE:
    CLR	    IO.REG.button_state
    BTSC    PORTD, #SW1
    BSET    IO.REG.button_state, #0
    BTSC    PORTD, #SW2
    BSET    IO.REG.button_state, #1
    BTSC    PORTD, #SW3
    BSET    IO.REG.button_state, #2
    RETURN						      
						      
                                                      ; End of routine
; --------------------------------------------------------------------
