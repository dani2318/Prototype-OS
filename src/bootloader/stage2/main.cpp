
#include <stdint.h>

#include <dev/MBR.hpp>
#include <arch/i686/RealMemory.hpp>
#include <arch/i686/BIOSDisk.hpp>
#include <core/Defs.hpp>
#include <core/arch/i686/VGATextDevice.hpp>
#include <core/arch/i686/E9Device.hpp>
#include <core/dev/TextDevice.hpp>
#include <core/dev/RangeBlockDevice.hpp>
#include <memory/Stage2Allocator.hpp>
#include <core/Debug.hpp>
#include "memdefs.h"

arch::i686::VGATextDevice g_VGADevice;
arch::i686::E9Device g_DebugDevice;

Stage2Allocator g_allocator(reinterpret_cast<void*>(MEMORY_MIN),MEMORY_MAX-MEMORY_MIN);

EXPORT void ASMCALL Start(uint16_t bootDrive, uint32_t partition){


  g_VGADevice.Clear();
  TextDevice screen(&g_VGADevice);
  Debug::AddOutputDevice(&screen, Debug::DebugLevel::LVL_INFO, false);
  TextDevice debug(&g_DebugDevice);
  Debug::AddOutputDevice(&debug, Debug::DebugLevel::LVL_DEBUG, true);
  RangeBlockDevice partRange;

  BIOSDisk disk(bootDrive);
  if(!disk.Initialize()){
    Debug::Critical("stage2"," Failed to initialize disk\r\n");
  }
  Debug::Info("stage2"," Disk initialize OK!\r\n");

  // Handle partition
  BlockDevice* part;
  BlockDevice* kernelPartition;
  if(bootDrive < 0x80){
    kernelPartition = &disk;
  }
  else{
    MBR_Entry* entry = arch::i686::ToLinear<MBR_Entry*>(partition);
    partRange.Initialize(&disk,entry->lbaStart,entry->size);
    kernelPartition = &partRange;
  }

end:
  for(;;);
}
