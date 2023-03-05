use crate::mmio::{mmio_read, mmio_write};
use crate::rpi_regs::PERIPHERAL_BASE;

const PULL_NONE: u32 = 0;

pub fn gpio_use_as_alt5(pin: u8) {
    gpio_pull(pin, PULL_NONE);
    gpio_function(pin, GPIO_FUNCTION_ALT5 as u32);
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

pub fn led_init() {
    let val: u32 = 1 << LED_GPFBIT;
    let reg = GPIO_BASE + LED_GPFSEL;
    let old = mmio_read(reg);
    let new = old | val;

    mmio_write(reg, new);
}

#[inline(never)]
pub fn led_blink() {
    for _ in 0..750000 {}
    mmio_write(GPIO_BASE + LED_GPCLR, 1 << LED_GPIO_BIT);

    for _ in 0..750000 {}
    mmio_write(GPIO_BASE + LED_GPSET, 1 << LED_GPIO_BIT);
}

const GPIO_BASE: u64 = PERIPHERAL_BASE + 0x200000;

const GPFSEL0: u64 = PERIPHERAL_BASE + 0x200000;
const GPSET0: u64 = PERIPHERAL_BASE + 0x20001C;
const GPCLR0: u64 = PERIPHERAL_BASE + 0x200028;
const GPPUPPDN0: u64 = PERIPHERAL_BASE + 0x2000E4;

const GPIO_GPFSEL0: u64 = 0;
const GPIO_GPFSEL1: u64 = 1;
const GPIO_GPFSEL2: u64 = 2;
const GPIO_GPFSEL3: u64 = 3;
const GPIO_GPFSEL4: u64 = 4;
const GPIO_GPFSEL5: u64 = 5;

const GPIO_GPSET0: u64 = 7;
const GPIO_GPSET1: u64 = 8;

const GPIO_GPCLR0: u64 = 10;
const GPIO_GPCLR1: u64 = 11;

const GPIO_GPLEV0: u64 = 13;
const GPIO_GPLEV1: u64 = 14;

const GPIO_GPEDS0: u64 = 16;
const GPIO_GPEDS1: u64 = 17;

const GPIO_GPREN0: u64 = 19;
const GPIO_GPREN1: u64 = 20;

const GPIO_GPFEN0: u64 = 22;
const GPIO_GPFEN1: u64 = 23;

const GPIO_GPHEN0: u64 = 25;
const GPIO_GPHEN1: u64 = 26;

const GPIO_GPLEN0: u64 = 28;
const GPIO_GPLEN1: u64 = 29;

const GPIO_GPAREN0: u64 = 31;
const GPIO_GPAREN1: u64 = 32;

const GPIO_GPAFEN0: u64 = 34;
const GPIO_GPAFEN1: u64 = 35;

const GPIO_GPPUD: u64 = 37;
const GPIO_GPPUDCLK0: u64 = 38;
const GPIO_GPPUDCLK1: u64 = 39;

const LED_GPFSEL: u64 = GPIO_GPFSEL4;
const LED_GPFBIT: u64 = 6;
const LED_GPSET: u64 = GPIO_GPSET1;
const LED_GPCLR: u64 = GPIO_GPCLR1;
const LED_GPIO_BIT: u64 = 10;

const GPIO_MAX_PIN: u8 = 53;
const GPIO_FUNCTION_ALT5: u8 = 2;
