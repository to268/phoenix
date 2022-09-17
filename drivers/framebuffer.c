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
#include <phoenix/framebuffer.h>
#include <phoenix/serial.h>
#include <phoenix/stivale2.h>
#include <phoenix/libpsf.h>

static struct stivale2_struct_tag_framebuffer* fb_tag;
static u32* fb_addr;
static u32 x_pos;
static u32 y_pos;
static u32 fb_scroll;
static u32 fb_size;
static u32 fb_max_y;
static PSF1_font font;

static Color bg;
static Color fg;

void framebuffer_init(struct stivale2_struct_tag_framebuffer* fb);
Color framebuffer_create_color(u8 red, u8 green, u8 blue);
u32 framebuffer_to_color(Color* color);
void framebuffer_draw_pixel(u32 x, u32 y, u32 color);
void framebuffer_clear(Color* color);
void framebuffer_set_pos(u32 x, u32 y);
void framebuffer_write(char c);
void framebuffer_writestring(char* str);
void framebuffer_remove_last_char(void);

void framebuffer_init(struct stivale2_struct_tag_framebuffer* fb)
{
    framebuffer_set_pos(0, 0);
    fb_tag  = fb;
    fb_addr = (u32*)fb_tag->framebuffer_addr;
    fb_size = fb_tag->framebuffer_width * fb_tag->framebuffer_height;

    fg = FRAMEBUFFER_COLOR_WHITE;
    bg = FRAMEBUFFER_COLOR_DARK;

    framebuffer_clear(&bg);
    font = psf1_init();

    fb_scroll = fb_size - (fb_tag->framebuffer_width * font.header->charsize);
    fb_max_y = fb_tag->framebuffer_height - font.header->charsize;
}

Color framebuffer_create_color(u8 red, u8 green, u8 blue)
{
    return (Color){red, green, blue};
}

u32 framebuffer_to_color(Color* color)
{
    return (color->red << 16) + (color->green << 8) + color->blue;
}

void framebuffer_draw_pixel(u32 x, u32 y, u32 color)
{
    u32 pixel = (fb_tag->framebuffer_pitch / sizeof(u32) * y) + x;
    fb_addr[pixel] = color;
}

void framebuffer_clear(Color* color)
{
    for (int y = 0; y < fb_tag->framebuffer_height; y++) {
        for (int x = 0; x < fb_tag->framebuffer_width; x++) {
            framebuffer_draw_pixel(x, y, framebuffer_to_color(color));
        }
    }
}

void framebuffer_set_pos(u32 x, u32 y)
{
    x_pos = x;
    y_pos = y;
}

void framebuffer_write(char c)
{
    /* Check if we are on the last line to auto scroll */
    if (x_pos * y_pos >= fb_scroll || y_pos >= fb_max_y) {
        for (u32 i = 0; i < font.header->charsize; i++) {
            u32 j = fb_tag->framebuffer_width;
            for (; j < fb_size; j++)
                fb_addr[j - fb_tag->framebuffer_width] = fb_addr[j];
        }

        /* Clear last row to avoid weird pixels */
        u32 i = fb_size - (fb_tag->framebuffer_width * font.header->charsize);
        for (; i < fb_size; i++) {
            u32 x = i % fb_tag->framebuffer_width;
            u32 y = i / fb_tag->framebuffer_width;
            framebuffer_draw_pixel(x, y, framebuffer_to_color(&bg));
        }

        x_pos = 0;
        y_pos = fb_max_y - font.header->charsize;
    } else if (x_pos >= fb_tag->framebuffer_width) {
        x_pos = 0;
        y_pos += font.header->charsize;
    }

    if (c == '\n') {
        x_pos = 0;
        y_pos += font.header->charsize;
        return;
    } else if (c == '\t') {
        x_pos += (fb_tag->framebuffer_bpp / sizeof(u32) * 4);
        return;
    } else if (c == '\r') {
        x_pos = 0;
        return;
    }

    u8* font_ptr = font.data + (c * font.header->charsize);
    for (u32 y = y_pos; y < y_pos + font.header->charsize; y++) {
        for (u32 x = x_pos; x < x_pos + 8; x++) {
            if ((*font_ptr & GET_BIT(x, x_pos)) > 0) {
                framebuffer_draw_pixel(x, y, framebuffer_to_color(&fg));
            } else {
                framebuffer_draw_pixel(x, y, framebuffer_to_color(&bg));
            }
        }
        font_ptr++;
    }

    x_pos += (fb_tag->framebuffer_bpp / sizeof(u32));
}

void framebuffer_writestring(char* str)
{
    while (*str) {
        framebuffer_write(*(str++));
    }
}

void framebuffer_remove_last_char(void)
{
    if (x_pos <= 0 && y_pos != 0) {
        x_pos = fb_tag->framebuffer_width;
        y_pos -= font.header->charsize;
    }

    x_pos -= (fb_tag->framebuffer_bpp / sizeof(u32));

    for (u32 y = y_pos; y < y_pos + font.header->charsize; y++) {
        for (u32 x = x_pos; x < x_pos + (fb_tag->framebuffer_bpp / sizeof(u32)); x++) {
            framebuffer_draw_pixel(x, y, framebuffer_to_color(&bg));
        }
    }
}

void framebuffer_set_color(Color new_fg, Color new_bg)
{
    fg = new_fg;
    bg = new_bg;
}
