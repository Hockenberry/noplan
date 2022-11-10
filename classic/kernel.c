#include "dma.h"
#include "machine.h"
#include "qemu_framebuffer.h"
#include "rpi_uart.h"
#include "serial.h"
#include "mbox.h"


void _exit(int status);

void kernel_main(void) {
  uart_init();

  kprintln("Hello noplan!"); 
  kprintln(itoa(0x1234, 16));


  unsigned int *box;
  if (mbox_buffer_alloc(8 * 4, (void**)&box)){
     box[0] = 8*4;                  // length of the message
     box[1] = MBOX_REQUEST;         // this is a request message
     box[2] = MBOX_TAG_GETSERIAL;   // get serial number command
     box[3] = 8;                    // buffer size
     box[4] = 8;
     box[5] = 0;                    // clear output buffer
     box[6] = 0;
     
     box[7] = MBOX_TAG_LAST;

     if (mbox_call(MBOX_CH_PROP)){
        uart_puts("My serial number is: ");
        uart_hex(box[6]);
        uart_hex(box[5]);
        uart_puts("\n");
     } else {
       uart_puts("Unable to query serial!\n");
     }
    
  }

  kprintln("Waiting...");

  while (1) {
    uart_send(uart_getc());
  }

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
