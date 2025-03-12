#pragma once

#include <core/fs/File.hpp>
#include <core/fs/FAT/FATHeaders.hpp>
#include <core/fs/FAT/FATFileEntry.hpp>


class FATFile : public File
{
  public:
  FATFile();
  bool Open(FATFileEntry* fileEntry);

  bool IsOpened() const {return Opened;}

  private:
    uint8_t m_Buffer[SectorSize];
    bool Opened;
    uint32_t m_Position;
    uint32_t m_Size;
    uint32_t m_FirstCluster;
    uint32_t m_CurrentCluster;
    uint32_t m_CurrentSectorInCluster;

};
