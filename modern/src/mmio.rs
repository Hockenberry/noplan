use core::ptr::{read_volatile, write_volatile};

pub fn mmio_write(reg: u64, val: u32) {
    unsafe {
        write_volatile(reg as *mut u32, val);
    }
}

pub fn mmio_read(reg: u64) -> u32 {
    unsafe { read_volatile(reg as *const u32) }
}
