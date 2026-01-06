module;

#include <windows.h>

export module FileManager;

import std.compat;
import WinHandle;

namespace petools::core::io {

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

	export class FileManager {
	public:
		FileManager() = delete;

		[[nodiscard]] static file_result<FileManager> OpenReadOnly(const std::filesystem::path& path);

		FileManager(FileManager&& other) noexcept;
		FileManager& operator=(FileManager&& other) noexcept;

		FileManager(const FileManager&) = delete;
		FileManager& operator=(const FileManager&) = delete;

		~FileManager() noexcept;

		[[nodiscard]] const std::filesystem::path& GetPath() const noexcept { return path_; }
		[[nodiscard]] std::size_t GetSize() const noexcept { return size_; }

		[[nodiscard]] std::span<const std::byte> GetBytes() const noexcept {
			return { static_cast<const std::byte*>(view_), size_ };
		}

	private:
		using FileHandle = petools::FileHandle;   
		using MappingHandle = petools::FileHandle;   

		explicit FileManager(std::filesystem::path path, FileHandle file, MappingHandle mapping, void* view, std::size_t size) noexcept;

		std::filesystem::path path_;
		FileHandle file_;      
		MappingHandle mapping_;   
		void* view_{ nullptr };   
		std::size_t size_{ 0 };
	};

} //namespace petools::core::io