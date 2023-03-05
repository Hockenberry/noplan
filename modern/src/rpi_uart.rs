use crate::mmio::{mmio_read, mmio_write};
use crate::rpi_gpio::gpio_use_as_alt5;

fn uart_speed_from_baud(baud: u32) -> u32 {
    ((AUX_UART_CLOCK / (baud * 8)) - 1) as u32
}

pub fn uart_init() {
   

    mmio_write(AUX_ENABLES, 1);
    mmio_write(AUX_MU_CNTL_REG, 0);
    mmio_write(AUX_MU_LCR_REG, 3); // 8 bits
    mmio_write(AUX_MU_MCR_REG, 0);
    mmio_write(AUX_MU_IER_REG, 0xD);
    mmio_write(AUX_MU_IIR_REG, 0xC6); // disable interrupts

    // mmio_write(AUX_MU_BAUD_REG, uart_speed_from_baud(115200));
    mmio_write(AUX_MU_BAUD_REG, 541);

    gpio_use_as_alt5(14);
    gpio_use_as_alt5(15);
    mmio_write(AUX_MU_CNTL_REG, 3); // enable RX/TX
}

fn uart_is_byte_ready_to_send() -> bool {
    mmio_read(AUX_MU_LSR_REG) & 0x20 > 0
}

fn uart_is_byte_ready_to_recv() -> bool {
    mmio_read(AUX_MU_LSR_REG) & 0x01 > 0
}

pub fn uart_send(ch: u8) {
    while !uart_is_byte_ready_to_send() {}
    mmio_write(AUX_MU_IO_REG, ch as u32);
}

pub fn uart_getc() -> u8 {
    while !uart_is_byte_ready_to_recv() {}
    mmio_read(AUX_MU_IO_REG) as u8
}

pub fn uart_hex(d: u32) {
    let mut c = 28;
    loop {
        if c < 0 {
            break;
        }

        let n = (d >> c) & 0x0f;

        // 0-9 => '0'-'9', 10-15 => 'A'-'F'
        let n = n + if n > 9 { 0x37 } else { 0x30 };

        uart_send(n as u8);

        c -= 4;
    }
}

pub fn uart_puts(text: &str) {
    let bytes = text.as_bytes();
    for b in bytes {
        uart_send(*b);
    }
}

use crate::rpi_regs::PERIPHERAL_BASE;

const AUX_BASE: u64 = PERIPHERAL_BASE + 0x00215000;
const AUX_ENABLES: u64 = AUX_BASE + 4;
const AUX_MU_IO_REG: u64 = AUX_BASE + 64;
const AUX_MU_IER_REG: u64 = AUX_BASE + 68;
const AUX_MU_IIR_REG: u64 = AUX_BASE + 72;
const AUX_MU_LCR_REG: u64 = AUX_BASE + 76;

const AUX_MU_MCR_REG: u64 = AUX_BASE + 80;
const AUX_MU_LSR_REG: u64 = AUX_BASE + 84;

const AUX_MU_MSR_REG: u64 = AUX_BASE + 88;
const AUX_MU_SCRATCH: u64 = AUX_BASE + 92;
const AUX_MU_CNTL_REG: u64 = AUX_BASE + 96;

const AUX_MU_STAT: u64 = AUX_BASE + 100;
const AUX_MU_BAUD_REG: u64 = AUX_BASE + 104;

const AUX_UART_CLOCK: u32 = 500000000;
const UART_MAX_QUEUE: u32 = 16 * 1024;
