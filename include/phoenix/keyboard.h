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
#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <phoenix/kernel.h>
#include <phoenix/pic.h>
#include <stddef.h>

#define KEYBOARD_IRQ        1
#define KEYBOARD_STATE_PORT 0x64
#define KEYBOARD_DATA_PORT  0x60

/* Commands */
#define KEYBOARD_ACK    0xfa
#define KEYBOARD_RESEND 0xfe
#define KEYBOARD_LED    0xed
#define KEYBOARD_DELAY  0xf3

/* Repeating delays */
#define KEYBOARD_DELAY_FASTER   0
#define KEYBOARD_DELAY_FAST     0x1
#define KEYBOARD_DELAY_SLOW     0x2
#define KEYBOARD_DELAY_SLOWEST  0x3
#define KEYBOARD_REPEAT_RATE(x) (x & 0x1f)

/* Special keys */
#define KEYBOARD_ESCAPE      0x1
#define KEYBOARD_LCTRL       0x1d
#define KEYBOARD_LSHIFT      0x2a
#define KEYBOARD_RSHIFT      0x36
#define KEYBOARD_LALT        0x38
#define KEYBOARD_CAPS_LOCK   0x3a
#define KEYBOARD_BACKSPACE   0xe
#define KEYBOARD_NUM_LOCK    0x45
#define KEYBOARD_SCROLL_LOCK 0x46

/* Function keys */
#define KEYBOARD_F1  0x3b
#define KEYBOARD_F10 0x44
#define KEYBOARD_F11 0x57
#define KEYBOARD_F12 0x58

#define KEYBORAD_EXTENDED_CODE 0xe0

#define KEYBOARD_SCROLL_LED (1 << 0)
#define KEYBOARD_NUMPAD_LED (1 << 1)
#define KEYBOARD_CAPS_LED   (1 << 2)

struct keyboard_state {
    u8 extended_scancode;
    u8 uppercase;
    u8 leds;
};

void keyboard_init(void);
void keyboard_handler(void);
void keyboard_change_repeating_delay(u8 repeat_rate, u8 delay);

#ifdef __cplusplus
}
#endif

#endif /* _KEYBOARD_H_*/
