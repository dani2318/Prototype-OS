#include <stdint.h>
#include "stdio.h"
#include "x86.h"
#include "disk.h"
#include "vbe.h"
#include "mbr.h"
#include "elf.h"
#include "fat.h"
#include "memdefs.h"
#include "memory.h"

uint8_t* KernelLoadBuffer = (uint8_t*)MEMORY_LOAD_KERNEL;
uint8_t* Kernel = (uint8_t*)MEMORY_KERNEL_ADDR;

#define COLOR(r,g,b) ((b)| (g << 8) | (r << 16))

typedef void (*KernelStart)();

void __attribute__((cdecl)) start(uint16_t bootDrive, void* partition){
  clscr();

  uint8_t driveType;
  uint16_t cylinders,sectors,heads;

  //printf("Hello from stage2!\r\n");

  DISK disk;
  if(!DISK_Initialize(&disk,bootDrive)){
    printf("DISK: init error \r\n");
    goto end;
  }

  Partition part;
  MBR_DetectPartiton(&part,&disk,partition);

  if(!FAT_Initialize(&part)){
    printf("FAT: init error \r\n");
    goto end;
  }
  KernelStart kernelEntry;

  // load kernel
  if(!ELF_Read(&part,"/boot/oskrnl.elf",(void**)&kernelEntry)){
    printf("Can't find /boot/oskrnl.bin!, boot halted!\r\n");
    goto end;
  }
  printf("Result ELF read: %x", kernelEntry);

  // exec kernel
  kernelEntry(&bootDrive);

end:
  for(;;);
}
