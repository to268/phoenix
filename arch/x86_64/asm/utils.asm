BITS 64

global halt_cpu
global load_page_directory_ptr
global enable_pae
global enable_paging

section .text

; Infinite loop of hlt instructions
halt_cpu:
    hlt
    jmp halt_cpu

load_page_directory_ptr:
    push rbp
    mov rbp, rsp
    mov rax, [rsp+8]
    mov cr3, rax
    mov rsp, rbp
    pop rbp
    ret

enable_pae:
    mov rax, cr4
    or rax, 1 << 5
    mov cr4, rax
    ret


enable_paging:
    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    mov rax, cr0
    or rax, 1 << 31
    mov cr0, rax
    ret
