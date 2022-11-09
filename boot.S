	.global _start
	.global __stack_top

__stack_top:	
_start:
	// Check processor ID is zero (executing on main core), else hang
	mrs     x1, mpidr_el1
	and     x1, x1, #3
	cbz     x1, 2f

	// We're not on the main core, so hang in an infinite wait loop
1:  	wfe
	b       1b

	// We're on the main core!
	// Set stack to start below our code
2:
	ldr 	x1, = _start
	mov 	sp, x1

	// clear bss
	ldr 	x1, = __bss_start
	ldr	w2, = __bss_size

3:	cbz	w2, 4f
	str 	xzr, [x1], #8
	sub 	w2, w2, #1
	cbnz	w2, 3b

	// Jump to our main() routine in C (make sure it doesn't return)
4:	bl 	kernel_main
	b 	1b
