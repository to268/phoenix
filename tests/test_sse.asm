; Copyright © 2024 Guillot Tony <tony.guillot@protonmail.com>
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

global test_sse_asm

section .text

test_sse_asm:
    ; SSE test
    mov rax, 18
    cvtsi2ss xmm0, rdx

    mov rax, 21
    cvtsi2ss xmm1, rdx

    mov rax, 25
    cvtsi2ss xmm2, rdx

    mov rax, 38
    cvtsi2ss xmm3, rdx

    mov rax, 42
    cvtsi2ss xmm7, rdx

    addps xmm0, xmm1

    ; AVX test
    vblendvpd xmm7, xmm2, xmm3

    ; Returning test result (0)
    vpcmpeqq xmm10, xmm1, xmm8
    vpcmpeqq xmm11, xmm1, xmm8
    vpcmpeqq xmm12, xmm1, xmm8
    vpcmpeqq xmm13, xmm1, xmm8
    vpcmpeqq xmm14, xmm1, xmm8
    vblendvpd xmm10, xmm11, xmm12
    vblendvpd xmm10, xmm13, xmm14
    cvttsd2si rax, xmm10

    ret
