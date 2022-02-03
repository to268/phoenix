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
#include <phoenix/stivale2.h>
#include <phoenix/kernel.h>
#include <phoenix/serial.h>
#include <phoenix/mem.h>
#include <phoenix/pmm.h>
#include <phoenix/vmm.h>

uptr* vmm_get_next_level(uptr* current_lvl, u8 entry)
{
    uptr* ret = 0;

    /* Check for the present flag */
    if (current_lvl[entry] & PT_PRESENT) {
        /* Return next level */
        return (uptr*)(current_lvl[entry] & ~(0xfff));
    } else {
        /* Allocate a new page table for the next level */
        ret = pmm_alloc(PT_SIZE);

        /* Set the new page table flags (present, writable) */
        current_lvl[entry] = (uptr)ret | (PT_PRESENT | PT_WRITABLE | PT_PRIV_S);
    }

    return ret;
}

void vmm_map(struct page_map* map, uptr virt_addr, uptr phys_addr, u64 flags)
{
    uptr pml4_entry = (virt_addr & ((u64)0x1ff << 39)) >> 39;
    uptr pml3_entry = (virt_addr & ((u64)0x1ff << 30)) >> 30;
    uptr pml2_entry = (virt_addr & ((u64)0x1ff << 21)) >> 21;
    uptr pml1_entry = (virt_addr & ((u64)0x1ff << 12)) >> 12;

    uptr* pml4;
    uptr* pml3;
    uptr* pml2;
    uptr* pml1;

    info("pml4 = %d, pml3 = %d, pml2 = %d, pml1 = %d\n", pml4_entry, pml3_entry,
                                                         pml2_entry, pml1_entry);
    /* Get page map levels */
    pml4 = map->top_lvl;
    pml3 = vmm_get_next_level(pml4, pml4_entry);
    pml2 = vmm_get_next_level(pml3, pml3_entry);
    pml1 = vmm_get_next_level(pml2, pml2_entry);

    pml1[pml1_entry] = phys_addr | flags;
}

struct page_map
vmm_new_pagemap(u8 lvl)
{
    struct page_map page_map;
    page_map.levels = lvl;
    page_map.top_lvl = pmm_alloc(PT_SIZE);

    return page_map;
}

void vmm_init(struct stivale2_struct* hdr)
{
    /* Set page map level to 4 */
    /* TODO: Get if the processor supports 5 level paging */
    struct page_map page_map = vmm_new_pagemap(4);

    // debug("[VMM] mapping base pages");
    // for (uptr i = 0; i < (BASE_MAP_AMOUNT / PAGE_SIZE); i++) {
    //     uptr addr = i * PAGE_SIZE;
    //
    //     vmm_map(&page_map, addr, addr, VMM_PRESENT | VMM_WRITE | VMM_PRIV_S);
    //     vmm_map(&page_map, addr, MEM_BASE + addr, VMM_PRESENT | VMM_WRITE | VMM_PRIV_S);
    // }

    debug("[VMM] mapping free entries");
    struct stivale2_struct_tag_memmap* memmap_tag;
    memmap_tag = (struct stivale2_struct_tag_memmap*)
                stivale2_get_tag(hdr, STIVALE2_STRUCT_TAG_MEMMAP_ID);

    for (uptr i = 0; i < memmap_tag->entries; i++) {
        uptr aligned_base = memmap_tag->memmap[i].base - memmap_tag->memmap[i].base % PAGE_SIZE;
        uptr aligned_length = ((memmap_tag->memmap[i].length / PAGE_SIZE) + 1) * PAGE_SIZE;

        for (uptr j = 0; j * PAGE_SIZE < aligned_length; j++) {
            uptr addr = aligned_base + j * PAGE_SIZE;

            // vmm_map(&page_map, addr, MEM_ADDR + addr, VMM_PRESENT | VMM_WRITE);

            if (memmap_tag->memmap[i].type == STIVALE2_MMAP_KERNEL_AND_MODULES) {
                vmm_map(&page_map, addr, MEM_BASE + addr, VMM_PRESENT | VMM_WRITE | VMM_PRIV_S);
            }
        }
    }

    info("done\n");
    asm volatile("mov %0, %%cr3" : : "a" ((uptr)page_map.top_lvl - MEM_BASE));
}

void vmm_flush_tlb_entry(uptr addr)
{
    asm volatile("invlpg (%0)" : : "a" (addr));
}
