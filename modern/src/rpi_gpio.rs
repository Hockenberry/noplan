use crate::mmio::{mmio_read, mmio_write};
use crate::rpi_regs::PERIPHERAL_BASE;


const Pull_None: u32 = 0;

pub fn gpio_use_as_alt5(pin: u8) {
    gpio_pull(pin, Pull_None);
}

fn gpio_set(pin: u8, val: u32) -> bool {
    gpio_call(pin, val, GPSET0, 1, GPIO_MAX_PIN)
}

fn gpio_clear(pin: u8, val: u32) -> bool {
    gpio_call(pin, val, GPCLR0, 1, GPIO_MAX_PIN)
}

fn gpio_pull(pin: u8, val: u32) -> bool {
    gpio_call(pin, val, GPPUPPDN0, 2, GPIO_MAX_PIN)
}

fn gpio_function(pin: u8, val: u32) -> bool {
    gpio_call(pin, val, GPFSEL0, 3, GPIO_MAX_PIN)
}

fn gpio_call(pin: u8, val: u32, base: u64, field_size: u8, field_max: u8) -> bool {
    let field_mask = (1 << field_size) - 1;

    if pin > field_max {
        return false;
    }
    if val > field_mask {
        return false;
    }

    let num_fields = 32 / field_size;
    let reg = base + ((pin / num_fields) * 4) as u64;
    let shift = (pin % num_fields) * field_size;

    let mut curval = mmio_read(reg);
    curval &= !(field_mask << shift);
    curval |= val << shift;

    mmio_write(reg, curval);

    true
}

const GPFSEL0: u64 = PERIPHERAL_BASE + 0x200000;
const GPSET0: u64 = PERIPHERAL_BASE + 0x20001C;
const GPCLR0: u64 = PERIPHERAL_BASE + 0x200028;
const GPPUPPDN0: u64 = PERIPHERAL_BASE + 0x2000E4;

const GPIO_MAX_PIN: u8 = 53;
const GPIO_FUNCTION_ALT5: u8 = 2;
