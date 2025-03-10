#pragma once
#include <stdint.h>

typedef struct
{
  uint8_t attributes;
  uint8_t chsStart[3];
  uint8_t partitionType;
  uint8_t chsEnd[3];
  uint32_t lbaStart;
  uint32_t size;
}__attribute__((packed)) MBR_Entry;
