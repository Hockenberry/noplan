#include "serial.h"

#include "qemu_uart.h"

void kprint(const char *s) {
  while (*s != '\0') {
    qemu_putchar(*s);
    s++;
  }
}

void kprintln(const char *s) {
  while (*s != '\0') {
    qemu_putchar(*s);
    s++;
  }
  qemu_putchar('\n');
}

char *itoa(int val, int base) {
  static const char DIGITS[] = "0123456789abcdef";
  static char buf[32] = {0};
  int i = 30;
  for (; val && i; --i, val /= base) {
    buf[i] = DIGITS[val % base];
  }
  return &buf[i + 1];
}

