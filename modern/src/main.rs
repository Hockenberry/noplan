// SPDX-License-Identifier: MIT

//! NOPLAN - a experiment without a plan

#![no_main]
#![no_std]

use core::panic::PanicInfo;

mod cpu;

/// init code
#[allow(dead_code)]
unsafe fn kernel_init() -> ! {
    panic!()
}

#[panic_handler]
fn panic(_: &PanicInfo) -> ! {
    cpu::wait_forever()
}