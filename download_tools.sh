
#!/bin/sh
if [! -e gcc-14.2.0.tar.gz]
wget https://ftp.gnu.org/gnu/gcc/gcc-14.2.0/gcc-14.2.0.tar.gz
tar -xvf gcc-14.2.0.tar.gz
fi
if [! -e binutils-2.44.tar.gz]
wget https://ftp.gnu.org/gnu/binutils/binutils-2.44.tar.gz
tar -xvf binutils-2.44.tar.gz
fi
if [! -e mtools-4.0.48.tar.gz]
wget http://ftp.gnu.org/gnu/mtools/mtools-4.0.48.tar.gz
tar -xvf mtools-4.0.48.tar.gz
fi


export PREFIX="$HOME/os/toolchain/i686-elf"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"

mkdir -p i686-elf

mkdir binutils-build
cd binutils-build


../binutils-2.44/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make -j 4

cd ..

mkdir gcc-build
cd gcc-build

../gcc-14.2.0/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror --enable-languages=c,c++ --without-headers
make all-gcc -j 8
make install-gcc
make install-target-libgcc
