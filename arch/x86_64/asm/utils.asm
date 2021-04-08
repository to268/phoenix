BITS 64

global halt_cpu

section .text

; Infinite loop of hlt instructions
halt_cpu:
    hlt
    jmp halt_cpu
