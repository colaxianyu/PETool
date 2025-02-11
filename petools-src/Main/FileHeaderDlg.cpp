//module;
//
//#include "resource.h"
//#include <windows.h>
//
//module FileHeaderDlg;
//
//import STL;
//import AnalysePE;
//
//using petools::show::SetDlgItemText_t;
//
//extern HINSTANCE app_inst;
//
//FileHeaderDlg* FileHeaderDlg::this_dlg_ = nullptr;
//
//FileHeaderDlg::FileHeaderDlg(HWND h_parent)
//	: DialogEX(IDD_DIALOG_FILEHEAD, h_parent)
//{
//
//}
//
//FileHeaderDlg::~FileHeaderDlg() {
//
//}
//
//void FileHeaderDlg::init_dlg() {
//    set_this_dlg();
//}
//
//void FileHeaderDlg::plant() {
//    DialogBox(app_inst, MAKEINTRESOURCE(id_template_), h_parent_dlg_, (DLGPROC)FileHeaderProc);
//}
//
//void FileHeaderDlg::close_dlg() {
//	timeStampDlg_.reset();
//	EndDialog(h_current_dlg_, 0);
//}
//
//void FileHeaderDlg::SetFileHeaderInfo() {
//    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_MACHINE, AnalysePE::GetAnalyse().GetHeaders().fileHeader->Machine, 4);
//    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_SECTIONS, AnalysePE::GetAnalyse().GetHeaders().fileHeader->NumberOfSections, 4);
//    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_TIMEDATA, AnalysePE::GetAnalyse().GetHeaders().fileHeader->TimeDateStamp, 8);
//    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_POINTER, AnalysePE::GetAnalyse().GetHeaders().fileHeader->PointerToSymbolTable, 4);
//    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_COFFTABLE, AnalysePE::GetAnalyse().GetHeaders().fileHeader->NumberOfSymbols, 4);
//    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_OPTSIZE, AnalysePE::GetAnalyse().GetHeaders().fileHeader->SizeOfOptionalHeader, 4);
//    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_CHA, AnalysePE::GetAnalyse().GetHeaders().fileHeader->Characteristics, 4);
//}
//
//void FileHeaderDlg::CreateTimeDlg() {
//    timeStampDlg_ = std::unique_ptr<TimeStampDlg>(new TimeStampDlg(h_current_dlg_));
//    timeStampDlg_->init_dlg();
//    timeStampDlg_->plant();
//}
//
//LRESULT CALLBACK FileHeaderDlg::FileHeaderProc(HWND hFile, UINT message, WPARAM w_param, LPARAM l_param) {
//    switch (message)
//    {
//        case WM_INITDIALOG:
//            this_dlg_->set_current_dlg_HWND(hFile);
//            this_dlg_->SetFileHeaderInfo();
//            break;
//        case WM_COMMAND:
//        {
//            int wmId = LOWORD(w_param);
//            switch (wmId)
//            {
//            case IDC_BUTTON_DETAILDATA:
//                this_dlg_->CreateTimeDlg();
//                break;
//            case IDOK:
//                this_dlg_->close_dlg();
//                break;
//            }
//            break;
//        }
//        case WM_CLOSE:
//            this_dlg_->close_dlg();
//            break;
//        default:
//            return FALSE;
//    }
//    return FALSE;
//}