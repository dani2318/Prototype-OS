#pragma once
#include <stdint.h>
#include "FATHeaders.hpp"

constexpr int SectorSize                =   512;
constexpr int MaxPathSize               =   256;
constexpr int MaxFileHandles            =   10;
constexpr int RootDirectoryHandle       =   -1;
constexpr int FatCacheSize              =   5;
constexpr uint32_t FAT_LFN_LAST         =   0x40;

struct FAT_Data
{
    union
    {
        FAT_BootSector BootSector;
        uint8_t BootSectorBytes[SectorSize];
    } BS;

    FAT_FileData RootDirectory;

    FAT_FileData OpenedFiles[MaxFileHandles];

    uint8_t FatCache[FatCacheSize * SectorSize];
    uint32_t FatCachePosition;

    FAT_LFNBlock LFNBlock[FAT_LFN_LAST];
    int LFNCount;

} ;
