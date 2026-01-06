module;

#define NOMINMAX
#include <windows.h>
// #include <sys/stat.h>
#include <limits>

module FileManager;

import STL;
//import AnalysePE;

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

namespace petools::core::io {

    namespace {
        inline std::error_code make_win32_error(DWORD win32) noexcept {
            return std::error_code(static_cast<int>(win32), std::system_category());
        }
    } // namespace detail

    FileManager::FileManager(std::filesystem::path path, FileHandle file, MappingHandle mapping, void* view, std::size_t size) noexcept
        : path_(std::move(path)),
        file_(std::move(file)),
        mapping_(std::move(mapping)),
        view_(view),
        size_(size)
    {
    }

	FileManager::FileManager(FileManager&& other) noexcept
		: path_(std::move(other.path_))
		, file_(std::move(other.file_))
		, mapping_(std::move(other.mapping_))
		, view_(std::exchange(other.view_, nullptr))
		, size_(std::exchange(other.size_, 0)) {
	}

	FileManager& FileManager::operator=(FileManager&& other) noexcept {
		if (this != &other) {
			if (view_) {
				::UnmapViewOfFile(view_);
				view_ = nullptr;
			}

			path_ = std::move(other.path_);
			file_ = std::move(other.file_);
			mapping_ = std::move(other.mapping_);
			view_ = std::exchange(other.view_, nullptr);
			size_ = std::exchange(other.size_, 0);
		}
		return *this;
	}

    file_result<FileManager> FileManager::OpenReadOnly(const std::filesystem::path& path) {
        // 1. 以只读方式打开文件
        HANDLE hfile = ::CreateFileW(
            path.c_str(),
            GENERIC_READ,
            FILE_SHARE_READ,
            nullptr,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            nullptr
        );

        if (hfile == INVALID_HANDLE_VALUE) {
            DWORD last = ::GetLastError();
            FileError err{
                .op = FileErrorOP::OpenFailed,
                .code = make_win32_error(last),
                .message = "Failed to open file."
            };
            return std::unexpected(err);
        }

        FileHandle file_handle{ hfile };

        // 2. 获取文件大小
        LARGE_INTEGER size_li{};
        if (!::GetFileSizeEx(hfile, &size_li) || size_li.QuadPart <= 0) {
            DWORD last = ::GetLastError();
            FileError err{
                .op = FileErrorOP::SizeQueryFailed,
                .code = make_win32_error(last),
                .message = "Failed to query file size."
            };
            return std::unexpected(err);
        }

        // 防御：避免超出 size_t 范围
        if (size_li.QuadPart > static_cast<LONGLONG>(std::numeric_limits<std::size_t>::max())) {
            FileError err{
                .op = FileErrorOP::SizeQueryFailed,
                .code = {},
                .message = "File too large to map into memory."
            };
            return std::unexpected(err);
        }

        std::size_t size = static_cast<std::size_t>(size_li.QuadPart);

        // 3. 创建只读文件映射对象
        HANDLE hmap = ::CreateFileMappingW(
            hfile,
            nullptr,
            PAGE_READONLY,
            0,
            0,
            nullptr
        );

        if (!hmap) {
            DWORD last = ::GetLastError();
            FileError err{
                .op = FileErrorOP::MappingFailed,
                .code = make_win32_error(last),
                .message = "Failed to create file mapping."
            };
            return std::unexpected(err);
        }

        MappingHandle mapping_handle{ hmap };

        // 4. 建立视图（映射整个文件）
        void* view = ::MapViewOfFile(hmap, FILE_MAP_READ, 0, 0, 0);

        if (!view) {
            DWORD last = ::GetLastError();
            FileError err{
                .op = FileErrorOP::ViewFailed,
                .code = make_win32_error(last),
                .message = "Failed to map view of file."
            };
            return std::unexpected(err);
        }

        return FileManager{
            path,
            std::move(file_handle),
            std::move(mapping_handle),
            view,
            size
        };
    }

    FileManager::~FileManager() noexcept {
        if (view_) {
            ::UnmapViewOfFile(view_);
            view_ = nullptr;
        }
    }

} //namespace petools::core::io