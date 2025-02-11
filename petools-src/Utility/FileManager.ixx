module;

#include <windows.h>
#include <string>
#include <memory>
#include "fast_io/fast_io.h"

export module FileManager;

import STL;

using tools::TcharToString;


struct FileBufferDeleter {
	void operator()(char* buffer) {
		if (buffer) {
			delete[] buffer;
		}
	}
};

using FileBufferPtr = std::unique_ptr<char[], FileBufferDeleter>;
using FilePtr = std::unique_ptr<fast_io::c_file>;

export class FileManager {
private:
	//FileManager(const FileManager&) = delete;
	//FileManager operator=(const FileManager&) = delete;

	void read_file_to_buffer();
	void calculate_file_size();
public:
	FileManager() = default;

	//FileManager(const char* file_path, const char* mode);
	//FileManager(const std::string& file_ath, const char* mode) : FileManager(file_ath.c_str(), mode) {};
	//FileManager(const TCHAR* file_path, const char* mode) : FileManager(TcharToString(file_path), mode) {};

	//~FileManager();

	bool open_file(std::string_view file_path);

	//[[nodiscard]] bool is_file_open() const { return file_.is_open(); }
	[[nodiscard]] DWORD get_file_size() const { return file_size_; }

	//const std::fstream& get_file() const { return file_; }
	const std::string_view get_file_path() const { return file_path_; }
	

	//void save_to_file(const char* file_buffer, DWORD file_szie, const char* path);
private:
	FilePtr file_;
	FileBufferPtr file_buffer_{ nullptr, FileBufferDeleter()};
	std::string file_path_;
	unsigned int file_size_ = 0;
};