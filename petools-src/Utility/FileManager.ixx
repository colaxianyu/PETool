module;

#include <windows.h>
#include "fast_io/fast_io.h"
#include "fast_io/fast_io_device.h"

export module FileManager;

import STL;


//struct FileBufferDeleter {
//	void operator()(char* buffer) {
//		if (buffer) {
//			delete[] buffer;
//		}
//	}
//};
//
//using FileBufferPtr = std::unique_ptr<char[], FileBufferDeleter>;
//using FilePtr = std::unique_ptr<fast_io::c_file>;
//
//export class FileManager {
//private:
//	//FileManager(const FileManager&) = delete;
//	//FileManager operator=(const FileManager&) = delete;
//
//	void read_file_to_buffer();
//	void calculate_file_size();
//public:
//	FileManager() = default;
//
//	//FileManager(const char* file_path, const char* mode);
//	//FileManager(const std::string& file_ath, const char* mode) : FileManager(file_ath.c_str(), mode) {};
//	//FileManager(const TCHAR* file_path, const char* mode) : FileManager(TcharToString(file_path), mode) {};
//
//	//~FileManager();
//
//	bool open_file(std::string_view file_path);
//
//	//[[nodiscard]] bool is_file_open() const { return file_.is_open(); }
//	[[nodiscard]] DWORD get_file_size() const { return file_size_; }
//
//	//const std::fstream& get_file() const { return file_; }
//	const std::string_view get_file_path() const { return file_path_; }
//	
//
//	//void save_to_file(const char* file_buffer, DWORD file_szie, const char* path);
//private:
//	FilePtr file_;
//	FileBufferPtr file_buffer_{ nullptr, FileBufferDeleter()};
//	std::string file_path_;
//	unsigned int file_size_ = 0;
//};

namespace petools {

	export class FileManager {
	public:
		explicit FileManager(std::string_view path) noexcept;

		~FileManager() = default;

		FileManager(const FileManager&) = delete;
		FileManager& operator=(const FileManager&) = delete;

		FileManager(FileManager&&) = delete;
		FileManager& operator=(FileManager&&) = delete;

		[[nodiscard]] std::size_t get_file_size() const { return file_size_; }
		[[nodiscard]] std::byte* get_file_buffer() const { return file_buffer_.get(); }
		[[nodiscard]] std::string_view get_file_path() const { return file_path_; }

	private:
		std::string file_path_;
		fast_io::native_file_loader file_;
		std::unique_ptr<std::byte[]> file_buffer_;
		std::size_t file_size_;

		void read_file_to_buffer() noexcept;
	};

} //namespace petools