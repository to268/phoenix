/*
 * Copyright © 2021 Guillot Tony <tony.guillot@protonmail.com>
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
