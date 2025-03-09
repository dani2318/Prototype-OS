#include "TextDevice.hpp"
#include <stdarg.h>

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

bool TextDevice::VFormat(const char* str, va_list args){
  FormatState state = FormatState::NORMAL;
  FormatLength length = FormatLength::DEFAULT;
  int radix = 10;
  bool sign = false;
  bool number = false;
  bool ok = true;
  while (*str)
  {
      switch (state)
      {
          case FormatState::NORMAL:
              switch (*str)
              {
                  case '%':   state = FormatState::LENGTH;
                              break;
                  default:    ok = ok && Write(*str);
                              break;
              }
              break;

          case FormatState::LENGTH:
              switch (*str)
              {
                  case 'h':   length = FormatLength::LONG;
                              state = FormatState::LENGTH_SHORT;
                              break;
                  case 'l':   length = FormatLength::LONG;
                              state = FormatState::LENGTH_LONG;
                              break;
                  default:    goto FormatState_Spec;
              }
              break;

          case FormatState::LENGTH_SHORT:
              if (*str == 'h')
              {
                  length = FormatLength::SHORT_SHORT;
                  state = FormatState::SPEC;
              }
              else goto FormatState_Spec;
              break;

          case FormatState::LENGTH_LONG:
              if (*str == 'l')
              {
                  length = FormatLength::LONG_LONG;
                  state = FormatState::SPEC;
              }
              else goto FormatState_Spec;
              break;

          case FormatState::SPEC:
          FormatState_Spec:
              switch (*str)
              {
                  case 'c':   ok = ok &&Write((char)va_arg(args, int));
                              break;

                  case 's':
                              ok = ok && Write(va_arg(args, const char*));
                              break;

                  case '%':   ok = ok && Write('%');
                              break;

                  case 'd':
                  case 'i':   radix = 10; sign = true; number = true;
                              break;

                  case 'u':   radix = 10; sign = false; number = true;
                              break;

                  case 'X':
                  case 'x':
                  case 'p':   radix = 16; sign = false; number = true;
                              break;

                  case 'o':   radix = 8; sign = false; number = true;
                              break;

                  // ignore invalid spec
                  default:    break;
              }

              if (number)
              {
                  if (sign)
                  {
                      switch (length)
                      {
                      case FormatLength::SHORT_SHORT:
                      case FormatLength::SHORT:
                      case FormatLength::DEFAULT:     ok = ok && Write(va_arg(args, int), radix);
                                                      break;

                      case FormatLength::LONG:        ok = ok && Write(va_arg(args, long), radix);
                                                      break;

                      case FormatLength::LONG_LONG:   ok = ok && Write(va_arg(args, long long), radix);
                                                      break;
                      }
                  }
                  else
                  {
                      switch (length)
                      {
                      case FormatLength::SHORT_SHORT:
                      case FormatLength::SHORT:
                      case FormatLength::DEFAULT:     ok = ok && Write(va_arg(args, unsigned int), radix);
                                                      break;

                      case FormatLength::LONG:        ok = ok && Write(va_arg(args, unsigned  long), radix);
                                                      break;

                      case FormatLength::LONG_LONG:   ok = ok && Write(va_arg(args, unsigned  long long), radix);
                                                      break;
                      }
                  }
              }

              // reset state
              state = FormatState::NORMAL;
              length = FormatLength::DEFAULT;
              radix = 10;
              sign = false;
              number = false;
              break;
      }

      str++;
  }
  return ok;
}

bool TextDevice::Format(const char* str, ...){
  bool ok = true;
  va_list args;
  va_start(args, str);
  ok = ok && VFormat(str, args);
  va_end(args);
  return ok;
}

bool TextDevice::FormatBuffer( const char* msg, const void* buffer, size_t count){
  const uint8_t* u8Buffer = (const uint8_t*)buffer;
  bool ok = true;
  ok = ok && Write(msg);
  for (uint16_t i = 0; i < count; i++)
  {
    ok = ok && Write(g_HexChars[u8Buffer[i] >> 4]);
    ok = ok && Write(g_HexChars[u8Buffer[i] & 0xF]);
  }
  ok = ok && Write("\n");
  return true;
}
