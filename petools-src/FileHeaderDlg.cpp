module;

#include "resource.h"
#include <windows.h>

module FileHeaderDlg;

import STL;
import AnalysePE;

using petools::show::SetDlgItemText_t;

extern HINSTANCE appInst;

FileHeaderDlg* FileHeaderDlg::thisDlg_ = nullptr;

FileHeaderDlg::FileHeaderDlg(HWND hParent)
	: DialogEX(IDD_DIALOG_FILEHEAD, hParent)
{

}

FileHeaderDlg::~FileHeaderDlg() {

}

void FileHeaderDlg::InitDlg() {
    SetThisDlg();
}

void FileHeaderDlg::Plant() {
    DialogBox(appInst, MAKEINTRESOURCE(idTemplate_), hParentDlg_, (DLGPROC)FileHeaderProc);
}

void FileHeaderDlg::CloseDlg() {
	timeStampDlg_.reset();
	EndDialog(hCurrentDlg_, 0);
}

void FileHeaderDlg::SetFileHeaderInfo() {
    SetDlgItemText_t(hCurrentDlg_, IDC_EDIT_MACHINE, AnalysePE::GetAnalyse().GetHeaders().fileHeader->Machine, 4);
    SetDlgItemText_t(hCurrentDlg_, IDC_EDIT_SECTIONS, AnalysePE::GetAnalyse().GetHeaders().fileHeader->NumberOfSections, 4);
    SetDlgItemText_t(hCurrentDlg_, IDC_EDIT_TIMEDATA, AnalysePE::GetAnalyse().GetHeaders().fileHeader->TimeDateStamp, 8);
    SetDlgItemText_t(hCurrentDlg_, IDC_EDIT_POINTER, AnalysePE::GetAnalyse().GetHeaders().fileHeader->PointerToSymbolTable, 4);
    SetDlgItemText_t(hCurrentDlg_, IDC_EDIT_COFFTABLE, AnalysePE::GetAnalyse().GetHeaders().fileHeader->NumberOfSymbols, 4);
    SetDlgItemText_t(hCurrentDlg_, IDC_EDIT_OPTSIZE, AnalysePE::GetAnalyse().GetHeaders().fileHeader->SizeOfOptionalHeader, 4);
    SetDlgItemText_t(hCurrentDlg_, IDC_EDIT_CHA, AnalysePE::GetAnalyse().GetHeaders().fileHeader->Characteristics, 4);
}

void FileHeaderDlg::CreateTimeDlg() {
    timeStampDlg_ = std::unique_ptr<TimeStampDlg>(new TimeStampDlg(hCurrentDlg_));
    timeStampDlg_->InitDlg();
    timeStampDlg_->Plant();
}

LRESULT CALLBACK FileHeaderDlg::FileHeaderProc(HWND hFile, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message)
    {
        case WM_INITDIALOG:
            thisDlg_->SetCurrentDlgHWND(hFile);
            thisDlg_->SetFileHeaderInfo();
            break;
        case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            switch (wmId)
            {
            case IDC_BUTTON_DETAILDATA:
                thisDlg_->CreateTimeDlg();
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