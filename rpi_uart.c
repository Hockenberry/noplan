#include "gpio.h"

#define AUX_ENABLE      ((volatile unsigned int*)(MMIO_BASE+0x00215004))
#define AUX_MU_IO       ((volatile unsigned int*)(MMIO_BASE+0x00215040))
#define AUX_MU_IER      ((volatile unsigned int*)(MMIO_BASE+0x00215044))
#define AUX_MU_IIR      ((volatile unsigned int*)(MMIO_BASE+0x00215048))
#define AUX_MU_LCR      ((volatile unsigned int*)(MMIO_BASE+0x0021504C))
#define AUX_MU_MCR      ((volatile unsigned int*)(MMIO_BASE+0x00215050))
#define AUX_MU_LSR      ((volatile unsigned int*)(MMIO_BASE+0x00215054))
#define AUX_MU_MSR      ((volatile unsigned int*)(MMIO_BASE+0x00215058))
#define AUX_MU_SCRATCH  ((volatile unsigned int*)(MMIO_BASE+0x0021505C))
#define AUX_MU_CNTL     ((volatile unsigned int*)(MMIO_BASE+0x00215060))
#define AUX_MU_STAT     ((volatile unsigned int*)(MMIO_BASE+0x00215064))
#define AUX_MU_BAUD     ((volatile unsigned int*)(MMIO_BASE+0x00215068))

void uart_init(void) {
  register unsigned int r;

  /* initialize UART */
  *AUX_ENABLE |=1;       // enable UART1, AUX mini uart
  *AUX_MU_CNTL = 0;
  *AUX_MU_LCR = 3;       // 8 bits
  *AUX_MU_MCR = 0;
  *AUX_MU_IER = 0;
  *AUX_MU_IIR = 0xc6;    // disable interrupts
  *AUX_MU_BAUD = 270;    // 115200 baud

  /* map UART1 to GPIO pins */
  r=*GPFSEL1;
  r&=~((7<<12)|(7<<15)); // gpio14, gpio15
  r|=(2<<12)|(2<<15);    // alt5
  *GPFSEL1 = r;
  *GPPUD = 0;            // enable pins 14 and 15
  r=150; while(r--) { asm volatile("nop"); }
  *GPPUDCLK0 = (1<<14)|(1<<15);
  r=150; while(r--) { asm volatile("nop"); }
  *GPPUDCLK0 = 0;        // flush GPIO setup
  *AUX_MU_CNTL = 3;      // enable Tx, Rx
}

/**
 * Send a character
 */
void uart_send(unsigned int c) {
    /* wait until we can send */
    do{asm volatile("nop");}while(!(*AUX_MU_LSR&0x20));
    /* write the character to the buffer */
    *AUX_MU_IO=c;
}


void uart_write_text(const char *buffer) {
  while (*buffer) {
    if (*buffer == '\n') uart_send('\r');
    uart_send(*buffer++);
  }
}

#if 0


#define MMIO_BASE       0x3F000000

#define GPFSEL0         ((volatile unsigned int*)(MMIO_BASE+0x00200000))
#define GPFSEL1         ((volatile unsigned int*)(MMIO_BASE+0x00200004))
#define GPFSEL2         ((volatile unsigned int*)(MMIO_BASE+0x00200008))
#define GPFSEL3         ((volatile unsigned int*)(MMIO_BASE+0x0020000C))
#define GPFSEL4         ((volatile unsigned int*)(MMIO_BASE+0x00200010))
#define GPFSEL5         ((volatile unsigned int*)(MMIO_BASE+0x00200014))
#define GPSET0          ((volatile unsigned int*)(MMIO_BASE+0x0020001C))
#define GPSET1          ((volatile unsigned int*)(MMIO_BASE+0x00200020))
#define GPCLR0          ((volatile unsigned int*)(MMIO_BASE+0x00200028))
#define GPLEV0          ((volatile unsigned int*)(MMIO_BASE+0x00200034))
#define GPLEV1          ((volatile unsigned int*)(MMIO_BASE+0x00200038))
#define GPEDS0          ((volatile unsigned int*)(MMIO_BASE+0x00200040))
#define GPEDS1          ((volatile unsigned int*)(MMIO_BASE+0x00200044))
#define GPHEN0          ((volatile unsigned int*)(MMIO_BASE+0x00200064))
#define GPHEN1          ((volatile unsigned int*)(MMIO_BASE+0x00200068))
#define GPPUD           ((volatile unsigned int*)(MMIO_BASE+0x00200094))
#define GPPUDCLK0       ((volatile unsigned int*)(MMIO_BASE+0x00200098))
#define GPPUDCLK1       ((volatile unsigned int*)(MMIO_BASE+0x0020009C))

// UART

enum {
  AUX_BASE        = PERIPHERAL_BASE + 0x215000,
  AUX_ENABLES     = AUX_BASE + 4,
  AUX_MU_IO_REG   = AUX_BASE + 64,
  AUX_MU_IER_REG  = AUX_BASE + 68,
  AUX_MU_IIR_REG  = AUX_BASE + 72,
  AUX_MU_LCR_REG  = AUX_BASE + 76,
  AUX_MU_MCR_REG  = AUX_BASE + 80,
  AUX_MU_LSR_REG  = AUX_BASE + 84,
  AUX_MU_CNTL_REG = AUX_BASE + 96,
  AUX_MU_BAUD_REG = AUX_BASE + 104,
  AUX_UART_CLOCK  = 500000000,
  UART_MAX_QUEUE  = 16 * 1024
};

#define AUX_MU_BAUD(baud) ((AUX_UART_CLOCK/(baud*8))-1)

void uart_init(void) {
  mmio_write(AUX_ENABLES, 1); //enable UART1
  mmio_write(AUX_MU_IER_REG, 0);
  mmio_write(AUX_MU_CNTL_REG, 0);
  mmio_write(AUX_MU_LCR_REG, 3); //8 bits
  mmio_write(AUX_MU_MCR_REG, 0);
  mmio_write(AUX_MU_IER_REG, 0);
  mmio_write(AUX_MU_IIR_REG, 0xC6); //disable interrupts
  mmio_write(AUX_MU_BAUD_REG, AUX_MU_BAUD(115200));

  gpio_useAsAlt5(14);
  gpio_useAsAlt5(15);

  mmio_write(AUX_MU_CNTL_REG, 3); //enable RX/TX
}

unsigned int uart_isWriteByteReady() { return mmio_read(AUX_MU_LSR_REG) & 0x20; }

void uart_writeByteBlockingActual(unsigned char ch) {
  while (!uart_isWriteByteReady()); 
  mmio_write(AUX_MU_IO_REG, (unsigned int)ch);
}

void uart_write_text(const char *buffer) {
  while (*buffer) {
    if (*buffer == '\n') uart_writeByteBlockingActual('\r');
    uart_writeByteBlockingActual(*buffer++);
  }
}

#endif
