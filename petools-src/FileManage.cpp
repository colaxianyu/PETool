module;

#include <windows.h>
#include <iostream>

module FileManage;

import STL;

using std::format;
using std::fstream;

void FileManageDetail::ReleasePath(char* path) {
	if (path != nullptr) {
		delete[] path;
		std::cout << format("path released!\n");
	}
}

FileManage::FileManage(const char* filePath, const char* mode){
	file_.open(filePath, std::fstream::in | std::fstream::out | std::fstream::binary);

	if (!file_.is_open()) {
		return;
	}

	char* tempFilePath = new char[strlen(filePath) + 1];
	memcpy(tempFilePath, filePath, strlen(filePath) + 1);
	filePath_ = FileManageDetail::filePathPointer(tempFilePath, FileManageDetail::ReleasePath);

	std::streampos currentPos = file_.tellg();
	file_.seekg(0, std::ios::end);
	std::streamsize size = file_.tellg();
	fileSize_ = static_cast<DWORD>(size);
	file_.seekg(currentPos);
}

void FileManage::SaveAsFile(const char* file_buffer, DWORD file_szie, const char* path) {
    if (path == nullptr || file_buffer == nullptr) {
        MessageBox(0, TEXT("Invalid path or file buffer"), TEXT("ERROR"), MB_OK);
        return;
    }

    fstream outFile(path, std::ios::out | std::ios::binary);
    if (!outFile.is_open()) {
        MessageBox(0, TEXT("Failed to open file for writing"), TEXT("ERROR"), MB_OK);
        return;
    }

    outFile.write(file_buffer, file_szie);
    if (!outFile) {
        MessageBox(0, TEXT("Failed to write to fileg"), TEXT("ERROR"), MB_OK);
    }
    outFile.close();
}
                                                                                                      