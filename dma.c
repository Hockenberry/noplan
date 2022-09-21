#include "dma.h"
#include "serial.h"
#include "machine.h"

#define QEMU_CFG_FILE_DIR 0x19

#define QEMU_CFG_DMA_CTL_ERROR 0x01
#define QEMU_CFG_DMA_CTL_READ 0x02
#define QEMU_CFG_DMA_CTL_SKIP 0x04
#define QEMU_CFG_DMA_CTL_SELECT 0x08
#define QEMU_CFG_DMA_CTL_WRITE 0x10

#define BASE_ADDR 0x09020000
#define BASE_ADDR_SELECTOR 0x9020008
#define BASE_ADDR_DATA 0x09020000
#define BASE_ADDR_ADDR 0x9020010

int memcmp_str(const void *str1, const void *str2, sU64 count) {
  const unsigned char *s1 = (const unsigned char *)str1;
  const unsigned char *s2 = (const unsigned char *)str2;
  while (count-- > 0) {
    if (*s1++ != *s2++)
      return s1[-1] < s2[-1] ? -1 : 1;
  }
  return 0;
}

static void mmio_write16(sU64 addr, sU16 val) {
  volatile sU16 *mmio = (volatile sU16 *)addr;
  *mmio = val;
}

static void mmio_write_bsw16(sU64 addr, sU16 val) {
  volatile sU16 *mmio = (volatile sU16 *)addr;
  *mmio = __builtin_bswap64(val);
}

static void mmio_write64(sU64 addr, sU64 val) {
  volatile sU64 *mmio = (volatile sU64 *)addr;
  *mmio = val;
}

static void mmio_write_bsw64(sU64 addr, sU64 val) {
  volatile sU64 *mmio = (volatile sU64 *)addr;
  *mmio = __builtin_bswap64(val);
}

static sU64 mmio_read_bsw64(sU64 addr) {
  volatile sU64 *mmio = (volatile sU64 *)addr;
  return __builtin_bswap64(*mmio);
}

static void qemu_cfg_dma_transfer(void *address, sU32 length, sU32 control) {
  QemuCfgDmaAccess access = {.address = __builtin_bswap64((sU64)address),
                             .length = __builtin_bswap32(length),
                             .control = __builtin_bswap32(control)};

  if (length == 0) {
    return;
  }

  __asm__("ISB");

  mmio_write_bsw64(BASE_ADDR_ADDR, (sU64)&access);

  while (__builtin_bswap32(access.control) & ~QEMU_CFG_DMA_CTL_ERROR) {
  }
}

static void qemu_cfg_read(void *buf, int len) {
  qemu_cfg_dma_transfer(buf, len, QEMU_CFG_DMA_CTL_READ);
}

static void qemu_cfg_read_entry(void *buf, int e, int len) {
  sU32 control = (e << 16) | QEMU_CFG_DMA_CTL_SELECT | QEMU_CFG_DMA_CTL_READ;
  qemu_cfg_dma_transfer(buf, len, control);
}

void qemu_cfg_write_entry(void *buf, sU32 e, sU32 len) {
  sU32 control = (e << 16) | QEMU_CFG_DMA_CTL_SELECT | QEMU_CFG_DMA_CTL_WRITE;
  qemu_cfg_dma_transfer(buf, len, control);
}

int qemu_cfg_find_file(void) {
  kprint("qemu_cfg_find_file");
  sU32 count, e, select;
  qemu_cfg_read_entry(&count, QEMU_CFG_FILE_DIR, sizeof(count));
  count = __builtin_bswap32(count);

  for (select = 0, e = 0; e < count; e++) {
    struct QemuCfgFile qfile;
    qemu_cfg_read(&qfile, sizeof(qfile));
    if (memcmp_str(qfile.name, "etc/ramfb", 10) == 0)
      select = __builtin_bswap16(qfile.select);
  }
  return select;
}

int check_fw_cfg_dma() {
  mmio_write16(BASE_ADDR_SELECTOR, 0x0000);
  union FwCfgSigRead cfg_sig_read;
  cfg_sig_read.u32 = *((volatile sU32 *)BASE_ADDR_DATA);
  if (cfg_sig_read.bytes[0] == 'Q' && cfg_sig_read.bytes[1] == 'E' &&
      cfg_sig_read.bytes[2] == 'M' && cfg_sig_read.bytes[3] == 'U') {
    if (mmio_read_bsw64(BASE_ADDR_ADDR) == 0x51454d5520434647) {
      return 1;
    }
  }
  return 0;
}
