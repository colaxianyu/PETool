#include "PeApplication.h"
#include "MainDlg.h"

HINSTANCE appInst;
int cmdShow;

void PeApplication::InitApp(HINSTANCE hInstance, int nCmdShow) {
    appInst = hInstance;
    cmdShow = nCmdShow;
}

void PeApplication::CreateMainDlg() {
    mainDlg_ = std::make_shared<MainDlg>();
    mainDlg_->InitDlg();
    mainDlg_->Plant();
}
