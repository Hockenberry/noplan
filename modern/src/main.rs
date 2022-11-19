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
mod driver;
mod panic_wait;
mod print;
mod sync;

/// Early init code.
unsafe fn kernel_init() -> ! {
    if let Err(x) = bsp::driver::init() {
        panic!("Error init BSP driver subsystem: {}", x);
    }
    driver::driver_manager().init_drivers();
    kernel_main()
}

fn kernel_main() -> ! {
    print!("[0] {} {}\n", env!("CARGO_PKG_NAME"), env!("CARGO_PKG_VERSION"));
    print!("[1] Booting on: {} (cpu: {})\n", bsp::board_name(), BOOT_CORE_ID);
    print!("[2] Drivers loaded:\n");

    driver::driver_manager().enumerate();

    print!("[3] Chars written: {}", console().chars_written());
    print!("[4] Echo input now\n");

    console().clear_rx();

    loop {
        let ch = console().read_char();
        console().write_char(ch);
    }
}
