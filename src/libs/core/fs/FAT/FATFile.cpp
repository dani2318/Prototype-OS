#include "FATFile.hpp"


bool FATFile::Open(FAT_DirectoryEntry* directoryEntry){
  m_Position = 0;
  m_Size = directoryEntry->Size;
  m_FirstCluster = directoryEntry->FirstClusterLow + ((uint32_t)directoryEntry->FirstClusterHigh << 16);
  m_CurrentCluster = m_FirstCluster;
  m_CurrentSectorInCluster = 0;
}
