#include "qemu_uart.h"

#define SERIAL_MMIO 0x09000000

static volatile unsigned int *uart = (unsigned int *)SERIAL_MMIO;

void qemu_putchar(char c) { *uart = c; }
