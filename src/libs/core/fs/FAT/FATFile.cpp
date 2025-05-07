#include "FATFile.hpp"
#include <core/Debug.hpp>
constexpr char *MODULE_NAME = "FATFile"; // Used for logging

FATFile::FATFile() : m_Opened(false),
  m_fs(nullptr),
  m_Position(),
  m_Size(),
  m_FirstCluster(),
  m_CurrentCluster(),
  m_CurrentSectorInCluster(),
  m_IsRootDir(false){
}


void FATFile::Seek(SeekPos pos, int rel) {
  switch (pos)
  {
  case SeekPos::Set:
    m_CurrentCluster = m_FirstCluster;
    m_CurrentSectorInCluster = 0;
    m_Position = 0;
    while (m_Position < rel)
    {
      /* code */
    }

    break;

  default:
    break;
  }
}

bool FATFile::OpenRootDirectory1216(FATFileSystem* fs,uint32_t rootDirLba,uint32_t rootDirSize){
  m_IsRootDir = true;
  m_Position = 0;
  m_Size = rootDirSize;
  m_FirstCluster = rootDirLba;
  m_CurrentCluster = m_FirstCluster;
  m_CurrentSectorInCluster = 0;


  if (!m_fs->ReadSector(rootDirLba, m_Buffer))
  {
    Debug::Error(MODULE_NAME, "FAT: read root directory failed\r\n");
    return false;
  }
}

bool FATFile::Open(FATFileSystem*fs,uint32_t firstCluster,uint32_t size)
{


  m_IsRootDir = false;
  m_Position = 0;
  m_Size = size;
  m_FirstCluster = firstCluster;
  m_CurrentCluster = m_FirstCluster;
  m_CurrentSectorInCluster = 0;


  if (!m_fs->ReadSectorFromCluster(m_CurrentCluster,m_CurrentSectorInCluster, m_Buffer))
  {
      Debug::Error(MODULE_NAME, "FAT (Open file %s): read error (cluster:%u , lba:%u)\r\n",m_CurrentCluster,fileEntry.Name);
      return false;
  }

  m_Opened = true;
}


// https://www.youtube.com/watch?v=e85bPjZatIc&list=PLFjM7v6KGMpiH2G-kT781ByCNC_0pKpPN&index=23&t=9868s

bool FATFile::ReadEntry(FAT_DirectoryEntry *directoryEntry)
{
  return Read(reinterpret_cast<uint8_t *>(directoryEntry), sizeof(FAT_DirectoryEntry)) == sizeof(FAT_DirectoryEntry);
}


FileEntry FATFile::GetNextFileEntry(const FileEntry& previous)
{
  FileEntry fe;

}
