ASM_INCLUDES ?= $(SRC_DIR)/utils/asm/
TARGET_ASMFLAGS += -f elf
TARGET_CFLAGS += -ffreestanding -nostdlib
TARGET_LIBS += -lgcc
TARGET_LINK_FLAGS += -T linker.ld -nostdlib
SOURCES_C = $(shell find . -type f -name "*.c")
SOURCES_ASM = $(shell find . -type f -name "*.asm")
OBJECT_C = $(patsubst %.c, $(BUILD_DIR)/stage2/c/%.obj, $(SOURCES_C))
OBJECT_ASM = $(patsubst %.asm, $(BUILD_DIR)/stage2/asm/%.obj, $(SOURCES_ASM))

.PHONY: all clean always

all: stage2

stage2: $(BUILD_DIR)/stage2.bin

$(BUILD_DIR)/stage2.bin: $(OBJECT_ASM) $(OBJECT_C)
	$(TARGET_LD) $(TARGET_LINK_FLAGS) -Wl,-Map=$(BUILD_DIR)/stage2.map -o $@ $^ $(TARGET_LIBS)
	@echo "--> Created  stage2.bin"

$(BUILD_DIR)/stage2/c/%.obj: %.c always
	$(TARGET_CC) $(TARGET_CFLAGS) -c -o $@ $<
	@echo "--> Compiled: " $<

$(BUILD_DIR)/stage2/asm/%.obj: %.asm always
	$(ASM) -I$(ASM_INCLUDES) $(TARGET_ASMFLAGS) -o $@ $<
	@echo "--> Compiled: " $<

always:
	mkdir -p $(BUILD_DIR)/stage2/c/
	mkdir -p $(BUILD_DIR)/stage2/asm/

clean:
	rm -f $(BUILD_DIR)/stage2.bin
