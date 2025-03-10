import os
import urllib.request
import tarfile


TARGET = "i686-elf"
BINUTILS_VER = "2.37"
BINUTILS_URL = f"https://ftp.gnu.org/gnu/binutils/binutils-" + BINUTILS_VER + ".tar.gz"

GCC_VER="11.2.0"
GCC_URL="https://ftp.gnu.org/gnu/gcc/gcc-"+GCC_VER+"/gcc-"+GCC_VER+".tar.gz"

MAIN_PATH = __file__.replace("build_scripts/download_toolchain.py","")

TOOLCHAIN_FOLDER = f"{MAIN_PATH}/toolchain/"

TOOLCHAIN_PREFIX = os.path.abspath(f"{TOOLCHAIN_FOLDER}/{TARGET}")

BINUTILS_SRC = f"{TOOLCHAIN_FOLDER}binutils-{BINUTILS_VER}"
BINUTILS_BUILD = f"{TOOLCHAIN_FOLDER}build-binutils-{BINUTILS_VER}"
GCC_BUILD = f"{TOOLCHAIN_FOLDER}build-gcc-{GCC_VER}"

formatted_str = "Target: %s\n\r Binutils build path: %s \n\r Binutils src path: %s \n\r gcc url: %s \n\r  GCC version: %s \n\r "
data = (TARGET,BINUTILS_BUILD,BINUTILS_SRC,GCC_URL,GCC_VER)
print(formatted_str % data)


BINUTLIS_FILENAME = "binutils-"+BINUTILS_VER+".tar.gz"
GCC_FILENAME = "gcc-"+GCC_VER+".tar.gz"

def show_progress(block_num, block_size, total_size):
    print("Downloading "+GCC_FILENAME+": "+str(round(block_num * block_size / total_size *100,2)), end="\r")

def download(url, where,filename):
    if not os.path.exists(where):
        os.mkdir(where)
    

    os.system("cd " + where)

    print("Moving to "+str(os.path.abspath(__file__))+"..")

    
    urllib.request.urlretrieve(url,"toolchain/" + filename,show_progress)

    return True


def download_binutils():
    os.system("cd "+ TOOLCHAIN_FOLDER)

    while(not os.path.exists(TOOLCHAIN_FOLDER)):
        continue

    if not os.path.exists("toolchain/"+BINUTLIS_FILENAME):
        download(BINUTILS_URL,TOOLCHAIN_FOLDER, BINUTLIS_FILENAME)

    if not os.path.exists(BINUTILS_BUILD):
        os.mkdir(BINUTILS_BUILD)

    os.system("cd "+ TOOLCHAIN_FOLDER + "&& tar -xf " + BINUTLIS_FILENAME)
    os.system("cd "+BINUTILS_BUILD+" && CFLAGS= ASMFLAGS= CC= CXX= LD= ASM= LINK_FLAGS= LIBS= && ../binutils-"+BINUTILS_VER+"/configure \
		--prefix=" + TOOLCHAIN_PREFIX +"\
		--target=" + TARGET+ " \
		--with-sysroot \
		--disable-nls \
		--disable-werror")
    
    os.system("make -j4 -C "+BINUTILS_BUILD)
    os.system("make -j4 -C "+BINUTILS_BUILD + " install")
    return;

def download_gcc():
    os.system("cd "+ TOOLCHAIN_FOLDER)

    while(not os.path.exists(TOOLCHAIN_FOLDER)):
        continue

    if not os.path.exists("toolchain/"+GCC_FILENAME):
        download(GCC_URL,TOOLCHAIN_FOLDER, GCC_FILENAME)

    if not os.path.exists(GCC_BUILD):
        os.mkdir(GCC_BUILD)

    os.system("cd "+ TOOLCHAIN_FOLDER + "&& tar -xf " + GCC_FILENAME)
    os.system("cd "+GCC_BUILD+" && CFLAGS= ASMFLAGS= CC= CXX= LD= ASM= LINK_FLAGS= LIBS= && ../gcc-"+GCC_VER+"/configure \
		--prefix=" + TOOLCHAIN_PREFIX +"\
		--target=" + TARGET+ " \
		--disable-nls \
		--enable-languages=c,c++ \
		--without-headers")
    
    os.system("make -j4 -C "+GCC_BUILD + " all-gcc all-target-libgcc")
    os.system("make -j4 -C "+GCC_BUILD + " install-gcc")
    os.system("make -j4 -C "+GCC_BUILD + " install-target-libgcc")
    return;

if __name__ == "__main__":

    if not os.path.exists(TOOLCHAIN_FOLDER):
        os.mkdir(TOOLCHAIN_FOLDER)

    print(__file__.replace("build_scripts/download_toolchain.py",""))
    download_binutils()
    download_gcc()
