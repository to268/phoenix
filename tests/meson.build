nasm = find_program('nasm')

c_sources = [
  'tests_runner.c',
  'test_sse.c',
]

asm_sources = [
  'test_sse.asm',
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

tests_lib = static_library('tests', [c_sources, asm_objs])
