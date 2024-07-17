module;

#include <windows.h>
#include <string>
#include <fstream>

export module FileManage;

import STL;

using std::unique_ptr;
using std::string;
using std::function;
using std::fstream;
using petools::TcharToString;

namespace FileManageDetail {
	using filePathPointer = unique_ptr<char, function<void(char*)>>;

	void ReleasePath(char* path);
}

export class FileManage {
public:
	FileManage(const FileManage&) = delete;
	FileManage operator=(const FileManage&) = delete;

	FileManage(const char* filePath, const char* mode);
	FileManage(const string& filePath, const char* mode) : FileManage(filePath.c_str(), mode) {};
	FileManage(const TCHAR* filePath, const char* mode) : FileManage(TcharToString(filePath), mode) {};

	~FileManage() = default;

	bool IsOpenFile(){ return file_.is_open(); }
	fstream& GetFile() { return file_; }
	char* const GetFilePath() { return filePath_.get(); }
	DWORD GetFileSize() { return fileSize_; }

	void SaveAsFile(const char* file_buffer, DWORD file_szie, const char* path);
private:
	fstream file_;
	unique_ptr<char, function<void(char*)>> filePath_ = nullptr;
	DWORD fileSize_ = 0;
};