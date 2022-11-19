use core::fmt;

use crate::console::{All, Read, Statistics, Write};

pub struct NullConsole;

pub static NULL_CONSOLE: NullConsole = NullConsole {};

impl Write for NullConsole {
    fn write_char(&self, _: char) {}

    fn write_fmt(&self, _: fmt::Arguments) -> core::fmt::Result {
        fmt::Result::Ok(())
    }

    fn flush(&self) {}
}

impl Read for NullConsole {
    fn clear_rx(&self) {}
}

impl Statistics for NullConsole {}

impl All for NullConsole {}