use winapi::shared::basetsd::ULONG_PTR;
use winapi::um::processthreadsapi::{CreateProcessA, ResumeThread, ExitProcess, STARTUPINFOA, PROCESS_INFORMATION};
use winapi::um::winbase::{Wow64GetThreadContext, Wow64SetThreadContext,CREATE_SUSPENDED};
use winapi::um::winnt::{WOW64_CONTEXT, HANDLE, CONTEXT_FULL, MEM_COMMIT, MEM_RESERVE, PAGE_EXECUTE_READWRITE};
use winapi::um::memoryapi::{VirtualAllocEx, ReadProcessMemory, WriteProcessMemory};
use std::ptr::{null, null_mut};
use winapi::ctypes::c_void;
use windows::Wdk::System::SystemServices::ZwUnmapViewOfSection;
use std::ffi::CString;
use petool::PEHeader;
use std::error::Error;

fn alloc_for_real_proc(handle: HANDLE, virtual_addr: u32, size: u32) -> Result<(), Box<dyn Error>>{
    unsafe{
        let result = VirtualAllocEx(
            handle,
            virtual_addr as *mut c_void, 
            size as ULONG_PTR, 
            MEM_RESERVE | MEM_COMMIT, 
            PAGE_EXECUTE_READWRITE
        );

        if result == null_mut(){
            return Err("alloc virtual fail".into());
        }
        else{
            return Ok(());
        }

    }  
}

fn unload_protector_image(handle: HANDLE, base_address: *mut c_void) -> Result<(), Box<dyn Error>>{
    unsafe{
        use windows::Win32::Foundation::HANDLE;
        match ZwUnmapViewOfSection(HANDLE(handle as isize), Some(base_address))
        {
            Ok(_) => return Ok(()),
            Err(_) => return Err("(unmap image fail)".into())
        }
    }
}

fn create_suspended_proc(path: CString) -> Result<PROCESS_INFORMATION, Box<dyn Error>> {
    unsafe{
        let mut si: STARTUPINFOA = std::mem::MaybeUninit::zeroed().assume_init();
        let mut pi: PROCESS_INFORMATION = std::mem::MaybeUninit::zeroed().assume_init();
        let is_create = CreateProcessA(
            path.as_ptr(),
            null_mut(),
            null_mut(),
            null_mut(),
            0,
            CREATE_SUSPENDED,
            null_mut(),
            null(),
            &mut si,
            &mut pi,
        );
        if is_create == 0{
            return Err("create suspended process fail".into());
        }
        else {
            return Ok(pi);
        }
    }  
}

fn get_context(thread_handle: HANDLE) -> Result<WOW64_CONTEXT, Box<dyn Error>>{
    unsafe{
        let mut context:WOW64_CONTEXT = std::mem::MaybeUninit::zeroed().assume_init();
        context.ContextFlags = CONTEXT_FULL;
        let result = Wow64GetThreadContext(thread_handle, &mut context);
        if result == 0{
            return Err("(get context fail)".into());
        }
        else {
            return Ok(context);
        }
    }
}

fn write_real_proc_to_virtual_mem(proc_handle: HANDLE, base_addr: u32, real_proc_data: &Vec<u8>) -> Result<(), Box<dyn Error>>{
    unsafe{
        let mut byte_size: usize= 0;
        let is_write = WriteProcessMemory(
            proc_handle, 
            base_addr as *mut c_void, 
            real_proc_data.as_ptr() as *const _, 
            real_proc_data.len(), 
            &mut byte_size
        );
        if is_write == 0 {
            return Err("can't write real proc to virtual memory".into());
        }
        else{
            return Ok(());
        }
    }
}

pub fn create_real_process(fake_proc_path: CString, real_process_data_image: &Vec<u8>){
    unsafe{
        // 1. 以挂起的形式创建一个外壳进程
        let suspended_proc_pi: PROCESS_INFORMATION = create_suspended_proc(fake_proc_path).unwrap();
        

        // 2. 获得挂起进程的context 
        let mut suspended_proc_context = get_context(suspended_proc_pi.hThread).unwrap();

        // 3. 卸载挂起进程的映像
        // context.ebx保存了peb的首地址， context.ebx + 8记录了baseAddress
        // 因为需要获得挂起进程的baseAddr，而不是当前进程的，故需要调用ReadProcessMemory
        let suspended_proc_base_addr = 0;
        ReadProcessMemory(
            suspended_proc_pi.hProcess, 
            (suspended_proc_context.Ebx + 8) as *const _, 
            &suspended_proc_base_addr as *const _ as *mut c_void, 
            4, 
            null_mut() as *mut usize
        );
        unload_protector_image(suspended_proc_pi.hProcess, suspended_proc_base_addr as *mut c_void).unwrap();

        // 4. 为真正的程序分配空间
        // 分配的位置 = 真正程序的imagebase，这样可以避免“重定位”过程
        let mut pe_header = PEHeader::new();
        pe_header.set_pe_header(real_process_data_image.as_ptr() as usize).unwrap();
        alloc_for_real_proc(
            suspended_proc_pi.hProcess, 
            (*pe_header.optional_header).ImageBase,
            real_process_data_image.len() as u32
        ).unwrap();

        // 5. 将real_process的image_data放入申请的空间中
        write_real_proc_to_virtual_mem(suspended_proc_pi.hProcess,
            (*pe_header.optional_header).ImageBase, 
            &real_process_data_image
        ).unwrap();
    

        // 6. 修复context的baseAddress和oep，结束当前进程
        let real_proc_base_addr = (*pe_header.optional_header).ImageBase;
        WriteProcessMemory(
            suspended_proc_pi.hProcess, 
            (suspended_proc_context.Ebx + 8) as *const u32 as *mut c_void, 
            &real_proc_base_addr as *const u32 as *const c_void,
            4, 
            null_mut());
        suspended_proc_context.Eax = (*pe_header.optional_header).ImageBase + (*pe_header.optional_header).AddressOfEntryPoint;
        Wow64SetThreadContext(suspended_proc_pi.hThread, &suspended_proc_context);
        ResumeThread(suspended_proc_pi.hThread);
        ExitProcess(0);
    }
}