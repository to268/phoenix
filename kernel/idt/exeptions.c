/*
 * Copyright © 2021 Guillot Tony <tony.guillot@protonmail.com>
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <phoenix/kernel.h>
#include <phoenix/idt.h>
#include <phoenix/io.h>

/* List of exceptions messages */
static char *exceptions[] = {
    [0] = "Division by Zero",
    [1] = "Debug",
    [2] = "Non Maskable Interrupt",
    [3] = "Breakpoint",
    [4] = "Overflow",
    [5] = "Bound Range Exceeded",
    [6] = "Invalid opcode",
    [7] = "Device not available",
    [8] = "Double Fault",
    [10] = "Invalid TSS",
    [11] = "Segment not present",
    [12] = "Stack Exception",
    [13] = "General Protection fault",
    [14] = "Page fault",
    [16] = "x87 Floating Point Exception",
    [17] = "Alignment check",
    [18] = "Machine check",
    [19] = "SIMD floating point Exception",
    [20] = "Virtualization Exception",
    [30] = "Security Exception"
};

NORETURN
void exception_handler(u64 vector)
{
    cli();
    panic(exceptions[vector]);
}
