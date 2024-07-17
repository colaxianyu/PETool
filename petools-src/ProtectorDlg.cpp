module;

#include "resource.h"
#include <windows.h>
#include <fstream>

module ProtectorDlg;

import AnalysePE;
import STL;

using std::unique_ptr;
using petools::config::filename_max;
using petools::config::formatFilter;
using petools::TcharToChar;
using petools::CharToTchar;

extern HINSTANCE appInst;

ProtectorDlg* ProtectorDlg::thisDlg_ = nullptr;

ProtectorDlg::ProtectorDlg(HWND hParent)
    : DialogEX(IDD_DIALOG_PROTECTOR, hParent)
{

}

ProtectorDlg::~ProtectorDlg() {

}

void ProtectorDlg::InitDlg() {
    SetThisDlg();
}

void ProtectorDlg::Plant() {
    DialogBox(appInst, MAKEINTRESOURCE(idTemplate_), hParentDlg_, (DLGPROC)ProtectorProc);
}

void ProtectorDlg::CloseDlg() {
    protector_.reset();
    rawProcess_.reset();
    protectorBuffer_.reset();
    protectorBuffer_.reset();
	EndDialog(hCurrentDlg_, 0);
}

void ProtectorDlg::SelectProtector() {
    TCHAR protectorName[filename_max];
    memset(protectorName, 0, filename_max);

    OPENFILENAME openFile;
    memset(&openFile, 0, sizeof(openFile));

    openFile.lStructSize = sizeof(OPENFILENAME);
    openFile.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
    openFile.hwndOwner = hParentDlg_;
    openFile.lpstrFilter = formatFilter;
    openFile.lpstrFile = protectorName;
    openFile.nMaxFile = MAX_PATH;

    if (GetOpenFileName(&openFile) == FALSE) {
        return;
    }
   
    protector_ = std::make_unique<FileManage>(protectorName, "rb");
    AnalysePE::GetAnalyse().Init(protector_->GetFile(), protector_->GetFileSize());
    ReadProtectorToBuffer();

    SetDlgItemText(hCurrentDlg_, IDC_EDIT_PROTECTOR, protectorName);
}

void ProtectorDlg::SelectRawProcess() {
    TCHAR rawProcessName[FILENAME_MAX];
    memset(rawProcessName, 0, FILENAME_MAX);

    OPENFILENAME openFile;
    memset(&openFile, 0, sizeof(openFile));

    openFile.lStructSize = sizeof(OPENFILENAME);
    openFile.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
    openFile.hwndOwner = hParentDlg_;
    openFile.lpstrFilter = formatFilter;
    openFile.lpstrFile = rawProcessName;
    openFile.nMaxFile = MAX_PATH;

    if (GetOpenFileName(&openFile) == FALSE) {
        return;
    }

    rawProcess_ = std::make_unique<FileManage>(rawProcessName, "rb");
    ReadRawProcessToBuffer();

    SetDlgItemText(hCurrentDlg_, IDC_EDIT_RAWPROCESS, rawProcessName);
}

void ProtectorDlg::ReadProtectorToBuffer() {
    DWORD protectorSize = protector_->GetFileSize();
    protectorBuffer_ = std::make_unique<char[]>(protectorSize);
    std::fstream& tempFile = protector_->GetFile();
    tempFile.read(protectorBuffer_.get(), protectorSize);
    if (tempFile.fail()) {
        return;
    }
}
 
void ProtectorDlg::ReadRawProcessToBuffer() {
    DWORD rawProcessSize = rawProcess_->GetFileSize();
    rawProcessBuffer_ = std::make_unique<char[]>(rawProcessSize);

    std::fstream& tempFile = rawProcess_->GetFile();
    tempFile.read(rawProcessBuffer_.get(), rawProcessSize);
    if (tempFile.fail()) {
        return;
    }
}

char* ProtectorDlg::EncodeRawProcess() {
    char* tempProcessBuffer = new char[rawProcess_->GetFileSize()];
    memcpy(tempProcessBuffer, rawProcessBuffer_.get(), rawProcess_->GetFileSize());
    char* index = tempProcessBuffer;
    for (int i = 0; i < rawProcess_->GetFileSize(); i++) {
        *index = ~*index;
        index++;
    }
    return tempProcessBuffer;
}

void ProtectorDlg::ProtectProcess() {
    if (protectorBuffer_ == nullptr || rawProcessBuffer_ == nullptr) {
        MessageBox(0, L"加壳失败", TEXT("失败"), MB_ERR_INVALID_CHARS);
    }
    AnalysePE::GetAnalyse().AddSection(rawProcess_->GetFileSize(), false);
    IMAGE_SECTION_HEADER* lastSectionHeader = AnalysePE::GetAnalyse().GetHeaders().sectionHeader
        + AnalysePE::GetAnalyse().GetHeaders().fileHeader->NumberOfSections - 1;
    char* newSection = (char*)((DWORD)AnalysePE::GetAnalyse().GetHeaders().dosHeader
        + lastSectionHeader->PointerToRawData);
    char* tempProcessBuffer = EncodeRawProcess();
    memcpy(newSection, tempProcessBuffer, rawProcess_->GetFileSize());
    SaveFile();
    //MessageBox(0, L"加壳成功", TEXT("成功"), MB_OK);
}

void ProtectorDlg::SaveFile() {
    OPENFILENAME openFile = { 0 };

    TCHAR* original_path = nullptr;
    CharToTchar(protector_->GetFilePath(), &original_path);

    TCHAR new_path[MAX_PATH] = { 0 };
    if (original_path != nullptr) {
        TCHAR* dot_pos = wcsrchr(original_path, '.');
        if (dot_pos != nullptr) {
            size_t insert_pos = dot_pos - original_path;
            wcsncpy_s(new_path, original_path, insert_pos);
            wcscat_s(new_path, MAX_PATH, TEXT("(2)"));
            wcscat_s(new_path, MAX_PATH, dot_pos);
        }
        else {
            wcsncpy_s(new_path, original_path, MAX_PATH - 4);
            wcscat_s(new_path, MAX_PATH, TEXT("(2)"));
        }
    }

    openFile.lStructSize = sizeof(OPENFILENAME);
    openFile.Flags = OFN_HIDEREADONLY | OFN_PATHMUSTEXIST;
    openFile.FlagsEx = OFN_EX_NOPLACESBAR;
    openFile.hwndOwner = hParentDlg_;
    openFile.lpstrFilter = TEXT("*.exe\0\0*.dll\0");
    openFile.lpstrFile = new_path;
    openFile.nMaxFile = MAX_PATH;
    GetSaveFileName(&openFile);

    char* path = nullptr;
    TcharToChar(new_path, &path);
    protector_->SaveAsFile(AnalysePE::GetAnalyse().GetFileBuffer(), AnalysePE::GetAnalyse().GetFileBufferSzie(), path);

}

LRESULT CALLBACK ProtectorDlg::ProtectorProc(HWND hProtector, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message)
    {
    case WM_INITDIALOG:
        thisDlg_->SetCurrentDlgHWND(hProtector);
        break;
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);

        switch (wmId)
        {
        case IDC_BUTTON_PROTECTOR:
            thisDlg_->SelectProtector();
            break;
        case IDC_BUTTON_RAWPROCESS:
            thisDlg_->SelectRawProcess();
            break;
        case IDC_BUTTON_PROTECTPROCESS:
            thisDlg_->ProtectProcess();
            break;
        case IDOK:
            thisDlg_->CloseDlg();
            break;
        }
        break;
    }
    case WM_CLOSE:
        thisDlg_->CloseDlg();
        break;
    default:
        return FALSE;
    }
    return FALSE;
}
