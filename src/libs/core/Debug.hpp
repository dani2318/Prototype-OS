#pragma once
#include <core/dev/TextDevice.hpp>


namespace Debug
{

  enum class DebugLevel{
    LVL_DEBUG    = 0,
    LVL_INFO     = 1,
    LVL_WARN     = 2,
    LVL_ERROR    = 3,
    LVL_CRITICAL = 4,
  };

  void AddOutputDevice(TextDevice* device, DebugLevel minLoglevel, bool colorOutput);



  void Debug(const char* module, const char* fmt, ...);
  void Info(const char* module, const char* fmt, ...);
  void Warn(const char* module, const char* fmt, ...);
  void Error(const char* module, const char* fmt, ...);
  void Critical(const char* module, const char* fmt, ...);
} // namespace Debug
