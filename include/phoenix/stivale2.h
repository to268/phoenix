/*
 * Copyright © 2022 Guillot Tony <tony.guillot@protonmail.com>
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
#ifndef __STIVALE2_H_
#define __STIVALE2_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <phoenix/kernel.h>
#include <phoenix/pmm.h>
#include <stivale2.h>
#include <stddef.h>

void* stivale2_get_tag(struct stivale2_struct* hdr, u64 id);
void stivale2_print_fb_tag(struct stivale2_struct_tag_framebuffer* fb_tag);
void stivale2_print_smp_tag(struct stivale2_struct_tag_smp* smp_tag);
void stivale2_print_memmap(struct stivale2_struct_tag_memmap* memmap_tag);
char* stivale2_get_cmdline(struct stivale2_struct* hdr);
void stivale2_process_tags(struct stivale2_struct* hdr);
struct free_memory_hdr
stivale2_get_free_memory(struct stivale2_struct* hdr);

#ifdef __cplusplus
}
#endif

#endif /*__STIVALE2_H_ */
