//module;
//
//#include "resource.h"
//#include <windows.h>
//#include <string>
//#include <chrono>
//
//module TimeStampDlg;
//
//import STL;
//import AnalysePE;
//
//
//using std::string;
//using std::time_t;
//using petools::win32::set_dlg_item_text_from_wstring;
//using petools::win32::to_wstring_hex;
//
//extern HINSTANCE app_inst;
//
//TimeStampDlg* TimeStampDlg::this_dlg_ = nullptr;
//
//using time_tuple = std::tuple<std::wstring, std::wstring>;
//
//static time_tuple decode_time_stamp(const time_t time_stamp) {
//    std::chrono::time_point tp = std::chrono::system_clock::from_time_t(time_stamp);
//
//    time_t tt = std::chrono::system_clock::to_time_t(tp);
//    tm time;
//    errno_t err = localtime_s(&time, &tt);
//
//    if (err != 0) {
//        MessageBox(0, TEXT("Decode Time Stamp Fail"), TEXT("ERROR"), MB_OK);
//        return std::make_tuple(L"", L"");
//    }
//
//    std::wstring timeYMD = std::format(L"{:04}-{:02}-{:02}", time.tm_year + 1900, time.tm_mon + 1, time.tm_mday);
//    std::wstring timeHMS = std::format(L"{:02}:{:02}:{:02}", time.tm_hour, time.tm_min, time.tm_sec);
//
//    return std::make_tuple(timeYMD, timeHMS);
//}
//
//
//TimeStampDlg::TimeStampDlg(HWND h_parent)
//	: DialogEX(IDD_DIALOG_TIMESTAMP, h_parent)
//{
//    
//}
//
//TimeStampDlg::~TimeStampDlg() {
//
//}
//
//void TimeStampDlg::init_dlg() {
//	set_this_dlg();
//}
//
//void TimeStampDlg::plant() {
//	DialogBox(app_inst, MAKEINTRESOURCE(id_template_), h_parent_dlg_, (DLGPROC)TsProc);
//}
//
//void TimeStampDlg::set_TSdlg_info() {
//    time_t ts = AnalysePE::GetAnalyse().GetHeaders().fileHeader->TimeDateStamp;
//
//    std::wstring ws_time_stamp = to_wstring_hex(ts);
//    auto [ws_time_YMD, ws_time_HMS] = decode_time_stamp(ts);
//
//    bool is_set_time_stamp = set_dlg_item_text_from_wstring(
// , IDC_EDIT_TimeEncode, ws_time_stamp);
//    bool is_set_time_YMD = set_dlg_item_text_from_wstring(h_current_dlg_, IDC_EDIT_TimeYMD, ws_time_YMD);
//    bool is_set_time_HMS = set_dlg_item_text_from_wstring(h_current_dlg_, IDC_EDIT_TimeHMS, ws_time_HMS);
//
//    std::wstring error_message;
//    if (!is_set_time_stamp) {
//        error_message += L"Failed to set time stamp.\n";
//    }
//    if (!is_set_time_YMD) {
//        error_message += L"Failed to set time YMD.\n";
//    }
//    if (!is_set_time_HMS) {
//        error_message += L"Failed to set time HMS.\n";
//    }
//
//    if (!error_message.empty()) {
//        MessageBox(0, error_message.c_str(), TEXT("ERROR"), MB_OK);
//    }
//}
//
//LRESULT CALLBACK TimeStampDlg::TsProc(HWND hTime, UINT message, WPARAM w_param, LPARAM l_param) {
//    switch (message)
//    {
//    case WM_INITDIALOG:
//        this_dlg_->set_current_dlg_HWND(hTime);
//        this_dlg_->set_TSdlg_info();
//        break;
//    case WM_COMMAND:
//    {
//        int wmId = LOWORD(w_param);
//        switch (wmId)
//        {
//        case IDOK:
//            this_dlg_->close_dlg();
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