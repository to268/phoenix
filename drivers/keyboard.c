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
#include <phoenix/keyboard.h>
#include <phoenix/pic.h>
#include <phoenix/io.h>

unsigned char keyboard_map[0xff] = {
    0, '\0', '1', '2',
    '3', '4', '5', '6',
    '7', '8', '9', '0',
    '-', '=', '\0', '\t',
    'Q', 'W', 'E', 'R',
    'T', 'Y', 'U', 'I',
    'O', 'P', '[', ']',
    '\n', '\0', 'A', 'S',
    'D', 'F', 'G', 'H',
    'J', 'K', 'L', ';',
    '\'', '`', '\0', '\\',
    'Z', 'X', 'C', 'V',
    'B', 'N', 'M', ',',
    '.', '/', '\0', '\0',
    '\0', ' ', '\0', '\0',
    '\0', '\0', '\0', '\0',
    '\0', '\0', '\0', '\0',
    '\0', '\0', '\0', '7',
    '8', '9', '-', '4',
    '5', '6', '+', '1',
    '2', '3', '0', '.',
    0, 0, 0, '\0',
    '\0', 0, 0, 0,
};

void keyboard_handler(void)
{
    u8 state = inb(KEYBOARD_STATE_PORT);

    /* Process keyboard data if the state is correct */
    if (state & 1 && (state & 0x20) == 0) {
        u8 keycode = inb(KEYBOARD_DATA_PORT);
        u8 scancode = keycode & 0x7f;
        u8 keystate = !(keycode & 0x80);

        if (keystate)
            info("%c", keyboard_map[scancode]);
    }

    pic_send_eoi(KEYBOARD_IRQ);
}
