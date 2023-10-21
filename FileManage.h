#pragma once
#include <memory>
#include <Windows.h>
#include <string>
#include <functional>
#include "AnalysePE.h"
import Utils;

using std::unique_ptr;
using std::string;
using std::function;

namespace FileManageDetail {
	extern void CloseFile(FILE* fp);
	extern void ReleasePath(char* path);
	using PFile = unique_ptr<FILE, function<void(FILE*)>>;
	using PFilePath = unique_ptr<char, function<void(char*)>>;
}

class FileManage{
public:
	FileManage(const FileManage&) = delete;
	FileManage operator=(const FileManage&) = delete;

	FileManage(const char* filePath, const char* mode);
	FileManage(const string& filePath, const char* mode) : FileManage(filePath.c_str(), mode) {};
	FileManage(const TCHAR* filePath, const char* mode) : FileManage(TcharToString(filePath), mode) {};

	~FileManage() = default;

	bool IsOpenFile();
	bool SaveFile(const char* savePath);

	FILE* const GetFile() { return file_.get(); }
	char* const GetFilePath() { return filePath_.get(); }
	DWORD GetFileSize() { return fileSize_; }

private:
	unique_ptr<FILE, function<void(FILE*)>> file_ = nullptr;
	unique_ptr<char, function<void(char*)>> filePath_ = nullptr;
	DWORD fileSize_ = 0;
};