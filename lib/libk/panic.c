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
#include <phoenix/serial.h>

NORETURN void panic(const char* msg) {
    serial_writestring(SERIAL_COM1, "Kernel Panic !\n");
    serial_writestring(SERIAL_COM1, msg);

    printk(KERN_FATAL, "Kernel Panic!\n");
    printk(KERN_FATAL, "%s", msg);

    NORETURN extern void halt_cpu(void);
    halt_cpu();
}
