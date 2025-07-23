module;

#include <windows.h>
#include <sys/stat.h>
#include "fast_io/fast_io.h"
#include "fast_io/fast_io_device.h"
#include "fast_io/fast_io_legacy.h"

module FileManager;

import STL;
import AnalysePE;

//FileManager::FileManager(const char* file_path, const char* mode){
//	file_.open(file_path, std::fstream::in | std::fstream::out | std::fstream::binary);
//
//	if (!file_.is_open()) {
//		return;
//	}
//
//	char* tempFilePath = new char[strlen(file_path) + 1];
//	memcpy(tempFilePath, file_path, strlen(file_path) + 1);
//
//	file_path_ = FileManagerDetail::FilePathPtr(tempFilePath, FileManagerDetail::FilePathDeleter());
//
//	std::streampos currentPos = file_.tellg();
//	file_.seekg(0, std::ios::end);
//	std::streamsize size = file_.tellg();
//	file_size_ = static_cast<DWORD>(size);
//	file_.seekg(currentPos);
//}
//
//FileManager::~FileManager() {
//    if (file_.is_open()) {
//        file_.close();
//    }
//}
//
//void FileManager::save_to_file(const char* file_buffer, DWORD file_size, const char* path) {
//    if (path == nullptr || file_buffer == nullptr) {
//        MessageBox(0, TEXT("Invalid path or file buffer"), TEXT("ERROR"), MB_OK);
//        return;
//    }
//
//    fstream outFile(path, std::ios::out | std::ios::binary);
//    if (!outFile.is_open()) {
//        MessageBox(0, TEXT("Failed to open file for writing"), TEXT("ERROR"), MB_OK);
//        return;
//    }
//
//    outFile.write(file_buffer, file_size);
//
//    if (!outFile) {
//        MessageBox(0, TEXT("Failed to write to fileg"), TEXT("ERROR"), MB_OK);
//    }
//    outFile.close();
//}

//bool FileManager::open_file(std::string_view file_path) {
//	file_ = std::make_unique<fast_io::c_file>(file_path, fast_io::open_mode::in | fast_io::open_mode::out);
//	if (!file_) {
//		return false;
//	}
//
//	file_path_ = std::string(file_path.begin(), file_path.end());
//	calculate_file_size();
//	read_file_to_buffer();
//	return true;
//}
//
//void FileManager::calculate_file_size() {
//	struct stat fileStat { 0 };
//	stat(file_path_.c_str(), &fileStat);
//	file_size_ = fileStat.st_size;
//}
//
//void FileManager::read_file_to_buffer() {
//	std::vector<char> file_vec(file_size_, 0);
//	fast_io::read(*file_.get(), file_vec.begin(), file_vec.end());
//	
//	char* file_buffer = new char[file_size_];
//	memcpy(file_buffer, file_vec.data(), file_size_);
//	file_buffer_.reset(file_buffer);
//}

namespace petools {

	FileManager::FileManager(std::string_view path) noexcept
		: file_(fast_io::mnp::os_c_str(path.data())),
		file_path_(std::string(path.begin(), path.end())),
		file_size_(file_.size())
	{
		pe_analyse.init(reinterpret_cast<const std::byte*>(file_.address_begin), file_size_);
	}

	FileManager::~FileManager() noexcept {
		pe_analyse.UnloadPeData();
	}

} //namespace petools