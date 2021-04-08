# Phoenix
An hobbyist operating system which aims to be simple and customizable.\
The prebuild iso file is available in the Release tab.

## Build
In order to build it you need to:
- Build the os toolchain
- Generate the config file
- Build the os
- Generate the iso image

To build the cross os toolchain:
```bash
make toolchain
```
(you can set the -j option to allow parallel build when using make).\

To Generate the config file:
```bash
make config
```

To build the os:
```bash
make
```

To generate the iso image:
```bash
make iso
```

## Contribution
Feel free to contribute with pull requests and issues.
