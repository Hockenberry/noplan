// SPDX-License-Identifier: MIT
use core::arch::asm;

#[inline(always)]
pub fn nop() {
    unsafe {
        asm!("nop")
    }
}

/// Spin for `n` cycles.
#[inline(always)]
pub fn spin_for_cycles(n: usize) {
    unsafe {
        for _ in 0..n {
            asm!("nop")
        }
    }
}

/// Pause execution.
#[inline(always)]
pub fn wait_forever() -> ! {
    unsafe {
        loop {
            asm!("wfe")
        }
    }
}
