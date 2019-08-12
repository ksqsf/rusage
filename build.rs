use cc;

fn main() {
    cc::Build::new()
        .file("src/rusage.c")
        .compile("librusage.a");
}
