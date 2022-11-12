// SPDX-License-Identifier: MIT OR Apache-2.0


//! The `noplan` kernal...

#![no_main]
#![no_std]

/// Early init code.
///
mod bsp;
mod cpu;
mod panic_wait;
mod print;
mod console;

/// Early init code.
unsafe fn kernel_init() -> ! {
    panic!()
}
