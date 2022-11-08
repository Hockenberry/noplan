#include "mmio.h"
#include "machine.h"

void mmio_write16(sU64 addr, sU16 val) {
  volatile sU16 *mmio = (volatile sU16 *)addr;
  *mmio = val;
}

void mmio_write_bsw16(sU64 addr, sU16 val) {
  volatile sU16 *mmio = (volatile sU16 *)addr;
  *mmio = __builtin_bswap64(val);
}

void mmio_write64(sU64 addr, sU64 val) {
  volatile sU64 *mmio = (volatile sU64 *)addr;
  *mmio = val;
}

void mmio_write_bsw64(sU64 addr, sU64 val) {
  volatile sU64 *mmio = (volatile sU64 *)addr;
  *mmio = __builtin_bswap64(val);
}

sU64 mmio_read_bsw64(sU64 addr) {
  volatile sU64 *mmio = (volatile sU64 *)addr;
  return __builtin_bswap64(*mmio);
}

unsigned int mmio_read(long reg) { return *(volatile unsigned int *)reg; }

void mmio_write(long reg, unsigned int val) {
  *(volatile unsigned int *)reg = val;
}
