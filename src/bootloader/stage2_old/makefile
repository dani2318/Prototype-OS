BUILD_DIR?=build/
ASM?= nasm
SRC_DIR ?= src
BUILD_DIR ?= build
ASM_INCLUDES ?= $(SRC_DIR)/utils/asm/
CFLAGS16 ?= -4 -d3 -s -wx -ms -zl -zq -za99
CC16 ?= /usr/bin/watcom/binl64/wcc
LD16 ?= /usr/bin/watcom/binl64/wlink

SOURCES_C = $(wildcard *.c)
SOURCES_ASM = $(wildcard *.asm)
OBJECT_C = $(patsubst %.c, $(BUILD_DIR)/stage2/c/%.obj, $(SOURCES_C))
OBJECT_ASM = $(patsubst %.asm, $(BUILD_DIR)/stage2/asm/%.obj, $(SOURCES_ASM))

.PHONY: all clean always

all: stage2

stage2: $(BUILD_DIR)/stage2.bin

$(BUILD_DIR)/stage2.bin: $(OBJECT_ASM) $(OBJECT_C)
	$(LD16) NAME $(BUILD_DIR)/stage2.bin FILE \{$(OBJECT_ASM) $(OBJECT_C) \} OPTION MAP=$(BUILD_DIR)/stage2.map @linker.lnk

$(BUILD_DIR)/stage2/c/%.obj: %.c always
	$(CC16) $(CFLAGS16) -fo=$@ $<

$(BUILD_DIR)/stage2/asm/%.obj: %.asm always
	$(ASM) -I$(ASM_INCLUDES) -f obj -o $@ $<

always:
	mkdir -p $(BUILD_DIR)/stage2/c/
	mkdir -p $(BUILD_DIR)/stage2/asm/

clean:
	rm -f $(BUILD_DIR)/stage2.bin
