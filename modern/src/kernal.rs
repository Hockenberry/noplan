// SPDX-License-Identifier: MIT

//! The `noplan` kernal...

#![no_main]
#![no_std]

use rpi_gpio::{led_blink, led_init};

use crate::rpi_regs::PERIPHERAL_BASE;
use crate::rpi_uart::{uart_init, uart_send};

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
    uart_init();
    led_init();

    stars_draw(10);
    kernel_main();
}

fn stars_draw(n: u8) {
    for _ in 0..n {
        uart_send(b'*');
        uart_send(b'\r');
        uart_send(b'\n');
    }
}

fn kernel_main() -> ! {
    print!(
        "[0] {} {}\n",
        env!("CARGO_PKG_NAME"),
        env!("CARGO_PKG_VERSION")
    );

    print!("PERIPHERAL_BASE = {PERIPHERAL_BASE:0X}\n");

    for _ in 0..20 {
        stars_draw(1);
        led_blink();
    }

    // print!(
    //     "[1] Booting on: {} (cpu: {})\n",
    //     bsp::board_name(),
    //     BOOT_CORE_ID
    // );

    print!("wait forever");

    loop {}

    // driver::driver_manager().enumerate();

    // print!("[3] Chars written: {}", console().chars_written());
    // print!("[4] Echo input now\n");

    // console().clear_rx();

    // loop {
    //     let ch = console().read_char();
    //     console().write_char(ch);
    // }
}
