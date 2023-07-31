# Phoenix
An hobbyist Unix-like operating system which aims to be simple and customizable.\
The prebuild iso file is available in the Release tab.

## Dependencies
In order to build it you need to install the following dependencies:
`GNU Make`, `nasm`, `xorriso`.

Furthermore, you need at least a supported compiler:
- `GCC` >= 10 (arguably a GCC version capable of compiling Binutils and GCC)
- `LLVM Clang` >= 9 is required.

## Build
In order to build it you need to:
- Build the os toolchain (If using GCC)
- Generate the config file (set `CONFIG_COMPILER` in the config to llvm if using Clang)
- Build the os
- Generate the iso image

Build the cross os toolchain:
```bash
make toolchain
```

Generate the config file:
```bash
make config
```

Build the os:
```bash
make
```

Generate the iso image:
```bash
make iso
```

## Contribution
Feel free to contribute with pull requests and issues.
