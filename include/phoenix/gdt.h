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
#ifndef _GDT_H_
#define _GDT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <phoenix/types.h>
#include <stdint.h>

struct gdt_pointer {
    u16 limit;
    u64 base;
} PACKED;

struct gdt_descriptor {
    u16 limit_low;   /* The low 16 bits of the limit of the segment */
    u16 base_low;    /* The low 16 bits of the base of the segment */
    u8  base_mid;    /* The middle 8 bits of the base of the segment */

    u8  access;      /* The 8 bits descriptor type */
    u8  limit_flags; /* The high 4 bits of the limit and The 4 bits flags */
    u8  base_high;   /* The high 8 bits of the base of the segment */
} PACKED;

void gdt_init(void);

#ifdef __cplusplus
}
#endif

#endif /* _GDT_H_*/
