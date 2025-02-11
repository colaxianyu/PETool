//module;
//
//#include "resource.h"
//#include <windows.h>
//
//module InjectImportDlg;
//
//import AnalysePE;
//import STL;
//
//extern HINSTANCE app_inst;
//using petools::win32::get_dlg_item_text_as_wstring;
//
//InjectImportDlg* InjectImportDlg::this_dlg_ = nullptr;
//
//InjectImportDlg::InjectImportDlg(HWND h_parent)
//    : DialogEX(IDD_DIALOG_INJECTIMPORT, h_parent)
//{
//
//}
//
//InjectImportDlg::~InjectImportDlg() {
//
//}
//
//void InjectImportDlg::init_dlg() {
//    set_this_dlg();
//}
//
//void InjectImportDlg::plant() {
//    DialogBox(app_inst, MAKEINTRESOURCE(id_template_), h_parent_dlg_, (DLGPROC)InjectImportProc);
//}
//
//void InjectImportDlg::SelectDll() {
//    std::wstring ws_dll_name = get_dlg_item_text_as_wstring(
// 
// , IDC_EDIT_INJECTDLL);
//	std::wstring ws_function_name = get_dlg_item_text_as_wstring(h_current_dlg_, IDC_EDIT_IMJECTFUNC);
//
//    // AnalysePE::GetAnalyse().DllInject(dll_name.c_str(), function_name.c_str());
//	AnalysePE::GetAnalyse().dll_inject(ws_dll_name, ws_function_name);
//    MessageBox(0, L"注入成功", TEXT("成功"), MB_OK);
//}
//
//LRESULT CALLBACK InjectImportDlg::InjectImportProc(HWND hInject, UINT message, WPARAM w_param, LPARAM l_param) {
//    switch (message)
//    {
//    case WM_INITDIALOG:
//        this_dlg_->set_current_dlg_HWND(hInject);
//        break;
//    case WM_COMMAND:
//    {
//        int wmId = LOWORD(w_param);
//        switch (wmId)
//        {
//        case IDC_BUTTON_DLLSELECT:
//            this_dlg_->SelectDll();
//            break;
//        case IDOK:
//            this_dlg_->close_dlg();
//            break;
//        default:
//            break;
//        }
//        break;
//    }
//    case WM_CLOSE:
//        this_dlg_->close_dlg();
//        break;
//    default:
//        return FALSE;
//    }
//    return FALSE;
//}
