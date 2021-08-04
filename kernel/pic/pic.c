#include <phoenix/kernel.h>
#include <phoenix/serial.h>
#include <phoenix/pic.h>
#include <phoenix/io.h>

void pic_remap(void)
{
    /* Initialization sequence */
    outb(PIC1_CMD, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC2_CMD, ICW1_INIT | ICW1_ICW4);
    io_wait();

    /* Set master PIC vector offset */
    outb(PIC1_DATA, PIC_MASTER_OFFSET);
    io_wait();

    /* Set slave PIC vector offset */
    outb(PIC2_DATA, PIC_SLAVE_OFFSET);
    io_wait();

    /* Tell master PIC of the slave PIC at IRQ2 */
    outb(PIC1_DATA, 4);
    io_wait();

    /* Tell slave PIC his cascade identity */
    outb(PIC2_DATA, 2);
    io_wait();

    /* Set 8086 mode to the master and the slave PIC */
    outb(PIC1_DATA, ICW4_8086);
    io_wait();
    outb(PIC2_DATA, ICW4_8086);
    io_wait();

    /* Set IRQ masks at zero */
    outb(PIC1_DATA, 0);
    outb(PIC2_DATA, 0);

    debug("[PIC] IRQs have been remapped\n");
}

void pic_send_eoi(unsigned char irq)
{
	if(irq >= 8)
		outb(PIC2_CMD, PIC_EOI);

	outb(PIC1_CMD, PIC_EOI);
}

void pic_irq_set_mask(unsigned char irq)
{
    uint16_t port;
    uint8_t value;

    if(irq < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        irq -= 8;
    }

    value = inb(port) | (1 << irq);
    outb(port, value);
}

void pic_irq_clear_mask(unsigned char irq)
{
    uint16_t port;
    uint8_t value;

    if(irq < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        irq -= 8;
    }

    value = inb(port) & ~(1 << irq);
    outb(port, value);
}

/* Send an OCW3 command */
uint16_t pic_get_irq_reg(int ocw3)
{
    /* Get register values */
    outb(PIC1_CMD, ocw3);
    outb(PIC2_CMD, ocw3);

    /* Format values PIC2 8-15 and PIC1 0-7 */
    return (inb(PIC2_CMD) << 8) | inb(PIC1_CMD);
}

/* Get values of the PICs IRQ request register */
uint16_t pic_get_irr(void)
{
    return pic_get_irq_reg(PIC_READ_IRR);
}

/* Get values of the PICs in-service register */
uint16_t pic_get_isr(void)
{
    return pic_get_irq_reg(PIC_READ_ISR);
}