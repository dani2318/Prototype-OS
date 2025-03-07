    symbol-file /home/dani23/Prototype-OS/build/i686_debug/kernel/oskrnl.bin
    set disassembly-flavor intel
    target remote | qemu-system-i386 -S -gdb stdio -m 32 -hda /home/dani23/Prototype-OS/build/i686_debug/image.img
