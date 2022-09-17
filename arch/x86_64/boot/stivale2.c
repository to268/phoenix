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
#include <phoenix/kernel.h>
#include <phoenix/stivale2.h>
#include <phoenix/serial.h>
#include <phoenix/framebuffer.h>
#include <stivale2.h>
#include <stddef.h>

static enum video_driver video = VGA_DRIVER;

/* Stack required by the stivale2 specifications */
static u8 stack[4096];

static struct stivale2_header_tag_framebuffer framebuffer_tag = {
    .tag = {
        .identifier = STIVALE2_HEADER_TAG_FRAMEBUFFER_ID,
        .next = 0,
    },
    .framebuffer_width  = 0,
    .framebuffer_height = 0,
    .framebuffer_bpp    = 0
};


/*
static struct stivale2_header_tag_smp smp_tag = {
    .tag = {
        .identifier = STIVALE2_HEADER_TAG_SMP_ID,
        .next = 0,
        //.next = (u64)&framebuffer_tag,
    },
    .flags = 0
};
*/

/* Base Stivale2 header */
SECTION(".stivale2hdr")
struct stivale2_header stivale_hdr = {
    .entry_point = (uptr)&init,
    .stack = (uptr)stack + sizeof(stack),
    .flags = (1 << 1) | (1 << 4),
    .tags = (u64)&framebuffer_tag,
};

inline enum video_driver stivale2_get_video_driver()
{
    return video;
}

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
    info("\tBlue mask size:  %d\n\n", fb_tag->blue_mask_shift);
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
    for (u64 i = 0; i < memmap_tag->entries; i++) {
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

void* stivale2_get_tag(struct stivale2_struct* hdr, u64 id)
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

struct free_memory_hdr
stivale2_get_free_memory(struct stivale2_struct* hdr)
{
    /* Get memmap tag */
    struct stivale2_struct_tag_memmap* memmap_tag;
    memmap_tag = (struct stivale2_struct_tag_memmap*)
                stivale2_get_tag(hdr, STIVALE2_STRUCT_TAG_MEMMAP_ID);

    /* Panic if the stivale2 memmap tag has not been found */
    if (memmap_tag == NULL) {
        panic("No stivale2 memmap tag has been found !\n");
    }

    /* Get free entries */
    struct free_memory_hdr free_hdr;
    struct free_memory entry;
    uptr highest_memory = 0;
    free_hdr.entries = 0;

    for (u64 i = 0; i < memmap_tag->entries; i++) {
        struct stivale2_mmap_entry* current_entry = &memmap_tag->memmap[i];

        free_hdr.total_memory += entry.length;

        if (current_entry->type == STIVALE2_MMAP_USABLE ||
            current_entry->type == STIVALE2_MMAP_KERNEL_AND_MODULES ||
            current_entry->type == STIVALE2_MMAP_BOOTLOADER_RECLAIMABLE) {

            /* Found an Usable Entry */
            entry.base = current_entry->base;
            entry.length = current_entry->length;
            entry.type = current_entry->type;
            free_hdr.segments[free_hdr.entries] = entry;

            debug("[STIVALE2_MEMMAP] Stored a free memory entry\n");

            highest_memory = entry.base + entry.length;

            /* Store the highest free memory */
            if (highest_memory > free_hdr.highest_memory)
                free_hdr.highest_memory = highest_memory;

            free_hdr.free_memory += entry.length;
            free_hdr.entries++;
        }
    }

    /* Panic if no entries are found */
    if (free_hdr.entries <= 0) {
        panic("No free memory has been found !\n");
    }

    return free_hdr;
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
    struct stivale2_struct_tag_framebuffer* fb_tag_ptr = NULL;
    struct stivale2_struct_tag_smp* smp_tag_ptr = NULL;
    struct stivale2_struct_tag_cmdline* cmdline_tag_ptr = NULL;
    struct stivale2_struct_tag_pxe_server_info* pxe_tag_ptr = NULL;
    struct stivale2_struct_tag_memmap* memmap_tag_ptr = NULL;

    /* Loop on all tags */
    for (;;) {
        if (current_tag == NULL)
            break;

        switch (current_tag->identifier) {

            /* Framebuffer Tag */
            case STIVALE2_STRUCT_TAG_FRAMEBUFFER_ID:
                fb_tag_ptr = (struct stivale2_struct_tag_framebuffer*)current_tag;
                framebuffer_init(fb_tag_ptr);
                video = FRAMEBUFFER_DRIVER;
                break;

            /* SMP Tag */
            case STIVALE2_STRUCT_TAG_SMP_ID:
                smp_tag_ptr = (struct stivale2_struct_tag_smp*)current_tag;
                break;

            /* Cmdline Tag */
            case STIVALE2_STRUCT_TAG_CMDLINE_ID:
                cmdline_tag_ptr = (struct stivale2_struct_tag_cmdline*)current_tag;
                break;

            /* PXE Tag */
            case STIVALE2_STRUCT_TAG_PXE_SERVER_INFO:
                pxe_tag_ptr = (struct stivale2_struct_tag_pxe_server_info*)current_tag;
                break;

            /* Memmap Tag */
            case STIVALE2_STRUCT_TAG_MEMMAP_ID:
                memmap_tag_ptr = (struct stivale2_struct_tag_memmap*)current_tag;
                break;

            /* Tag not supported */
            default:
                break;
        }
        /* Get Next Tag */
        current_tag = (void*)current_tag->next;
    }

    /* Print tags info */
    info("Bootloader brand:     %s\n",      hdr->bootloader_brand);
    info("Bootloader version:   %s\n\n",    hdr->bootloader_version);

    if (memmap_tag_ptr != NULL)
        stivale2_print_memmap(memmap_tag_ptr);

    if (fb_tag_ptr != NULL)
        stivale2_print_fb_tag(fb_tag_ptr);

    if (smp_tag_ptr != NULL)
        stivale2_print_smp_tag(smp_tag_ptr);

    if (pxe_tag_ptr != NULL)
        info("PXE IP: %d\n\n", pxe_tag_ptr->server_ip);

    if (cmdline_tag_ptr != NULL)
        info("Cmdline: %s\n\n", (char*)cmdline_tag_ptr->cmdline);
}
