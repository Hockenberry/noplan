// SPDX-License-Identifier: MIT

//! Conditional reexporting of Board Support Packages.

#[cfg(any(feature = "bsp_rpi3", feature = "bsp_rpi4"))]
mod rpi;

#[cfg(any(feature = "bsp_rpi3", feature = "bsp_rpi4"))]
pub use rpi::*;
