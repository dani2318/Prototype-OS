#pragma once
#include <stddef.h>

const char* strchr(const char* str, char chr);
const char* strcpy(char* dst, const char* src);
int strcmp(const char* a, const char* b);
unsigned strlen(const char* str);
wchar_t* utf16_to_codepoint(wchar_t* string, int* codepoint);
char* codepoint_to_utf8(int codepoint, char* stringOut);
