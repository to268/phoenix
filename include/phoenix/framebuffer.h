/*
 * Copyright © 2023 Guillot Tony <tony.guillot@protonmail.com>
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
#ifndef _FRAMEBUFFER_H_
#define _FRAMEBUFFER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <phoenix/kernel.h>
#include <limine.h>

#define GET_BIT(x, x_target) ((1 << 7) >> (x - x_target))

typedef struct {
    u8 red;
    u8 green;
    u8 blue;
} Color;

#define FRAMEBUFFER_COLOR(r, g, b) framebuffer_create_color(r, g, b)

/* Colorscheme */
#define FRAMEBUFFER_COLOR_DARK    FRAMEBUFFER_COLOR(0x00, 0x00, 0x00)
#define FRAMEBUFFER_COLOR_BLUE    FRAMEBUFFER_COLOR(0x80, 0x80, 0xff)
#define FRAMEBUFFER_COLOR_GREEN   FRAMEBUFFER_COLOR(0x8e, 0xc0, 0x7c)
#define FRAMEBUFFER_COLOR_CYAN    FRAMEBUFFER_COLOR(0x83, 0xa5, 0x98)
#define FRAMEBUFFER_COLOR_RED     FRAMEBUFFER_COLOR(0xfb, 0x49, 0x34)
#define FRAMEBUFFER_COLOR_MAGENTA FRAMEBUFFER_COLOR(0xd3, 0x60, 0x9b)
#define FRAMEBUFFER_COLOR_BROWN   FRAMEBUFFER_COLOR(0xb6, 0x4d, 0x0e)
#define FRAMEBUFFER_COLOR_BLACK   FRAMEBUFFER_COLOR(0x28, 0x28, 0x28)
#define FRAMEBUFFER_COLOR_GREY    FRAMEBUFFER_COLOR(0x50, 0x49, 0x45)
#define FRAMEBUFFER_COLOR_ORANGE  FRAMEBUFFER_COLOR(0xfe, 0x80, 0x19)
#define FRAMEBUFFER_COLOR_WHITE   FRAMEBUFFER_COLOR(0xfb, 0xf1, 0xc7)

#define FRAMEBUFFER_COLOR_LIGHT_GREY    FRAMEBUFFER_COLOR(0xeb, 0xdb, 0xb2)
#define FRAMEBUFFER_COLOR_LIGHT_BLUE    FRAMEBUFFER_COLOR(0xab, 0xb8, 0xe6)
#define FRAMEBUFFER_COLOR_LIGHT_GREEN   FRAMEBUFFER_COLOR(0xb8, 0xbb, 0x26)
#define FRAMEBUFFER_COLOR_LIGHT_CYAN    FRAMEBUFFER_COLOR(0xe0, 0xff, 0xff)
#define FRAMEBUFFER_COLOR_LIGHT_RED     FRAMEBUFFER_COLOR(0xff, 0x7f, 0x7f)
#define FRAMEBUFFER_COLOR_LIGHT_MAGENTA FRAMEBUFFER_COLOR(0xd3, 0x86, 0x9b)
#define FRAMEBUFFER_COLOR_LIGHT_BROWN   FRAMEBUFFER_COLOR(0xbd, 0xae, 0x93)

void framebuffer_init(struct limine_framebuffer_request* fb_req);
Color framebuffer_create_color(u8 red, u8 green, u8 blue);
void framebuffer_draw_pixel(u32 x, u32 y, u32 color);
void framebuffer_set_pos(u32 x, u32 y);
void framebuffer_set_color(Color new_fg, Color new_bg);
void framebuffer_write(char c);
void framebuffer_writestring(char* str);
void framebuffer_remove_last_char(void);
void framebuffer_set_color(Color new_fg, Color new_bg);

#ifdef __cplusplus
}
#endif

#endif /* _FRAMEBUFFER_H_ */
