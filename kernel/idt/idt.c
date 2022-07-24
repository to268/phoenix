/*
 * Copyright © 2022 Guillot Tony <tony.guillot@protonmail.com>
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
#include <phoenix/serial.h>
#include <phoenix/idt.h>
#include <phoenix/pic.h>
#include <phoenix/nmi.h>
#include <phoenix/io.h>
#include <phoenix/keyboard.h>

/* IDT static variables */
ALIGNED(0x10)
static struct idt_descriptor idt[IDT_MAX_DESCRIPTORS];
static struct idt_pointer idtr = {
        .base = (u64)&idt,
        .limit = (u16)sizeof(struct idt_descriptor) * IDT_MAX_DESCRIPTORS - 1
};

/* Assembly symbols */
extern void* isr_exception_stub_table[];
extern void pit_handler_irq(void);
extern void keyboard_handler_irq(void);
extern void pic_send_eoi_master(void);
extern void pic_send_eoi_slave(void);

void idt_set_descriptor(u8 vector, void *isr, uint8_t flags)
{
    struct idt_descriptor *descriptor = &idt[vector];

    descriptor->isr_low         = (u64)isr & 0xffff;
    descriptor->selector        = 0x8;
    descriptor->ist             = 0;
    descriptor->attributes      = flags;
    descriptor->isr_mid         = ((u64)isr >> 16) & 0xffff;
    descriptor->isr_high        = ((u64)isr >> 32) & 0xffffffff;
    descriptor->reserved        = 0;
}

void idt_init(void)
{
    /* Remap the PIC */
    pic_remap();

    /* Map exceptions vectors */
    for (u8 vector = 0; vector < 32; vector++)
        idt_set_descriptor(vector, isr_exception_stub_table[vector], IDT_INTERRUPT_GATE);

    /* Map IRQs */
    idt_set_descriptor(IRQ(0), (void*)&pit_handler_irq, IDT_INTERRUPT_GATE);
    idt_set_descriptor(IRQ(1), (void*)&keyboard_handler_irq, IDT_INTERRUPT_GATE);

    pic_irq_clear_mask(1);
    /* All others IRQ are unmasked when initialised */

    for (u8 i = IRQ(2); i < IRQ(7); i++) {
        idt_set_descriptor(i, pic_send_eoi_master, IDT_INTERRUPT_GATE);
    }

    for (u8 i = IRQ(7); i < IRQ(15); i++) {
        idt_set_descriptor(i, pic_send_eoi_slave, IDT_INTERRUPT_GATE);
    }

    /* Load IDT and enable interrupts */
    asm volatile("lidt %0" : : "memory"(idtr));
    sti();
    debug("[IDT] Loaded\n");

    /* Enable NMI */
    nmi_enable();
}
