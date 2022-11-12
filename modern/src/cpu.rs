// SPDX-License-Identifier: MIT

#[cfg(target_arch = "aarch64")]
#[path = "arch/aarch64/cpu.rs"]
mod arch_cpu;

mod boot;
pub use arch_cpu::wait_forever;
