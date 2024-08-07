/*
 * Copyright © 2024 Guillot Tony <tony.guillot@protonmail.com>
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
#ifndef _BOOT_H_
#define _BOOT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <phoenix/kernel.h>

#define LIMIT_SEGMENTS 32

struct free_memory {
    u64 base;
    u64 length;
};

struct free_memory_hdr {
    u64 entries;
    u64 free_memory;
    u64 total_memory;
    uptr hdmm_offset;
    uptr highest_memory;
    struct free_memory segments[LIMIT_SEGMENTS];
};

struct boot_info {
    struct free_memory_hdr free_memory_hdr;
};

#ifdef __cplusplus
}
#endif

#endif /* _BOOT_H_ */
