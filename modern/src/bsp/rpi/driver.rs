use core::sync::atomic::{AtomicBool, Ordering};

use crate::bsp::bcm2xxx_gpio::GPIO;
use crate::bsp::bcm2xxx_pl011_uart::PL011Uart;
use crate::bsp::physical_memory_map::mmio;
use crate::console;
use crate::driver::{DeviceDriverDesc, driver_manager};

static UART: PL011Uart = unsafe { PL011Uart::new(mmio::PL011_UART_START) };
static GPIO: GPIO = unsafe { GPIO::new(mmio::GPIO_START) };

fn post_init_uart() -> Result<(), &'static str> {
    console::register_console(&UART);
    Ok(())
}

fn post_init_gpio() -> Result<(), &'static str> {
    GPIO.map_pl011_uart();
    Ok(())
}

fn driver_uart() -> Result<(), &'static str> {
    let desc = DeviceDriverDesc::new(&UART, Some(post_init_uart));
    driver_manager().register_driver(desc);
    Ok(())
}

fn driver_gpio() -> Result<(), &'static str> {
    let desc = DeviceDriverDesc::new(&GPIO, Some(post_init_gpio));
    driver_manager().register_driver(desc);
    Ok(())
}


pub unsafe fn init() -> Result<(), &'static str> {
    static INIT_DONE: AtomicBool = AtomicBool::new(false);
    if INIT_DONE.load(Ordering::Relaxed) {
        return Err("Init already done");
    }
    driver_uart()?;
    driver_gpio()?;
    Ok(())
}