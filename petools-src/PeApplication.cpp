module;

module PeApplication;

HINSTANCE appInst;
int cmdShow;

void PeApplication::InitApp(HINSTANCE hInstance, int nCmdShow) {
    appInst = hInstance;
    cmdShow = nCmdShow;
}

void PeApplication::CreateMainDlg() {
    mainDlg_ = std::make_unique<MainDlg>();
    mainDlg_->InitDlg();
    mainDlg_->Plant();
}
