#pragma once

#include <stdbool.h>
#include "stdint.h"

void memset(void* dest, int val, size_t n);
void memcpy(void* dest, const void* src, size_t n);
void strcpy(char* d, const char* s);
bool strcmp(const char* a, const char* b);
bool strncmp(const char* a, const char* b, size_t n);
int strlen(const char* s);
void strcat(char* buffer, const char* src);
void strncat(char* buffer, const char* src, int n);