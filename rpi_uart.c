#include "gpio.h"
#include "machine.h"

#if 0
void uart_init(void) {
  register unsigned int r;

  /* initialize UART */
  *AUX_ENABLE |= 1; // enable UART1, AUX mini uart
  *AUX_MU_CNTL = 0;
  *AUX_MU_LCR = 3; // 8 bits
  *AUX_MU_MCR = 0;
  *AUX_MU_IER = 0;
  *AUX_MU_IIR = 0xc6; // disable interrupts
  *AUX_MU_BAUD = 270; // 115200 baud

  /* map UART1 to GPIO pins */
  r = *GPFSEL1;
  r &= ~((7 << 12) | (7 << 15)); // gpio14, gpio15
  r |= (2 << 12) | (2 << 15);    // alt5
  *GPFSEL1 = r;
  *GPPUD = 0; // enable pins 14 and 15
  r = 150;
  while (r--) {
    asm volatile("nop");
  }
  *GPPUDCLK0 = (1 << 14) | (1 << 15);
  r = 150;
  while (r--) {
    asm volatile("nop");
  }
  *GPPUDCLK0 = 0;   // flush GPIO setup
  *AUX_MU_CNTL = 3; // enable Tx, Rx
}

/**
 * Send a character
 */
void uart_send(unsigned int c) {
  /* wait until we can send */
  do {
    asm volatile("nop");
  } while (!(*AUX_MU_LSR & 0x20));
  /* write the character to the buffer */
  *AUX_MU_IO = c;
}

void uart_write_text(const char *buffer) {
  while (*buffer) {
    if (*buffer == '\n')
      uart_send('\r');
    uart_send(*buffer++);
  }
}
#endif

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

unsigned int uart_isWriteByteReady() {
  return mmio_read(AUX_MU_LSR_REG) & 0x20;
}

void uart_writeByteBlockingActual(unsigned char ch) {
  while (!uart_isWriteByteReady())
    ;
  mmio_write(AUX_MU_IO_REG, (unsigned int)ch);
}

void uart_write_text(const char *buffer) {
  while (*buffer) {
    if (*buffer == '\n')
      uart_writeByteBlockingActual('\r');
    uart_writeByteBlockingActual(*buffer++);
  }
}
