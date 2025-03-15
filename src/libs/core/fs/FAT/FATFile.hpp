#pragma once

#include <core/fs/File.hpp>
#include <core/fs/FAT/FATHeaders.hpp>
#include <core/fs/FileEntry.hpp>
#include <core/fs/FATFileSystem.hpp>

class FATFile : public File
{
public:
  FATFile();
  bool Open(FATFileSystem*fs,const FileEntry& fileEntry);
  bool OpenRootDirectory1216(FATFileSystem*fs);

  FileEntry GetNextFileEntry(const FileEntry& previous) override;
  bool IsOpened() const { return Opened; }
  bool ReadEntry(FAT_DirectoryEntry *directoryEntry);
  virtual void Seek(SeekPos pos, int rel) = 0;
  virtual size_t Write(const uint8_t* data, size_t size) = 0;
  virtual size_t Read(uint8_t* data, size_t size) = 0;
  virtual size_t Size() = 0;
  virtual size_t Position() = 0;
private:
  uint8_t m_Buffer[SectorSize];
  FATFileSystem* m_fs;
  bool Opened;
  bool m_IsRootDir;
  uint32_t m_Position;
  uint32_t m_Size;
  uint32_t m_FirstCluster;
  uint32_t m_CurrentCluster;
  uint32_t m_CurrentSectorInCluster;
};
