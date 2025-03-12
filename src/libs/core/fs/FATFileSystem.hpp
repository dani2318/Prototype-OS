#pragma once

#include "FileSystem.hpp"
#include <core/fs/FAT/FATData.hpp>

constexpr int FATReqMemory = 0x10000;

class FATFileSystem : public FileSystem {

  public:
    FATFileSystem(void* fsMemoryRegion);
    virtual bool Initialize(BlockDevice* device) override;
    virtual FileEntry* GetNextFileEntry(FileEntry* parent, FileEntry* previous) override;
    virtual File* Open(FileEntry* parent,FileOpenMode mode) override;
  private:
  BlockDevice* m_device;
  FAT_Data* m_data;
  uint32_t m_DataSectionLba;
  uint8_t m_FatType;
  uint32_t m_TotalSectors;
  uint32_t m_SectorsPerFat;
};
