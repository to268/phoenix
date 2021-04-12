#ifndef _IDT_H_
#define _IDT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/* Long Mode IDT Entry */
typedef struct {
    uint16_t offset_low16; /* Bits 0->15 of the address of the ISR */
    uint16_t cs; /* The CS to be switched to upon interrupt */
    uint8_t ist; /* Contains an offset to the TSS for that IST */
    uint8_t attributes; /* Contains info regarding the DPL and gate type etc... */
    uint16_t offset_mid16; /* Bits 15->31 of the address of the ISR */
    uint32_t offset_high32; /* Bits 31->63 of the address of the ISR */
    uint32_t zero; /* Always zero */
} __attribute__((packed)) idt_entry_t;

typedef struct {
    uint16_t size; /* The size of the IDT */
    uint64_t offset; /* The address of the IDT */
} __attribute__((packed)) idtr_t;

/* Protected Mode IDT Entry */
typedef struct {
    uint16_t offset_low16; /* The lower 16 bits of the address of the ISR */
    uint16_t cs; /* The CS to be switched to upon interrupt */
    uint8_t zero; /* Always zero */
    uint8_t attributes; /* Contains info regarding the DPL and gate type etc... */
    uint16_t offset_high16; /* The higher 16 bits of the address of the ISR */
} __attribute__((packed)) idt_entry16_t;

typedef struct {
    uint8_t gate_type:4;
    uint8_t zero:1;
    uint8_t dpl:2;
    uint8_t p:1;
} attributes;

#ifdef __cplusplus
}
#endif

#endif
