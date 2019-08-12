#[link(name = "rusage")]
extern {
    pub fn proc_disk_read_bytes() -> u64;
    pub fn proc_disk_written_bytes() -> u64;
}
