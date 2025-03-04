export CFLAGS = -std=99 -g #! remove this if binutils and gcc do not compile correctly!
export ASMFLAGS =
export CC = gcc
export CXX = g++
export LD = gcc
export LINK_FLAGS =
export LIBS =
#https://www.youtube.com/watch?v=db_5skZaneg&list=PLFjM7v6KGMpiH2G-kT781ByCNC_0pKpPN&index=11

export TARGET = i686-elf
export TARGET_CFLAGS = -std=c99 -g #-O2
export ASM = nasm
export TARGET_ASMFLAGS =
export TARGET_CC = $(TARGET)-gcc
export TARGET_CXX = $(TARGET)-g++
export TARGET_LD = $(TARGET)-gcc
export TARGET_LINK_FLAGS =
export TARGET_LIBS =

CC16 = /usr/bin/watcom/binl64/wcc
LD16 = /usr/bin/watcom/binl64/wlink
SRC_DIR = src
BUILD_DIR = build
ASM_INCLUDES := $(SRC_DIR)/utils/asm/

BINUTILS_VER = 2.37
BINUTILS_URL = https://ftp.gnu.org/gnu/binutils/binutils-$(BINUTILS_VER).tar.gz

GCC_VER = 11.2.0
GCC_URL = https://ftp.gnu.org/gnu/gcc/gcc-$(GCC_VER)/gcc-$(GCC_VER).tar.gz

