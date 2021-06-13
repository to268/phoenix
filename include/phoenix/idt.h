#ifndef _IDT_H_
#define _IDT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

struct idt_descriptor{
	uint16_t    isr_low;      /* The lower 16 bits of the ISR's address */
	uint16_t    kernel_cs;    /* The GDT segment selector */
	uint8_t	    ist;          /* The IST in the TSS */
	uint8_t     attributes;   /* Type and attributes */
	uint16_t    isr_mid;      /* The higher 16 bits of the lower 32 bits of the ISR's address */
	uint32_t    isr_high;     /* The higher 32 bits of the ISR's address */
	uint32_t    reserved;     /* Set to zero */
} __attribute__((packed));

struct idt_pointer{
    uint16_t    limit;  /* The limit of the IDT */
    uint64_t    base;   /* The base of the IDT */
} __attribute__((packed));

#define IDT_MAX_DESCRIPTORS 256

/* Gate types */
#define IDT_INTERRUPT_GATE  0x8e
#define IDT_TASK_GATE       0x8c
#define IDT_TRAP_GATE       0x8f

void idt_init(void);
__attribute__((noreturn))
void exception_handler(uint64_t vector);

#ifdef __cplusplus
}
#endif

#endif /* _IDT_H_*/
