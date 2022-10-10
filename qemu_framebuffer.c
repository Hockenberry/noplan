#include "qemu_framebuffer.h"


void memcpy_(void *dest, const void *src, sU64 n) {
  const sU8 *s = (const sU8 *)src;
  sU8 *d = (sU8 *)dest;
  for (int i = 0; i < n; i++) {
    d[i] = s[i];
  }
}

void write_rgb256_pixel(const fb_info *fb, sU16 x, sU16 y, const sU8 pixel[3]) {
  // offset one byte (xrgb)
  void *d = (void *)fb->fb_addr + ((y * fb->fb_stride) + (x * fb->fb_bpp));
  memcpy_(d, pixel, 4);
}

void write_xrgb256_pixel(const fb_info *fb, sU16 x, sU16 y, sU8 pixel[4]) {
  memcpy_((void *)fb->fb_addr + ((y * fb->fb_stride) + (x * fb->fb_bpp)), pixel,
          4);
}
