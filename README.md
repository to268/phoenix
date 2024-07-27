# Phoenix

An hobbyist Unix-like operating system which aims to be simple and customizable.\
The prebuild iso file is available in the Release tab.

## Dependencies

In order to build it you need to install the following dependencies:
`meson`, `ninja`, `nasm`, `xorriso`.

Furthermore, you need `LLVM Clang` >= 18.

## Build

Creates the build dir:
```bash
./configure.py
```

Compiles the kernel:
```bash
cd build
ninja
```

Builds the ISO:
```bash
ninja phoenix-x86_64.iso
```

## Contribution

Feel free to contribute with pull requests and issues.
