ENTRY(init)

SECTIONS {
    kernel_phys_offset = 0xffffffff80200000;
    . = kernel_phys_offset;

    .stivale2hdr : {
        KEEP(*(.stivale2hdr))
    }

    .text : {
        *(.text*)
    }

    .rodata : {
        *(..rodata.*)
    }

    .data : {
        *(.data*)
    }

    .bss : {
        *(COMMON)
        *(.bss*)
    }

    kernel_end = .;
}
