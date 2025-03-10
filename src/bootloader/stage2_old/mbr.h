#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "disk.h"

typedef struct
{
  DISK* disk;
  uint32_t partitionOffset;
  uint32_t partitionSize;
} Partition;


void MBR_DetectPartiton(Partition* part,DISK* disk, void* partition);
bool Partition_ReadSectors(Partition* part, uint32_t lba, uint8_t sectors, void * lowerdataOut);
