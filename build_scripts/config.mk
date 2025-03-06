MAKE_DISK_SIZE = 16777216 # 16MB

export CFLAGS = -std=99 -g #! remove this if binutils and gcc do not compile correctly!
export ASMFLAGS =
export CC = gcc
export CXX = g++
export LD = gcc
export LINK_FLAGS =
export LIBS =

export TARGET = i686-elf
export TARGET_CFLAGS = -std=c99 -g #-O2
export ASM = nasm
export TARGET_ASMFLAGS =
export TARGET_CC = $(TARGET)-gcc
export TARGET_CXX = $(TARGET)-g++
export TARGET_LD = $(TARGET)-gcc
export TARGET_LINK_FLAGS =
export TARGET_LIBS =
export SOURCE_DIR = $(abspath .)

SRC_DIR = src
export BUILD_DIR = $(abspath build)
ASM_INCLUDES := $(SRC_DIR)/utils/asm/

BINUTILS_VER = 2.37
BINUTILS_URL = https://ftp.gnu.org/gnu/binutils/binutils-$(BINUTILS_VER).tar.gz

GCC_VER = 11.2.0
GCC_URL = https://ftp.gnu.org/gnu/gcc/gcc-$(GCC_VER)/gcc-$(GCC_VER).tar.gz

