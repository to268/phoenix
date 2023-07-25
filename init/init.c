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
#include <phoenix/limine.h>
#include <phoenix/keyboard.h>
#include <phoenix/kernel.h>
#include <phoenix/serial.h>
#include <phoenix/pcspk.h>
#include <phoenix/boot.h>
#include <phoenix/gdt.h>
#include <phoenix/idt.h>
#include <phoenix/pit.h>
#include <phoenix/rtc.h>
#include <phoenix/pmm.h>
#include <phoenix/vmm.h>
#include <phoenix/vga.h>
#include <stddef.h>

void init(void) {
    struct boot_info boot_info;

    serial_init(SERIAL_COM1, 1);
    limine_handle_requests(&boot_info);
    gdt_init();
    idt_init();
    keyboard_init();
    pit_init(1000);
    pcspk_init();
    rtc_init();
    pmm_init(&boot_info);
    /* vmm_init(&boot_info); */
    kernel_main();
}
