use core::ptr::{read_volatile, write_volatile};

#[inline(never)]
pub fn mmio_write(dst: u64, val: u32) {
    unsafe {
        write_volatile(dst as *mut u32, val);
    }
}

#[inline(never)]
pub fn mmio_read(src: u64) -> u32 {
    unsafe { read_volatile(src as *const u32) }
}
