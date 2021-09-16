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
#ifndef _IDT_H_
#define _IDT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <phoenix/types.h>
#include <stdint.h>

struct idt_descriptor{
	u16 isr_low;    /* The lower 16 bits of the ISR's address */
	u16 selector;   /* The GDT segment selector */
	u8  ist;        /* The IST in the TSS */
	u8  attributes; /* Type and attributes */
	u16 isr_mid;    /* The higher 16 bits of the lower 32 bits of the ISR's address */
	u32 isr_high;   /* The higher 32 bits of the ISR's address */
	u32 reserved;   /* Set to zero */
} PACKED;

struct idt_pointer{
    u16 limit;  /* The limit of the IDT */
    u64 base;   /* The base of the IDT */
} PACKED;

#define IDT_MAX_DESCRIPTORS 256

/* Gate types */
#define IDT_INTERRUPT_GATE  0x8e
#define IDT_TASK_GATE       0x8c
#define IDT_TRAP_GATE       0x8f

void idt_init(void);
NORETURN
void exception_handler(u64 vector);

#ifdef __cplusplus
}
#endif

#endif /* _IDT_H_*/
