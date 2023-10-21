#include "FileHeaderDlg.h"
#include "resource.h"
#include "PeApplication.h"
extern HINSTANCE appInst;

FileHeaderDlg* FileHeaderDlg::thisDlg_ = nullptr;

FileHeaderDlg::FileHeaderDlg(HWND hParent)
	: DialogEX(IDD_DIALOG_FILEHEAD, hParent)
{

}

FileHeaderDlg::~FileHeaderDlg() {
    timeStampDlg_.reset();
}

void FileHeaderDlg::InitDlg() {
    SetThisDlg();
}

void FileHeaderDlg::Plant() {
    DialogBox(appInst, MAKEINTRESOURCE(idTemplate_), hParentDlg_, (DLGPROC)FileHeaderProc);
}

void FileHeaderDlg::SetFileHeaderInfo() {
    TCHAR machine[10] = { 0 };
    wsprintfW(machine, L"%04X", AnalysePE::GetAnalyse().GetHeaders().fileHeader->Machine);
    SetDlgItemText(hCurrentDlg_, IDC_EDIT_MACHINE, machine);

    TCHAR sectionSize[10] = { 0 };
    wsprintfW(sectionSize, L"%04X", AnalysePE::GetAnalyse().GetHeaders().fileHeader->NumberOfSections);
    SetDlgItemText(hCurrentDlg_, IDC_EDIT_SECTIONS, sectionSize);

    TCHAR time[10] = { 0 };
    wsprintfW(time, L"%08X", AnalysePE::GetAnalyse().GetHeaders().fileHeader->TimeDateStamp);
    SetDlgItemText(hCurrentDlg_, IDC_EDIT_TIMEDATA, time);

    TCHAR coffPointer[10] = { 0 };
    wsprintfW(coffPointer, L"%08X", AnalysePE::GetAnalyse().GetHeaders().fileHeader->PointerToSymbolTable);
    SetDlgItemText(hCurrentDlg_, IDC_EDIT_POINTER, coffPointer);

    TCHAR coffTable[10] = { 0 };
    wsprintfW(coffTable, L"%08X", AnalysePE::GetAnalyse().GetHeaders().fileHeader->NumberOfSymbols);
    SetDlgItemText(hCurrentDlg_, IDC_EDIT_COFFTABLE, coffTable);

    TCHAR optHeaderSize[10] = { 0 };
    wsprintfW(optHeaderSize, L"%04X", AnalysePE::GetAnalyse().GetHeaders().fileHeader->SizeOfOptionalHeader);
    SetDlgItemText(hCurrentDlg_, IDC_EDIT_OPTSIZE, optHeaderSize);

    TCHAR character[10] = { 0 };
    wsprintfW(character, L"%04X", AnalysePE::GetAnalyse().GetHeaders().fileHeader->Characteristics);
    SetDlgItemText(hCurrentDlg_, IDC_EDIT_CHA, character);

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