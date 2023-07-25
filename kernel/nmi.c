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
#include <phoenix/nmi.h>
#include <phoenix/io.h>

void nmi_enable(void) {
    outb(0x70, inb(0x70) & 0x7f);
    inb(0x71);
}

void nmi_disable(void) {
    outb(0x70, inb(0x70) | 0x80);
    inb(0x71);
}

/*
 * Store control port values with 16 bits where
 * - bits 0-7: Control port A
 * - bits 8-15: Control port B
 */
u16 nmi_get_control_ports(void) { return (inb(0x61) << 8) | inb(0x92); }
