module;

#include "resource.h"
#include <windows.h>

module OptHeaderDlg;

import STL;
import AnalysePE;

using petools::show::SetDlgItemText_t;


extern HINSTANCE appInst;

OptHeaderDlg* OptHeaderDlg::thisDlg_ = nullptr;

OptHeaderDlg::OptHeaderDlg(HWND hParent)
	: DialogEX(IDD_DIALOG_OPTHEADER, hParent)
{

}

OptHeaderDlg::~OptHeaderDlg() {

}

void OptHeaderDlg::InitDlg() {
    SetThisDlg();
}

void OptHeaderDlg::Plant() {
    DialogBox(appInst, MAKEINTRESOURCE(idTemplate_), hParentDlg_, (DLGPROC)OptHeaderProc);

    
}

void OptHeaderDlg::SetOptHeaderInfo() {
    SetDlgItemText_t(hCurrentDlg_, IDC_EDIT_OPTMAGIC, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->Magic, 4);
    SetDlgItemText_t(hCurrentDlg_, IDC_EDIT_OPTMAJORLINK, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->MajorLinkerVersion, 2);
    SetDlgItemText_t(hCurrentDlg_, IDC_EDIT_OPTMINORLINK, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->MinorLinkerVersion, 2);
    SetDlgItemText_t(hCurrentDlg_, IDC_EDIT_OPTCODESIZE, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->SizeOfCode, 8);
    SetDlgItemText_t(hCurrentDlg_, IDC_EDIT_OPTINITDATA, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->SizeOfInitializedData, 8);
    SetDlgItemText_t(hCurrentDlg_, IDC_EDIT_OPTUNINITDATA, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->SizeOfUninitializedData, 8);
    SetDlgItemText_t(hCurrentDlg_, IDC_EDIT_OPTENTRY, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->AddressOfEntryPoint, 8);
    SetDlgItemText_t(hCurrentDlg_, IDC_EDIT_OPTCODEBASE, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->BaseOfCode, 8);
    SetDlgItemText_t(hCurrentDlg_, IDC_EDIT_OPTDATABASE, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->BaseOfData, 8);
    SetDlgItemText_t(hCurrentDlg_, IDC_EDIT_OPTIMAGEBASE, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->ImageBase, 8);
    SetDlgItemText_t(hCurrentDlg_, IDC_EDIT_OPTSECALIGN, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->SectionAlignment, 8);
    SetDlgItemText_t(hCurrentDlg_, IDC_EDIT_OPTFILEALIGN, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->FileAlignment, 8);
    SetDlgItemText_t(hCurrentDlg_, IDC_EDIT_OPTMAJOROS, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->MajorOperatingSystemVersion, 4);
    SetDlgItemText_t(hCurrentDlg_, IDC_EDIT_OPTMINOROS, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->MinorOperatingSystemVersion, 4);
    SetDlgItemText_t(hCurrentDlg_, IDC_EDIT_OPTMAJORIMAGE, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->MajorImageVersion, 4);
    SetDlgItemText_t(hCurrentDlg_, IDC_EDIT_OPTMINORIMAGE, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->MinorImageVersion, 4);
    SetDlgItemText_t(hCurrentDlg_, IDC_EDIT_OPTMAJORSUBSYS, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->MajorSubsystemVersion, 4);
    SetDlgItemText_t(hCurrentDlg_, IDC_EDIT_OPTMINORSUBSYS, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->MinorSubsystemVersion, 4);
    SetDlgItemText_t(hCurrentDlg_, IDC_EDIT_OPTWINVERSION, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->Win32VersionValue, 8);
    SetDlgItemText_t(hCurrentDlg_, IDC_EDIT_OPTIMAGESIZE, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->SizeOfImage, 8);
    SetDlgItemText_t(hCurrentDlg_, IDC_EDIT_OPTHEADERSSIZE, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->SizeOfHeaders, 8);
    SetDlgItemText_t(hCurrentDlg_, IDC_EDIT_OPTCHECK, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->CheckSum, 8);
    SetDlgItemText_t(hCurrentDlg_, IDC_EDIT_OPTSUBSYS, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->Subsystem, 4);
    SetDlgItemText_t(hCurrentDlg_, IDC_EDIT_OPTDLLCHA, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->DllCharacteristics, 4);
    SetDlgItemText_t(hCurrentDlg_, IDC_EDIT_OPTSTACKRES, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->SizeOfStackReserve, 8);
    SetDlgItemText_t(hCurrentDlg_, IDC_EDIT_OPTSTACKCOM, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->SizeOfStackCommit, 8);
    SetDlgItemText_t(hCurrentDlg_, IDC_EDIT_OPTHEAPRES, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->SizeOfHeapReserve, 8);
    SetDlgItemText_t(hCurrentDlg_, IDC_EDIT_OPTHEAPCOM, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->SizeOfHeapCommit, 8);
    SetDlgItemText_t(hCurrentDlg_, IDC_EDIT_OPTFLAGS, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->LoaderFlags, 8);
    SetDlgItemText_t(hCurrentDlg_, IDC_EDIT_OPTRVAANDSIZE, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->NumberOfRvaAndSizes, 8);
}

LRESULT CALLBACK OptHeaderDlg::OptHeaderProc(HWND hOpt, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message)
    {
    case WM_INITDIALOG:
        thisDlg_->SetCurrentDlgHWND(hOpt);
        thisDlg_->SetOptHeaderInfo();
        break;
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        switch (wmId)
        {
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