#!/bin/bash

set -e

# Reset values
export CC=gcc
export LD=ld
export AR=ar
export NM=nm
export CFLAGS=
export LDFLAGS=

BINUTILSVERSION=2.36.1
GCCVERSION=10.2.0

PREFIX="$(pwd)"
TARGET=x86_64-elf
export PATH="$PREFIX/bin:$PATH"

JOBS="$1"
[[ $JOBS == *"-j"* ]] || JOBS=""

EXT=".tar.gz"

if [ ! -f binutils-$BINUTILSVERSION$EXT ]; then
    wget https://ftp.gnu.org/gnu/binutils/binutils-$BINUTILSVERSION$EXT
fi
if [ ! -f gcc-$GCCVERSION$EXT ]; then
    wget https://ftp.gnu.org/gnu/gcc/gcc-$GCCVERSION/gcc-$GCCVERSION$EXT
fi

rm -rf bin/ gcc-$GCCVERSION/ binutils-$BINUTILSVERSION/ include/ share/ lib* build-* x86_64-elf/

tar -xvf binutils-$BINUTILSVERSION$EXT
tar -xvf gcc-$GCCVERSION$EXT

mkdir build-binutils
cd build-binutils
../binutils-$BINUTILSVERSION/configure --target=$TARGET --prefix="$PREFIX" \
    --with-sysroot --disable-nls --disable-werror

make $JOBS
make install
cd ..

cd gcc-$GCCVERSION
contrib/download_prerequisites
patch -p1 < ../patches/gcc.patch
cd ..
mkdir build-gcc
cd build-gcc
../gcc-$GCCVERSION/configure --target=$TARGET --prefix="$PREFIX" --disable-nls \
    --enable-languages=c,c++ --without-headers

make $JOBS all-gcc
make $JOBS all-target-libgcc
make install-gcc
make install-target-libgcc
cd ..
