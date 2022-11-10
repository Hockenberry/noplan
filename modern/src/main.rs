// SPDX-License-Identifier: MIT

//! NOPLAN - a experiment without a plan

#![no_main]
#![no_std]

use core::panic::PanicInfo;
use core::unimplemented;

#[panic_handler]
fn panic(_: &PanicInfo) -> ! {
    unimplemented!()
}