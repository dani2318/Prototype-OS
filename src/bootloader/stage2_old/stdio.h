#pragma once
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include "x86.h"

void putc(char c);
void puts(const char* str);
void printf(const char* fmt, ...);
void clscr();
void print_buffer(const char* msg, const void* buffer, uint32_t count);
