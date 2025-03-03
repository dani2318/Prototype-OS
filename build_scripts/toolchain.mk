
TOOLCHAIN_PREFIX = $(abspath toolchain/$(TARGET))
toolchain: toolchain_binutils toolchain_gcc

export PATH:=$(TOOLCHAIN_PREFIX)/bin:$(PATH)
export TARGET_CC = $(TARGET)-gcc
export TARGET_CXX = $(TARGET)-g++
export TARGET_LD = $(TARGET)-gcc

BINUTILS_BUILD = toolchain/build-binutils-$(BINUTILS_VER)


toolchain_binutils:
	mkdir toolchain
	cd toolchain && wget $(BINUTILS_URL)
	cd toolchain && tar -xf binutils-$(BINUTILS_VER).tar.gz
	mkdir $(BINUTILS_BUILD)
	cd $(BINUTILS_BUILD) && ../binutils-$(BINUTILS_VER)/configure \
		--prefix=$(TOOLCHAIN_PREFIX) \
		--target=$(TARGET) \
		--with-sysroot \
		--disable-nls \
		--disable-werror

		$(MAKE) -j4 -C $(BINUTILS_BUILD)
		$(MAKE) -C $(BINUTILS_BUILD) install

GCC_BUILD = toolchain/build-gcc-$(GCC_VER)

toolchain_gcc: toolchain_binutils

	cd toolchain && wget $(GCC_URL)
	cd toolchain && tar -xf gcc-$(GCC_VER).tar.gz
	mkdir $(GCC_BUILD) 
	cd $(GCC_BUILD)  && ../gcc-$(GCC_VER)/configure \
		--prefix=$(TOOLCHAIN_PREFIX) \
		--target=$(TARGET) \
		--disable-nls \
		--enable-languages=c,c++ \
		--without-headers

	$(MAKE) -j4 -C $(GCC_BUILD) all-gcc all-target-libgcc
	$(MAKE) -C $(GCC_BUILD) install-target-libgcc
