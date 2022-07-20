#!/bin/bash

set -ex

# Reset values to make sure to have clean env variables
export CC=gcc
export LD=ld
export AR=ar
export NM=nm
export CFLAGS="-O2 -pipe"
export LDFLAGS=

BINUTILSVERSION=2.38
GCCVERSION=12.1.0

PREFIX="$(pwd)"
TARGET=x86_64-elf
export PATH="$PREFIX/bin:$PATH"

JOBS="$1"
EXT=".tar.gz"

if [ ! -f binutils-$BINUTILSVERSION$EXT ]; then
    wget https://ftp.gnu.org/gnu/binutils/binutils-$BINUTILSVERSION$EXT
fi
if [ ! -f gcc-$GCCVERSION$EXT ]; then
    wget https://ftp.gnu.org/gnu/gcc/gcc-$GCCVERSION/gcc-$GCCVERSION$EXT
fi

rm -rf bin/ gcc-$GCCVERSION/ binutils-$BINUTILSVERSION/ include/ share/ lib* build-* x86_64-elf/

tar -zxf binutils-$BINUTILSVERSION$EXT
tar -zxf gcc-$GCCVERSION$EXT

mkdir build-binutils
cd build-binutils
../binutils-$BINUTILSVERSION/configure CFLAGS="$CFLAGS" CXXFLAGS="$CFLAGS" --target=$TARGET \
    --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror

make $JOBS
make install
cd ..

cd gcc-$GCCVERSION
contrib/download_prerequisites
patch -p1 < ../patches/gcc.patch
cd ..
mkdir build-gcc
cd build-gcc
../gcc-$GCCVERSION/configure CFLAGS="$CFLAGS" CXXFLAGS="$CFLAGS" --target=$TARGET \
    --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers

make $JOBS all-gcc
make $JOBS all-target-libgcc
make install-gcc
make install-target-libgcc
cd ..

rm -rf build-binutils
rm -rf build-gcc
rm -rf binutils-$BINUTILSVERSION/
rm -rf gcc-$GCCVERSION/
