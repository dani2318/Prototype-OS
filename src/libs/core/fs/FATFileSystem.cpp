#include "FATFileSystem.hpp"
#include <core/Debug.hpp>

constexpr char *MODULE_NAME = "FAT"; // Used for logging

FATFileSystem::FATFileSystem()
    : m_device(nullptr),
      m_data(new FAT_Data()),
      m_DataSectionLba(),
      m_FatType(),
      m_TotalSectors(),
      m_SectorsPerFat()
{
}

bool FATFileSystem::ReadSector(uint32_t lba, uint8_t *buffer,uint32_t count)
{
  m_device->Seek(SeekPos::Set, lba * SectorSize);
  return (m_device->Read(buffer, count * SectorSize)) == count * SectorSize;
}

bool FATFileSystem::ReadBootSector()
{
  return ReadSector(0, m_data->BS.BootSectorBytes);
}

uint32_t FATFileSystem::ClusterToLba(uint32_t cluster)
{
  return m_DataSectionLba + (cluster - 2) * m_data->BS.BootSector.SectorsPerCluster;
}

void FATFileSystem::DetectFatType()
{

  uint32_t dataClusters = (m_TotalSectors - m_DataSectionLba) / m_data->BS.BootSector.SectorsPerCluster;
  if (dataClusters < 0xFF5)
  {
    m_FatType = FAT12;
  }
  else if (m_data->BS.BootSector.SectorsPerFat != 0)
  {
    m_FatType = FAT16;
  }
  else
    m_FatType = FAT32;
}

bool FATFileSystem::ReadSectorFromCluster(uint32_t cluster, uint32_t sectorOffset, uint8_t *buffer)
{
  return ReadSector(ClusterToLba(cluster) + sectorOffset, buffer);
}

bool FATFileSystem::Initialize(BlockDevice *device)
{
  m_device = device;

  if (!ReadBootSector())
  {
    Debug::Error(MODULE_NAME, "FAT: read boot sector failed\r\n");
    return false;
  }

  // read FAT
  m_data->FatCachePosition = 0xFFFFFFFF;

  m_TotalSectors = m_data->BS.BootSector.TotalSectors;
  if (m_TotalSectors == 0)
  {
    // FAT32
    m_TotalSectors = m_data->BS.BootSector.LargeSectorCount;
  }

  bool isFat32 = false;
  m_SectorsPerFat = m_data->BS.BootSector.SectorsPerFat;
  if (m_SectorsPerFat == 0)
  {
    // FAT32
    isFat32 = true;
    m_SectorsPerFat = m_data->BS.BootSector.EBR32.sectorsPerFat;
  }

  // open root directory file
  uint32_t rootDirLba;
  uint32_t rootDirSize = 0;

  if (isFat32)
  {
    m_DataSectionLba = m_data->BS.BootSector.ReservedSectors + m_SectorsPerFat * m_data->BS.BootSector.FatCount;
    rootDirLba = ClusterToLba(m_data->BS.BootSector.EBR32.RootDirectoryCluster);
    rootDirSize = 0;

    FileEntry rootEntry;
    FAT_DirectoryEntry *rootDirEntry = reinterpret_cast<FAT_DirectoryEntry *>(rootEntry.FSData);
    rootDirEntry->FirstClusterLow = rootDirLba & 0xFFFF;
    rootDirEntry->FirstClusterHigh = rootDirLba & 0xFF;
    rootDirEntry->Size = sizeof(FAT_DirectoryEntry) * m_data->BS.BootSector.DirEntryCount;
    if(!m_data->RootDirectory.Open(this,rootEntry))
      return false;
  }
  else
  {
    rootDirLba = m_data->BS.BootSector.ReservedSectors + m_SectorsPerFat * m_data->BS.BootSector.FatCount;
    rootDirSize = sizeof(FAT_DirectoryEntry) * m_data->BS.BootSector.DirEntryCount;
    uint32_t rootDirSectors = (rootDirSize + m_data->BS.BootSector.BytesPerSector - 1) / m_data->BS.BootSector.BytesPerSector;
    m_DataSectionLba = rootDirLba + rootDirSectors;

    if(!m_data->RootDirectory.OpenRootDirectory1216(this, rootDirLba,rootDirSize))
      return false;
  }

  // Detect FAT type
  DetectFatType();

  // reset open files
  for (int i = 0; i < MaxFileHandles; i++)
    m_data->OpenedFiles[i] = FATFile();

  m_data->LFNCount = 0;

  return true;
}


File *FATFileSystem::Rootdirectory()
{
  return &m_data->RootDirectory;
}

File *FATFileSystem::Open(FileEntry *file, FileOpenMode mode)
{

  // find empty handle
  int handle = -1;
  for (int i = 0; i < MaxFileHandles && handle < 0; i++)
  {
    if (!m_data->OpenedFiles[i].IsOpened())
      handle = i;
  }

  // out of handles
  if (handle < 0)
  {
    Debug::Error(MODULE_NAME, "FAT: out of file handles\r\n");
    return nullptr;
  }

  const FAT_DirectoryEntry* directoryEntry = reinterpret_cast<const FAT_DirectoryEntry*>(file->FSData);
  uint32_t size = directoryEntry->Size;
  uint32_t firstCluster = directoryEntry->FirstClusterLow + ((uint32_t)directoryEntry->FirstClusterHigh << 16);
  // setup vars
  m_data->OpenedFiles[handle].Open(this,firstCluster,size);
  return &m_data->OpenedFiles[handle];
}


uint32_t FATFileSystem::GetNextCluster(uint32_t currentCluster)
{
    // Determine the byte offset of the entry we need to read
    uint32_t fatIndex;
    if (m_FatType == 12) {
        fatIndex = currentCluster * 3 / 2;
    }
    else if (m_FatType == 16) {
        fatIndex = currentCluster * 2;
    }
    else /*if (m_FatType == 32)*/ {
        fatIndex = currentCluster * 4;
    }

    // Make sure cache has the right number
    uint32_t fatIndexSector = fatIndex / SectorSize;
    if (fatIndexSector < m_data->FatCachePosition
        || fatIndexSector >= m_data->FatCachePosition + FatCacheSize)
    {
        ReadFat(fatIndexSector);
        m_data->FatCachePosition = fatIndexSector;
    }

    fatIndex -= (m_data->FatCachePosition * SectorSize);

    uint32_t nextCluster;
    if (m_FatType == 12) {
        if (currentCluster % 2 == 0)
            nextCluster = (*(uint16_t*)(m_data->FatCache + fatIndex)) & 0x0FFF;
        else
            nextCluster = (*(uint16_t*)(m_data->FatCache + fatIndex)) >> 4;

        if (nextCluster >= 0xFF8) {
            nextCluster |= 0xFFFFF000;
        }
    }
    else if (m_FatType == 16) {
        nextCluster = *(uint16_t*)(m_data->FatCache + fatIndex);
        if (nextCluster >= 0xFFF8) {
            nextCluster |= 0xFFFF0000;
        }
    }
    else /*if (m_FatType == 32)*/ {
        nextCluster = *(uint32_t*)(m_data->FatCache + fatIndex);
    }

    return nextCluster;
}


bool FATFileSystem::ReadFat(uint32_t lbaOffset)
{
    return ReadSector(m_data->BS.BootSector.ReservedSectors + lbaOffset,m_data->FatCache,FatCacheSize);
}
