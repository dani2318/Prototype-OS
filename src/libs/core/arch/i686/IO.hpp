#pragma once
#include <stdint.h>
#include <core/Defs.hpp>

namespace arch
{
  namespace i686{
    EXPORT void ASMCALL OutB(uint16_t port, uint8_t value);
    EXPORT uint8_t ASMCALL InB(uint16_t port);
    EXPORT void Panic();
  }
}
