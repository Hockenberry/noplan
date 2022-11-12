// SPDX-License-Identifier: MIT

//! The `noplan` kernal...

#![no_main]
#![no_std]

use crate::bsp::cpu::BOOT_CORE_ID;
use crate::console::console;

/// Early init code.
///
mod bsp;
mod console;
mod cpu;
mod panic_wait;
mod print;
mod sync;

/// Early init code.
unsafe fn kernel_init() -> ! {
    print!("NOPLAN kernal v0.1 (cpu: {})\n\n", BOOT_CORE_ID);

    print!("chars written: {}", console().chars_written());

    cpu::wait_forever()
}
