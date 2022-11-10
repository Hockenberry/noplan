#ifndef QEMU_FRAMEBUFFER_H
#define QEMU_FRAMEBUFFER_H

#include "machine.h"

#define fourcc_code(a, b, c, d)						\
  ((sU32)(a) | ((sU32)(b) << 8) | ((sU32)(c) << 16) | ((sU32)(d) << 24))

#define DRM_FORMAT_RGB565                                                      \
  fourcc_code('R', 'G', '1', '6') /* [15:0] R:G:B 5:6:5 little endian */
#define DRM_FORMAT_RGB888                                                      \
  fourcc_code('R', 'G', '2', '4') /* [23:0] R:G:B little endian */
#define DRM_FORMAT_XRGB8888                                                    \
  fourcc_code('X', 'R', '2', '4') /* [31:0] x:R:G:B 8:8:8:8 little endian */


#pragma pack(push, 1)

typedef struct {
  sU64 fb_addr;
  sU32 fb_width;
  sU32 fb_height;
  sU32 fb_bpp;
  sU32 fb_stride;
  sU32 fb_size;
} fb_info;

#pragma pack(pop)




int ramfb_setup(const fb_info *fb);
void write_xrgb256_pixel(const fb_info *fb, sU16 x, sU16 y, sU8 pixel[4]);
void write_rgb256_pixel(const fb_info *fb, sU16 x, sU16 y, const sU8 pixel[3]);
void draw_rgb256_map(fb_info *fb, sU32 x_res, sU32 y_res, sU8 *rgb_map);


#endif
