nasm = find_program('nasm')

asm_sources = [
    'gdt.asm',
    'idt.asm',
    'sse.asm',
    'utils.asm'
]

asm_objs = []

foreach asm_file : asm_sources
    asm_obj = custom_target(
        asm_file + '_obj',
        input: asm_file,
        output: asm_file.replace('.asm', '.o'),
        command: [nasm, '-f', 'elf64', '-o', '@OUTPUT@', '@INPUT@']
    )
    asm_objs += asm_obj
endforeach

arch_asm_lib = static_library('arch_asm', asm_objs)
