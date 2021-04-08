#include <phoenix/gdt.h>
#include <stdint.h>

void gdt_init(void);

// Initializing basic GDT structs
static struct gdt_descriptor gdt[5];
struct gdt_pointer gdt_ptr = {.limit = sizeof(gdt) - 1, .base = (uint64_t)gdt};

void gdt_init(void) {
    // GDT descriptor 0 (null)
    gdt[0] = (struct gdt_descriptor) {.flags = 0};

    // GDT descriptor 1 (kernel code)
    gdt[1] = (struct gdt_descriptor) {.flags =  LONG_BIT | PRESENT_BIT | PRIVILEGE(0)
                                                | DESCRIPTOR_BIT | EX_BIT | RW_BIT};
    // GDT descriptor 2 (kernel data)
    gdt[2] = (struct gdt_descriptor) {.flags =  LONG_BIT | PRESENT_BIT | PRIVILEGE(0)
                                                | DESCRIPTOR_BIT | RW_BIT};
    // GDT descriptor 3 (userspace code)
    gdt[3] = (struct gdt_descriptor) {.flags =  LONG_BIT | PRESENT_BIT | PRIVILEGE(3)
                                                | DESCRIPTOR_BIT | EX_BIT | RW_BIT};
    // GDT descriptor 4 (userspace data)
    gdt[4] = (struct gdt_descriptor) {.flags =  LONG_BIT | PRESENT_BIT | PRIVILEGE(3)
                                                | DESCRIPTOR_BIT | RW_BIT};
    // Load GDT
    asm volatile("lgdt %0"
                :
                : "m"(gdt_ptr)
                : "memory");

    // Reload GDT
    asm volatile(
                "mov %%rsp, %%rax\n"
                "push $0x10\n"
                "push %%rax\n"
                "pushf\n"
                "push $0x8\n"
                "push $1f\n"
                "iretq\n"
                "1:\n"
                "mov $0x10, %%ax\n"
                "mov %%ax, %%ds\n"
                "mov %%ax, %%es\n"
                "mov %%ax, %%ss\n"
                "mov %%ax, %%fs\n"
                "mov %%ax, %%gs\n"
                :
                :
                : "rax", "memory");
}
