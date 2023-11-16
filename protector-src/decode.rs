extern crate math;
use std::ptr::copy_nonoverlapping;
use std::slice;
use winapi::um::winnt::{IMAGE_SECTION_HEADER,IMAGE_SIZEOF_SECTION_HEADER};
use petool::PEHeader;

fn get_data<T>(data_address: *mut T, data_size: usize) -> &'static mut [u8] {
    return unsafe {
        slice::from_raw_parts_mut(data_address as *mut u8, data_size)
    };
}

fn decode(encode_data: &[u8]) -> Vec<u8>{
    let mut decode_data = encode_data.to_vec();
    for byte in &mut decode_data{
        *byte = !*byte;
    }
    return decode_data;
}

fn get_raw_process_data(image_buffer_addr: usize) -> Vec<u8>{
    let mut pe_header = PEHeader::new();
    match pe_header.set_pe_header(image_buffer_addr){
        Ok(_) => {},
        Err(e) => println!("error: {}", e) 
    }
    unsafe{
        let last_section_header 
        = match pe_header.get_section_header_from_index((*pe_header.file_header).NumberOfSections) {
            Ok(section_header) => section_header,
            Err(e) => panic!("error: {}", e)
        };
 
        let last_section = pe_header.get_section_address(last_section_header);
        
        let encode_process_data = get_data(last_section, (*last_section_header).SizeOfRawData as usize);
        let decode_process_data = decode(&encode_process_data);
        return decode_process_data;
    } 
}

fn get_zero_image_buffer(proc_data: &Vec<u8>, last_section_edge: usize) -> Vec<u8>{
    unsafe{
        let mut pe_header = PEHeader::new();
        pe_header.set_pe_header(proc_data.as_ptr() as usize).unwrap();

        let last_section_header = pe_header.get_last_section_header();
        let last_section_max_info_size
         = std::cmp::max((*last_section_header).SizeOfRawData, *(*last_section_header).Misc.VirtualSize());
    
        if have_other_info(proc_data.len(), last_section_edge){
            let alignment_last_section = ((last_section_max_info_size as f64)
                / ((*pe_header.optional_header).SectionAlignment as f64)).ceil();
            let size = (alignment_last_section
                * (*pe_header.optional_header).SectionAlignment as f64) as usize 
                + (*last_section_header).VirtualAddress as usize
                + (proc_data.len() - last_section_edge);
            let buffer: Vec<u8> = vec![0; size];
            return buffer;
        }
        else{
            
            let image_size = (*pe_header.optional_header).SizeOfImage;
            let real_max_size = (*last_section_header).VirtualAddress
            + std::cmp::max(*(*last_section_header).Misc.VirtualSize(), (*last_section_header).SizeOfRawData);
            let size = std::cmp::max(image_size, real_max_size) as usize;

            (*pe_header.optional_header).SizeOfImage as usize;
            let buffer: Vec<u8> = vec![0; size];
            return buffer;
        }
    }
}

pub fn get_process_data_with_image(image_buffer_addr: usize) -> Vec<u8>{
    unsafe{
        let raw_proc_data = get_raw_process_data(image_buffer_addr);
        let mut pe_header = PEHeader::new();
        match pe_header.set_pe_header(raw_proc_data.as_ptr() as usize){
            Ok(_) => {},
            Err(e) => println!("error: {}", e) 
        }

        let last_section_header = pe_header.get_last_section_header();

        let mut image_process_data = get_zero_image_buffer(
            &raw_proc_data, 
            ((*last_section_header).PointerToRawData + (*last_section_header).SizeOfRawData) as usize
        );
        (*pe_header.optional_header).SizeOfImage = image_process_data.len() as u32;
        copy_nonoverlapping(
            pe_header.dos_header as *const u8,
            image_process_data.as_mut_ptr() as *mut u8,
            (*pe_header.optional_header).SizeOfHeaders as usize
        );

        
        let mut section_header_index = pe_header.section_header;
        let mut i = 0;
        while i < (*pe_header.file_header).NumberOfSections {
            let dst: u32 = image_process_data.as_ptr() as u32 + (*section_header_index).VirtualAddress;
            let src: u32 = pe_header.dos_header as u32 + (*section_header_index).PointerToRawData;
            let size = (*section_header_index).SizeOfRawData;
            copy_nonoverlapping(
                src as *const u8,
                dst as *mut u8,   
                size as usize
            );
            i += 1;
            section_header_index = (section_header_index as usize + IMAGE_SIZEOF_SECTION_HEADER) as *mut IMAGE_SECTION_HEADER;          
        }

        if raw_proc_data.len() as u32 > (*last_section_header).PointerToRawData + (*last_section_header).SizeOfRawData {
            let other_info = ((*last_section_header).PointerToRawData + (*last_section_header).SizeOfRawData)
             + pe_header.dos_header as u32;

            let other_info_size = raw_proc_data.len() as u32 - ((*last_section_header).PointerToRawData + (*last_section_header).SizeOfRawData);
            
            let alignment_last_section = 
                (std::cmp::max(*(*last_section_header).Misc.VirtualSize(), (*last_section_header).SizeOfRawData) as f32
                / (*pe_header.optional_header).SectionAlignment as f32).ceil() * (*pe_header.optional_header).SectionAlignment as f32;
            
            let dst2 = image_process_data.as_ptr() as u32 + (*last_section_header).VirtualAddress + alignment_last_section as u32;
            copy_nonoverlapping(
                other_info as *const u8,
                dst2 as *mut u8,   
                other_info_size as usize
            );
        }
        return image_process_data;
    }
}

fn have_other_info(raw_proc_data_size: usize, last_section_edge: usize) -> bool{
    if raw_proc_data_size > last_section_edge{
        return true;
    }
    else {
        return false;
    }
}