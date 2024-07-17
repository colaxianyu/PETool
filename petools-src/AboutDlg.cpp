module;

#include "resource.h"
#include <windows.h>

module AboutDlg;

extern HINSTANCE appInst;

AboutDlg* AboutDlg::thisDlg_ = nullptr;

AboutDlg::AboutDlg(HWND hParent)
	: DialogEX(IDD_DIALOG_ABOUT, hParent)
{

}

AboutDlg::~AboutDlg() {

}

void AboutDlg::InitDlg() {
    SetThisDlg();
}

void AboutDlg::Plant() {
    DialogBox(appInst, MAKEINTRESOURCE(idTemplate_), hParentDlg_, (DLGPROC)AboutProc);
}

LRESULT CALLBACK AboutDlg::AboutProc(HWND hAbout, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message)
    {
    case WM_INITDIALOG:
        thisDlg_->SetCurrentDlgHWND(hAbout);
        break;
    case WM_CLOSE:
        thisDlg_->CloseDlg();
        break;
    default:
        return FALSE;
    }
    return FALSE;
}
