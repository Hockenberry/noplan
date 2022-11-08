#ifndef NOPLAN__MMIO_H
#define NOPLAN__MMIO_H

#pragma once

#include "machine.h"

void mmio_write16(sU64 addr, sU16 val);
void mmio_write_bsw16(sU64 addr, sU16 val);
void mmio_write64(sU64 addr, sU64 val);
void mmio_write_bsw64(sU64 addr, sU64 val);
sU64 mmio_read_bsw64(sU64 addr);

void mmio_write(long reg, unsigned int val);

unsigned int mmio_read(long reg);

#endif // NOPLAN__MMIO_H
