#include "string.h"
#include <stdbool.h>
#include <stdint.h>

void memset(void* dest, int val, size_t n) { for (int i = 0; i < n; i++) (((char*)dest)[i]) = val; }
void memcpy(void* dest, const void* src, size_t n) { for (int i = 0; i < n; i++) (((char*)dest)[i]) = (((char*)src)[i]); }
void strcpy(char* d, const char* s) {
    int i = 0; while (s[i]) { d[i] = s[i]; i++; } d[i] = 0;
}
bool strcmp(const char* a, const char* b) {
    int i = 0;
    while (a[i] && b[i]) { if (a[i] != b[i]) return false; i++; }
    return a[i] == b[i];
}
bool strncmp(const char* a, const char* b, size_t n) {
    int i = 0;
    while (a[i] && b[i]) {
        if (i >= n) break;
        if (a[i] != b[i]) return false; i++;
    }
    return a[i] == b[i];
}
int strlen(const char* s) {
    int n = 0; while (s[n]) n++; return n;
}
void strcat(char* buffer, const char* src) { memcpy(buffer + strlen(buffer), src, strlen(src)); }
void strncat(char* buffer, const char* src, int n) { memcpy(buffer + strlen(buffer), src, n); }