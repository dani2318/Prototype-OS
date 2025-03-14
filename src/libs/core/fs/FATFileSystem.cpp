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

bool FATFileSystem::ReadSector(uint32_t lba, uint8_t *buffer)
{
  m_device->Seek(SeekPos::Set, lba * SectorSize);
  return (m_device->Read(buffer, SectorSize)) == SectorSize;
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
  }
  else
  {
    rootDirLba = m_data->BS.BootSector.ReservedSectors + m_SectorsPerFat * m_data->BS.BootSector.FatCount;
    rootDirSize = sizeof(FAT_DirectoryEntry) * m_data->BS.BootSector.DirEntryCount;
    uint32_t rootDirSectors = (rootDirSize + m_data->BS.BootSector.BytesPerSector - 1) / m_data->BS.BootSector.BytesPerSector;
    m_DataSectionLba = rootDirLba + rootDirSectors;
  }

  // Detect FAT type
  DetectFatType();

  FATFileEntry rootEntry;
  rootEntry.directoryEntry.FirstClusterLow = rootDirLba & 0xFFFF;
  rootEntry.directoryEntry.FirstClusterHigh = rootDirLba & 0xFF;
  rootEntry.directoryEntry.Size = sizeof(FAT_DirectoryEntry) * m_data->BS.BootSector.DirEntryCount;
  m_data->RootDirectory.Open(&rootEntry);

  // reset open files
  for (int i = 0; i < MaxFileHandles; i++)
    m_data->OpenedFiles[i] = FATFile();

  m_data->LFNCount = 0;

  return true;
}

FileEntry* FATFileSystem::GetNextFileEntry(File* parent, const FileEntry& previous)
{
  static FileEntry fe;
  return &fe;
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

  // setup vars
  m_data->OpenedFiles[handle].Open((FATFileEntry *)file);
  return &m_data->OpenedFiles[handle];
}
