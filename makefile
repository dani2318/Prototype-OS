

include build_scripts/config.mk

.PHONY: all floppy_image kernel bootloader clean always



all: floppy_image

include build_scripts/toolchain.mk

floppy_image: $(BUILD_DIR)/main_floppy.img

$(BUILD_DIR)/main_floppy.img: bootloader kernel
	dd if=/dev/zero of=$(BUILD_DIR)/main_floppy.img bs=512 count=2880
	mkfs.fat -F 12 -n "PROTOS" $(BUILD_DIR)/main_floppy.img
	dd if=$(BUILD_DIR)/stage1.bin of=$(BUILD_DIR)/main_floppy.img conv=notrunc
	mcopy -i $(BUILD_DIR)/main_floppy.img $(BUILD_DIR)/stage2.bin "::stage2.bin"
	mcopy -i $(BUILD_DIR)/main_floppy.img $(BUILD_DIR)/oskrnl.bin "::oskrnl.bin"
	mcopy -i $(BUILD_DIR)/main_floppy.img test.txt "::test.txt"
	mmd -i $(BUILD_DIR)/main_floppy.img "::mydir"
	mcopy -i $(BUILD_DIR)/main_floppy.img test.txt "::mydir/test.txt"


bootloader: stage1 stage2

stage1: $(BUILD_DIR)/stage1.bin
stage2: $(BUILD_DIR)/stage2.bin
kernel: $(BUILD_DIR)/oskrnl.bin

$(BUILD_DIR)/stage1.bin: always
	$(MAKE) -C $(SRC_DIR)/bootloader/stage1 BUILD_DIR=$(abspath $(BUILD_DIR)) ASM_INCLUDES=$(abspath $(ASM_INCLUDES))

$(BUILD_DIR)/stage2.bin: always
	$(MAKE) -C $(SRC_DIR)/bootloader/stage2 BUILD_DIR=$(abspath $(BUILD_DIR)) ASM_INCLUDES=$(abspath $(ASM_INCLUDES))

$(BUILD_DIR)/oskrnl.bin: always
	$(MAKE) -C $(SRC_DIR)/kernel BUILD_DIR=$(abspath $(BUILD_DIR)) ASM_INCLUDES=$(abspath $(ASM_INCLUDES))

always:
	mkdir -p $(BUILD_DIR)

clear:
	$(MAKE) -C $(SRC_DIR)/bootloader/stage1 BUILD_DIR=$(abspath $(BUILD_DIR)) ASM_INCLUDES=$(abspath $(ASM_INCLUDES)) clean
	$(MAKE) -C $(SRC_DIR)/bootloader/stage2 BUILD_DIR=$(abspath $(BUILD_DIR)) ASM_INCLUDES=$(abspath $(ASM_INCLUDES)) clean
	$(MAKE) -C $(SRC_DIR)/kernel BUILD_DIR=$(abspath $(BUILD_DIR)) ASM_INCLUDES=$(abspath $(ASM_INCLUDES)) clean
	rm -rf $(BUILD_DIR)/*

