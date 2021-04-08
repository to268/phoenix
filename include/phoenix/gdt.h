#ifndef _GDT_H_
#define _GDT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

struct gdt_pointer {
    uint16_t    limit;
    uint64_t    base;
} __attribute__((packed));

struct gdt_descriptor {
    uint16_t    limit_low;      // The low 16 bits of the limit of the segment
    uint16_t    base_low;       // The low 16 bits of the base of the segment
    uint8_t     base_mid;       // The middle 8 bits of the base of the segment
    uint16_t    flags;          // The descriptor Flags
    uint8_t     base_high;      // The high 8 bits of the base of the segment
} __attribute__((packed));

// Flags
#define GRANULARITY_BIT (1 << 0xf)              // Granularity (0: 1B - 1MB, 1: 4kB - 4GB)
#define SIZE_BIT        (1 << 0xe)              // Size (0: 16 bit, 1: 32 bit)
#define LONG_BIT        (1 << 0xd)              // Long mode
#define FREE_BIT        (1 << 0xc)              // Free to use
#define PRESENT_BIT     (1 << 0x7)              // Present
#define PRIVILEGE(x)    ((x & 0x03) << 0x05)    // Privilege level (0 - 3)
#define DESCRIPTOR_BIT  (1 << 0x4)              // Descriptor type (0: system, 1: code/data)
#define EX_BIT          (1 << 3)                // Executable bit
#define RW_BIT          (1 << 1)                // Read/Write bit

// Functions
void gdt_init(void);

#ifdef __cplusplus
}
#endif

#endif
