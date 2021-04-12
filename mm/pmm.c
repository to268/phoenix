#include <phoenix/stivale2.h>
#include <phoenix/kernel.h>
#include <phoenix/serial.h>
#include <phoenix/pmm.h>

void pmm_init(struct stivale2_struct* hdr);

void pmm_init(struct stivale2_struct* hdr)
{
    struct usable_memory_hdr usable_hdr;
    usable_hdr = stivale2_get_usable_memory(hdr);

    /* Print Usable memory / total memory status for now */
    info("\n%d/%d Mb is usable\n\n", (usable_hdr.usable_memory/1024/1024),
    (usable_hdr.total_memory/1024/1024));

    debug("[PMM] initialized\n");
}
