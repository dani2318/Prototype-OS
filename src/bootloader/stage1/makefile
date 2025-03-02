BUILD_DIR?=build/
ASM?= nasm
SRC_DIR ?= src
BUILD_DIR ?= build
ASM_INCLUDES ?= -I../utils/asm/

.PHONY: all clean

all: stage1

stage1: $(BUILD_DIR)/stage1.bin

$(BUILD_DIR)/stage1.bin:
	$(ASM) -I$(ASM_INCLUDES) boot.asm -f bin -o $(BUILD_DIR)/stage1.bin

clean:
	rm -f $(BUILD_DIR)/stage1.bin
