ASM_INCLUDES ?= $(SRC_DIR)/utils/asm/
TARGET_ASMFLAGS += -f elf
TARGET_CFLAGS += -ffreestanding -nostdlib
TARGET_LIBS += -lgcc
TARGET_LINK_FLAGS += -T linker.ld -nostdlib
SOURCES_C = $(wildcard *.c)
SOURCES_ASM = $(wildcard *.asm)
OBJECT_C = $(patsubst %.c, $(BUILD_DIR)/kernel/c/%.obj, $(SOURCES_C))
OBJECT_ASM = $(patsubst %.asm, $(BUILD_DIR)/kernel/asm/%.obj, $(SOURCES_ASM))

.PHONY: all clean always

all: kernel

kernel: $(BUILD_DIR)/oskrnl.bin

$(BUILD_DIR)/oskrnl.bin: $(OBJECT_ASM) $(OBJECT_C)
	$(TARGET_LD) $(TARGET_LINK_FLAGS) -Wl,-Map=$(BUILD_DIR)/oskrnl.map -o $@ $^ $(TARGET_LIBS)
	@echo "--> Created  oskrnl.bin"

$(BUILD_DIR)/kernel/c/%.obj: %.c always
	$(TARGET_CC) $(TARGET_CFLAGS) -c -o $@ $<
	@echo "--> Compiled: " $<

$(BUILD_DIR)/kernel/asm/%.obj: %.asm always
	$(ASM) -I$(ASM_INCLUDES) $(TARGET_ASMFLAGS) -o $@ $<
	@echo "--> Compiled: " $<

always:
	mkdir -p $(BUILD_DIR)/kernel/c/
	mkdir -p $(BUILD_DIR)/kernel/asm/

clean:
	rm -f $(BUILD_DIR)/oskrnl.bin
