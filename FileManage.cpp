#include "FileManage.h"
#include <iostream>
using namespace std;

void FileManageDetail::CloseFile(FILE* fp) {
	if (fp != nullptr) {
		fclose(fp);
	}
}
void FileManageDetail::ReleasePath(char* path) {
	if (path != nullptr) {
		delete[] path;
	}
}

FileManage::FileManage(const char* filePath, const char* mode){
	if (file_ != nullptr) {
		return;
	}

	FILE* tempFile = nullptr;
	errno_t e = fopen_s(&tempFile, filePath, mode);
	if (e != 0) {
		return;
	}
	file_ = FileManageDetail::PFile(tempFile, FileManageDetail::CloseFile);

	char* tempFilePath = new char[strlen(filePath) + 1];
	memcpy(tempFilePath, filePath, strlen(filePath) + 1);
	filePath_ = FileManageDetail::PFilePath(tempFilePath, FileManageDetail::ReleasePath);
	fseek(file_.get(), 0, SEEK_END);
	fileSize_ = ftell(file_.get());
	fseek(file_.get(), 0, SEEK_SET);
}

bool FileManage::IsOpenFile() {
	if (file_ == nullptr) {
		return false;
	}
	else {
		return true;
	}
}

bool FileManage::SaveFile(const char* savePath) {
	FILE* file = nullptr;
	fopen_s(&file, savePath, "wb");
	if (file == nullptr) {
		MessageBox(0, TEXT("±£¥Ê ß∞‹!"), TEXT("ERROR"), MB_OK);
		return false;
	}

	DWORD fileBufferSize = AnalysePE::GetAnalyse().GetFileBufferSzie();
	DWORD writeSize = fwrite(AnalysePE::GetAnalyse().GetHeaders().dosHeader, sizeof(BYTE), fileBufferSize, file);
	if (writeSize == 0) {
		MessageBox(0, TEXT("±£¥Ê ß∞‹!"), TEXT("ERROR"), MB_OK);
		fclose(file);
		file = nullptr;
		return false;
	}
	fclose(file);
	return true;
}
