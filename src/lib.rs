use std::io::{Result, Error};
use std::mem::MaybeUninit;

#[derive(Debug, Clone, Eq, PartialEq)]
#[repr(C)]
pub struct DiskInfo {
    /// Number of bytes that are actually read from disk.
    pub read_bytes: u64,

    /// Number of bytes that are actually written to disk.
    pub write_bytes: u64,
}

pub fn proc_disk_info() -> Result<DiskInfo> {
    let mut buf = MaybeUninit::uninit();
    let ret = unsafe { raw::proc_disk_info(buf.as_mut_ptr()) };
    if ret == 0 {
        Ok(unsafe { buf.assume_init() })
    } else {
        Err(Error::from_raw_os_error(ret))
    }
}

mod raw {
    use super::DiskInfo;

    #[link(name = "rusage")]
    extern {
        pub(super) fn proc_disk_info(buf: *mut DiskInfo) -> i32;
    }
}
