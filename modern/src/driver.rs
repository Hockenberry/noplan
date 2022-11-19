use crate::print;
use crate::sync::{Mutex, NullLock};

pub trait DeviceDriver {
    fn compatible(&self) -> &str;

    unsafe fn init(&self) -> Result<(), &'static str> {
        Ok(())
    }
}

pub type DeviceDriverPostUnitCallback = fn() -> Result<(), &'static str>;

#[derive(Copy, Clone)]
pub struct DeviceDriverDesc {
    device_driver: &'static (dyn DeviceDriver + Sync),
    post_init_callback: Option<DeviceDriverPostUnitCallback>,
}

const NUM_DRIVERS: usize = 5;

struct DriverManagerInner {
    next_index: usize,
    descriptors: [Option<DeviceDriverDesc>; NUM_DRIVERS],
}

pub struct DriverManager {
    inner: NullLock<DriverManagerInner>,
}

static DRIVER_MANAGER: DriverManager = DriverManager::new();

pub fn driver_manager() -> &'static DriverManager {
    &DRIVER_MANAGER
}

impl DriverManagerInner {
    pub const fn new() -> Self {
        Self {
            next_index: 0,
            descriptors: [None; NUM_DRIVERS],
        }
    }
}

impl DeviceDriverDesc {
    pub fn new(
        device_driver: &'static (dyn DeviceDriver + Sync),
        post_init_callback: Option<DeviceDriverPostUnitCallback>,
    ) -> Self {
        Self {
            device_driver,
            post_init_callback,
        }
    }
}

impl DriverManager {
    pub const fn new() -> Self {
        Self {
            inner: NullLock::new(DriverManagerInner::new()),
        }
    }

    pub fn register_driver(&self, descriptor: DeviceDriverDesc) {
        self.inner.lock(|inner| {
            inner.descriptors[inner.next_index] = Some(descriptor);
            inner.next_index += 1;
        })
    }

    pub fn for_each_desc<'a>(&'a self, f: impl FnMut(&'a DeviceDriverDesc)) {
        self.inner.lock(|inner| {
            inner
                .descriptors
                .iter()
                .filter_map(|x| x.as_ref())
                .for_each(f)
        });
    }

    pub unsafe fn init_drivers(&self) {
        self.for_each_desc(|desc| {
            if let Err(x) = desc.device_driver.init() {
                panic!(
                    "Error initializing driver: {}: {}",
                    desc.device_driver.compatible(),
                    x
                );
            }

            if let Some(callback) = desc.post_init_callback {
                if let Err(x) = callback() {
                    panic!(
                        "Error during driver post-init callback: {}: {}",
                        desc.device_driver.compatible(),
                        x
                    );
                }
            }
        });
    }

    pub fn enumerate(&self) {
        let mut i = 1;
        self.for_each_desc(|desc| {
            print!("    {}. {}", i, desc.device_driver.compatible());
            i += 1;
        });
    }
}
