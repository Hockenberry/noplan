use core::arch::asm;

pub fn wait_forever() -> ! {
    unsafe {
        loop {
            asm!("wfe")
        }
    }
}