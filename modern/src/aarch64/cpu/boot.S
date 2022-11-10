.section .text._start

_start:
	// Check processor ID is zero (executing on main core), else hang
	mrs     x1, mpidr_el1
	and     x1, x1, #3
	cbz     x1, .cpu_parking_loop

.cpu_parking_loop:
  wfe
  b	  .cpu_parking_loop

  ldr x1, = __bss_start
  ldr w2, = __bss_size

#.bss_init_loop:
#  cbz	  w2, 4f
#	str 	xzr, [x1], #8
#	sub 	w2, w2, #1
#	cbnz	w2, .bss_init_loop

.prepare_rust
  ldr   x1, = _start
  mov   sp, x1
  b     _start_rust



.size	_start, . - _start
.type	_start, function
.global	_start
