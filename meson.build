project(
  'phoenix',
  'c',
  default_options: [
    'c_std=c23',
    'warning_level=2',
    'werror=false',
    'b_staticpic=false',
    'default_library=static'
  ],
  meson_version: '>=1.1.0',
  version: '0.3.3-alpha',
)

native_compiler = meson.get_compiler('c', native: true)
cross_compiler = meson.get_compiler('c', native: false)
arch = target_machine.cpu_family()

if not meson.is_cross_build()
  error('Native build support: ', 'native build support is not supported')
endif

if not meson.can_run_host_binaries()
  error('Run host binaries: ', 'host system should be able to run binaries')
endif

if cross_compiler.get_id() == 'clang'
  if cross_compiler.version().version_compare('<18')
    error('Unsupported Clang verison: ', 'version should be at least 18')
  endif

else
  error('Unsupported C compiler: ', 'Only Clang is supported')
endif

# Basic compiler flags
add_project_arguments(
  [
    '-Wimplicit-fallthrough',
    '-Wunused',
    '-ffreestanding',
    '-fno-stack-protector',
    '-fno-stack-check',
    '-fno-PIE',
    '-fno-PIC',
    '-mno-80387',
    '-mno-mmx',
    '-mno-sse',
    '-mno-sse2',
    '-mno-red-zone',
    '-mcmodel=kernel',
    '--sysroot=' + meson.current_source_dir(),
    '-isystem=/include'
  ],
  language: 'c'
)

# Basic linker flags
add_project_link_arguments(
  [
    '-T',
    meson.current_source_dir() / 'arch' / arch / 'limine.ld',
    '-nostdlib',
    '-static',
    '-z',
    'max-page-size=0x1000'
  ],
  language: 'c'
)

# Set additional flags depending of build type
buildtype = get_option('buildtype')

if buildtype == 'release'
  add_project_arguments(
    [
      '-flto'
    ],
    language: 'c'
  )

  add_project_link_arguments(
    [
      '-flto'
    ],
    language: 'c'
  )
endif

# Add compile options
add_project_arguments(
  [
    '-DARCH_' + arch.to_upper(),
    '-DKEYBOARD_' + get_option('keyboard_layout').to_upper()
  ],
  language: 'c'
)

# Add test configuration if necessary
if get_option('enable_tests')
  add_project_arguments(
    [
      '-DTEST_ENV'
    ],
    language: 'c'
  )
endif

subdir('arch' / arch)
subdir('drivers')
subdir('kernel')
subdir('lib')
subdir('mm')

if get_option('enable_tests')
  subdir('tests')
endif

libs = []

foreach lib: arch_libs
  libs += lib
endforeach

foreach lib: kernel_libs
  libs += lib
endforeach

foreach lib: custom_libs
  libs += lib
endforeach

libs += drivers_lib
libs += mm_lib

if get_option('enable_tests')
  libs += tests_lib
endif

# Links kernel
kernel = executable('phoenix.elf', ['init/init.c'], link_with: libs)

# List kernel symbols
nm = find_program('nm')

kernel_mn = custom_target(
  'kernel_nm',
  output: 'phoenix.elf.map',
  command: [nm, '-n', kernel],
  depends: kernel,
  build_by_default : true,
  capture: true
)

# Creates iso
b2sum_bin = find_program('b2sum')
xorriso_bin = find_program('xorriso')
python_bin = find_program('python3', 'python')

limine_dir = meson.current_source_dir() / 'utils' / 'limine'
iso_dir = meson.current_build_dir() / 'isodir'
iso_file = meson.current_build_dir() / meson.project_name() + '-' + arch + '.iso'
create_iso_script = meson.current_source_dir() / 'utils' / 'scripts' / 'create_iso.py'

kernel_iso = custom_target(
  'kernel_iso',
  output: meson.project_name() + '-' + arch + '.iso',
  command: [
    python_bin, create_iso_script, kernel, limine_dir, iso_dir, '@OUTPUT@'
  ],
  build_by_default : true,
  depends: [kernel, kernel_mn]
)

# Qemu run targets
qemu_flags = [
  '-enable-kvm',
  '-cpu',
  'host',
  '-m',
  '2G',
  '-no-reboot',
  '-no-shutdown'
]

qemu_runner = find_program('qemu-system-' + arch, required: false)

if qemu_runner.found()
  run_target(
    'run',
    command: [
      qemu_runner, qemu_flags, '-cdrom', iso_file
    ],
    depends: kernel_iso
  )

  run_target(
    'run_serial',
    command: [
      qemu_runner, qemu_flags, '-serial', 'file:serial.log', '-cdrom', iso_file
    ],
    depends: kernel_iso
  )

  run_target(
    'run_debug',
    command: [
      qemu_runner, qemu_flags, '-s', '-S', '-cdrom', iso_file
    ],
    depends: kernel_iso
  )
endif
