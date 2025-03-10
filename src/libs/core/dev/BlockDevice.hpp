#pragma once
#include <core/dev/CharacterDevice.hpp>

enum class SeekPos{
  Set,
  Current,
  End
};

class BlockDevice:CharacterDevice{
public:
  virtual ~BlockDevice() {};
  virtual void Seek(SeekPos pos, int rel) = 0;
  virtual size_t Write(const uint8_t* data, size_t size) = 0;
  virtual size_t Read(uint8_t* data, size_t size) = 0;
  virtual size_t Size() = 0;
  virtual size_t Position() = 0;
};
