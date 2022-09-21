#ifndef NOPLAN_DMA_H
#define NOPLAN_DMA_H

#include "machine.h"

union FwCfgSigRead {
  sU32 u32;
  char bytes[sizeof(int)];
};

typedef struct {
  sU32 control;
  sU32 length;
  sU64 address;
} __attribute__((__packed__)) QemuCfgDmaAccess;

struct __attribute__((__packed__)) QemuRAMFBCfg {
  sU64 addr;
  sU32 fourcc;
  sU32 flags;
  sU32 width;
  sU32 height;
  sU32 stride;
};

struct QemuCfgFile {
  sU32 size;   /* file size */
  sU16 select; /* write this to 0x510 to read it */
  sU16 reserved;
  char name[56];
};

void qemu_cfg_write_entry(void *buf, sU32 e, sU32 len);
int qemu_cfg_find_file();
int check_fw_cfg_dma();

#endif
