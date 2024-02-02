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
#include <phoenix/framebuffer.h>
#include <phoenix/limine.h>
#include <phoenix/vga.h>

MAYBE_UNUSED int putchar(int ic, u8 severity) {
    char c = (char)ic;
    /* Add color severity */
    switch (severity) {
    case KERN_VERBOSE:
        framebuffer_set_color(FRAMEBUFFER_COLOR_GREY, FRAMEBUFFER_COLOR_DARK);
        break;

    case KERN_INFO:
        framebuffer_set_color(FRAMEBUFFER_COLOR_WHITE, FRAMEBUFFER_COLOR_DARK);
        break;

    case KERN_WARN:
        framebuffer_set_color(FRAMEBUFFER_COLOR_MAGENTA,
                              FRAMEBUFFER_COLOR_DARK);
        break;

    case KERN_ERROR:
        framebuffer_set_color(FRAMEBUFFER_COLOR_ORANGE, FRAMEBUFFER_COLOR_DARK);
        break;

    case KERN_FATAL:
        framebuffer_set_color(FRAMEBUFFER_COLOR_RED, FRAMEBUFFER_COLOR_DARK);
        break;

    default:
        framebuffer_set_color(FRAMEBUFFER_COLOR_WHITE, FRAMEBUFFER_COLOR_DARK);
        break;
    }

    framebuffer_write(c);
    return ic;
}
