use crate::sync::{Mutex, NullLock};
use crate::uart_send;

use core::fmt;

pub trait Write2 {
    /// Writes a formatted string.
    fn write_fmt(&self, args: fmt::Arguments) -> fmt::Result;
}

pub trait All2: Write2 {}

struct UART0 {}

impl core::fmt::Write for UART0 {
    fn write_str(&mut self, s: &str) -> fmt::Result {
        for b in s.bytes() {
            uart_send(b);
        }
        Ok(())
    }
}
struct Uart {
    inner: NullLock<UART0>,
}

impl Uart {
    const fn new() -> Self {
        Self {
            inner: NullLock::new(UART0 {}),
        }
    }
}

impl Write2 for Uart {
    fn write_fmt(&self, args: fmt::Arguments) -> fmt::Result {
        self.inner
            .lock(|inner| core::fmt::Write::write_fmt(inner, args))
    }
}

impl All2 for Uart {}

static SIMPLE_UART: Uart = Uart::new();

static SIMPLE: NullLock<&'static (dyn All2 + Sync)> = NullLock::new(&SIMPLE_UART);

pub fn simple_console() -> &'static dyn All2 {
    SIMPLE.lock(|c| *c)
}
