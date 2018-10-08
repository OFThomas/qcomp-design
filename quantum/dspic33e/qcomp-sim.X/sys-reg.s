/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
* File Name: sys-reg.s
*
* Type: SOURCE
*
* Title: System Registers
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

.include "p33EP512MU810.inc" 
    
; ===================== REGISTER DECLARATIONS ========================
    
.global SYS.REG.STAT
.global SYS.REG.CallIndex
.global SYS.REG.REQUEST_FIFO
.global SYS.REG.REQUEST_BUFFER.pointer
.global SYS.REG.REQUEST_BUFFER.service
.global SYS.REG.REQUEST_BUFFER.gap
    
.global SYS.w0
.global SYS.w1
.global SYS.w2
.global SYS.w3
.global SYS.w4
.global SYS.w5
.global SYS.w6
.global SYS.w7
.global SYS.w8
.global SYS.w9
.global SYS.w10
.global SYS.w11
.global SYS.w12
.global SYS.w13
.global SYS.w14
.global SYS.w15
    
.global SYS.REG.EventCounter
    
; =============================== DATA ===============================

.section SYS.REG, data, near
SYS.REG.STAT:   .word 0x0000
; This register contains flags which request actions from the
; system routine.
.equiv sys_request,      0   ;   Bit 0:  1 if a request is pending
                             ;           0 if no request is pending
.equiv sys_usb_hold,     1   ;   Bit 1:  1 if USB module hold active
                             ;           0 if no USB hold
.equiv sys_can_hold,     2   ;   Bit 2:  1 if CAN module hold active
                             ;           0 if no CAN hold
.equiv sys_spi_hold,     3   ;   Bit 3:  1 if SPI module hold active
                             ;           0 if no SPI hold
.equiv sys_wf10_cl_hold, 4   ;   Bit 4:  1 if WF10 active : com loop
                             ;           0 if no hold
.equiv sys_wf10_ml_hold, 5   ;   Bit 5:  1 if WF10 active : motor loop
                             ;           0 if no hold
.equiv sys_wf10_pl_hold, 6   ;   Bit 6:  1 if WF10 active : pres. loop
                             ;           0 if no hold
.equiv sys_can_ip_hold,  7   ;   Bit 7:  1 if adding a setpoint
                             ;           0 if no hold
.equiv sys_i2c_hold,     8   ;   Bit 8:  1 if I2C is running
                             ;           0 if no hold

SYS.REG.CallIndex: .word 0x0000
;

.section SYS.REG.REQUEST_BUFFER, data, near, align(16)
SYS.REG.REQUEST_FIFO:          .space 16
; Circular buffer for requests to the system management routine.
; The buffer is aligned for easy pointer reset to the start of the
; buffer

.section SYS.REG.REQUEST_BUFFER_REGISTERS, data, near
SYS.REG.REQUEST_BUFFER.pointer:.word .startof.(SYS.REG.REQUEST_BUFFER)
; Pointer to the first available space in the FIFO request buffer.
; For use by peripheral systems when loading requests.

SYS.REG.REQUEST_BUFFER.service:.word .startof.(SYS.REG.REQUEST_BUFFER)
; Pointer to the next request in the buffer which should be serviced
; For use by the system management routine when processing requests.

SYS.REG.REQUEST_BUFFER.gap: .word 0x0000
; Holds the gap between the two pointers.

.section SYS.REG.COUNTER, data, near
.word 0x0000
; Counts the number of system management calls since start up. 
; Counts up to 0xFFFF then resets

; --------------------------------------------------------------------
; Working register backup
; --------------------------------------------------------------------
; These are used by the system interrupt service routine to perform a
; backup of the working registers before responding to the interrupt
; event. The working registers are restored to their original
; state at the end of the interrupt service routine
SYS.w0:     .word 0x0000
SYS.w1:     .word 0x0000
SYS.w2:     .word 0x0000
SYS.w3:     .word 0x0000
SYS.w4:     .word 0x0000
SYS.w5:     .word 0x0000
SYS.w6:     .word 0x0000
SYS.w7:     .word 0x0000
SYS.w8:     .word 0x0000
SYS.w9:     .word 0x0000
SYS.w10:    .word 0x0000
SYS.w11:    .word 0x0000
SYS.w12:    .word 0x0000
SYS.w13:    .word 0x0000
SYS.w14:    .word 0x0000
SYS.w15:    .word 0x0000

; --------------------------------------------------------------------
; Working register backup
; --------------------------------------------------------------------
; Debug registers

SYS.REG.EventCounter: .word 0x0000








