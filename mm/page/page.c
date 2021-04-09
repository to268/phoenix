#include <phoenix/kernel.h>
#include <phoenix/serial.h>
#include <phoenix/page.h>
#include <stdint.h>
#include <stddef.h>

// Page Directory Pointer Table
static uint64_t page_dir_ptr_tab[4] __attribute__((aligned(32)));

// Page Directory
static uint64_t page_directory[512] __attribute__((aligned(0x1000)));

void page_init(void) {
    page_dir_ptr_tab[0] = (uint64_t)&page_directory | 1;
    page_directory[0] = 0b10000011; //Address=0, 2MIB, RW and present
    debug("Page Directory Setup\n");

    extern void load_page_directory_ptr(unsigned int*);
    extern void enable_pae();
    extern void enable_paging();

    enable_pae();
    debug("PAE enabled\n");
    /*load_page_directory_ptr((unsigned int*)page_dir_ptr_tab);
    debug("Page Directory ptr Loaded\n");
    enable_paging();
    debug("Page Directory Loaded\n");*/
}
