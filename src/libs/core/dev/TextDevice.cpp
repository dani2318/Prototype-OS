#include "TextDevice.hpp"

const char TextDevice::g_HexChars[] = "0123456789abcdef";

TextDevice::TextDevice(CharacterDevice* dev)
  : m_dev(dev)
{
}

bool TextDevice::Write(char c){
  return m_dev->Write(reinterpret_cast<const uint8_t*>(&c),sizeof(c)) == sizeof(c);
}
bool TextDevice::Write(const char* str){
  bool ok = true;
  while(*str && ok)
  {
      ok = ok && Write(*str);
      str++;
  }

  return ok;
}
bool TextDevice::VFormat(const char* str, va_list file){

}
bool TextDevice::Format(const char* str, ...){

}
bool TextDevice::FormatBuffer( const char* msg, const void* buffer, size_t count);
