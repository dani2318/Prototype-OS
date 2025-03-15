#include "FATFile.hpp"
#include <core/Debug.hpp>
constexpr char *MODULE_NAME = "FATFile"; // Used for logging

FATFile::FATFile() : Opened(false),
  m_fs(nullptr),
  m_Position(),
  m_Size(),
  m_FirstCluster(),
  m_CurrentCluster(),
  m_CurrentSectorInCluster(),
  m_IsRootDir(false){
}

bool FATFile::OpenRootDirectory1216(FATFileSystem*fs){
  m_IsRootDir = true;
  m_Position = 0;
  m_Size = directoryEntry->Size;
  m_FirstCluster = directoryEntry->FirstClusterLow + ((uint32_t)directoryEntry->FirstClusterHigh << 16);
  m_CurrentCluster = m_FirstCluster;
  m_CurrentSectorInCluster = 0;

  if (!m_fs->ReadSector(rootDirLba, m_data->RootDirectory.Buffer))
  {
    Debug::Error(MODULE_NAME, "FAT: read root directory failed\r\n");
    return false;
  }
}

bool FATFile::Open(FATFileSystem*fs,const FileEntry& fileEntry)
{
  const FAT_DirectoryEntry* directoryEntry = reinterpret_cast<const FAT_DirectoryEntry*>(&fileEntry.FSData);
  m_IsRootDir = false;
  m_Position = 0;
  m_Size = directoryEntry->Size;
  m_FirstCluster = directoryEntry->FirstClusterLow + ((uint32_t)directoryEntry->FirstClusterHigh << 16);
  m_CurrentCluster = m_FirstCluster;
  m_CurrentSectorInCluster = 0;

  if (!m_fs->ReadSector(rootDirLba, m_data->RootDirectory.Buffer))
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
