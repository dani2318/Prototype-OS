include "config.mk"
TOOLCHAIN_PREFIX = $(abspath toolchain/$(TARGET))
toolchain: toolchain_binutils toolchain_gcc

export PATH:=$(TOOLCHAIN_PREFIX)/bin:$(PATH)

BINUTILS_SRC = toolchain/binutils-$(BINUTILS_VER)
BINUTILS_BUILD = toolchain/build-binutils-$(BINUTILS_VER)

toolchain_binutils: $(TOOLCHAIN_PREFIX)/bin/i686-elf-ld

$(TOOLCHAIN_PREFIX)/bin/i686-elf-ld: $(BINUTILS_SRC).tar.gz
	cd toolchain && tar -xf binutils-$(BINUTILS_VER).tar.gz
	mkdir -p $(BINUTILS_BUILD)
	cd $(BINUTILS_BUILD)&& CFLAGS= ASMFLAGS= CC= CXX= LD= ASM= LINK_FLAGS= LIBS= && ../binutils-$(BINUTILS_VER)/configure \
		--prefix=$(TOOLCHAIN_PREFIX) \
		--target=$(TARGET) \
		--with-sysroot \
		--disable-nls \
		--disable-werror

		$(MAKE) -j4 -C $(BINUTILS_BUILD)
		$(MAKE) -C $(BINUTILS_BUILD) install

$(BINUTILS_SRC).tar.gz:
	mkdir -p toolchain
	cd toolchain && wget $(BINUTILS_URL)

GCC_SRC = toolchain/gcc-$(GCC_VER)
GCC_BUILD = toolchain/build-gcc-$(GCC_VER)

toolchain_gcc: $(TOOLCHAIN_PREFIX)/bin/i686-elf-gcc

$(TOOLCHAIN_PREFIX)/bin/i686-elf-gcc: toolchain_binutils $(GCC_SRC).tar.gz
	cd toolchain && tar -xf gcc-$(GCC_VER).tar.gz
	mkdir -p $(GCC_BUILD)
	cd $(GCC_BUILD) && CFLAGS= ASMFLAGS= CC= CXX= LD= ASM= LINK_FLAGS= LIBS= && ../gcc-$(GCC_VER)/configure \
		--target=$(TARGET) \
		--prefix=$(TOOLCHAIN_PREFIX) \
		--disable-nls \
		--enable-languages=c,c++ \
		--without-headers

	$(MAKE) -j4 -C $(GCC_BUILD) all-gcc all-target-libgcc
	$(MAKE) -C $(GCC_BUILD) install-gcc
	$(MAKE) -C $(GCC_BUILD) install-target-libgcc

$(GCC_SRC).tar.gz:
	mkdir -p toolchain
	cd toolchain && wget $(GCC_URL)

clean-toolchain:
	rm -rf $(GCC_BUILD)$(GCC_SRC) $(BINUTILS_BUILD) $(BINUTILS_SRC)

clean-toolchain-all:
	rm -rf toolchain/*


.PHONY: toolchain toolchain_binutils toolchain_gcc clean-toolchain clean-toolchain-all
