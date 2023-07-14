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
#include <phoenix/io.h>
#include <phoenix/pit.h>
#include <phoenix/serial.h>

void pcspk_init(void)
{
    outb(0x61, inb(0x61) | 0x1);
    serial_writestring(SERIAL_COM1, "[PCSPK] Initialized\n");
}

void pcspk_play(u32 hz)
{
    u32 div = PIT_FREQ / hz;

    /* Set the PIT to the desired frequency */
    cli();
    outb(0x43, 0xb6);
    outb(0x42, (u8)(div & 0xff));
    outb(0x42, (u8)(div >> 8));
    sti();

    /* Play the sound with the PC speaker */
    u32 tmp = inb(0x61);
    if (tmp != (tmp | 3))
        outb(0x61, tmp | 3);
}

void pcspk_stop(void) { outb(0x61, inb(0x61) & 0xfc); }

void pcspk_beep(u32 hz, u32 time)
{
    pcspk_play(hz);
    pit_sleep(time);
    pcspk_stop();
}
