#include "Dialog.h"

DialogEX::DialogEX(INT idTemplate, HWND hParent)
    : idTemplate_(idTemplate),   
    hParentDlg_(hParent),
    hCurrentDlg_(nullptr)
{

}

void DialogEX::SetCurrentDlgHWND(HWND hDlg) {
    hCurrentDlg_ = hDlg;
}

void DialogEX::CloseDlg() {
    EndDialog(hCurrentDlg_, 0);
}
