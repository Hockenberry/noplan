// SPDX-License-Identifier: MIT

pub use arch_cpu::nop;
pub use arch_cpu::spin_for_cycles;
pub use arch_cpu::wait_forever;

#[cfg(target_arch = "aarch64")]
#[path = "arch/aarch64/cpu.rs"]
mod arch_cpu;

pub(crate) mod boot;
