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


  // uint16_t pickedMode = 0xffff;
  // uint32_t* fb = NULL;
  // const int desiderWidth = 800;
  // const int desiderHeight = 600;
  // const int desiderBPP = 32;

  // VbeInfoBlock* info = (VbeInfoBlock*)MEMORY_VESA_INFO;
  // VbeModeInfo* modeinfo = (VbeModeInfo*)MEMORY_MODE_INFO;
  // // Init graphics
  // if(VBE_GetControllerInfo(info)){
  //   uint16_t* mode = (uint16_t*) info->VideoModePtr;

  //   for(int i = 0;mode[i] != 0xFFFF; i++){
  //     if(!VBE_GetModeInfo(mode[i],modeinfo)){
  //       printf("Can't get mode info %x\r\n",mode[i]);
  //       continue;
  //     }
  //     //printf("mode res: (%dx%d),",modeinfo->width, modeinfo->height);

  //     bool hasFB = (modeinfo->attributes & 0x90) == 0x90;

  //     if( hasFB &&
  //         modeinfo->width == desiderWidth &&
  //         modeinfo->height == desiderHeight &&
  //         modeinfo->bpp == desiderBPP ){

  //         pickedMode = mode[i];
  //         break;

  //     }
  //   }
  //   if(pickedMode != 0xFFFF) { // && VBE_SetMode(pickedMode) ! NOTE: TO ENABLE VBE ADD THIS TO THE IF CONDITION! (FOR NOW IT'S DISABLED)
  //     fb = (uint32_t*)(modeinfo->framebuffer);
  //   }
  // }else{
  //   printf("No VBE extensions.\r\n");
  // }



  // exec kernel
  kernelEntry(&bootDrive);

end:
  for(;;);
}
