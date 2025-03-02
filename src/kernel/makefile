BUILD_DIR?=build/
ASM?= nasm
SRC_DIR ?= src
BUILD_DIR ?= build
ASM_INCLUDES ?= -I$(SRC_DIR)/utils/asm/

.PHONY: all clean

all: kernel

kernel: $(BUILD_DIR)/oskrnl.bin

$(BUILD_DIR)/oskrnl.bin:
	$(ASM) -I$(ASM_INCLUDES) kernel.asm -f bin -o $(BUILD_DIR)/oskrnl.bin

clean:
	rm -f $(BUILD_DIR)/oskrnl.bin
