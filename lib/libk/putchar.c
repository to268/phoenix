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
#include <phoenix/vga.h>

int putchar(int ic, u8 severity);

int putchar(int ic, u8 severity) {
	char c = (char) ic;
    /* Add color severity */
    switch (severity) {
        case KERN_VERBOSE:
            vga_setcolor(vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));
            break;

        case KERN_INFO:
            vga_setcolor(vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
            break;

        case KERN_WARN:
            vga_setcolor(vga_entry_color(VGA_COLOR_LIGHT_MAGENTA, VGA_COLOR_BLACK));
            break;

        case KERN_ERROR:
            vga_setcolor(vga_entry_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK));
            break;

        case KERN_FATAL:
            vga_setcolor(vga_entry_color(VGA_COLOR_RED, VGA_COLOR_BLACK));
            break;

        default:
            vga_setcolor(vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
            break;
    }
	vga_write(&c, sizeof(c));
	return ic;
}
