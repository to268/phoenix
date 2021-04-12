#ifndef __STIVALE2_H_
#define __STIVALE2_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <phoenix/kernel.h>
#include <phoenix/pmm.h>
#include <stivale2.h>
#include <stdint.h>
#include <stddef.h>

void* stivale2_get_tag(struct stivale2_struct* hdr, uint64_t id);
void stivale2_print_fb_tag(struct stivale2_struct_tag_framebuffer* fb_tag);
void stivale2_print_smp_tag(struct stivale2_struct_tag_smp* smp_tag);
void stivale2_print_memmap(struct stivale2_struct_tag_memmap* memmap_tag);
void stivale2_process_tags(struct stivale2_struct* hdr);
struct usable_memory_hdr
stivale2_get_usable_memory(struct stivale2_struct* hdr);

#ifdef __cplusplus
}
#endif

#endif
