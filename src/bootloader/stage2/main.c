#include <stdint.h>
#include "stdio.h"
#include "x86.h"
#include "disk.h"
#include "fat.h"
#include "memdefs.h"
#include "memory.h"

uint8_t* KernelLoadBuffer = (uint8_t*)MEMORY_LOAD_KERNEL;
uint8_t* Kernel = (uint8_t*)MEMORY_KERNEL_ADDR;

typedef void (*KernelStart)();

void __attribute__((cdecl)) start(uint16_t bootDrive){
  clscr();

  uint8_t driveType;
  uint16_t cylinders,sectors,heads;

  //printf("Hello from stage2!\r\n");

  DISK disk;
  if(!DISK_Initialize(&disk,bootDrive)){
    printf("DISK: init error \r\n");
    goto end;
  }

  if(!FAT_Initialize(&disk)){
    printf("FAT: init error \r\n");
    goto end;
  }

  // load kernel
  FAT_File * fd = FAT_Open(&disk, "/oskrnl.bin");
  uint32_t read;

  uint8_t* kernelbuffer = Kernel;

  while ((read = FAT_Read(&disk, fd, MEMORY_LOAD_SIZE,KernelLoadBuffer)))
  {
    memcpy(kernelbuffer,KernelLoadBuffer, read);
    kernelbuffer += read;
  }
  FAT_Close(fd);

  // exec kernel
  KernelStart kernelstart = (KernelStart)Kernel;
  kernelstart();

end:
  for(;;);
}
