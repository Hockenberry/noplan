// SPDX-License-Identifier: MIT

//! The `noplan` kernal...

#![no_main]
#![no_std]

use crate::bsp::cpu::BOOT_CORE_ID;

/// Early init code.
///
mod bsp;
mod console;
mod cpu;
mod panic_wait;
mod print;

/// Early init code.
unsafe fn kernel_init() -> ! {
    print!("NOPLAN kernal v0.1 (cpu: {})\n\n", BOOT_CORE_ID);
    panic!()
}
