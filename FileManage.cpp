#include "FileManage.h"
#include "AnalysePE.h"
#include <string>

void FileManage::Reset() {
	filePath_.reset();
	file_.reset();
	fileSize_ = 0;
}

void FileManage::SetFileName(TCHAR* tFilePath) {
	char* cPath = nullptr;
	AnalysePE::GetAnalyse().TcharToChar(tFilePath, &cPath);
	filePath_ = unique_ptr<char>(cPath);
	cPath = nullptr;
}

bool FileManage::OpenFile() {
	FILE* file = nullptr;
	errno_t e = fopen_s(&file, filePath_.get(), "rb");
	if (e != 0) {
		return false;
	}
	file_ = unique_ptr<FILE>(file);

	fseek(file_.get(), 0, SEEK_END);
	fileSize_ = ftell(file_.get());
	fseek(file_.get(), 0, SEEK_SET);
	return true;
}

bool FileManage::CloseFile() {
	fclose(file_.get());
	file_.reset();
	return true;
}

void FileManage::GetFilePath(TCHAR* tPath) {
	TCHAR* p = nullptr; 
	AnalysePE::GetAnalyse().CharToTchar(filePath_.get(), &p); 
	memcpy(tPath, p, MAX_PATH);
}