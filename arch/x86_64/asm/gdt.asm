BITS 64

global gdt_load
global gdt_flush

section .text

; Takes the gdt_ptr stored into rdi and load it
gdt_load:
    lgdt [rdi]

; Flushes the gdt with a far jump
gdt_flush:
    mov rax, rsp
    push qword 0x10
    push rax
    pushf
    push qword 0x8
    push .reload_cs
    iretq

; Reload segment registers
.reload_cs:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    ret
