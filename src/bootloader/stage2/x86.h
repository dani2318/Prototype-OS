#pragma once
#include <stdint.h>
#include <stdbool.h>

void __attribute__((cdecl)) x86_outb(uint16_t port, uint8_t value);
uint8_t __attribute__((cdecl)) x86_inb(uint16_t port);
bool __attribute__((cdecl)) x86_Disk_GetDriveParams(uint8_t drive, uint8_t* driveTypeOut ,uint16_t* cylindersOut, uint16_t* sectorsOut, uint16_t* headsOut);
