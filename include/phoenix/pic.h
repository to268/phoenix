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
#ifndef _PIC_H_
#define _PIC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <phoenix/types.h>
#include <stdint.h>

#define PIC1                0x20		/* Master PIC */
#define PIC2                0xa0		/* Slave PIC */
#define PIC1_CMD	        PIC1
#define PIC1_DATA           (PIC1+1)
#define PIC2_CMD            PIC2
#define PIC2_DATA           (PIC2+1)
#define PIC_READ_IRR        0x0a        /* Raised interrupts */
#define PIC_READ_ISR        0x0b        /* Serviced interrupts */
#define PIC_EOI             0x20        /* End of interrupt */

/* PIC remaps values */
#define ICW1_ICW4	        0x01		/* ICW4 */
#define ICW1_SINGLE	        0x02		/* Single cascade mode */
#define ICW1_INTERVAL4	    0x04		/* Call address interval 4 */
#define ICW1_LEVEL	        0x08		/* Level triggered mode */
#define ICW1_INIT	        0x10		/* Initialization */
#define ICW4_8086	        0x01		/* 8086 mode */
#define ICW4_AUTO           0x02		/* Auto EOI */
#define ICW4_BUF_SLAVE	    0x08		/* Buffered mode/slave */
#define ICW4_BUF_MASTER	    0x0C		/* Buffered mode/master */
#define ICW4_SFNM           0x10		/* Special fully nested */
#define PIC_MASTER_OFFSET   0x20        /* Vector offset of the master IRQ */
#define PIC_SLAVE_OFFSET    0x28        /* Vector offset of the slave IRQ */

/* Convert an IRQ number to a vector number */
#define IRQ(x)              (PIC_MASTER_OFFSET + x)

void pic_remap(void);
void pic_send_eoi(u8 irq);
void pic_irq_set_mask(u8 irq);
void pic_irq_clear_mask(u8 irq);
u16 pic_get_irr(void);
u16 pic_get_isr(void);

#ifdef __cplusplus
}
#endif

#endif /* _PIC_H_*/
