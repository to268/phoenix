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
#include <phoenix/pit.h>
#include <phoenix/rtc.h>
#include <phoenix/serial.h>
#include <phoenix/mem.h>
#include <phoenix/io.h>
#include <cpuid.h>

void kernel_main(void) {
    rtc_print_date_time();
    info("Startup took %d msecs\n", pit_get_ticks());
    info("\n");
    info("Welcome to Phoenix !\n");

    /* Halt CPU */
    NORETURN extern void halt_cpu(void);
    halt_cpu();
}
