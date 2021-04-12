BITS 64

global halt_cpu
global enable_pae

section .text

; Infinite loop of hlt instructions
halt_cpu:
    hlt
    jmp halt_cpu

; Enable the pae flags
enable_pae:
    mov rax, cr4
    or rax, 1 << 5
    mov cr4, rax
    ret
