//module;
//
//#include <windows.h>
//#include "resource.h"
//
//module AboutDlg;
//
//import DialogManager;
//
//AboutDlg::AboutDlg(INT template_id, HWND h_parent) noexcept
//    : DialogEX(template_id, h_parent) {
//
//}
//
//AboutDlg::~AboutDlg() noexcept = default;
//
//void AboutDlg::show_dialog() {
//    ShowWindow(h_current_dlg_, cmd_show_);
//    UpdateWindow(h_current_dlg_);
//}
//
//void AboutDlg::close_dialog() {
//    if (DialogManager::instance().find_dialog(get_template_id()).get()) {
//        DialogManager::instance().unregister_dialog(get_template_id());
//    }
//}
//
//
//LRESULT AboutDlg::dialog_proc(HWND h_about, UINT message, WPARAM w_param, LPARAM l_param) {
//    switch (message) {
//    case WM_CLOSE:
//        dialog_mgr.change_state(template_id_, DialogEvent::Close);
//        return TRUE;
//    default:
//        return FALSE;
//    }
//}