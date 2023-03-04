// SPDX-License-Identifier: MIT

//! The `noplan` kernal...

#![no_main]
#![no_std]

use rpi_gpio::led_blink;

use crate::rpi_uart::{uart_init, uart_send};

/// Early init code.
///
mod bsp;
mod cpu;
mod mmio;
mod panic_wait;
mod print;
mod rpi_gpio;
mod rpi_regs;
mod rpi_uart;
mod simple_console;
mod sync;

/// Early init code.
unsafe fn kernel_init() -> ! {
    for _ in 0..20 {
        led_blink();
    }

    uart_init();
    loop {
        uart_send(b'*');
        uart_send(b'\n');
    }

    //if let Err(x) = bsp::driver::init() {
    //    panic!("Error init BSP driver subsystem: {}", x);
    //}
    //driver::driver_manager().init_drivers();

    //kernel_main()
}

fn kernel_main() -> ! {
    loop {
        uart_send(b'A');
    }
    // uart_send(b'\n');

    // print!(
    //     "[0] {} {}\n",
    //     env!("CARGO_PKG_NAME"),
    //     env!("CARGO_PKG_VERSION")
    // );
    // print!(
    //     "[1] Booting on: {} (cpu: {})\n",
    //     bsp::board_name(),
    //     BOOT_CORE_ID
    // );

    // print!("PERIPHERAL_BASE = {PERIPHERAL_BASE:0X}\n");

    // loop {
    //     print!("*");
    // }

    // driver::driver_manager().enumerate();

    // print!("[3] Chars written: {}", console().chars_written());
    // print!("[4] Echo input now\n");

    // console().clear_rx();

    // loop {
    //     let ch = console().read_char();
    //     console().write_char(ch);
    // }
}
