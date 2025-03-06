include build_scripts/config.mk

.PHONY: all floppy_image kernel bootloader clean always

all: floppy_image

include build_scripts/toolchain.mk

floppy_image: $(BUILD_DIR)/main_floppy.img

$(BUILD_DIR)/main_floppy.img: bootloader kernel
	@./build_scripts/make_floppy.sh $@
	@echo "---> Creating " $@

disk_image: $(BUILD_DIR)/main_disk.raw

$(BUILD_DIR)/main_disk.raw: bootloader kernel
	@./build_scripts/make_disk.sh $@ $(MAKE_DISK_SIZE)
	@echo "---> Creating " $@

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

run:
	qemu-system-i386 -fda $(BUILD_DIR)/main_floppy.img
