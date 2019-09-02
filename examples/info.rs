use rusage::*;
use std::fs;
use std::io::Write;

fn main() {
    let mut f = fs::OpenOptions::new()
        .create(true)
        .write(true)
        .open("file")
        .unwrap();
    println!("read = {:?}", proc_disk_info().unwrap());

    for i in 0..10000000 {
        f.write(format!("{}\n", i).as_bytes()).unwrap();
        f.flush().unwrap();
        println!("write = {:?}", proc_disk_info().unwrap());
    }

    drop(f);
    fs::remove_file("file").unwrap();
}
