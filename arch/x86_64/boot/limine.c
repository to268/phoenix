/*
 * Copyright © 2023 Guillot Tony <tony.guillot@protonmail.com>
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
#include <phoenix/types/gcc.h>
#include <stddef.h>
#include <limine.h>

struct limine_bootloader_info_request bootloader_info_request = {
    .id = LIMINE_BOOTLOADER_INFO_REQUEST,
    .revision = 0,
    .response = NULL,
};

struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0,
    .response = NULL,
};

struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0,
    .response = NULL,
};

struct limine_module_request module_request = {
    .id = LIMINE_MODULE_REQUEST,
    .revision = 1,
    .response = NULL,
    .internal_module_count = 0,
    .internal_modules = NULL,
};

extern void halt_cpu();

static const char* memmap_type[] = {
    [0] = "USABLE",
    [1] = "RESERVED",
    [2] = "ACPI_RECLAIMABLE",
    [3] = "ACPI_NVS",
    [4] = "BAD_MEMORY",
    [5] = "BOOTLOADER_RECLAIMABLE",
    [6] = "KERNEL_AND_MODULES",
    [7] = "FRAMEBUFFER",
};

static void limine_print_memmap(void) {
    info("Memmap:\n");
    for (u64 i = 0; i < memmap_request.response->entry_count; i++) {
        struct limine_memmap_entry* entry = memmap_request.response->entries[i];

        info("\t[0x%x + 0x%x] -> ", entry->base, entry->length);
        info("%s\n", memmap_type[entry->type]);
    }
}

static void limine_get_free_memmap(struct free_memory_hdr* memory_hdr) {
    u8 entries = 0;
    u64 free_memory = 0;
    u64 total_memory = 0;
    uptr highest_memory = 0;

    for (u64 i = 0; i < memmap_request.response->entry_count; i++) {
        struct limine_memmap_entry* entry = memmap_request.response->entries[i];
        struct free_memory* free_entry = &memory_hdr->segments[entries];

        switch (entry->type) {
        case LIMINE_MEMMAP_USABLE:
        case LIMINE_MEMMAP_KERNEL_AND_MODULES:
        case LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE:
        case LIMINE_MEMMAP_ACPI_RECLAIMABLE:
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
    debug("[LIMINE] built free segment memmap");
}

void limine_handle_requests(struct boot_info* boot_info) {
    debug("[LIMINE] handling requests");

    if (framebuffer_request.response->framebuffer_count < 1)
        halt_cpu();

    framebuffer_init(&framebuffer_request);
    debug("[LIMINE] initialized framebuffer");

    info("Bootloader: %s, revision %d, version: %s\n",
         bootloader_info_request.response->name,
         bootloader_info_request.response->revision,
         bootloader_info_request.response->version);

    info("Modules count: %d\n\n", module_request.internal_module_count);

    limine_print_memmap();
    limine_get_free_memmap(&boot_info->free_memory_hdr);
}
