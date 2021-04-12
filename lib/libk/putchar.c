#include <phoenix/kernel.h>
#include <phoenix/vga.h>
#include <stdint.h>

int putchar(int ic, uint8_t severity);

int putchar(int ic, uint8_t severity) {
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
