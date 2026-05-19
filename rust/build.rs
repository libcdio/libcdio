use std::env;
use std::path::{Path, PathBuf};

fn main() {
    let headers = Path::new("../include/");
    let lib = Path::new("../lib/driver/");
    let root = Path::new("../"); // for config.h

    let out_path = PathBuf::from(env::var("OUT_DIR").unwrap());

    bindgen::Builder::default()
        .header(headers.join("cdio/mmc.h").to_string_lossy())
        .header(lib.join("mmc/mmc_private.h").to_string_lossy())
        .clang_arg("-I".to_owned() + headers.to_str().unwrap())
        .clang_arg("-I".to_owned() + root.to_str().unwrap()) // config.h
        .clang_arg("-DHAVE_CONFIG_H")
        .parse_callbacks(Box::new(bindgen::CargoCallbacks::new()))
        .default_enum_style(bindgen::EnumVariation::ModuleConsts)
        .generate()
        .expect("Unable to generate bindings")
        .write_to_file(out_path.join("bindings.rs"))
        .expect("Couldn't write bindings!");
}
