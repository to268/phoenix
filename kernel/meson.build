subdir('idt')

c_sources = [
  'gdt.c',
  'nmi.c',
  'pic.c'
]

kernel_lib = static_library('kernel', c_sources)

kernel_libs = [kernel_lib, idt_lib]
