#pragma once
#include <stdint.h>

struct FAT_ExtendedBootRecord
{
  // extended boot record
  uint8_t DriveNumber;
  uint8_t _Reserved;
  uint8_t Signature;
  uint32_t VolumeId;       // serial number, value doesn't matter
  uint8_t VolumeLabel[11]; // 11 bytes, padded with spaces
  uint8_t SystemId[8];
} __attribute__((packed));

struct FAT32_ExtendedBootRecord
{
  // extended boot record
  uint32_t sectorsPerFat;
  uint16_t flags;
  uint16_t fatVersionNumber;
  uint32_t RootDirectoryCluster;
  uint32_t FSinfoSector;
  uint16_t BackupBootSector;
  uint8_t _Reserved[12];
  FAT_ExtendedBootRecord EBR;
} __attribute__((packed));

struct FAT_BootSector
{
  uint8_t BootJumpInstruction[3];
  uint8_t OemIdentifier[8];
  uint16_t BytesPerSector;
  uint8_t SectorsPerCluster;
  uint16_t ReservedSectors;
  uint8_t FatCount;
  uint16_t DirEntryCount;
  uint16_t TotalSectors;
  uint8_t MediaDescriptorType;
  uint16_t SectorsPerFat;
  uint16_t SectorsPerTrack;
  uint16_t Heads;
  uint32_t HiddenSectors;
  uint32_t LargeSectorCount;

  union
  {
    FAT_ExtendedBootRecord EBR1216;
    FAT32_ExtendedBootRecord EBR32;
  };

  // ... we don't care about code ...

} __attribute__((packed));

struct FAT_FileData
{
  uint8_t Buffer[SECTOR_SIZE];
  FAT_File Public;
  bool Opened;
  uint32_t FirstCluster;
  uint32_t CurrentCluster;
  uint32_t CurrentSectorInCluster;
};

struct FAT_LFNBlock
{
  uint8_t order;
  int16_t chars[13];
};

