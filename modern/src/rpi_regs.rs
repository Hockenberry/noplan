#[cfg(feature = "bsp_rpi3")]
pub const PERIPHERAL_BASE: u64 = 0x3F000000;

#[cfg(feature = "bsp_rpi4")]
pub const PERIPHERAL_BASE: u64 = 0xFE000000;
