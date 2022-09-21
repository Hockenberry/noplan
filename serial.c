#define SERIAL_MMIO 0x09000000

static volatile unsigned int *uart = (unsigned int *)SERIAL_MMIO;

void putchar(char c) { *uart = c; }

void kprint(const char *s) {
  while (*s != '\0') {
    putchar(*s);
    s++;
  }
}

void kprintln(const char *s) {
  while (*s != '\0') {
    putchar(*s);
    s++;
  }
  putchar('\n');
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
