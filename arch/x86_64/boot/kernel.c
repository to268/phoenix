#include <phoenix/kernel.h>
#include <phoenix/serial.h>
#include <phoenix/vga.h>
#include <cpuid.h>

void kernel_main(void)
{
    info("Welcome to Phoenix !\n");

    /* Halt CPU */
    extern void halt_cpu(void);
    halt_cpu();
}
