#include "mbr.h"
#include "memory.h"
#include "stdio.h"

typedef struct
{
  uint8_t attributes;
  uint8_t chsStart[3];
  uint8_t partitionType;
  uint8_t chsEnd[3];
  uint32_t lbaStart;
  uint32_t size;
}__attribute__((packed)) MBR_Entry;



void MBR_DetectPartiton(Partition* part,DISK* disk, void* partition){

  if(disk->id < 0x80){
    part->partitionOffset =  0;
    part->partitionSize = (uint32_t)(disk->cylinders) * (uint32_t)(disk->heads) * (uint32_t)(disk->sectors);
  }else{
    MBR_Entry* entry = (MBR_Entry*) so_addr_to_lin(partition);
    part->partitionOffset = entry->lbaStart;
    part->partitionSize = entry->size;
    printf("============== MBR INFO ==============\r\n");
    printf("attributes=%x\r\n",entry->attributes);
    printf("lbaStart=%x\r\n",entry->lbaStart);
    printf("partitionType=%x\r\n",entry->partitionType);
    printf("size=%x\r\n",entry->size);
    printf("============== MBR INFO ==============\r\n");
  }
}

bool Partition_ReadSectors(Partition* part, uint32_t lba, uint8_t sectors, void * lowerdataOut){
  return DISK_ReadSectors(part->disk, lba + part->partitionOffset, sectors, lowerdataOut);
}
