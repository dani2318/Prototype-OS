#!/bin/bash
TARGET=i686-elf
TOOLCHAIN_PREFIX=$(realpath toolchain/${TARGET})

BINUTILS_VER="2.37"
GCC_VER="11.2.0"

BINUTILS_SRC= "toolchain/binutils-${BINUTILS_VER}"
BINUTILS_BUILD= "toolchain/build-binutils-${BINUTILS_VER}"

export PATH=${TOOLCHAIN_PREFIX}/bin:$PATH

if [ ! -d "toolchain/" ]; then
  mkdir -p toolchain
fi

GCC_URL="https://ftp.gnu.org/gnu/gcc/gcc-${GCC_VER}/gcc-${GCC_VER}.tar.gz"
BINUTILS_URL="https://ftp.gnu.org/gnu/binutils/binutils-${BINUTILS_VER}.tar.gz"

cd toolchain && wget $(BINUTILS_URL)
cd toolchain && tar -xf binutils-$(BINUTILS_VER).tar.gz
mkdir -p $(BINUTILS_BUILD)
cd $(BINUTILS_BUILD)&& CFLAGS= ASMFLAGS= CC= CXX= LD= ASM= LINK_FLAGS= LIBS= && ../binutils-$(BINUTILS_VER)/configure \
  --prefix=$(TOOLCHAIN_PREFIX) \
  --target=$(TARGET) \
  --with-sysroot \
  --disable-nls \
  --disable-werror

$(make -j4 -C ${BINUTILS_BUILD})
$(make -C ${BINUTILS_BUILD} install)
