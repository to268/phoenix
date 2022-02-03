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
#include <phoenix/serial.h>
#include <phoenix/gdt.h>

/* Flags */
#define GRANULARITY_BIT (1 << 0x0f)             /* Granularity (0: 1B - 1MB, 1: 4kB - 4GB) */
#define SIZE_BIT        (1 << 0x0e)             /* Size (0: 16 bit, 1: 32 bit) */
#define LONG_BIT        (1 << 0x0d)             /* Long mode */
#define FREE_BIT        (1 << 0x0c)             /* Free to use */
#define PRESENT_BIT     (1 << 0x07)             /* Present */
#define PRIVILEGE(x)    ((x & 0x03) << 0x05)    /* Privilege level (0 - 3) */
#define DESCRIPTOR_BIT  (1 << 0x04)             /* Descriptor type (0: system, 1: code/data) */
#define EX_BIT          (1 << 0x03)             /* Executable bit */
#define RW_BIT          (1 << 0x01)             /* Read/Write bit */

/* Descriptors */
#define KERNEL_CODE     (GRANULARITY_BIT  | LONG_BIT | \
                        PRESENT_BIT | PRIVILEGE(0) | DESCRIPTOR_BIT | \
                        RW_BIT | EX_BIT)

#define KERNEL_DATA     (GRANULARITY_BIT  | SIZE_BIT | \
                        PRESENT_BIT | PRIVILEGE(0) | DESCRIPTOR_BIT | \
                        RW_BIT)

#define USERSPACE_CODE  (GRANULARITY_BIT | LONG_BIT | \
                        PRESENT_BIT | PRIVILEGE(3) | DESCRIPTOR_BIT | \
                        RW_BIT | EX_BIT)

#define USERSPACE_DATA  (GRANULARITY_BIT | SIZE_BIT | \
                        PRESENT_BIT | PRIVILEGE(3) | DESCRIPTOR_BIT | \
                        RW_BIT)

/* Initializing GDT structs */
static struct gdt_descriptor gdt[5];
static struct gdt_pointer gdt_ptr = {.base = (u64)gdt, .limit = sizeof(gdt) - 1};

void create_descriptor(u32 base, uint32_t limit, u16 access,
                        struct gdt_descriptor* descriptor)
{
    /* Access */
    descriptor->access      =   (access & 0x00ff);

    /* Base bits */
    descriptor->base_low    =   (base & 0x0000ffff);        /* set base bits 15:0 */
    descriptor->base_mid    =   (base & 0x00ff0000) >> 16;  /* set base bits 23:16 */
    descriptor->base_high   =   (base & 0xff000000) >> 24;  /* set base bits 31:24 */

    /* Limit bits */
    descriptor->limit_low   =   (limit & 0x0ffff);          /* set limit bits 15:0 */
    descriptor->limit_flags =   (limit & 0xf0000) >> 16;    /* set limit bits 19:16 */

    /* Flags */
    descriptor->limit_flags |=   (access & 0xf000) >> 8;
}

void gdt_init(void)
{
    create_descriptor(0, 0, 0, &gdt[0]);
    create_descriptor(0, 0xfffff, KERNEL_CODE,      &gdt[1]);
    create_descriptor(0, 0xfffff, KERNEL_DATA,      &gdt[2]);
    create_descriptor(0, 0xfffff, USERSPACE_CODE,   &gdt[3]);
    create_descriptor(0, 0xfffff, USERSPACE_DATA,   &gdt[4]);
    /* Add TSS descriptor here */

    debug("[GDT] Created Descriptors\n");

    /* Load GDT */
    extern void gdt_flush(u64);
    gdt_flush((u64)&gdt_ptr);

    debug("[GDT] Loaded\n");
}
