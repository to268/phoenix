c_sources = [
    'psf.c'
]

font_target = custom_target(
  'font_obj',
  input: 'font.psf',
  output: 'font.o',
  command: [
      'llvm-objcopy',
      '-O',
      'elf64-x86-64',
      '-B',
      'i386',
      '-I',
      'binary',
      '@INPUT@',
      '@OUTPUT@'
  ],
)

lib_psf = static_library('psf', [c_sources, font_target])
