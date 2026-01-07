use std::ptr;
use std::ffi::CString;
mod createproc;
mod decode;

fn main() {
    unsafe{
        let hmodule = kernel32::GetModuleHandleW(ptr::null());
        let data = decode::get_process_data_with_image(hmodule as usize);

        // 提供一个外壳程序
        let fake_proc_path= CString::new("fake.exe").unwrap();
        createproc::create_real_process(fake_proc_path, &data);
    }
}
