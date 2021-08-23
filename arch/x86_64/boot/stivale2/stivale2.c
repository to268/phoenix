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
#include <phoenix/kernel.h>
#include <phoenix/stivale2.h>
#include <phoenix/serial.h>
#include <phoenix/pmm.h>
#include <phoenix/vga.h>
#include <stivale2.h>
#include <stdint.h>
#include <stddef.h>

/* Stack required by the stivale2 specifications */
static uint8_t stack[4096];

/*
static struct stivale2_header_tag_framebuffer framebuffer_tag = {
    .tag = {
        .identifier = STIVALE2_HEADER_TAG_FRAMEBUFFER_ID,
        .next = 0,
    },
    .framebuffer_width  = 0,
    .framebuffer_height = 0,
    .framebuffer_bpp    = 0
};


static struct stivale2_header_tag_smp smp_tag = {
    .tag = {
        .identifier = STIVALE2_HEADER_TAG_SMP_ID,
        .next = 0,
        //.next = (uint64_t)&framebuffer_tag,
    },
    .flags = 0
};
*/

/* Base Stivale2 header */
__attribute__((section(".stivale2hdr"), used))
struct stivale2_header stivale_hdr = {
    .entry_point = (uintptr_t)&init,
    .stack = (uintptr_t)stack + sizeof(stack),
    .flags = 0,
    .tags = 0,
    /*.tags = (uint64_t)&smp_tag, */
};

void stivale2_print_fb_tag(struct stivale2_struct_tag_framebuffer* fb_tag)
{
    info("Framebuffer tag:\n");
    info("\tAddress: 0x%x\n",   fb_tag->framebuffer_addr);
    info("\tWidth:   %d\n",     fb_tag->framebuffer_width);
    info("\tHeight:  %d\n",     fb_tag->framebuffer_height);
    info("\tPitch:   %d\n",     fb_tag->framebuffer_pitch);
    info("\tBPP:     %d\n",     fb_tag->framebuffer_bpp);
    info("\tMemory model:    %d\n", fb_tag->memory_model);
    info("\tRed mask size:   %d\n", fb_tag->red_mask_size);
    info("\tRed mask size:   %d\n", fb_tag->red_mask_shift);
    info("\tGreen mask size: %d\n", fb_tag->green_mask_size);
    info("\tGreen mask size: %d\n", fb_tag->green_mask_shift);
    info("\tBlue mask size:  %d\n", fb_tag->blue_mask_size);
    info("\tBlue mask size:  %d\n", fb_tag->blue_mask_shift);
}

void stivale2_print_smp_tag(struct stivale2_struct_tag_smp* smp_tag)
{
    info("SMP Info:\n");
    info("\tFlags:        0x%x\n", smp_tag->flags);
    info("\tBSP LAPIC ID:   %d\n", smp_tag->bsp_lapic_id);
    info("\tCPU Count:      %d\n\n", smp_tag->cpu_count);
    for (size_t i = 0; i < smp_tag->cpu_count; i++) {
        struct stivale2_smp_info *cpu_info = &smp_tag->smp_info[i];
        info("\t\tProcessor ID:     %d\n", cpu_info->processor_id);
        info("\t\tLAPIC ID:         %d\n", cpu_info->lapic_id);
        info("\t\tTarget Stack:   0x%x\n", cpu_info->target_stack);
        info("\t\tGOTO Address:   0x%x\n", cpu_info->goto_address);
        info("\t\tExtra Argument: 0x%x\n", cpu_info->extra_argument);
    }
    info("\n");
}

void stivale2_print_memmap(struct stivale2_struct_tag_memmap* memmap_tag)
{
    info("Memmap Entries: %d\n", memmap_tag->entries);
    for (uint64_t i = 0; i < memmap_tag->entries; i++) {
        struct stivale2_mmap_entry* current_entry = &memmap_tag->memmap[i];
        switch (current_entry->type) {

            case STIVALE2_MMAP_USABLE:
                info("[0x%x+0x%x] -> MMAP_USABLE\n", current_entry->base,
                current_entry->length);
                break;

            case STIVALE2_MMAP_RESERVED:
                info("[0x%x+0x%x] -> MMAP_RESERVED\n", current_entry->base,
                current_entry->length);
                break;

            case STIVALE2_MMAP_ACPI_RECLAIMABLE:
                info("[0x%x+0x%x] -> MMAP_ACPI_RECLAIMABLE\n", current_entry->base,
                current_entry->length);
                break;

            case STIVALE2_MMAP_ACPI_NVS:
                info("[0x%x+0x%x] -> MMAP_ACPI_NVS\n", current_entry->base,
                current_entry->length);
                break;

            case STIVALE2_MMAP_BAD_MEMORY:
                info("[0x%x+0x%x] -> MMAP_BAD_MEMORY\n", current_entry->base,
                current_entry->length);
                break;

            case STIVALE2_MMAP_BOOTLOADER_RECLAIMABLE:
                info("[0x%x+0x%x] -> MMAP_BOOTLOADER_RECLAIMABLE\n", current_entry->base,
                current_entry->length);
                break;

            case STIVALE2_MMAP_KERNEL_AND_MODULES:
                info("[0x%x+0x%x] -> MMAP_KERNEL_AND_MODULES\n", current_entry->base,
                current_entry->length);
                break;

            default:
                break;
        }
    }
    info("\n");
}

void* stivale2_get_tag(struct stivale2_struct* hdr, uint64_t id)
{
    struct stivale2_tag* current_tag = (void*)hdr->tags;
    for (;;) {
        if (current_tag == NULL) {
            /* Return NULL if not found */
            return NULL;
        }

        if (current_tag->identifier == id) {
            /* Return the pointer of the tag if found */
            return current_tag;
        }

        /* Set the pointer to the next item */
        current_tag = (void*)current_tag->next;
    }
}

struct usable_memory_hdr
stivale2_get_usable_memory(struct stivale2_struct* hdr)
{
    /* Get memmap tag */
    struct stivale2_struct_tag_memmap* memmap_tag;
    memmap_tag = (struct stivale2_struct_tag_memmap*)
                stivale2_get_tag(hdr, STIVALE2_STRUCT_TAG_MEMMAP_ID);

    /* Panic if the stivale2 memmap tag has not been found */
    if (memmap_tag == NULL) {
        panic("No stivale2 memmap tag has been found !\n");
    }

    /* Get Usable Entries */
    struct usable_memory_hdr usable_hdr;
    struct usable_memory entry;
    usable_hdr.entries = 0;
    uint64_t i;
    for (i = 0; i < memmap_tag->entries; i++) {
        struct stivale2_mmap_entry* current_entry = &memmap_tag->memmap[i];

        usable_hdr.total_memory += entry.length;

        if (current_entry->type == STIVALE2_MMAP_USABLE ||
            current_entry->type == STIVALE2_MMAP_KERNEL_AND_MODULES ||
        current_entry->type == STIVALE2_MMAP_BOOTLOADER_RECLAIMABLE) {

            /* Found an Usable Entry */
            entry.base = current_entry->base;
            entry.length = current_entry->length;
        entry.type = current_entry->type;
            usable_hdr.segments[usable_hdr.entries] = entry;

            debug("[STIVALE2_MEMMAP] Found an usable entry\n");

            usable_hdr.usable_memory += entry.length;
            usable_hdr.entries++;
        }
    }

    /* Panic if no entries are found */
    if (usable_hdr.entries <= 0) {
        panic("No usable memory has been found !\n");
    }

    return usable_hdr;
}

char* stivale2_get_cmdline(struct stivale2_struct* hdr)
{
    /* Get cmdline tag */
    struct stivale2_struct_tag_cmdline* cmdline_tag;
    cmdline_tag = (struct stivale2_struct_tag_cmdline*)
                stivale2_get_tag(hdr, STIVALE2_STRUCT_TAG_CMDLINE_ID);

    /* Panic if the stivale2 cmdline tag has not been found */
    if (cmdline_tag == NULL) {
        panic("No stivale2 cmdline tag has been found !\n");
    }

    return (char*)cmdline_tag->cmdline;
}

void stivale2_process_tags(struct stivale2_struct* hdr)
{
    /* Tags */
    struct stivale2_tag* current_tag = (void*)hdr->tags;
    struct stivale2_struct_tag_framebuffer* fb_tag_ptr;
    struct stivale2_struct_tag_smp* smp_tag_ptr;
    struct stivale2_struct_tag_cmdline* cmdline_tag_ptr;
    struct stivale2_struct_tag_pxe_server_info* pxe_tag_ptr;
    struct stivale2_struct_tag_memmap* memmap_tag_ptr;

    /* Bootloader Info */
    info("Bootloader brand:     %s\n",      hdr->bootloader_brand);
    info("Bootloader version:   %s\n\n",    hdr->bootloader_version);

    /* Loop on all tags */
    for (;;) {
        if (current_tag == NULL)
            break;

        switch (current_tag->identifier) {

            /* Framebuffer Tag */
            case STIVALE2_STRUCT_TAG_FRAMEBUFFER_ID:
                fb_tag_ptr = (struct stivale2_struct_tag_framebuffer*)current_tag;
                stivale2_print_fb_tag(fb_tag_ptr);
                break;

            /* SMP Tag */
            case STIVALE2_STRUCT_TAG_SMP_ID:
                smp_tag_ptr = (struct stivale2_struct_tag_smp*)current_tag;
                stivale2_print_smp_tag(smp_tag_ptr);
                break;

            /* Cmdline Tag */
            case STIVALE2_STRUCT_TAG_CMDLINE_ID:
                cmdline_tag_ptr = (struct stivale2_struct_tag_cmdline*)current_tag;
                info("Cmdline: %s\n\n", (char*)cmdline_tag_ptr->cmdline);
                break;

            /* PXE Tag */
            case STIVALE2_STRUCT_TAG_PXE_SERVER_INFO:
                pxe_tag_ptr = (struct stivale2_struct_tag_pxe_server_info*)current_tag;
                info("PXE IP: %d\n\n", pxe_tag_ptr->server_ip);
                break;

            /* Memmap Tag */
            case STIVALE2_STRUCT_TAG_MEMMAP_ID:
                memmap_tag_ptr = (struct stivale2_struct_tag_memmap*)current_tag;
                stivale2_print_memmap(memmap_tag_ptr);
                break;

            /* Tag not supported */
            default:
                break;
        }
        /* Get Next Tag */
        current_tag = (void*)current_tag->next;
    }
}
