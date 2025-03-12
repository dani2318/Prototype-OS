#pragma once
#include <core/dev/BlockDevice.hpp>
#include <core/fs/FileEntry.hpp>
#include "File.hpp"

enum FileOpenMode{
  Read,
  Write,
  Append
};

class FileSystem
{
  public:
    FileSystem() {};
    ~FileSystem(){};
    virtual bool Initialize(BlockDevice* device) = 0;
    virtual FileEntry* GetNextFileEntry(FileEntry* parent, FileEntry* previous) = 0;
    virtual File* Open(FileEntry* parent,FileOpenMode mode) = 0;
};

