#pragma once
#include <stdarg.h>
#include <dev/CharacterDevice.hpp>

void fputc(char c, CharacterDevice file);
void fputs(const char* str, CharacterDevice file);
void vfprintf(CharacterDevice file, const char* fmt, va_list args);
void fprintf(CharacterDevice file, const char* fmt, ...);
void fprint_buffer(CharacterDevice file, const char* msg, const void* buffer, uint32_t count);
