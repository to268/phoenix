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

#endif /* _GDT_H_*/
