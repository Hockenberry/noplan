#include "dma.h"
#include "machine.h"
#include "serial.h"

#pragma pack(push, 1)

#define fourcc_code(a, b, c, d)                                                \
  ((sU32)(a) | ((sU32)(b) << 8) | ((sU32)(c) << 16) | ((sU32)(d) << 24))

#define DRM_FORMAT_RGB565                                                      \
  fourcc_code('R', 'G', '1', '6') /* [15:0] R:G:B 5:6:5 little endian */
#define DRM_FORMAT_RGB888                                                      \
  fourcc_code('R', 'G', '2', '4') /* [23:0] R:G:B little endian */
#define DRM_FORMAT_XRGB8888                                                    \
  fourcc_code('X', 'R', '2', '4') /* [31:0] x:R:G:B 8:8:8:8 little endian */

#pragma pack(pop)

typedef struct {
  sU64 fb_addr;
  sU32 fb_width;
  sU32 fb_height;
  sU32 fb_bpp;
  sU32 fb_stride;
  sU32 fb_size;
} fb_info;

int ramfb_setup(const fb_info *fb);

void write_xrgb256_pixel(const fb_info *fb, sU16 x, sU16 y, sU8 pixel[4]);
void write_rgb256_pixel(const fb_info *fb, sU16 x, sU16 y, const sU8 pixel[3]);
void draw_rgb256_map(fb_info *fb, sU32 x_res, sU32 y_res, sU8 *rgb_map);

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

int ramfb_setup(const fb_info *fb) {

  kprint("Huhu\n");

  sU32 select = qemu_cfg_find_file();

  if (select == 0) {
    return 1;
  }

  const struct QemuRAMFBCfg cfg = {
      .addr = __builtin_bswap64(fb->fb_addr),
      .fourcc = __builtin_bswap32(DRM_FORMAT_XRGB8888),
      .flags = __builtin_bswap32(0),
      .width = __builtin_bswap32(fb->fb_width),
      .height = __builtin_bswap32(fb->fb_height),
      .stride = __builtin_bswap32(fb->fb_stride),
  };

  qemu_cfg_write_entry(&cfg, select, sizeof(cfg));

  return 0;
}

void write_xrgb256_pixel(const fb_info *fb, sU16 x, sU16 y, sU8 pixel[4]) {
  memcpy_((void *)fb->fb_addr + ((y * fb->fb_stride) + (x * fb->fb_bpp)), pixel,
          4);
}

void _exit(int status);

void kernel_main(void) {
  extern sU64 _stack_top;

  sU64 heap_start = (sU64)&_stack_top;

  kprintln(itoa(_stack_top, 16));

  kprint("Hello world!\n");

  int has_fw_cfg = check_fw_cfg_dma();

  if (has_fw_cfg) {
    kprint("has fw cfg!\n");
  } else {
    kprint("NO fw cfg!\n");
    return;
  }

  sU32 w = 1920;
  sU32 h = 1080;

  sU32 bpp = 4;
  sU32 stride = bpp * w;

  kprint("huhu 0\n");

  const fb_info fb = {
      .fb_addr = heap_start,
      .fb_width = w,
      .fb_height = h,
      .fb_bpp = bpp,
      .fb_stride = stride,
      .fb_size = stride * h,
  };

  kprint("huhu1\n");

  if (ramfb_setup(&fb) != 0) {
    kprint("error setting up ramfb \n");
  }

  kprint("setup ramfb successfull\n");

  sU8 pixel[3] = {255, 255, 255};

  for (int i = 0; i < fb.fb_width; i++) {
    for (int j = 0; j < fb.fb_height; j++) {
      write_rgb256_pixel(&fb, i, j, pixel);
    }
  }

  // draw_rgb256_map(&fb, 500, 500, (uint8_t*)&img[0]);

  while (1)
    ;

  // _exit(0);
}

/* Only 0 is supported for now, arm semihosting cannot handle it. */

// https://github.com/cirosantilli/linux-kernel-module-cheat/blob/c53ccb02782e6b5ba94c38c72597101cde86c4ff/baremetal/arch/aarch64/semihost_exit.S
void _exit(int status) {
#if defined(__arm__)
  __asm__ __volatile__("mov r0, #0x18; ldr r1, =#0x20026; svc 0x00123456");
#elif defined(__aarch64__)
  /* TODO actually use the exit value here, just for fun. */
  __asm__ __volatile__("mov x1, #0x26\n"
                       "movk x1, #2, lsl #16\n"
                       "str x1, [sp,#0]\n"
                       "mov x0, #15\n"
                       "str x0, [sp,#8]\n"
                       "mov x1, sp\n"
                       "mov w0, #0x18\n"
                       "hlt 0xf000\n");
#endif
}
