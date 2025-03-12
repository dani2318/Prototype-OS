#pragma once

#include "FileSystem.hpp"
#include <core/fs/FAT/FATData.hpp>
#include <core/fs/FAT/FATFileEntry.hpp>
#include <core/fs/FileEntry.hpp>
#include <core/fs/FAT/FATHeaders.hpp>

constexpr int FATReqMemory = 0x10000;

class FATFileSystem : public FileSystem {

  public:
    FATFileSystem(void* fsMemoryRegion);
    virtual bool Initialize(BlockDevice* device) override;
    virtual File* Open(FileEntry* file,FileOpenMode mode) override;
    virtual FileEntry* GetNextFileEntry(File* parent, const FileEntry& previous);
  private:

  bool ReadBootSector();
  bool ReadSector(uint32_t lba, uint8_t* buffer);
  uint32_t ClusterToLba(uint32_t cluster);
  void DetectFatType();
  BlockDevice* m_device;
  FAT_Data* m_data;
  uint32_t m_DataSectionLba;
  uint8_t m_FatType;
  uint32_t m_TotalSectors;
  uint32_t m_SectorsPerFat;

};
