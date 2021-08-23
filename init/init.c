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
#include <phoenix/stivale2.h>
#include <phoenix/kernel.h>
#include <phoenix/serial.h>
#include <phoenix/gdt.h>
#include <phoenix/idt.h>
#include <phoenix/pmm.h>
#include <phoenix/vga.h>
#include <stdint.h>
#include <stddef.h>

/* Kernel early entry point */
void init(struct stivale2_struct* hdr)
{
    /*  TODO list:
     *  Init PMM
     *  Init IDT
     *  Load Keyboard Driver
     */

    /* Initialize serial */
    serial_init(SERIAL_COM1, 1);

    /* Initialize vga buffer */
    vga_init();

    /* Process Each supported tag */
    stivale2_process_tags(hdr);

    /* Init GDT */
    gdt_init();

    /* Init IDT */
    idt_init();

    /* Init pmm */
    pmm_init(hdr);

    /* Jump in kernel_main */
    kernel_main();
}
