; Copyright © 2022 Guillot Tony <tony.guillot@protonmail.com>
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
BITS 64

global gdt_flush

section .text

; Takes the gdt_ptr stored into rdi and load it
; Flushes the gdt with a far jump
gdt_flush:
    lgdt [rdi]
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
