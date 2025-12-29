module;

#include <windows.h>

export module FileManager;

import std.compat;
import WinHandle;

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

	export enum class WriteMode {
		SPARSE,
		BLOCK_COW,
		FULL_COPY
	};

	export class FileManager {
	public:
		explicit FileManager(std::string_view path) noexcept;

		~FileManager() noexcept;

		FileManager(const FileManager&) = delete;
		FileManager& operator=(const FileManager&) = delete;

		FileManager(FileManager&&) = delete;
		FileManager& operator=(FileManager&&) = delete;

		[[nodiscard]] std::size_t get_file_size() const { return file_size_; }
		[[nodiscard]] std::string_view get_file_path() const { return file_path_; }
		//[[nodiscard]] std::span<const char> get_file_span() const {
		//	//return std::span<const char>(
		//	//	file_.address_begin,
		//	//	file_size_
		//	//);
		//}

	private:
		//fast_io::native_file_loader file_;
		std::string file_path_;
		
		std::size_t file_size_;
	};

	export enum class FileErrorOP {
		OpenFailed,
		SizeQueryFailed,
		MappingFailed,
		ViewFailed,
		Unknown
	};

	export struct FileError {
		FileErrorOP	op{};
		std::error_code code;
		std::string message;    
	};

	template <typename T>
	using file_result = std::expected<T, FileError>;

	export class FileManager2 {
	public:
		FileManager2() = delete;

		[[nodiscard]] static file_result<FileManager2> OpenReadOnly(const std::filesystem::path& path);

		FileManager2(FileManager2&&) noexcept = default;
		FileManager2& operator=(FileManager2&&) noexcept = default;

		FileManager2(const FileManager2&) = delete;
		FileManager2& operator=(const FileManager2&) = delete;

		~FileManager2() noexcept;

		[[nodiscard]] const std::filesystem::path& GetPath() const noexcept { return path_; }
		[[nodiscard]] std::size_t GetSize() const noexcept { return size_; }

		[[nodiscard]] std::span<const std::byte> GetBytes() const noexcept {
			return { static_cast<const std::byte*>(view_), size_ };
		}

	private:
		using FileHandle = petools::FileHandle;   
		using MappingHandle = petools::FileHandle;   

		explicit FileManager2(std::filesystem::path path,
			FileHandle file,
			MappingHandle mapping,
			void* view,
			std::size_t size) noexcept;

		std::filesystem::path path_;
		FileHandle            file_;      
		MappingHandle         mapping_;   
		void* view_{ nullptr };   
		std::size_t           size_{ 0 };
	};

} //namespace petools