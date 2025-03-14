#include "FATFile.hpp"
#include <core/Debug.hpp>
constexpr char *MODULE_NAME = "FATFile"; // Used for logging

bool FATFile::Open(const fileEntry& fileEntry)
{
  m_Position = 0;
  m_Size = fileEntry->directoryEntry.Size;
  m_FirstCluster = fileEntry->directoryEntry.FirstClusterLow + ((uint32_t)fileEntry->directoryEntry.FirstClusterHigh << 16);
  m_CurrentCluster = m_FirstCluster;
  m_CurrentSectorInCluster = 0;

  if (!ReadSector(rootDirLba, m_data->RootDirectory.Buffer))
  {
    Debug::Error(MODULE_NAME, "FAT: read root directory failed\r\n");
    return false;
  }
}

bool FATFile::ReadEntry(FAT_DirectoryEntry *directoryEntry)
{
  return Read(reinterpret_cast<uint8_t *>(directoryEntry), sizeof(FAT_DirectoryEntry)) == sizeof(FAT_DirectoryEntry);
}


FileEntry FATFile::GetNextFileEntry(const FileEntry& previous)
{
  FileEntry fe;

}
