#[cfg(target_arch = "aarch64")]
#[path = "../arch/aarch64/cpu/boot.rs"]
mod arch_boot;

#[no_mangle]
#[link_section = ".text"]
pub static BOOT_CORE_ID: u64 = 0;
