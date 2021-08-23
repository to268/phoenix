/*
 * Copyright © 2021 Guillot Tony <tony.guillot@protonmail.com>
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
#include <phoenix/serial.h>
#include <phoenix/mem.h>
#include <phoenix/vga.h>
#include <phoenix/io.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static const size_t TAB_LENGTH = 4;
static uint16_t* const VGA_MEMORY = (uint16_t*) 0xB8000;

static size_t vga_row;
static size_t vga_column;
static uint8_t vga_color;
static uint16_t* vga_buffer = VGA_MEMORY;

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

uint16_t vga_cursor_get_pos(void)
{
    uint16_t pos = 0;
    outb(0x3d4, 0x0f);
    pos |= inb(0x3d5);
    outb(0x3d4, 0x0e);
    pos |= ((uint16_t) inb(0x3d5)) << 8;
    return pos;
}

void vga_cursor_set_pos(uint8_t x, uint8_t y)
{
    uint16_t pos = y * VGA_WIDTH + x;
    outb(0x3d4, 0x0f);
    outb(0x3d5, (uint8_t) (pos & 0xff));
    outb(0x3d4, 0x0e);
    outb(0x3d5, (uint8_t) ((pos >> 8) & 0xff));
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

void vga_setcolor(uint8_t color)
{
	vga_color = color;
}

void vga_putentryat(unsigned char c, uint8_t color, size_t x, size_t y)
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
