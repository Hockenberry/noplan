use core::fmt;

use crate::sync::{Mutex, NullLock};

pub mod null_console;

pub trait Write {
    /// Writes a single character `ch`.
    fn write_char(&self, ch: char);

    /// Writes a formatted string.
    fn write_fmt(&self, args: fmt::Arguments) -> fmt::Result;

    /// Blocks until all buffered characters put on TX line.
    fn flush(&self);
}

pub trait Read {
    fn read_char(&self) -> char {
        ' '
    }

    /// Clears the items from the receive buffer.
    fn clear_rx(&self);
}

pub trait Statistics {
    fn chars_written(&self) -> usize {
        0
    }
    fn chars_read(&self) -> usize {
        0
    }
}

pub trait All: Write + Read + Statistics {}

static CUR_CONSOLE: NullLock<&'static (dyn All + Sync)> =
    NullLock::new(&null_console::NULL_CONSOLE);

pub fn register_console(new_console: &'static (dyn All + Sync)) {
    CUR_CONSOLE.lock(|con| *con = new_console)
}

/// Return a reference to the console.
pub fn console() -> &'static dyn All {
    CUR_CONSOLE.lock(|con| *con)
}
