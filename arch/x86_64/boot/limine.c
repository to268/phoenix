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
#include <phoenix/kernel.h>
#include <phoenix/limine.h>
#include <phoenix/serial.h>
#include <phoenix/framebuffer.h>
#include <phoenix/boot.h>
#include <phoenix/mem.h>
#include <stddef.h>
#include <limine.h>

SECTION(".limine_requests_start_marker")
USED static volatile LIMINE_REQUESTS_START_MARKER

    SECTION(".limine_requests") USED static volatile LIMINE_BASE_REVISION(2);

SECTION(".limine_requests")
USED static volatile struct limine_bootloader_info_request
    bootloader_info_request = {
        .id = LIMINE_BOOTLOADER_INFO_REQUEST,
        .revision = 1,
        .response = NULL,
};

SECTION(".limine_requests")
USED static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 1,
    .response = NULL,
};

SECTION(".limine_requests")
USED static volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 1,
    .response = NULL,
};

SECTION(".limine_requests")
USED static volatile struct limine_module_request module_request = {
    .id = LIMINE_MODULE_REQUEST,
    .revision = 1,
    .response = NULL,
    .internal_module_count = 0,
    .internal_modules = NULL,
};

SECTION(".limine_requests")
USED static volatile struct limine_paging_mode_request paging_request = {
    .id = LIMINE_PAGING_MODE_REQUEST,
    .revision = 1,
    .response = NULL,
    .mode = LIMINE_PAGING_MODE_MAX,
    .min_mode = LIMINE_PAGING_MODE_MIN,
    .max_mode = LIMINE_PAGING_MODE_MAX,
};

SECTION(".limine_requests")
USED static volatile struct limine_hhdm_request hhdm_request = {
    .id = LIMINE_HHDM_REQUEST, .revision = 0, .response = NULL};

SECTION(".limine_requests")
USED static volatile struct limine_kernel_address_request
    kernel_address_request = {.id = LIMINE_KERNEL_ADDRESS_REQUEST,
                              .revision = 0};

SECTION(".limine_requests_end_marker")
USED static volatile LIMINE_REQUESTS_END_MARKER

    NORETURN extern void
    halt_cpu();

NONNULL static void limine_get_free_memmap(struct free_memory_hdr* memory_hdr) {
    u8 entries = 0;
    u64 free_memory = 0;
    u64 total_memory = 0;
    uptr highest_memory = 0;

    for (u64 i = 0; i < memmap_request.response->entry_count; i++) {
        auto entry = memmap_request.response->entries[i];
        auto free_entry = &memory_hdr->segments[entries];

        if (entry->type == LIMINE_MEMMAP_USABLE) {
            free_entry->base = entry->base;
            free_entry->length = entry->length;

            uptr address = entry->base + entry->length;
            if (address > highest_memory)
                highest_memory = address;

            free_memory += entry->length;
            entries++;
        }
        total_memory += entry->length;
    }

    if (entries == 0)
        panic("cannot find any free memory chucks\n");

    memory_hdr->entries = entries;
    memory_hdr->free_memory = free_memory;
    memory_hdr->total_memory = total_memory;
    memory_hdr->highest_memory = highest_memory;
    memory_hdr->hdmm_offset = hhdm_request.response->offset;
    debug("[LIMINE] built free segment memmap");
}

NONNULL void limine_handle_requests(struct boot_info* boot_info) {
    debug("[LIMINE] handling requests");

    if (LIMINE_BASE_REVISION_SUPPORTED == false)
        halt_cpu();

    if (framebuffer_request.response->framebuffer_count < 1)
        halt_cpu();

    framebuffer_init(&framebuffer_request);
    debug("[LIMINE] initialized framebuffer");

    info("Bootloader: %s, revision %d, version: %s\n",
         bootloader_info_request.response->name,
         bootloader_info_request.response->revision,
         bootloader_info_request.response->version);

    if (paging_request.response == NULL)
        panic("No paging available");

    info("paging mode: LVL%d\n", paging_request.mode ? 5 : 4);
    info("Modules count: %d\n", module_request.internal_module_count);

    limine_get_free_memmap(&boot_info->free_memory_hdr);
}
