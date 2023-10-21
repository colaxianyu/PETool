#include "PeEdit.h"
#include "AnalysePE.h"
#include "resource.h"
#include <CommCtrl.h>
#include <string>
#include <Windows.h>
import Utils;

extern HINSTANCE appInst;

const static TCHAR editDlgTitle[10] = TEXT("PE±à¼­Æ÷ - ");

PeEditDlg* PeEditDlg::thisDlg_ = nullptr;

PeEditDlg::PeEditDlg(HWND hParent)
    : DialogEX(IDD_DIALOG_PEEDIT, hParent)
{
    memset(fileName_, 0, FILENAME_MAX);
}

PeEditDlg::~PeEditDlg() {
    fileHeaderDlg_.reset();
    optHeaderDlg_.reset();
    secHeaderDlg_.reset();
    dirHeaderDlg_.reset();
    calcDlg_.reset();
    timeCalcDlg_.reset();
    injectImpDlg_.reset();
}

void PeEditDlg::InitDlg() {
    SetThisDlg();
}

void PeEditDlg::Plant() {
    DialogBox(appInst, MAKEINTRESOURCE(idTemplate_), hParentDlg_, (DLGPROC)EditProc); 
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
    TCHAR entry[10] = { 0 };
    wsprintfW(entry, L"%08X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->AddressOfEntryPoint);
    //HWND h = GetDlgItem(hEdit, IDC_EDIT_ENTRY);
    //SetWindowText(h, t);    
    SetDlgItemText(hCurrentDlg_, IDC_EDIT_ENTRY, entry);

    TCHAR imageBase[10] = { 0 };
    wsprintfW(imageBase, L"%08X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->ImageBase);
    SetDlgItemText(hCurrentDlg_, IDC_EDIT_IMAGEBASE, imageBase);

    TCHAR imageSize[10] = { 0 };
    wsprintfW(imageSize, L"%08X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->SizeOfImage);
    SetDlgItemText(hCurrentDlg_, IDC_EDIT_IMAGESIZE, imageSize);

    TCHAR codeBase[10] = { 0 };
    wsprintfW(codeBase, L"%08X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->BaseOfCode);
    SetDlgItemText(hCurrentDlg_, IDC_EDIT_CODEBASE, codeBase);

    TCHAR dataBase[10] = { 0 };
    wsprintfW(dataBase, L"%08X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->BaseOfData);
    SetDlgItemText(hCurrentDlg_, IDC_EDIT_DATABASE, dataBase);

    TCHAR sectionAlign[10] = { 0 };
    wsprintfW(sectionAlign, L"%08X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->SectionAlignment);
    SetDlgItemText(hCurrentDlg_, IDC_EDIT_SECTIONALIGN, sectionAlign);

    TCHAR fileAlign[10] = { 0 };
    wsprintfW(fileAlign, L"%08X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->FileAlignment);
    SetDlgItemText(hCurrentDlg_, IDC_EDIT_FILEALIGN, fileAlign);

    TCHAR headerSize[10] = { 0 };
    wsprintfW(headerSize, L"%08X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->SizeOfHeaders);
    SetDlgItemText(hCurrentDlg_, IDC_EDIT_HEADERSIZE, headerSize);

    TCHAR optHeaderSize[10] = { 0 };
    wsprintfW(optHeaderSize, L"%08X", AnalysePE::GetAnalyse().GetHeaders().fileHeader->SizeOfOptionalHeader);
    SetDlgItemText(hCurrentDlg_, IDC_EDIT_OPTHEADERSIZE, optHeaderSize);

    TCHAR sectionSize[10] = { 0 };
    wsprintfW(sectionSize, L"%08X", AnalysePE::GetAnalyse().GetHeaders().fileHeader->NumberOfSections);
    SetDlgItemText(hCurrentDlg_, IDC_EDIT_SECTIONSIZE, sectionSize);

    TCHAR character[10] = { 0 };
    wsprintfW(character, L"%08X", AnalysePE::GetAnalyse().GetHeaders().fileHeader->Characteristics);
    SetDlgItemText(hCurrentDlg_, IDC_EDIT_CHARACT, character);

    TCHAR timeStamp[10] = { 0 };
    wsprintfW(timeStamp, L"%08X", AnalysePE::GetAnalyse().GetHeaders().fileHeader->TimeDateStamp);
    SetDlgItemText(hCurrentDlg_, IDC_EDIT_TIMESTAMP, timeStamp);

    TCHAR subSys[10] = { 0 };
    wsprintfW(subSys, L"%08X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->Subsystem);
    SetDlgItemText(hCurrentDlg_, IDC_EDIT_SUBSYS, subSys);

    TCHAR direSize[10] = { 0 };
    wsprintfW(direSize, L"%08X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->NumberOfRvaAndSizes);
    SetDlgItemText(hCurrentDlg_, IDC_EDIT_DIRSIZE, direSize);
}

void PeEditDlg::CreateFileDlg() {
    fileHeaderDlg_ = std::unique_ptr<FileHeaderDlg>(new FileHeaderDlg(hCurrentDlg_));
    fileHeaderDlg_->InitDlg();
    fileHeaderDlg_->Plant();
};

void PeEditDlg::CreateOptDlg() {
    optHeaderDlg_ = std::unique_ptr<OptHeaderDlg>(new OptHeaderDlg(hCurrentDlg_));
    optHeaderDlg_->InitDlg();
    optHeaderDlg_->Plant();
}

void PeEditDlg::CreateSecDlg() {
    secHeaderDlg_ = std::unique_ptr<SecHeaderDlg>(new SecHeaderDlg(hCurrentDlg_));
    secHeaderDlg_->InitDlg();
    secHeaderDlg_->Plant();
}

void PeEditDlg::CreateDirDlg() {
    dirHeaderDlg_= std::unique_ptr<DirectoryDlg>(new DirectoryDlg(hCurrentDlg_));
    dirHeaderDlg_->InitDlg();
    dirHeaderDlg_->Plant();
}

void PeEditDlg::CreateCalcDlg() {
    calcDlg_ = std::unique_ptr<CalculateDlg>(new CalculateDlg(hCurrentDlg_));
    calcDlg_->InitDlg();
    calcDlg_->Plant();
}

void PeEditDlg::CreateTimeCalcDlg() {
    timeCalcDlg_ = std::unique_ptr<TimeCalcDlg>(new TimeCalcDlg(hCurrentDlg_));
    timeCalcDlg_->InitDlg();
    timeCalcDlg_->Plant();
}

void PeEditDlg::CreateInjectImportDlg() {
    injectImpDlg_ = std::unique_ptr<InjectImportDlg>(new InjectImportDlg(hCurrentDlg_));
    injectImpDlg_->InitDlg();
    injectImpDlg_->Plant();
}

void PeEditDlg::SaveFile() {
    OPENFILENAME openFile = {0};
    //TCHAR tPath[MAX_PATH] = { 0 };
    //FileManage::GetFileManage().GetTcharFilePath(tPath);

    TCHAR* tPath = nullptr;
    CharToTchar(fileManage_->GetFilePath(), &tPath); 

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
    fileManage_->SaveFile(path);

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
        case IDC_BUTTON_TIMECALC:
            thisDlg_->CreateTimeCalcDlg();
            break;
        case IDC_BUTTON_INJECT:
            thisDlg_->CreateInjectImportDlg();
            break;
        case IDC_BUTTON_SAVE:
            thisDlg_->SaveFile();
            break;
        case IDOK:
            AnalysePE::GetAnalyse().UnloadPeData();
            thisDlg_->CloseDlg();
            delete thisDlg_;
            break;
        }
        break;
    }
    case WM_CLOSE: 
        AnalysePE::GetAnalyse().UnloadPeData();
        thisDlg_->CloseDlg();
        delete thisDlg_;
        break;
    default:
        return FALSE;
    }
    return FALSE;
}