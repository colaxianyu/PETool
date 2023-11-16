#include "ProtectorDlg.h"
#include "resource.h"
#include <iostream>
#include <Windows.h>

extern HINSTANCE appInst;

ProtectorDlg* ProtectorDlg::thisDlg_ = nullptr;
static TCHAR formatFilter[50] = TEXT("*.exe;*.dll;*.scr;*.drv;*.sys");

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

void ProtectorDlg::SelectProtector() {
    TCHAR protectorName[FILENAME_MAX];
    memset(protectorName, 0, FILENAME_MAX);

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
    AnalysePE::GetAnalyse().Init(protector_.get());
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

    protectorBuffer_ = unique_ptr<char>(new char[protectorSize]);

    //FILE* tempFile = const_cast<FILE*>(FileManage::GetFileManage().GetFile());
    FILE* tempFile = protector_->GetFile();
    DWORD readSize = fread(protectorBuffer_.get(), protectorSize, 1, tempFile);
    if (readSize == 0) {
        return;
    }
    tempFile = nullptr;
}
 
void ProtectorDlg::ReadRawProcessToBuffer() {
    DWORD rawProcessSize = rawProcess_->GetFileSize();
    std::cout<< std::hex << rawProcessSize;
    rawProcessBuffer_ = unique_ptr<char>(new char[rawProcessSize]);

    FILE* tempFile = rawProcess_->GetFile();
    DWORD readSize = fread(rawProcessBuffer_.get(), rawProcessSize, 1, tempFile);
    if (readSize == 0) {
        return;
    }
    tempFile = nullptr;
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
    MessageBox(0, L"加壳成功", TEXT("成功"), MB_OK);
}

void ProtectorDlg::SaveFile() {
    OPENFILENAME openFile = { 0 };

    TCHAR* tPath = nullptr;
    CharToTchar(protector_->GetFilePath(), &tPath);

    openFile.lStructSize = sizeof(OPENFILENAME);
    openFile.Flags = OFN_HIDEREADONLY | OFN_PATHMUSTEXIST;
    openFile.FlagsEx = OFN_EX_NOPLACESBAR;
    openFile.hwndOwner = hParentDlg_;
    openFile.lpstrFilter = TEXT("*.exe\0\0*.dll\0");
    openFile.lpstrFile = tPath;
    openFile.nMaxFile = MAX_PATH;
    GetSaveFileName(&openFile);

    char* path = nullptr;
    TcharToChar(tPath, &path);
    protector_->SaveFile(path);

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
