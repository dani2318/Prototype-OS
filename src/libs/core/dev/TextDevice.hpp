#pragma once
#include <stdarg.h>
#include <dev/CharacterDevice.hpp>
enum class FormatState
{
  NORMAL = 0,
  LENGTH = 1,
  LENGTH_SHORT = 2,
  LENGTH_LONG = 3,
  SPEC = 4
};

enum class FormatLength
{
  DEFAULT = 0,
  SHORT_SHORT = 1,
  SHORT = 2,
  LONG = 3,
  LONG_LONG = 4
};

class TextDevice
{
private:
  /* data */
public:
  TextDevice(CharacterDevice *dev);
  ~TextDevice() {};

  bool Write(char c);
  bool Write(const char *str);
  bool VFormat(const char *str, va_list file);
  bool Format(const char *str, ...);
  bool FormatBuffer(const char *msg, const void *buffer, size_t count);

  template<typename TNumber>
  bool Write(TNumber number, int base);

private:
  CharacterDevice *m_dev;
  static const char g_HexChars[];
};


template<typename TNumber>
bool TextDevice::Write(TNumber number, int base){

}
