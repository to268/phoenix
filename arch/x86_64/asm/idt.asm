; Copyright © 2021 Guillot Tony <tony.guillot@protonmail.com>
; This program is free software: you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation, either version 3 of the License, or
; (at your option) any later version.
;
; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU General Public License for more details.
;
; You should have received a copy of the GNU General Public License
; along with this program.  If not, see <http://www.gnu.org/licenses/>.
;
global isr_exception_stub_table
global keyboard_handler_irq
extern exception_handler

; Valid exception
%macro isr_exception_stub 1
isr_stub_%+%1:
    ; Pass the vector number
    mov rdi, %+%1
    call exception_handler
    iretq
%endmacro

; Invalid exception
%macro isr_none_stub 1
isr_stub_%+%1:
    iretq
%endmacro

; Exceptions
isr_exception_stub  0
isr_exception_stub  1
isr_exception_stub  2
isr_exception_stub  3
isr_exception_stub  4
isr_exception_stub  5
isr_exception_stub  6
isr_exception_stub  7
isr_exception_stub  8
isr_none_stub       9
isr_exception_stub  10
isr_exception_stub  11
isr_exception_stub  12
isr_exception_stub  13
isr_exception_stub  14
isr_exception_stub  15
isr_exception_stub  16
isr_exception_stub  17
isr_exception_stub  18
isr_exception_stub  19
isr_exception_stub  20
isr_none_stub       21
isr_none_stub       22
isr_none_stub       23
isr_none_stub       24
isr_none_stub       25
isr_none_stub       26
isr_none_stub       27
isr_none_stub       28
isr_none_stub       29
isr_exception_stub  30
isr_none_stub       31

; Table storing the address of all exception stubs
isr_exception_stub_table:
%assign i 0
%rep    32
    dq isr_stub_%+i
%assign i i+1
%endrep

; Keyboard handler IRQ stub
keyboard_handler_irq:
    ;pushaq
    ;cld
    ;call keyboard_handler
    ;popaq
    iretq
