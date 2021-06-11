#include <phoenix/kernel.h>
#include <phoenix/serial.h>
#include <phoenix/pic.h>
#include <phoenix/vga.h>
#include <phoenix/idt.h>
#include <phoenix/io.h>

/* IDT static variables */
__attribute__((aligned(0x10)))
static struct idt_descriptor idt[256];
static struct idt_pointer idtr = {
        .base = (uint64_t)&idt,
        .limit = (uint16_t)sizeof(struct idt_descriptor) * IDT_MAX_DESCRIPTORS -1
};

void idt_set_descriptor(uint8_t vector, void *isr, uint8_t flags)
{
    struct idt_descriptor *descriptor = &idt[vector];

    descriptor->isr_low         = (uint64_t)isr & 0xffff;
    descriptor->kernel_cs       = 0;
    descriptor->ist             = 0;
    descriptor->attributes      = flags;
    descriptor->isr_high        = ((uint64_t)isr >> 16) & 0xffff;
    descriptor->isr_high        = ((uint64_t)isr >> 32) & 0xffffffff;
    descriptor->reserved        = 0;
}

extern void *isr_stub_table[];

void idt_init(void)
{
    /* Remap the PIC */
    pic_remap();

    /* Map interrupt gates */
    for (uint8_t vector = 0; vector < 31; vector++) {
        if (vector == 9 || (vector > 20 && vector < 30)) {
            /* Skip vector 9 and vectors between 20 and 30 */
            continue;
        }

        idt_set_descriptor(vector, isr_stub_table[vector], IDT_INTERRUPT_GATE);
    }

    /* Load IDT and enable interrupts */
    asm volatile("lidt %0" : : "memory"(idtr));
    sti();
    debug("[IDT] Loaded\n");
}
