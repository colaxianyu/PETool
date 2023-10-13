#pragma once
#include <memory>
#include <Windows.h>
using std::unique_ptr;

class FileManage {
private:
	FileManage() = default;
	~FileManage() = default;
public:
	FileManage(const FileManage&) = delete;
	FileManage(FileManage&&) = delete;
	FileManage& operator=(const FileManage&) = delete;
	FileManage& operator=(FileManage&&) = delete;

	static FileManage& GetFileManage() {
		static FileManage fileManage;
		return fileManage;
	}

	void Reset();
	void SetFileName(TCHAR* tFilePath);
	bool OpenFile();
	bool CloseFile();

	const FILE* GetFile() { return file_.get(); }
	const char* GetFilePath() { return filePath_.get(); };
	const DWORD GetFileSize() { return fileSize_; };
	void GetFilePath(TCHAR* tPath);
	
private:
	unique_ptr<char> filePath_ = nullptr;
	unique_ptr<FILE> file_ = nullptr;
	DWORD fileSize_ = 0;
};