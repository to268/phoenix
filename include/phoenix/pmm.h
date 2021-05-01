#ifndef _PMM_H_
#define _PMM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stivale2.h>
#include <stdint.h>

#define LIMIT_SEGMENTS 64

struct usable_memory {
    uint64_t base;
    uint64_t length;
    uint32_t type;
}__attribute((packed));

struct usable_memory_hdr {
    uint64_t entries;
    uint64_t usable_memory;
    uint64_t total_memory;
    struct usable_memory segments[LIMIT_SEGMENTS];
}__attribute((packed));

void pmm_init(struct stivale2_struct* hdr);

#ifdef __cplusplus
}
#endif

#endif /* _PMM_H_*/
