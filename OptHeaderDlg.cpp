#include "OptHeaderDlg.h"
#include "resource.h"
#include "AnalysePE.h"

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
    TCHAR magic[10] = { 0 };
    wsprintfW(magic, L"%04X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->Magic);
    SetDlgItemText(hCurrentDlg_, IDC_EDIT_OPTMAGIC, magic);

    TCHAR majorLink[10] = { 0 };
    wsprintfW(majorLink, L"%02X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->MajorLinkerVersion);
    SetDlgItemText(hCurrentDlg_, IDC_EDIT_OPTMAJORLINK, majorLink);

    TCHAR minorLink[10] = { 0 };
    wsprintfW(minorLink, L"%02X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->MinorLinkerVersion);
    SetDlgItemText(hCurrentDlg_, IDC_EDIT_OPTMINORLINK, minorLink);

    TCHAR codeSize[10] = { 0 };
    wsprintfW(codeSize, L"%08X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->SizeOfCode);
    SetDlgItemText(hCurrentDlg_, IDC_EDIT_OPTCODESIZE, codeSize);

    TCHAR initDataSize[10] = { 0 };
    wsprintfW(initDataSize, L"%08X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->SizeOfInitializedData);
    SetDlgItemText(hCurrentDlg_, IDC_EDIT_OPTINITDATA, initDataSize);

    TCHAR uninitDataSize[10] = { 0 };
    wsprintfW(uninitDataSize, L"%08X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->SizeOfUninitializedData);
    SetDlgItemText(hCurrentDlg_, IDC_EDIT_OPTUNINITDATA, uninitDataSize);

    TCHAR entry[10] = { 0 };
    wsprintfW(entry, L"%08X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->AddressOfEntryPoint);
    SetDlgItemText(hCurrentDlg_, IDC_EDIT_OPTENTRY, entry);

    TCHAR codeBase[10] = { 0 };
    wsprintfW(codeBase, L"%08X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->BaseOfCode);
    SetDlgItemText(hCurrentDlg_, IDC_EDIT_OPTCODEBASE, codeBase);

    TCHAR dataBase[10] = { 0 };
    wsprintfW(dataBase, L"%08X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->BaseOfData);
    SetDlgItemText(hCurrentDlg_, IDC_EDIT_OPTDATABASE, dataBase);

    TCHAR imageBase[10] = { 0 };
    wsprintfW(imageBase, L"%08X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->ImageBase);
    SetDlgItemText(hCurrentDlg_, IDC_EDIT_OPTIMAGEBASE, imageBase);

    TCHAR secAlign[10] = { 0 };
    wsprintfW(secAlign, L"%08X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->SectionAlignment);
    SetDlgItemText(hCurrentDlg_, IDC_EDIT_OPTSECALIGN, secAlign);

    TCHAR fileAlign[10] = { 0 };
    wsprintfW(fileAlign, L"%08X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->FileAlignment);
    SetDlgItemText(hCurrentDlg_, IDC_EDIT_OPTFILEALIGN, fileAlign);

    TCHAR majorOS[10] = { 0 };
    wsprintfW(majorOS, L"%04X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->MajorOperatingSystemVersion);
    SetDlgItemText(hCurrentDlg_, IDC_EDIT_OPTMAJOROS, majorOS);

    TCHAR minorOs[10] = { 0 };
    wsprintfW(minorOs, L"%04X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->MinorOperatingSystemVersion);
    SetDlgItemText(hCurrentDlg_, IDC_EDIT_OPTMINOROS, minorOs);

    TCHAR majorImage[10] = { 0 };
    wsprintfW(majorImage, L"%04X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->MajorImageVersion);
    SetDlgItemText(hCurrentDlg_, IDC_EDIT_OPTMAJORIMAGE, majorImage);

    TCHAR minorImage[10] = { 0 };
    wsprintfW(minorImage, L"%04X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->MinorImageVersion);
    SetDlgItemText(hCurrentDlg_, IDC_EDIT_OPTMINORIMAGE, minorImage);

    TCHAR majorSubSys[10] = { 0 };
    wsprintfW(majorSubSys, L"%04X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->MajorSubsystemVersion);
    SetDlgItemText(hCurrentDlg_, IDC_EDIT_OPTMAJORSUBSYS, majorSubSys);

    TCHAR minorSubSys[10] = { 0 };
    wsprintfW(minorSubSys, L"%04X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->MinorSubsystemVersion);
    SetDlgItemText(hCurrentDlg_, IDC_EDIT_OPTMINORSUBSYS, minorSubSys);

    TCHAR win32Version[10] = { 0 };
    wsprintfW(win32Version, L"%08X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->Win32VersionValue);
    SetDlgItemText(hCurrentDlg_, IDC_EDIT_OPTWINVERSION, win32Version);

    TCHAR imageSize[10] = { 0 };
    wsprintfW(imageSize, L"%08X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->SizeOfImage);
    SetDlgItemText(hCurrentDlg_, IDC_EDIT_OPTIMAGESIZE, imageSize);

    TCHAR headersSize[10] = { 0 };
    wsprintfW(headersSize, L"%08X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->SizeOfHeaders);
    SetDlgItemText(hCurrentDlg_, IDC_EDIT_OPTHEADERSSIZE, headersSize);

    TCHAR check[10] = { 0 };
    wsprintfW(check, L"%08X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->CheckSum);
    SetDlgItemText(hCurrentDlg_, IDC_EDIT_OPTCHECK, check);

    TCHAR subSys[10] = { 0 };
    wsprintfW(subSys, L"%04X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->Subsystem);
    SetDlgItemText(hCurrentDlg_, IDC_EDIT_OPTSUBSYS, subSys);

    TCHAR dllCha[10] = { 0 };
    wsprintfW(dllCha, L"%04X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->DllCharacteristics);
    SetDlgItemText(hCurrentDlg_, IDC_EDIT_OPTDLLCHA, dllCha);

    TCHAR stackResSize[10] = { 0 };
    wsprintfW(stackResSize, L"%08X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->SizeOfStackReserve);
    SetDlgItemText(hCurrentDlg_, IDC_EDIT_OPTSTACKRES, stackResSize);

    TCHAR stackComSize[10] = { 0 };
    wsprintfW(stackComSize, L"%08X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->SizeOfStackCommit);
    SetDlgItemText(hCurrentDlg_, IDC_EDIT_OPTSTACKCOM, stackComSize);

    TCHAR heapResSize[10] = { 0 };
    wsprintfW(heapResSize, L"%08X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->SizeOfHeapReserve);
    SetDlgItemText(hCurrentDlg_, IDC_EDIT_OPTHEAPRES, heapResSize);

    TCHAR heapComSize[10] = { 0 };
    wsprintfW(heapComSize, L"%08X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->SizeOfHeapCommit);
    SetDlgItemText(hCurrentDlg_, IDC_EDIT_OPTHEAPCOM, heapComSize);

    TCHAR loaderFlags[10] = { 0 };
    wsprintfW(loaderFlags, L"%08X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->LoaderFlags);
    SetDlgItemText(hCurrentDlg_, IDC_EDIT_OPTFLAGS, loaderFlags);

    TCHAR rvaAndSize[10] = { 0 };
    wsprintfW(rvaAndSize, L"%08X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->NumberOfRvaAndSizes);
    SetDlgItemText(hCurrentDlg_, IDC_EDIT_OPTRVAANDSIZE, rvaAndSize);
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