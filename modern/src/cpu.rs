#[cfg(target_arch = "aarch64")]
#[path = "aarch64/cpu.rs"]
mod arch_cpu;

pub use arch_cpu::wait_forever;