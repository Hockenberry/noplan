#include "rpi_uart.h"

#include "gpio.h"
#include "mmio.h"

#include <stdbool.h>

// UART

enum : sU64 {
  PERIPHERAL_BASE = 0x3F000000,
  AUX_BASE = PERIPHERAL_BASE + 0x215000,
  AUX_ENABLES = AUX_BASE + 0x04,
  AUX_MU_IO_REG = AUX_BASE + 0x40,
  AUX_MU_IER_REG = AUX_BASE + 0x44,
  AUX_MU_IIR_REG = AUX_BASE + 0x48,
  AUX_MU_LCR_REG = AUX_BASE + 0x4C,

  AUX_MU_MCR_REG = AUX_BASE + 0x50,
  AUX_MU_LSR_REG = AUX_BASE + 0x54,

  AUX_MU_MSR_REG = AUX_BASE + 0x58,
  AUX_MU_SCRATCH = AUX_BASE + 0x5C,
  AUX_MU_CNTL_REG = AUX_BASE + 0x60,

  AUX_MU_STAT = AUX_BASE + 0x64,
  AUX_MU_BAUD_REG = AUX_BASE + 0x68,

  AUX_UART_CLOCK = 500000000,
  UART_MAX_QUEUE = 16 * 1024
};

#define AUX_MU_BAUD(baud) ((AUX_UART_CLOCK / (baud * 8)) - 1)

void uart_init(void) {
  mmio_write(AUX_ENABLES, 1); // enable UART1
  mmio_write(AUX_MU_IER_REG, 0);
  mmio_write(AUX_MU_CNTL_REG, 0);
  mmio_write(AUX_MU_LCR_REG, 3); // 8 bits
  mmio_write(AUX_MU_MCR_REG, 0);
  mmio_write(AUX_MU_IER_REG, 0);
  mmio_write(AUX_MU_IIR_REG, 0xC6); // disable interrupts
  mmio_write(AUX_MU_BAUD_REG, AUX_MU_BAUD(115200));

  gpio_useAsAlt5(14);
  gpio_useAsAlt5(15);

  mmio_write(AUX_MU_CNTL_REG, 3); // enable RX/TX
}

static unsigned int uart_is_byte_ready_to_send(void) {
  return mmio_read(AUX_MU_LSR_REG) & 0x20;
}

static bool uart_is_byte_ready_to_recv(void) {
  return (mmio_read(AUX_MU_LSR_REG) & 0x01) != 0;
}


void uart_send(unsigned char ch) {
  while (!uart_is_byte_ready_to_send())
    ;
  mmio_write(AUX_MU_IO_REG, (unsigned int)ch);
}

void uart_hex(unsigned int d) {
  for (int c = 28; c >= 0; c-=4) {
    unsigned int n = (d >> c) & 0xF;
    // 0-9 => '0'-'9', 10-15 => 'A'-'F'
    n += n > 9 ? 0x37 : 0x30;
    uart_send(n);
  }
}


/**
 * Receive a character
 */
unsigned char uart_getc(void) {
    unsigned char r;

    while (!uart_is_byte_ready_to_recv())
      ;
    
    r = mmio_read(AUX_MU_IO_REG);

    /* convert carriage return to newline */
    return r == '\r' ? '\n' : r;
}


void uart_puts(const char *buffer) {
  while (*buffer) {
    if (*buffer == '\n') {
      uart_send('\r');
    }
    uart_send(*buffer++);
  }
}
