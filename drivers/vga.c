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
#include <phoenix/serial.h>
#include <phoenix/mem.h>
#include <phoenix/vga.h>
#include <phoenix/io.h>
#include <stddef.h>
#include <stdbool.h>

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static const size_t TAB_LENGTH = 4;
static u16* const VGA_MEMORY = (u16*) 0xB8000;

static size_t vga_row;
static size_t vga_column;
static u8 vga_color;
static u16* vga_buffer = VGA_MEMORY;

void vga_cursor_enable(void)
{
    outb(0x3d4, 0x0a);
    outb(0x3d5, (inb(0x3d5) & 0xc0) | 14);
    outb(0x3d4, 0x0b);
    outb(0x3d5, (inb(0x3d5) & 0xe0) | 15);
}

void vga_cursor_disable(void)
{
    outb(0x3d4, 0x0a);
    outb(0x3d5, 0x20);
}

u16 vga_cursor_get_pos(void)
{
    u16 pos = 0;
    outb(0x3d4, 0x0f);
    pos |= inb(0x3d5);
    outb(0x3d4, 0x0e);
    pos |= ((u16) inb(0x3d5)) << 8;
    return pos;
}

void vga_cursor_set_pos(u8 x, uint8_t y)
{
    u16 pos = y * VGA_WIDTH + x;
    outb(0x3d4, 0x0f);
    outb(0x3d5, (u8) (pos & 0xff));
    outb(0x3d4, 0x0e);
    outb(0x3d5, (u8) ((pos >> 8) & 0xff));
}

void vga_clear(void)
{
	vga_row = 0;
	vga_column = 0;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			vga_buffer[index] = vga_entry(' ', vga_color);
		}
	}
}

void vga_init(void)
{
	vga_row = 0;
	vga_column = 0;
	vga_color = vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);

    /* Apply the selected color to the screen */
    vga_clear();
    /* enable cursor */
    vga_cursor_enable();

    debug("[VGA] Initialized\n");
}

void vga_setcolor(u8 color)
{
	vga_color = color;
}

void vga_putentryat(unsigned char c, u8 color, size_t x, size_t y)
{
	const size_t index = y * VGA_WIDTH + x;
	vga_buffer[index] = vga_entry(c, color);
}

void vga_putchar(const char c)
{
    unsigned char uc = c;
    /* Check if the cursor is on the last line to auto scroll */
    if (vga_cursor_get_pos() >= VGA_WIDTH * (VGA_HEIGHT - 1)) {
        size_t i = VGA_WIDTH;
        for (; i < VGA_WIDTH * VGA_HEIGHT; i++)
	        vga_buffer[i - VGA_WIDTH] = vga_buffer[i];
        vga_row--;
        vga_column = 0;
    }

    /* Check for special characters */
    if (!strcmp(&c, "\n")) {
        vga_row++;
        vga_column = 0;
    } else if (!strcmp(&c, "\t")) {
        vga_column += TAB_LENGTH;
    } else if (!strcmp(&c, "\r")) {
        vga_column = 0;
    } else {
        vga_putentryat(uc, vga_color, vga_column, vga_row);

        /* Create a new line if the cursor is at the end of the line */
        if (vga_column == VGA_WIDTH - 1) {
            vga_row++;
            vga_column = 0;
        } else {
            vga_column++;
        }
    }
}

void vga_write(const char* data, size_t size)
{
	for (size_t i = 0; i < size; i++)
		vga_putchar(data[i]);
    vga_cursor_set_pos(vga_column, vga_row);
}

void vga_writestring(const char* string)
{
	vga_write(string, strlen(string));
    vga_cursor_set_pos(vga_column, vga_row);
}

void vga_remove_last_char(void)
{
    /* Check if the cursor is at the beginning of the row */
    if (vga_column == 0) {
        vga_row--;
        vga_column = VGA_WIDTH - 1;
    } else {
        vga_column--;
    }

    /* Make sure to update the cursor */
    vga_cursor_set_pos(vga_column, vga_row);

    /* Clear the character */
    size_t index = (vga_row * VGA_WIDTH) + vga_column;
    vga_buffer[index] = vga_entry(' ', vga_color);
}
