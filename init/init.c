#include <phoenix/stivale2.h>
#include <phoenix/kernel.h>
#include <phoenix/serial.h>
#include <phoenix/gdt.h>
#include <phoenix/pmm.h>
#include <phoenix/vga.h>
#include <stdint.h>
#include <stddef.h>

/* Kernel early entry point */
void init(struct stivale2_struct* hdr)
{
    /*  TODO list:
     *  Init PMM
     *  Init IDT
     *  Load Keyboard Driver
     */

    /* Initialize serial */
    serial_init(SERIAL_COM1, 1);

    /* Initialize vga buffer */
    vga_init();

    /* Process Each supported tag */
    stivale2_process_tags(hdr);

    /* Init GDT */
    gdt_init();

    /* Init pmm */
    pmm_init(hdr);

    /* Jump in kernel_main */
    kernel_main();
}
