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
#include <phoenix/keyboard_layouts.h>
#include <phoenix/kernel.h>
#include <phoenix/keyboard.h>
#include <phoenix/pic.h>
#include <phoenix/vga.h>
#include <phoenix/io.h>

static struct keyboard_state kb_state;

static void keybard_toggle_led(u8 led)
{
    kb_state.leds ^= led;

    outb(KEYBOARD_DATA_PORT, KEYBOARD_LED);
    while (inb(KEYBOARD_DATA_PORT) != KEYBOARD_ACK);
    outb(KEYBOARD_DATA_PORT, kb_state.leds);
}

static void handle_function_keys(u8 scancode)
{
    if (scancode <= KEYBOARD_F10)
        info("<F%d>", (scancode - KEYBOARD_F1) + 1);
    else
        info("<F%d>", (scancode - KEYBOARD_F11) + 11);
}

static u8 keyboard_handle_special_press(u8 scancode)
{
    switch (scancode) {
        case KEYBOARD_ESCAPE:
            info("<ESC>");
            return 1;

        case KEYBOARD_LCTRL:
            info("<LCTRL>");
            return 1;

        case KEYBOARD_LSHIFT:
            kb_state.uppercase = !kb_state.uppercase;
            return 1;

        case KEYBOARD_LALT:
            info("<LALT>");
            return 1;

        case KEYBOARD_NUM_LOCK:
            keybard_toggle_led(KEYBOARD_NUMPAD_LED);
            return 1;

        case KEYBOARD_SCROLL_LOCK:
            keybard_toggle_led(KEYBOARD_SCROLL_LED);
            return 1;

        case KEYBOARD_CAPS_LOCK:
            kb_state.uppercase = !kb_state.uppercase;
            keybard_toggle_led(KEYBOARD_CAPS_LED);
            return 1;

        case KEYBOARD_BACKSPACE:
            /* TODO: Make a generic function to remove the last character  */
            vga_remove_last_char();
            return 1;

        case KEYBORAD_EXTENDED_CODE:
            kb_state.extended_scancode = 1;
            return 1;
    }

    /* Check if it's a function key */
    if ((scancode >= KEYBOARD_F1 && scancode <= KEYBOARD_F10) ||
        (scancode == KEYBOARD_F11 || scancode == KEYBOARD_F12))
    {
        handle_function_keys(scancode);
        return 1;
    }

    return 0;
}

static u8 keyboard_handle_special_release(u8 scancode)
{
    switch (scancode) {
        case KEYBOARD_ESCAPE:
        case KEYBOARD_LALT:
        case KEYBOARD_BACKSPACE:
        case KEYBOARD_CAPS_LOCK:
        case KEYBOARD_NUM_LOCK:
        case KEYBOARD_SCROLL_LOCK:
            return 1;

        case KEYBOARD_LCTRL:
            info("<LCTRL>");
            return 1;

        case KEYBOARD_LSHIFT:
            kb_state.uppercase = !kb_state.uppercase;
            return 1;
    }

    return 0;
}


static u8 keyboard_handle_special(u8 scancode, u8 keystate)
{
    /* info("scancode %x state %d\n", scancode, keystate); */

    /* TODO: Handle function keys */
    if (keystate)
        return keyboard_handle_special_press(scancode);
    else
        return keyboard_handle_special_release(scancode);
}

static void keyboard_handle_normal(u8 scancode, u8 keystate)
{
    if (keystate) {
        if (!kb_state.uppercase)
            info("%c", keyboard_map_lowercase[scancode]);
        else
            info("%c", keyboard_map_uppercase[scancode]);
    }
}

static void keyboard_handle_extended_scancode(u8 scancode, u8 keystate)
{
    /* TODO: Add extended scancodes support */
    warn("extended scancode 0x%x with state %d is not supported\n", scancode, keystate);
    kb_state.extended_scancode = 0;
}

void keyboard_handler(void)
{
    u8 state = inb(KEYBOARD_STATE_PORT);

    /* Process keyboard data if the state is correct */
    if (state & 1 && (state & 0x20) == 0) {
        u8 keycode = inb(KEYBOARD_DATA_PORT);
        u8 scancode = keycode & 0x7f;
        u8 keystate = !(keycode & 0x80);

        /* Handle extended keys */
        if (keycode == KEYBORAD_EXTENDED_CODE) {
            kb_state.extended_scancode = 1;
            goto end;
        } else if (kb_state.extended_scancode) {
            keyboard_handle_extended_scancode(scancode, keystate);
            goto end;
        }

        /* Handle non printable keys */
        if (keyboard_handle_special(scancode, keystate))
            goto end;

        /* handle normal keys */
        keyboard_handle_normal(scancode, keystate);
    }

end:
    pic_send_eoi(KEYBOARD_IRQ);
}
