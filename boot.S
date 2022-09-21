.global _start

_start:
    // Check processor ID is zero (executing on main core), else hang
    mrs     x1, mpidr_el1
    and     x1, x1, #3
    cbz     x1, 2f

    // We're not on the main core, so hang in an infinite wait loop
1:  wfe
    b       1b

    // We're on the main core!
    // Set stack to start below our code
2:
    ldr x30, = _stack_top
    mov sp, x30

    // Jump to our main() routine in C (make sure it doesn't return)
    bl kernel_main
    b .