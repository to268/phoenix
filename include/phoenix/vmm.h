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
#ifndef _VMM_H_
#define _VMM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <phoenix/kernel.h>
#include <limine.h>

#define PT_PRESENT  (1 << 0) /* Present flag */
#define PT_WRITABLE (1 << 1) /* Read Write flag */
#define PT_PRIV_S   (1 << 2) /* Page Privilege flag, 0: User, 1: Supervisor */
#define PT_WTHR     (1 << 3) /* Write Thought flag  */
#define PT_CD       (1 << 4) /* Cache disabled flag */
#define PT_ACCESED  (1 << 5) /* Accesed flag (read only) */
#define PT_HUGE     (1 << 7) /* Page Size flag, 0: 4Kb, 1: 2Mb*/
#define PT_SIZE     0x1000

#define VMM_PRESENT (1 << 0)
#define VMM_WRITE   (1 << 1)
#define VMM_PRIV_S  (1 << 2)

#define BASE_MAP_AMOUNT 0x2000000
#define MEM_BASE        0xffffffff80000000
#define MEM_ADDR        0xffff800000000000
#define FRAME_ADDR      0xfffffffffffff000

#define PG_GET_PHYS_ADDR(x) ((x)&FRAME_ADDR)

/* TODO: Add support for huge pages (2 Mb and 1 Gb (probably) page size) */
struct page_map {
    u8 levels;
    uptr* top_lvl;
};

void vmm_init(struct boot_info* boot_info);

#ifdef __cplusplus
}
#endif

#endif /* _VMM_H_*/
