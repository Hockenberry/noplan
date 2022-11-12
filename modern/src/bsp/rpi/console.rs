use core::fmt;
use core::fmt::Arguments;

use crate::console::{All, Statistics, Write};
use crate::sync::{Mutex, NullLock};

struct QEMUOutputData {
    chars_written: usize,
}

/// A mystical, magical device for generating QEMU output out of the void.

struct QEMUOutput {
    inner: NullLock<QEMUOutputData>,
}


static QEMUOUTPUT: QEMUOutput = QEMUOutput::new();


impl QEMUOutputData {
    const fn new() -> QEMUOutputData {
        QEMUOutputData { chars_written: 0 }
    }

    fn write_char(&mut self, c: char) {
        unsafe {
            core::ptr::write_volatile(0x3F20_1000 as *mut u8, c as u8);
        }
        self.chars_written += 1;
    }
}

/// Implementing `core::fmt::Write` enables usage of the `format_args!` macros, which in turn are
/// used to implement the `kernel`'s `print!` and `println!` macros. By implementing `write_str()`,
/// we get `write_fmt()` automatically.
///
/// See [`src/print.rs`].
///
/// [`src/print.rs`]: ../../print/index.html
impl fmt::Write for QEMUOutputData {
    fn write_str(&mut self, s: &str) -> fmt::Result {
        for c in s.chars() {
            if c == '\n' {
                self.write_char('\r');
            }
            self.write_char(c);
        }

        Ok(())
    }
}


impl QEMUOutput {
    pub const fn new() -> QEMUOutput {
        QEMUOutput {
            inner: NullLock::new(QEMUOutputData::new()),
        }
    }
}

/// Return a reference to the console.
pub fn console_ref() -> &'static dyn All {
    &QEMUOUTPUT
}

impl Write for QEMUOutput {
    fn write_fmt(&self, args: Arguments<'_>) -> fmt::Result {
        self.inner.lock(|inner| fmt::Write::write_fmt(inner, args))
    }
}

impl Statistics for QEMUOutput {
    fn chars_written(&self) -> usize {
        self.inner.lock(|inner| inner.chars_written)
    }
}

impl All for QEMUOutput {}