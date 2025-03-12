#pragma once
#include <core/dev/BlockDevice.hpp>
#include "File.hpp"

enum class FileType{
  File,
  Directory
};

struct FileEntry
{
  char Name[256];
  FileType type;
};

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

