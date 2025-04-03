use winapi::um::winnt::{
    IMAGE_DOS_HEADER, IMAGE_NT_HEADERS32,  IMAGE_FILE_HEADER, IMAGE_OPTIONAL_HEADER32, IMAGE_SECTION_HEADER,
    PIMAGE_DOS_HEADER, PIMAGE_NT_HEADERS32, PIMAGE_FILE_HEADER, PIMAGE_OPTIONAL_HEADER32, PIMAGE_SECTION_HEADER,
    IMAGE_SIZEOF_FILE_HEADER, IMAGE_SIZEOF_SECTION_HEADER, IMAGE_DOS_SIGNATURE, IMAGE_NT_SIGNATURE
};
use std::ptr::null_mut;
use std::error::Error;

pub struct PEHeader{
    pub dos_header: PIMAGE_DOS_HEADER,
	pub nt_headers: PIMAGE_NT_HEADERS32,
	pub file_header: PIMAGE_FILE_HEADER,
	pub optional_header: PIMAGE_OPTIONAL_HEADER32,
	pub section_header: PIMAGE_SECTION_HEADER,
}
impl PEHeader {
    pub fn new() -> PEHeader{
        PEHeader { dos_header: null_mut(),
            nt_headers: null_mut(),
            file_header: null_mut(),
            optional_header: null_mut(),
            section_header: null_mut(),
        }
    }
    
    fn is_pefile(&mut self, image_buffer_addr: usize) -> bool{
        unsafe{
            let dos_signature = image_buffer_addr as *mut u16;
            let pe_signature = ((*(image_buffer_addr as *mut IMAGE_DOS_HEADER)).e_lfanew
            + image_buffer_addr as i32) as *mut u32;
            if *dos_signature == IMAGE_DOS_SIGNATURE && *pe_signature == IMAGE_NT_SIGNATURE{
                return true;
            }
            else {
                return false;
            }
        }
    }

    pub fn set_pe_header(&mut self, image_buffer_addr: usize) -> Result<(), Box<dyn Error>>{
        unsafe{
            if self.is_pefile(image_buffer_addr) == false {
                return Err("not a pe file".into());
            }
            self.dos_header = image_buffer_addr as *mut IMAGE_DOS_HEADER;
            self.nt_headers = ((*self.dos_header).e_lfanew + self.dos_header as i32)
                as *mut IMAGE_NT_HEADERS32;
            self.file_header = (4 + self.nt_headers as i32)
                as *mut IMAGE_FILE_HEADER;
            self.optional_header = (self.file_header as usize + IMAGE_SIZEOF_FILE_HEADER)
                as *mut IMAGE_OPTIONAL_HEADER32;
            self.section_header  = (self.optional_header as usize + (*self.file_header).SizeOfOptionalHeader as usize)
                as *mut IMAGE_SECTION_HEADER;  
            return Ok(());
        }
    }

    pub fn get_section_header_from_index(&mut self, index: u16) -> Result<PIMAGE_SECTION_HEADER, Box<dyn Error>>{    
        unsafe{
            if index > (*self.file_header).NumberOfSections {
                return Err("index out of range".into());
            }
            else{
                let section_header = ((self.section_header as usize) 
                + IMAGE_SIZEOF_SECTION_HEADER * (index - 1) as usize) as PIMAGE_SECTION_HEADER;
                return Ok(section_header);
            }
        }
    }

    pub fn get_last_section_header(&mut self) -> PIMAGE_SECTION_HEADER {
        unsafe{
            let section_header = ((self.section_header as usize) 
            + IMAGE_SIZEOF_SECTION_HEADER * ((*self.file_header).NumberOfSections - 1) as usize) as PIMAGE_SECTION_HEADER;
            return section_header;
        }
    }

    pub fn get_section_address(&mut self, section_header: *mut IMAGE_SECTION_HEADER) -> *mut usize{
        unsafe{
            let section = ((self.dos_header) as usize 
            + (*section_header).VirtualAddress as usize) as *mut usize;

            return section;
        }
    }
}
