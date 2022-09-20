
void _exit(int status);

volatile unsigned int *uart = (unsigned int *) 0x09000000;
 
void putchar(char c) {
    *uart = c;
}
 
void print(const char *s) {
    while(*s != '\0') {
        putchar(*s);
        s++;
    }
}
 
void kmain(void) {
     print("Hello world!\n");
     // _exit(0);
}


/* Only 0 is supported for now, arm semihosting cannot handle it. */

// https://github.com/cirosantilli/linux-kernel-module-cheat/blob/c53ccb02782e6b5ba94c38c72597101cde86c4ff/baremetal/arch/aarch64/semihost_exit.S
void _exit(int status) {
#if defined(__arm__)
    __asm__ __volatile__ ("mov r0, #0x18; ldr r1, =#0x20026; svc 0x00123456");
#elif defined(__aarch64__)
    /* TODO actually use the exit value here, just for fun. */

    __asm__ __volatile__ (
		"mov x1, #0x26\n" \
		"movk x1, #2, lsl #16\n" \
		"str x1, [sp,#0]\n" \
		"mov x0, #15\n" \
		"str x0, [sp,#8]\n" \
		"mov x1, sp\n" \
		"mov w0, #0x18\n" \
		"hlt 0xf000\n"
    );
#endif

}
