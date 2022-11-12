// SPDX-License-Identifier: MIT
//
// Copyright (c) 2021-2022 Andre Richter <andre.o.richter@gmail.com>

.macro ADR_REL register, symbol
	adrp	\register, \symbol
	add	\register, \register, #:lo12:\symbol
.endm

.section .text._start

_start:
	// Only proceed on the boot core. Park it otherwise.
	mrs	x0, MPIDR_EL1
	and	x0, x0, #3
	ldr	x1, BOOT_CORE_ID      // provided by bsp/__board_name__/cpu.rs
	cmp	x0, x1
	bne	.parking_loop

	// If execution reaches here, it is the boot core.

	// Initialize DRAM.
	ADR_REL	x0, __bss_start
	ADR_REL x1, __bss_end_exclusive

.bss_init_loop:
	cmp	x0, x1
	beq	.prepare_rust
	stp	xzr, xzr, [x0], #16
	b	.bss_init_loop

	// Prepare the jump to Rust code.
.prepare_rust:
	// Set the stack pointer.
	ADR_REL	x0, __boot_core_stack_end_exclusive
	mov	sp, x0

	// Jump to Rust code.
	b	_start_rust
	b 	.parking_loop

	// Infinitely wait for events (aka "park the core").
.parking_loop:
	wfe
	b	.parking_loop

	
.size	_start, . - _start
.type	_start, function
.global	_start
