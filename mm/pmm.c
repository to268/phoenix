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
#include <phoenix/structs/bitmap.h>
#include <phoenix/stivale2.h>
#include <phoenix/kernel.h>
#include <phoenix/serial.h>
#include <phoenix/mem.h>
#include <phoenix/pmm.h>

static Bitmap bitmap;

void pmm_init(struct stivale2_struct* hdr)
{
    /* Get usable memory info */
    struct free_memory_hdr free_hdr;
    free_hdr = stivale2_get_free_memory(hdr);

    /* Print free memory for now */
    uptr free_mem = convert_to_mb(free_hdr.free_memory);
    info("\n%d Mb is free\n", free_mem);

    /* Compute the size of the bitmap */
    // FIXME: fix printing issues with gcc
    bitmap.size = DIV_ROUNDUP(free_hdr.highest_memory, PAGE_SIZE) / 8;
    info("bitmap size is %d bytes\n\n", bitmap.size);

    /* Find a place for the bitmap */
    for (u64 entry = 0; entry < free_hdr.entries; entry++) {
        if (free_hdr.entries < entry)
            panic("No free segment has been found to store the bitmap");

        struct free_memory* free_segment = &free_hdr.segments[entry];

        if (free_segment->length >= bitmap.size) {
            /* Set the start of the bitmap */
            bitmap.bitmap = (u8*)free_segment->base;
            /* Fill the bitmap of zeroes */
            memset(bitmap.bitmap, 0x00, bitmap.size);

            debug("[PMM] We found a place for the bitmap\n");
            break;
        }
    }

    /* Find the next segment to store pages */
    for (uptr entry = 0; entry < free_hdr.entries; entry++) {
        if (free_hdr.entries < entry)
            panic("No free segment has been found to store pages");

        struct free_memory* free_segment = &free_hdr.segments[entry];

        /* Do no use the same segment of the bitmap */
        if (free_segment->base != (uptr)bitmap.bitmap) {
            bitmap.base = (uptr*)free_segment->base;

            debug("[PMM] We found a place to store pages\n");
            break;
        }
    }

    debug("[PMM] Initialized\n");
}

inline u64 pmm_get_index(void* address, Bitmap* bitmap)
{
    return (u64)((uptr*)address - bitmap->base) / PAGE_SIZE;
}

void pmm_reserve_page(u64 index)
{
    bitmap_set(&bitmap, index);
    debug("[PMM] reseved page\n");
}

void pmm_reserve_pages(void* address, u64 pages)
{
    for (u64 i = 0; i < pages; i++) {
        u64 index = pmm_get_index(address + (i * PAGE_SIZE), &bitmap);
        pmm_reserve_page(index);
    }
}

void pmm_zero(void* address, u64 pages)
{
    for (uptr i = (uintptr_t)address; i < (pages * PAGE_SIZE); i++) {
        memset(address, 0,(uptr)address + (i * PAGE_SIZE));
    }
}

int pmm_check_next_pages(u64 start_bit, uint64_t pages)
{
    for (u64 bit = start_bit; bit < (start_bit + pages); bit++) {
        if (bitmap_check(&bitmap, bit)) {
            return 0;
        }
    }
    return 1;
}

void* pmm_alloc(u64 length)
{
    u64 pages_number = DIV_ROUNDUP(length, PAGE_SIZE);
    void* addr;

    /* TODO: Enhance PMM by traking the first free chunk and the bitmap tail */
    for (u64 bit = 0; bit < bitmap.size * 8; bit++) {
        for (u64 page = 0; page < pages_number; page++) {

            /* Check if a page is already allocated */
            if (bitmap_check(&bitmap, bit)) {
                break;
            } else if (!bitmap_check(&bitmap, bit) && page == pages_number - 1) {
                /* Check if we can allocate requested pages continuously */
                if (!pmm_check_next_pages(bit, pages_number))
                    break;

                addr = (void*)bitmap.base + (bit * PAGE_SIZE);
                /* Set the page as used */
                pmm_reserve_pages(addr, pages_number);
                /* Fill page with zeroes */
                pmm_zero(addr, pages_number);
                return addr;
            }
        }
    }

    return NULL;
}

void pmm_free(void* address, u64 pages)
{
    for (u64 i = 0; i < pages; i++) {
        u64 index = pmm_get_index(address + (i * PAGE_SIZE), &bitmap);
        bitmap_clear(&bitmap, index);
        debug("[PMM] freed page\n");
    }
}
