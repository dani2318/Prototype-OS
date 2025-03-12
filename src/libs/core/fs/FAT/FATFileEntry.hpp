#pragma once
#include <core/fs/FileEntry.hpp>

class FATFileEntry : public FileEntry
{
  public:
    FAT_DirectoryEntry directoryEntry;
};

