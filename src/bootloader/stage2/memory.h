#pragma once
#include "stdint.h"

#define SEG(segoff) (segoff >> 16)
#define OFF(segoff) (segoff & 0xFFFF)

void * memcpy(void * dst, const void * src, uint16_t num);
void * memset(void * ptr, int value, uint16_t num);
int memcmp(const void * ptr1, const void * ptr2, uint16_t num);
void* so_addr_to_lin(void* addr);
