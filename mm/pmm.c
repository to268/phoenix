#include <phoenix/stivale2.h>
#include <phoenix/kernel.h>
#include <phoenix/serial.h>
#include <phoenix/pmm.h>

void pmm_init(struct stivale2_struct* hdr);

void pmm_init(struct stivale2_struct* hdr)
{
    /* Get usable memory info */
    struct usable_memory_hdr usable_hdr;
    usable_hdr = stivale2_get_usable_memory(hdr);

    /* Print Usable memory / total memory status for now */
    uintptr_t usable_mem = convert_to_mb(usable_hdr.usable_memory);
    uintptr_t total_mem = convert_to_mb(usable_hdr.total_memory);
    info("\n%d/%d Mb is usable\n\n", usable_mem, total_mem);

    debug("[PMM] Initialized\n");
}
