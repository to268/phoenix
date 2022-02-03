ENTRY(init)

SECTIONS {
    kernel_phys_offset = 0xffffffff80000000;
    kernel_start = .;
    . = kernel_phys_offset + 0x100000;

    .stivale2hdr ALIGN(4K): {
        KEEP(*(.stivale2hdr))
    }

    .text ALIGN(4K): {
        *(.text)
        *(.text*)
    }

    .rodata ALIGN(4K): {
        *(.rodata)
        *(.rodata*)
    }

    .data ALIGN(4K): {
        *(.data)
        *(.data*)
    }

    .bss ALIGN(4K): {
        *(.bss)
        *(COMMON)
        *(.bss*)
    }

    kernel_end = .;
}
