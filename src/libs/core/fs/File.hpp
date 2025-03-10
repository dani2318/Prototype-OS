#pragma once
#include <stddef.h>
#include <stdint.h>
#include <dev/BlockDevice.hpp>

enum class SeekPos{
  Set,
  Current,
  End
};

class File : BlockDevice{
public:

};
