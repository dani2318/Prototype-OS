#pragma once
#include <dev/CharacterDevice.hpp>

enum class SeekPos{
  Set,
  Current,
  End
};

class BlockDevice:CharacterDevice{
public:
  virtual ~BlockDevice() {};
  virtual void Seek(SeekPos pos, int rel) = 0;
  virtual size_t Size() = 0;
};
