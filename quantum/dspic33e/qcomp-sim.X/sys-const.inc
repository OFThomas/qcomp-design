/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * File Name: sys-const.inc
 *
 * Type: Definition
 *
 * Title: Constant symbol definitions
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

; ======================= SYS.REG.STAT Register Layout =========================
			     
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
