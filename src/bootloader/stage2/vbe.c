#include "vbe.h"
#include "x86.h"
#include "stdio.h"

#define SEG(segoff) (segoff >> 16)
#define OFF(segoff) (segoff & 0xFFFF)
#define SEGOFF2LIN(segoff) ((SEG(segoff) << 4 ) + OFF(segoff))

bool VBE_GetControllerInfo(VbeInfoBlock* info){

  if(x86_Video_GetVbeInfo(info) == 0x004f){
    // Convert seg:off to linear addr.
    info->VideoModePtr = SEGOFF2LIN(info->VideoModePtr);
    return true;
  }

  return false;

}


bool VBE_GetModeInfo(uint16_t mode, VbeModeInfo* info){
  if(x86_Video_GetModeInfo(mode,info) == 0x004f){
    return true;
  }
  return false;
}

bool VBE_SetMode(uint16_t mode){

  return x86_Video_SetMode(mode) == 0x004f;
}
