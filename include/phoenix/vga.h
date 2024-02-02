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
#ifndef _VGA_H_
#define _VGA_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <phoenix/kernel.h>
#include <stddef.h>

enum ENUM_EXTENSIBILITY(closed) FLAG_ENUM vga_color : u8{
    VGA_COLOR_BLACK = 0,        VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,        VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,          VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,        VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10, VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,   VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14, VGA_COLOR_WHITE = 15,
};

void vga_init(void);
void vga_clear(void);
void vga_cursor_disable(void);
void vga_cursor_enable(void);
NODISCARD u16 vga_cursor_get_pos(void);
void vga_cursor_set_pos(u8 x, uint8_t y);
void vga_setcolor(u8 color);
void vga_putchar(const char c);
void vga_write(const char* data, size_t size);
void vga_writestring(const char* string);
void vga_remove_last_char(void);

NODISCARD static inline u8 vga_entry_color(enum vga_color fg,
                                           enum vga_color bg) {
    return fg | bg << 4;
}

NODISCARD static inline u16 vga_entry(unsigned char uc, u8 color) {
    return (u16)uc | (uint16_t)color << 8;
}

#ifdef __cplusplus
}
#endif

#endif /* _VGA_H_*/
