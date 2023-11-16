#include "DirectoryDlg.h"

#include "AboutDlg.h"
#include "resource.h"
#include "AnalysePE.h"

extern HINSTANCE appInst;

DirectoryDlg* DirectoryDlg::thisDlg_ = nullptr;

DirectoryDlg::DirectoryDlg(HWND hParent)
    : DialogEX(IDD_DIALOG_DIRECTORY, hParent)
{

}

DirectoryDlg::~DirectoryDlg() {
    exportDlg_.reset();
    importDlg_.reset();
    relocationDlg_.reset();
}

void DirectoryDlg::InitDlg() {
    SetThisDlg();
}

void DirectoryDlg::Plant() {
    DialogBox(appInst, MAKEINTRESOURCE(idTemplate_), hParentDlg_, (DLGPROC)DirProc);
}

void DirectoryDlg::CreateExpertDlg() {
   // if(AnalysePE::GetAnalyse().IsHaveExport()){
        exportDlg_ = std::unique_ptr<ExportDlg>(new ExportDlg(hCurrentDlg_));
        exportDlg_->InitDlg();
        exportDlg_->Plant();
    //}
    //else {
        //MessageBox(hCurrentDlg_, L"No Export", L"ERROR", MB_ICONERROR);
    //}
}

void DirectoryDlg::CreateImportDlg() {
    importDlg_ = std::unique_ptr<ImportDlg>(new ImportDlg(hCurrentDlg_));
    importDlg_->InitDlg();
    importDlg_->Plant();
}

void DirectoryDlg::CreateRelocationDlg() {
    relocationDlg_ = std::unique_ptr<RelocationDlg>(new RelocationDlg(hCurrentDlg_));
    relocationDlg_->InitDlg();
    relocationDlg_->Plant();
}

void DirectoryDlg::SetDirHeaderInfo() {
        TCHAR exportRva[9] = { 0 };
        TCHAR exportSize[9] = { 0 };
        wsprintfW(exportRva, L"%08X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->DataDirectory[0].VirtualAddress);
        wsprintfW(exportSize, L"%08X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->DataDirectory[0].Size);
        SetDlgItemText(hCurrentDlg_, IDC_EDIT_EXPORTRAV, exportRva);
        SetDlgItemText(hCurrentDlg_, IDC_EDIT_EXPORTSIZE, exportSize);

        TCHAR importRva[9] = { 0 };
        TCHAR importSize[9] = { 0 };
        wsprintfW(importRva, L"%08X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->DataDirectory[1].VirtualAddress);
        wsprintfW(importSize, L"%08X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->DataDirectory[1].Size);
        SetDlgItemText(hCurrentDlg_, IDC_EDIT_IMPORTRAV, importRva);
        SetDlgItemText(hCurrentDlg_, IDC_EDIT_IMPORTSIZE, importSize);

        TCHAR resRva[9] = { 0 };
        TCHAR resSize[9] = { 0 };
        wsprintfW(resRva, L"%08X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->DataDirectory[2].VirtualAddress);
        wsprintfW(resSize, L"%08X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->DataDirectory[2].Size);
        SetDlgItemText(hCurrentDlg_, IDC_EDIT_RESOURCERVA, resRva);
        SetDlgItemText(hCurrentDlg_, IDC_EDIT_RESOURCESIZE, resSize);

        TCHAR exceptRva[9] = { 0 };
        TCHAR exceptSize[9] = { 0 };
        wsprintfW(exceptRva, L"%08X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->DataDirectory[3].VirtualAddress);
        wsprintfW(exceptSize, L"%08X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->DataDirectory[3].Size);
        SetDlgItemText(hCurrentDlg_, IDC_EDIT_EXCEPTIONRVA, exceptRva);
        SetDlgItemText(hCurrentDlg_, IDC_EDIT_EXCEPTIONSIZE, exceptSize);

        TCHAR securityRva[9] = { 0 };
        TCHAR securitySize[9] = { 0 };
        wsprintfW(securityRva, L"%08X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->DataDirectory[4].VirtualAddress);
        wsprintfW(securitySize, L"%08X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->DataDirectory[4].Size);
        SetDlgItemText(hCurrentDlg_, IDC_EDIT_SECURITYRVA, securityRva);
        SetDlgItemText(hCurrentDlg_, IDC_EDIT_SECURITYSIZE, securitySize);

        TCHAR relocateRva[9] = { 0 };
        TCHAR relocateSize[9] = { 0 };
        wsprintfW(relocateRva, L"%08X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->DataDirectory[5].VirtualAddress);
        wsprintfW(relocateSize, L"%08X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->DataDirectory[5].Size);
        SetDlgItemText(hCurrentDlg_, IDC_EDIT_RELOCATIONRVA, relocateRva);
        SetDlgItemText(hCurrentDlg_, IDC_EDIT_RELOCATIONSIZE, relocateSize);

        TCHAR debugRva[9] = { 0 };
        TCHAR debugSize[9] = { 0 };
        wsprintfW(debugRva, L"%08X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->DataDirectory[6].VirtualAddress);
        wsprintfW(debugSize, L"%08X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->DataDirectory[6].Size);
        SetDlgItemText(hCurrentDlg_, IDC_EDIT_DEBUGRVA, debugRva);
        SetDlgItemText(hCurrentDlg_, IDC_EDIT_DEBUGSIZE, debugSize);

        TCHAR archDataRva[9] = { 0 };
        TCHAR archDataSize[9] = { 0 };
        wsprintfW(archDataRva, L"%08X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->DataDirectory[7].VirtualAddress);
        wsprintfW(archDataSize, L"%08X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->DataDirectory[7].Size);
        SetDlgItemText(hCurrentDlg_, IDC_EDIT_ASDRVA, archDataRva);
        SetDlgItemText(hCurrentDlg_, IDC_EDIT_ASDSIZE, archDataSize);

        TCHAR GPRva[9] = { 0 };
        TCHAR GPSize[9] = { 0 };
        wsprintfW(GPRva, L"%08X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->DataDirectory[8].VirtualAddress);
        wsprintfW(GPSize, L"%08X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->DataDirectory[8].Size);
        SetDlgItemText(hCurrentDlg_, IDC_EDIT_RVAGPRVA, GPRva);
        SetDlgItemText(hCurrentDlg_, IDC_EDIT_RVAGPSIZE, GPSize);

        TCHAR TLSRva[9] = { 0 };
        TCHAR TLSSize[9] = { 0 };
        wsprintfW(TLSRva, L"%08X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->DataDirectory[9].VirtualAddress);
        wsprintfW(TLSSize, L"%08X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->DataDirectory[9].Size);
        SetDlgItemText(hCurrentDlg_, IDC_EDIT_TLSRVA, TLSRva);
        SetDlgItemText(hCurrentDlg_, IDC_EDIT_TLSSIZE, TLSSize);

        TCHAR loadRva[9] = { 0 };
        TCHAR loadSize[9] = { 0 };
        wsprintfW(loadRva, L"%08X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->DataDirectory[10].VirtualAddress);
        wsprintfW(loadSize, L"%08X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->DataDirectory[10].Size);
        SetDlgItemText(hCurrentDlg_, IDC_EDIT_LOADRVA, loadRva);
        SetDlgItemText(hCurrentDlg_, IDC_EDIT_LOADSIZE, loadSize);

        TCHAR boundRva[9] = { 0 };
        TCHAR boundSize[9] = { 0 };
        wsprintfW(boundRva, L"%08X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->DataDirectory[11].VirtualAddress);
        wsprintfW(boundSize, L"%08X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->DataDirectory[11].Size);
        SetDlgItemText(hCurrentDlg_, IDC_EDIT_BOUNDRVA, boundRva);
        SetDlgItemText(hCurrentDlg_, IDC_EDIT_BOUNDSIZE, boundSize);

        TCHAR IATRva[9] = { 0 };
        TCHAR IATSize[9] = { 0 };
        wsprintfW(IATRva, L"%08X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->DataDirectory[12].VirtualAddress);
        wsprintfW(IATSize, L"%08X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->DataDirectory[12].Size);
        SetDlgItemText(hCurrentDlg_, IDC_EDIT_IATRVA, IATRva);
        SetDlgItemText(hCurrentDlg_, IDC_EDIT_IATSIZE, IATSize);

        TCHAR delayRva[9] = { 0 };
        TCHAR delaySize[9] = { 0 };
        wsprintfW(delayRva, L"%08X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->DataDirectory[13].VirtualAddress);
        wsprintfW(delaySize, L"%08X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->DataDirectory[13].Size);
        SetDlgItemText(hCurrentDlg_, IDC_EDIT_DELAYRVA, delayRva);
        SetDlgItemText(hCurrentDlg_, IDC_EDIT_DELAYSIZE, delaySize);

        TCHAR COMRva[9] = { 0 };
        TCHAR COMSize[9] = { 0 };
        wsprintfW(COMRva, L"%08X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->DataDirectory[14].VirtualAddress);
        wsprintfW(COMSize, L"%08X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->DataDirectory[14].Size);
        SetDlgItemText(hCurrentDlg_, IDC_EDIT_COMRVA, COMRva);
        SetDlgItemText(hCurrentDlg_, IDC_EDIT_COMSIZE, COMSize);

        TCHAR reserveRva[9] = { 0 };
        TCHAR reserveSize[9] = { 0 };
        wsprintfW(reserveRva, L"%08X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->DataDirectory[15].VirtualAddress);
        wsprintfW(reserveSize, L"%08X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->DataDirectory[15].Size);
        SetDlgItemText(hCurrentDlg_, IDC_EDIT_RESERVEDRVA, reserveRva);
        SetDlgItemText(hCurrentDlg_, IDC_EDIT_RESERVEDSIZE, reserveSize);
}

LRESULT CALLBACK DirectoryDlg::DirProc(HWND hDir, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message)
    {
    case WM_INITDIALOG:
        thisDlg_->SetCurrentDlgHWND(hDir);
        thisDlg_->SetDirHeaderInfo();
        break;
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        switch (wmId)
        {
        case IDC_BUTTON_EXPORT:
            thisDlg_->CreateExpertDlg();
            break;
        case IDC_BUTTON_IMPORT:
            thisDlg_->CreateImportDlg();
            break;
        case IDC_BUTTON_RELOCATION:
            thisDlg_->CreateRelocationDlg();
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
