module;

#include "resource.h"
#include <string>
#include <windows.h>
#include <iostream>

module PeEditDlg;

import Dialog;
import STL;
import FileManage;
import AnalysePE;

using petools::show::SetDlgItemText_t;
using petools::config::filename_max;
using petools::CharToTchar;
using petools::TcharToChar;

extern HINSTANCE appInst;

constexpr TCHAR editDlgTitle[10] = TEXT("PE±à¼­Æ÷ - ");

PeEditDlg* PeEditDlg::thisDlg_ = nullptr;

PeEditDlg::PeEditDlg(HWND hParent)
    : DialogEX(IDD_DIALOG_PEEDIT, hParent)
{
    memset(fileName_, 0, filename_max);
}

PeEditDlg::~PeEditDlg() {

}

void PeEditDlg::InitDlg() {
    SetThisDlg();
}

void PeEditDlg::Plant() {
    DialogBox(appInst, MAKEINTRESOURCE(idTemplate_), hParentDlg_, (DLGPROC)EditProc); 
}

void PeEditDlg::CloseDlg() {
    for (auto& dlg : dlg_list_) {
        auto name = typeid(*dlg).name();
        dlg.reset();
    }
    fileManage_.reset();
    EndDialog(hCurrentDlg_, 0);
}

void PeEditDlg::SetFileName(TCHAR* fileName) {
    wsprintfW(fileName_, L"%s", fileName);
}

void PeEditDlg::OpenFile(const TCHAR* filePath, const char* mode) {
    fileManage_ = std::make_unique<FileManage>(filePath, mode);
}

void PeEditDlg::SetEditTitle() {
    TCHAR tempTitle[280] = { 0 };
    lstrcat(tempTitle, editDlgTitle);
    lstrcat(tempTitle, fileName_);
    SetWindowText(hCurrentDlg_, tempTitle);
}

void PeEditDlg::SetPEMainInfo() {
    SetDlgItemText_t(hCurrentDlg_, IDC_EDIT_ENTRY, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->AddressOfEntryPoint, 8);
    SetDlgItemText_t(hCurrentDlg_, IDC_EDIT_IMAGEBASE, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->ImageBase, 8);
    SetDlgItemText_t(hCurrentDlg_, IDC_EDIT_IMAGESIZE, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->SizeOfImage, 8);
    SetDlgItemText_t(hCurrentDlg_, IDC_EDIT_CODEBASE, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->BaseOfCode, 8);
    SetDlgItemText_t(hCurrentDlg_, IDC_EDIT_DATABASE, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->BaseOfData, 8);
    SetDlgItemText_t(hCurrentDlg_, IDC_EDIT_SECTIONALIGN, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->SectionAlignment, 8);
    SetDlgItemText_t(hCurrentDlg_, IDC_EDIT_FILEALIGN, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->FileAlignment, 8);
    SetDlgItemText_t(hCurrentDlg_, IDC_EDIT_HEADERSIZE, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->SizeOfHeaders, 8);
    SetDlgItemText_t(hCurrentDlg_, IDC_EDIT_OPTHEADERSIZE, AnalysePE::GetAnalyse().GetHeaders().fileHeader->SizeOfOptionalHeader, 8);
    SetDlgItemText_t(hCurrentDlg_, IDC_EDIT_SECTIONSIZE, AnalysePE::GetAnalyse().GetHeaders().fileHeader->NumberOfSections, 8);
    SetDlgItemText_t(hCurrentDlg_, IDC_EDIT_CHARACT, AnalysePE::GetAnalyse().GetHeaders().fileHeader->Characteristics, 8);
    SetDlgItemText_t(hCurrentDlg_, IDC_EDIT_TIMESTAMP, AnalysePE::GetAnalyse().GetHeaders().fileHeader->TimeDateStamp, 8);
    SetDlgItemText_t(hCurrentDlg_, IDC_EDIT_SUBSYS, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->Subsystem, 8);
    SetDlgItemText_t(hCurrentDlg_, IDC_EDIT_DIRSIZE, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->NumberOfRvaAndSizes, 8);
}

void PeEditDlg::CreateFileDlg() {
    CreateDlg<FileHeaderDlg>(hCurrentDlg_, dlg_list_);
};

void PeEditDlg::CreateOptDlg() {
    CreateDlg<OptHeaderDlg>(hCurrentDlg_, dlg_list_);
}

void PeEditDlg::CreateSecDlg() {
    CreateDlg<SecHeaderDlg>(hCurrentDlg_, dlg_list_);
}

void PeEditDlg::CreateDirDlg() {
    CreateDlg<DirectoryDlg>(hCurrentDlg_, dlg_list_);
}

void PeEditDlg::CreateCalcDlg() {
    CreateDlg<CalculateDlg>(hCurrentDlg_, dlg_list_);
}

void PeEditDlg::CreateInjectImportDlg() {
    CreateDlg<InjectImportDlg>(hCurrentDlg_, dlg_list_);
}

void PeEditDlg::SaveAsFile() {
    OPENFILENAME openFile = {0};

    TCHAR* original_path = nullptr;
    CharToTchar(fileManage_->GetFilePath(), &original_path);

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
    fileManage_->SaveAsFile(AnalysePE::GetAnalyse().GetFileBuffer(), AnalysePE::GetAnalyse().GetFileBufferSzie(), path);

}

LRESULT CALLBACK PeEditDlg::EditProc(HWND hEdit, UINT message, WPARAM wParam, LPARAM lParam) {
    NMHDR* phdr = (NMHDR*)lParam;
    switch (message)
    {
    case WM_INITDIALOG:
        thisDlg_->SetCurrentDlgHWND(hEdit);
        thisDlg_->SetEditTitle();
        thisDlg_->SetPEMainInfo();
        break;
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);

        switch (wmId)
        {
        case IDC_BUTTON_FILEHEADER:
            thisDlg_->CreateFileDlg();
            break;
        case IDC_BUTTON_OPTHEADER:
            thisDlg_->CreateOptDlg();
            break;
        case IDC_BUTTON_SECTIONS:
            thisDlg_->CreateSecDlg();
            break;
        case IDC_BUTTON_DIRECTORIES:
            thisDlg_->CreateDirDlg();
            break;
        case IDC_BUTTON_CALCFOA:
            thisDlg_->CreateCalcDlg();
            break;
        case IDC_BUTTON_INJECT:
            thisDlg_->CreateInjectImportDlg();
            break;
        case IDC_BUTTON_SAVE:
            thisDlg_->SaveAsFile();
            break;
        case IDOK:
            AnalysePE::GetAnalyse().UnloadPeData();

            thisDlg_->CloseDlg();
            break;
        }
        break;
    }
    case WM_CLOSE: 
        AnalysePE::GetAnalyse().UnloadPeData();
        thisDlg_->CloseDlg();
        break;
    default:
        return FALSE;
    }
    return FALSE;
}