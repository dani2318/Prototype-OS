#pragma once
#include <dev/BlockDevice.hpp>

enum class FileType{
  File,
  Directory
};

struct FileEntry
{
  char Name[256];
  FileType type;
};


class FileSystem
{
  public:
    FileSystem();
    ~FileSystem(){};
    virtual bool Initialize(BlockDevice* device) = 0;
};

