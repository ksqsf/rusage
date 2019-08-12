use rusage::*;
use std::fs;
use std::io::Write;

fn main() {
    let mut f = fs::OpenOptions::new()
        .create(true)
        .write(true)
        .open("file")
        .unwrap();
    unsafe {
        println!("read = {}", proc_disk_read_bytes());
    }

    for i in 0..10000000 {
        f.write(format!("{}\n", i).as_bytes()).unwrap();
        f.flush().unwrap();
        unsafe {
            println!("write = {}", proc_disk_written_bytes());
        }
    }

    drop(f);
    fs::remove_file("file");
}
