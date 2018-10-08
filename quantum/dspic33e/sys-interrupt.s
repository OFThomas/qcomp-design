/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
* File Name: sys-interrupt.s
*
* Type: SOURCE
*
* Title: USB Interrupt Service Routine
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

.include "p33EP512MU810.inc"  
.include "sys.inc"    
    
; ========================== DECLARATIONS ============================
.global __T1Interrupt


.text
; =============================== CODE ===============================


; --------------------------------------------------------------------
; Timer 1 Interrupt Service Routine
; --------------------------------------------------------------------
; The function of this timer is to call the system management routine
; at regular intervals. The system management routine is used to
; control program execution. Peripheral systems running on the
; microcontroller can submit requests to the management routine
; by writing to system registers. The management routine reads these
; registers each time it is called and accordingly redirects
; program execution to handle the requests. This procedure allows
; the control of the device from external sources (such as the USB
; link to a Windows application.)


__T1Interrupt:

    ; Back up working registers
    MOV     w0, SYS.w0
    MOV     w1, SYS.w1
    MOV     w2, SYS.w2
    MOV     w3, SYS.w3
    MOV     w4, SYS.w4
    MOV     w5, SYS.w5
    MOV     w6, SYS.w6
    MOV     w7, SYS.w7
    MOV     w8, SYS.w8
    MOV     w9, SYS.w9
    MOV     w10, SYS.w10
    MOV     w11, SYS.w11
    MOV     w12, SYS.w12
    MOV     w13, SYS.w13
    MOV     w14, SYS.w14
    MOV     w15, SYS.w15

    ; Call the system management routine
    ;BTG     LATD, #red    ; toggle red led
    ; Call the system management routine
    RCALL   SYS.SUB.MANAGE
    ; Enable user interrupts (may not have been disabled)
    ;BCLR    CORCON, #IPL3   ; CPU interrupt greater than 7
    ; Clear the interrupt flag
    BCLR    IFS0, #T1IF

    ; Restore working registers
    MOV     SYS.w0, w0
    MOV     SYS.w1, w1
    MOV     SYS.w2, w2
    MOV     SYS.w3, w3
    MOV     SYS.w4, w4
    MOV     SYS.w5, w5
    MOV     SYS.w6, w6
    MOV     SYS.w7, w7
    MOV     SYS.w8, w8
    MOV     SYS.w9, w9
    MOV     SYS.w10, w10
    MOV     SYS.w11, w11
    MOV     SYS.w12, w12
    MOV     SYS.w13, w13
    MOV     SYS.w14, w14
    MOV     SYS.w15, w15

    ; Return from interrupt
    RETFIE

