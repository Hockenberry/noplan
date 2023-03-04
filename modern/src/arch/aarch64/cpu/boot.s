.section ".text.boot"  // Make sure the linker puts this at the start of the kernel image

_start:
    // Check processor ID is zero (executing on main core), else hang
    mrs     x1, mpidr_el1
    and     x1, x1, #3
    cmp     x0, x1
    b.ne    .parking_loop

    // We're not on the main core, so hang in an infinite wait loop
.parking_loop:  
    wfe
    b       .parking_loop

2:  // We're on the main core!

    // Set stack to start below our code
    ldr     x1, =_start
    mov     sp, x1

    // Clean the BSS section
    ldr     x1, =__bss_start     // Start address
    ldr     w2, =__bss_size      // Size of the section

3:  cbz     w2, 4f               // Quit loop if zero
    str     xzr, [x1], #8
    sub     w2, w2, #1
    cbnz    w2, 3b               // Loop if non-zero

    // Jump to our main() routine in C (make sure it doesn't return)
4:  bl      _start_rust

    // In case it does return, halt the master core too
    b       .parking_loop

.global _start  
.type   _start, function
.size   _start, . - _start
