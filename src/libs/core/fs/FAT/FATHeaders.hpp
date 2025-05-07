#pragma once
#include <stdint.h>
#include <core/fs/FAT/FATFile.hpp>
constexpr int SectorSize                =   512;

struct FAT_LongFileEntry
{

  uint8_t Order;
  int16_t Chars1[5];
  uint8_t Attribute;
  uint8_t LongEntryType;
  uint8_t Checksum;
  int16_t Chars2[6];
  uint16_t _AlwaysZero;
  int16_t Chars3[2];

} __attribute__((packed));



enum FAT_TYPE
{
  FAT12 = 12,
  FAT16 = 16,
  FAT32 = 32
};

enum FAT_Attributes
{
  FAT_ATTRIBUTE_READ_ONLY = 0x01,
  FAT_ATTRIBUTE_HIDDEN = 0x02,
  FAT_ATTRIBUTE_SYSTEM = 0x04,
  FAT_ATTRIBUTE_VOLUME_ID = 0x08,
  FAT_ATTRIBUTE_DIRECTORY = 0x10,
  FAT_ATTRIBUTE_ARCHIVE = 0x20,
  FAT_ATTRIBUTE_LFN = FAT_ATTRIBUTE_READ_ONLY | FAT_ATTRIBUTE_HIDDEN | FAT_ATTRIBUTE_SYSTEM | FAT_ATTRIBUTE_VOLUME_ID
};

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
  uint8_t Buffer[SectorSize];
  FATFile Public;
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

struct FAT_DirectoryEntry
{
  uint8_t Name[11];
  uint8_t Attributes;
  uint8_t _Reserved;
  uint8_t CreatedTimeTenths;
  uint16_t CreatedTime;
  uint16_t CreatedDate;
  uint16_t AccessedDate;
  uint16_t FirstClusterHigh;
  uint16_t ModifiedTime;
  uint16_t ModifiedDate;
  uint16_t FirstClusterLow;
  uint32_t Size;
} __attribute__((packed));
