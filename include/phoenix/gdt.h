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
    uint16_t    limit_low;      /* The low 16 bits of the limit of the segment */
    uint16_t    base_low;       /* The low 16 bits of the base of the segment */
    uint8_t     base_mid;       /* The middle 8 bits of the base of the segment */

    uint8_t     access;         /* The 8 bits descriptor type */
    uint8_t     limit_high: 4;  /* The high 4 bits of the limit of the segment */
    uint8_t     flags: 4;       /* The 4 bits flags (Free, Long, Size, Gran) */
    uint8_t     base_high;      /* The high 8 bits of the base of the segment */
} __attribute__((packed));

void gdt_init(void);

#ifdef __cplusplus
}
#endif

#endif
