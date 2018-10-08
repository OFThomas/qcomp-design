/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
* File Name: sys.s
*
* Type: SOURCE
*
* Title: System Execution Control
*
* Purpose: This routine is called periodically after the device has
*          started. It is used to redirect execution between the
*          available routines on the device. The routine used a system
*          of registers to allow other processes to request changes to
*          program execution. When this routine is called it reads
*          the registers and makes a decision about whether to
*          modify program execution. The routine modifies program
*          execution by writing to the program counter and executing
*          a return instruction.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    
; ======================== INCLUDE FILES =============================

.include "p33EP512MU810.inc"
.include "sys-const.inc"
.include "sys-debug-options.inc"

; ==================== SUBROUTINE DECLARATIONS =======================
.global SYS.SUB.CONFIG
.global SYS.SUB.MANAGE
    
.global SYS.SUB.pointer_increment
.global SYS.SUB.service_increment

.text
; ============================= CODE =================================


; --------------------------------------------------------------------
; System Initial Configuration Routine
; --------------------------------------------------------------------
; This routine should be called from _main. It performs basic
; operations like clock configuration and setting up the system
; routines for subsequently controlling the operation of the
; processor.
; --------------------------------------------------------------------

;.section SYS.SUB.CONFIG, code
SYS.SUB.CONFIG:
    ; Use Primary Oscillator with PLL --------------------------------
    ; Prepare PLL settings for clock switch - New clock (Fosc) = 50MHz
    MOV     #0x0030, w0
    MOV     w0, PLLFBD      ; M = 50
    MOV     #0x3040, w0
    MOV     w0, CLKDIV      ; N1 = 2; N2 = 4
    ; Place 0b011 in W0 (to write 'Primary Clock with PLL' to FNOSC)
    MOV     #0x3, w0
    ; OSCCONH Unlock Sequence
    MOV     #OSCCONH, w1    ; Store address of OSCCONH in w1
    MOV     #0x78, w2       ; Prepare for unlock
    MOV     #0x9A, w3
    MOV.B   w2, [w1]        ; unlock OSCCONH for writing
    MOV.B   w3, [w1]        ;   (refer to 'Oscillator Module' FRM)
    ; Set New Oscillator Selection - Primary Clock with PLL
    MOV.B   w0, [w1]
    ; OSCCONL Unlock Sequence
    MOV     #OSCCONL, w1    ; Store address of OSCCONL in w1
    MOV     #0x46, w2       ; Prepare for unlock
    MOV     #0x57, w3
    MOV.B   w2, [w1]        ; unlock OSCCONL for writing
    MOV.B   w3, [w1]        ;   (refer to 'Oscillator Module' FRM)
    ; Enable Clock Switch
    BSET    OSCCON, #0      ; Set OSWEN bit to request clock switch
0:  BTSC    OSCCONL, #OSWEN
    BRA     0b

/*   THIS MUST BE ENABLED FOR NORMAL OPERATION

    ; Enable timer 1 for use by system -------------------------------
    MOV     #0x0030, w0
    MOV     w0, T1CON       ; Timer 1 used in 16 bit mode
    MOV     #0x00FF, w0
    MOV     w0, PR1         ; Write the period
    ; Enable interrupts from timer 1.
    MOV     #0x4444, w0
    MOV     w0, IPC0        ; Write priority level
    BSET    IEC0, #T1IE     ; Turn on interrupts
    ; Turn on timer 1
    BSET    T1CON, #TON     ; Turn on timer

*/

    ; Return to caller -----------------------------------------------
    RETURN

                                                      ; End of routine
; --------------------------------------------------------------------


; --------------------------------------------------------------------
; System Management
; --------------------------------------------------------------------
; This routine is called at regular intervals throughout program
; execution to control the operation of the processor. The caller is
; the interrupt service of timer 1.
;
; Any peripheral system can make a request to run a particular
; routine. This existence of the request is indicated by flagging
; the sys_request bit in the SYS.REG.STAT register. The actual
; request is loaded into a FIFO circular buffer. Peripheral systems
; should use the SYS.REG.REQUEST_FIFO.pointer registers which
; contains the address of the first available space in the buffer.
;
; Some processes must not be interrupted by this management routine.
; If a process is underway that must not be interrupted a system
; hold bit is flagged. If this routine sees any system hold flag it
; will not modify program execution even if a request is pending.
; The process which caused the hold condition will clear the hold flag
; when the process has finished, which allows this routine to
; commence with processing the request.
;
; --------------------------------------------------------------------

;   .section SYS.SUB.MANAGE, code
SYS.SUB.MANAGE:
    ; Test if the system is held by a peripheral system.
    ; ---------------------------------------------------------------
    ; NOTE:
    ; It is very important that this check is performed first,
    ; otherwise this routine may currupt working registers while
    ; they are being used by another process.
    ; ---------------------------------------------------------------
0:  BTSC    SYS.REG.STAT, #sys_usb_hold
    RETURN    ; USB system is holding the execution

    ; Raise the CPU priority to stop interrupts
    ;BSET    CORCON, #IPL3   ; CPU interrupt greater than 7

    ; Check for requests
    MOV     SYS.REG.REQUEST_BUFFER.gap, w0
    MOV     #0, w1
    CPSNE   w0, w1  ; Skip if the gap is non-zero
    RETURN    ; Leave routine if no requests are pending
    ; Read the requested action from the FIFO buffer
    MOV     SYS.REG.REQUEST_BUFFER.service, w0
    MOV     [w0], w3        ; w3 contains request code
    ; Increment the service pointer
    RCALL   SYS.SUB.service_increment
    ; w3 now contains jump table offset, aligned for program memory
    MOV     #.startof.(SYS.SUB.JumpTable), w0
    ADD     w0, w3, w3
    ; Point to the timer 1 interrupt software stack entry
    SUB     w15, #8, w15
    ; Read and save SFA bit
    BTSC    [w15], #0       ; Test SFA bit
    BSET    w3, #0          ; Modify lower 2 bytes accordingly
    ; Copy the status register and IPL bit
    MOV     [w15+2], w0
    MOV     #0xFF80, w1
    AND     w0, w1, w0      ; upper byte of program word is zero
    ; Update the software stack
    PUSH     w3
    PUSH     w0
    ; Point at the entry for this routine in preparation for return
    ADD     w15, #4, w15
.ifdecl SYS.OPTION.DEBUG.SwitchOnCounter
    ; Count the number of times the routine has been called
    MOV     SYS.REG.EventCounter, w0
    INC     w0, w0
    MOV     w0, SYS.REG.EventCounter
    ; Check whether limit has been
    MOV     #SYS.OPTION.DEBUG.EventCounterLimit, w1
    CPSNE   w0, w1
    ; Put a break on the next line
    NOP
    NOP
    NOP
.endif

    ; Execute return. When the timer 1 interrupt service routine
    ; executes its RETFIE the CPU will execute the requested
    ; routine.
    RETURN


                                                      ; End of routine
; --------------------------------------------------------------------

; --------------------------------------------------------------------
; Routines for incrementing the buffer pointer
; --------------------------------------------------------------------
; These routines are called to modify the FIFO request buffer pointers
; after either a request has been written or a request has been
; serviced.
; --------------------------------------------------------------------

SYS.SUB.pointer_increment:

    PUSH.S
    ; Increment the request pointer, or reset to start of FIFO
    MOV     SYS.REG.REQUEST_BUFFER.pointer, w0
    INC2    w0, w0
    AND     #0x000F, w0
    MOV     #.startof.(SYS.REG.REQUEST_BUFFER), w1
    ADD     w0, w1, w0
    ; Load new service pointer into the register.
    MOV     w0, SYS.REG.REQUEST_BUFFER.pointer
    ; Increment the pointer gap
    MOV     SYS.REG.REQUEST_BUFFER.gap, w0
    INC     w0, w0
    MOV     w0, SYS.REG.REQUEST_BUFFER.gap
    POP.S
    ; Return to caller
    RETURN


SYS.SUB.service_increment:

    PUSH.S
    ; Increment the service pointer, or reset to start of FIFO
    MOV     SYS.REG.REQUEST_BUFFER.service, w0
    INC2    w0, w0
    AND     #0x000F, w0
    MOV     #.startof.(SYS.REG.REQUEST_BUFFER), w1
    ADD     w0, w1, w0
    ; Load new service pointer into the register.
    MOV     w0, SYS.REG.REQUEST_BUFFER.service
    ; Decrement the pointer gap
    MOV     SYS.REG.REQUEST_BUFFER.gap, w0
    DEC     w0, w0
    MOV     w0, SYS.REG.REQUEST_BUFFER.gap
    POP.S
    ; Return to caller
    RETURN






                                                      ; End of routine
; --------------------------------------------------------------------





; --------------------------------------------------------------------
; System Management Jump Table
; --------------------------------------------------------------------
; For use by the system management routine

.section SYS.SUB.JumpTable, code

    BRA     IO.SUB.green_on      ; Example function here





