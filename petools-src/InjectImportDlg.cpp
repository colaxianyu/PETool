module;

#include "resource.h"
#include <windows.h>

module InjectImportDlg;

import AnalysePE;

extern HINSTANCE appInst;

InjectImportDlg* InjectImportDlg::thisDlg_ = nullptr;

InjectImportDlg::InjectImportDlg(HWND hParent)
    : DialogEX(IDD_DIALOG_INJECTIMPORT, hParent)
{

}

InjectImportDlg::~InjectImportDlg() {

}

void InjectImportDlg::InitDlg() {
    SetThisDlg();
}

void InjectImportDlg::Plant() {
    DialogBox(appInst, MAKEINTRESOURCE(idTemplate_), hParentDlg_, (DLGPROC)InjectImportProc);
}

void InjectImportDlg::SelectDll() {
    TCHAR tDllName[260] = { 0 };
    SendMessage(GetDlgItem(hCurrentDlg_, IDC_EDIT_INJECTDLL), WM_GETTEXT, 260, (LPARAM)tDllName);
    TCHAR tFuncName[260] = { 0 };
    SendMessage(GetDlgItem(hCurrentDlg_, IDC_EDIT_IMJECTFUNC), WM_GETTEXT, 260, (LPARAM)tFuncName);

    AnalysePE::GetAnalyse().DllInject(tDllName, tFuncName);
    MessageBox(0, L"注入成功", TEXT("成功"), MB_OK);
}

LRESULT CALLBACK InjectImportDlg::InjectImportProc(HWND hInject, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message)
    {
    case WM_INITDIALOG:
        thisDlg_->SetCurrentDlgHWND(hInject);
        break;
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        switch (wmId)
        {
        case IDC_BUTTON_DLLSELECT:
            thisDlg_->SelectDll();
            break;
        case IDOK:
            thisDlg_->CloseDlg();
            break;
        default:
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
