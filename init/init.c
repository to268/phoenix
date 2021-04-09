#include <phoenix/kernel.h>
#include <phoenix/stivale.h>
#include <phoenix/serial.h>
#include <phoenix/gdt.h>
#include <phoenix/vga.h>
#include <phoenix/page.h>
#include <stdint.h>
#include <stddef.h>

// Kernel early entry point
void init(struct stivale2_struct* hdr) {
    /*  TODO list:
    *   Init IDT
    *   Init PMM
    *   Load Keyboard Driver
    */

    // Initialize serial
    serial_init(SERIAL_COM1, 1);

    // Initialize vga buffer
    vga_init();

    // Bootloader Info
    info("Bootloader brand:     %s\n",      hdr->bootloader_brand);
    info("Bootloader version:   %s\n\n",    hdr->bootloader_version);

    // Check for tags
    if ((void*)hdr->tags == NULL) {
        warn("No tag found !\n");
    } else {
        stivale2_print_tags(hdr);
    }

    // Init GDT
    gdt_init();

    // Init Paging
    page_init();

    // Jump in kernel_main
    kernel_main();
}
