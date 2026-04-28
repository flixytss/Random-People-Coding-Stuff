#include "drivers/vga.h"
#include <stdbool.h>
#include <stdint.h>
#define NANOPRINTF_IMPLEMENTATION
#include "terminal/printf.h"
#include "terminal/terminal.h"
#include "stdarg.h"

uint8_t printkf_text_color = VGA_COLOR_WHITE;
uint8_t previus_text_color = VGA_COLOR_WHITE;
bool text_color_restart = 0;

void set_printkf_color(enum VGA_COLOR color, bool restart /* Restart the printkf text color to the previus color after one use */) {
    previus_text_color = printkf_text_color;
    printkf_text_color = color;
    text_color_restart = restart;
}

static void wrapper(int c, void *ctx) {
    (void)ctx;
    putchar(c, printkf_text_color);
}

int printkf(const char *fmt, ...) {
    va_list a;
    va_start(a, fmt);
    int r = npf_vpprintf(wrapper, NULL, fmt, a);
    if (text_color_restart) printkf_text_color = previus_text_color;
    va_end(a);
    return r;
}

int snprintkf(char *restrict buf, size_t siz, const char *restrict fmt, ...) {
  va_list a;
  va_start(a, fmt);
  int r = npf_vsnprintf(buf, siz, fmt, a);
  va_end(a);
  return r;
}
