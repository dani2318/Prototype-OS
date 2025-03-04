ASM_INCLUDES ?= $(SRC_DIR)/utils/asm/
TARGET_ASMFLAGS += -f elf
TARGET_CFLAGS += -ffreestanding -nostdlib -I.
TARGET_LIBS += -lgcc
TARGET_LINK_FLAGS += -T linker.ld -nostdlib

SOURCES_C = $(shell find . -type f -name "*.c")
HEADERS_C = $(shell find . -type f -name "*.h")
OBJECT_C = $(patsubst %.c, $(BUILD_DIR)/kernel/c/%.obj, $(SOURCES_C))

SOURCES_ASM = $(shell find . -type f -name "*.asm")
HEADERS_ASM = $(shell find . -type f -name "*.inc")
OBJECT_ASM = $(patsubst %.asm, $(BUILD_DIR)/kernel/asm/%.obj, $(SOURCES_ASM))

.PHONY: all clean

all: kernel

kernel: arch/i686/isrs_gen.c arch/i686/isrs_gen.inc $(BUILD_DIR)/oskrnl.bin

$(BUILD_DIR)/oskrnl.bin: $(OBJECT_ASM) $(OBJECT_C)
	$(TARGET_LD) $(TARGET_LINK_FLAGS) -Wl,-Map=$(BUILD_DIR)/oskrnl.map -o $@ $^ $(TARGET_LIBS)
	@echo "--> Created  oskrnl.bin"

$(BUILD_DIR)/kernel/c/%.obj: %.c $(HEADERS_C)
	@mkdir -p $(@D)
	$(TARGET_CC) $(TARGET_CFLAGS) -c -o $@ $<
	@echo "--> Compiled: " $<

$(BUILD_DIR)/kernel/asm/%.obj: %.asm $(HEADERS_ASM)
	@mkdir -p $(@D)
	$(ASM) -I$(ASM_INCLUDES) $(TARGET_ASMFLAGS) -o $@ $<
	@echo "--> Compiled: " $<

clean:
	rm -f $(BUILD_DIR)/oskrnl.bin


arch/i686/isrs_gen.c arch/i686/isrs_gen.inc:
	@$(SOURCE_DIR)/build_scripts/generate_isrs.sh $(SOURCE_DIR)/src/kernel/arch/i686/isrs_gen.c $(SOURCE_DIR)/src/kernel/arch/i686/isrs_gen.inc
