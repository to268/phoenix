#ifndef _PIC_H_
#define _PIC_H_

#ifdef __cplusplus
extern "C" {
#endif

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
void pic_send_eoi(unsigned char irq);
void pic_irq_set_mask(unsigned char irq);
void pic_irq_clear_mask(unsigned char irq);
uint16_t pic_get_irr(void);
uint16_t pic_get_isr(void);

#ifdef __cplusplus
}
#endif

#endif /* _PIC_H_*/
