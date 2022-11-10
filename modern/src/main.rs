// SPDX-License-Identifier: MIT

//! NOPLAN - a experiment without a plan

#![no_main]
#![no_std]

mod bsp;
mod console;
mod cpu;
mod panic_wait;
mod print;

/// init code
#[allow(dead_code)]
unsafe fn kernel_init() -> ! {
    print!("Hello from Rust!\n");
    panic!()
}
