//module;
//
//#include "resource.h"
//#include <windows.h>
//
//module CalculateDlg;
//
//import STL;
//import AnalysePE;
//
//using std::unordered_set;
//using petools::win32::set_IDC_EDIT_read_only;
//using petools::win32::set_IDC_EDIT_text_limit;
//using petools::TcharToChar;
//using petools::TcharToDword;
//using enum PositionInPE;
//
//
//extern HINSTANCE app_inst;
//
//WNDPROC RvaOldProc;
//WNDPROC VaOldProc;
//WNDPROC FoaOldProc;
//
//CalculateDlg* CalculateDlg::this_dlg_ = nullptr;
//
//static std::unordered_set<TCHAR> get_hex_list() {
//	std::unordered_set<TCHAR> hex_list;
//
//    for (TCHAR ch = L'0'; ch <= L'9'; ++ch) {
//        hex_list.emplace(ch);
//    }
//
//    for (TCHAR ch = L'A'; ch <= L'F'; ++ch) {
//        hex_list.emplace(ch);
//        hex_list.emplace(ch + 32);  
//    }
//
//    return hex_list;
//}
//
//static std::unordered_set<TCHAR> hex_list = get_hex_list();
//
//static BOOL is_tchar_hex(TCHAR t_ch) {
//    return hex_list.count(t_ch) != 0;
//}
//
//CalculateDlg::CalculateDlg(HWND h_parent)
//    : DialogEX(IDD_DIALOG_CALCULATE, h_parent)
//{
//
//}
//
//CalculateDlg::~CalculateDlg() {
//    
//}
//
//void CalculateDlg::init_dlg() {
//    set_this_dlg();
//}
//
//void CalculateDlg::plant() {
//    DialogBox(app_inst, MAKEINTRESOURCE(id_template_), h_parent_dlg_, (DLGPROC)calc_proc);
//}
//
//void CalculateDlg::init_radio_button() {
//    CheckRadioButton(h_current_dlg_, IDC_RADIO_VA, IDC_RADIO_FOA, IDC_RADIO_RVA);
//    set_IDC_EDIT_read_only(h_current_dlg_, IDC_EDIT_VA);
//    set_IDC_EDIT_read_only(h_current_dlg_, IDC_EDIT_FOA);
//
//	constexpr int limit = 8;
//    set_IDC_EDIT_text_limit(h_current_dlg_, IDC_EDIT_VA, limit);
//    set_IDC_EDIT_text_limit(h_current_dlg_, IDC_EDIT_RVA, limit);
//    set_IDC_EDIT_text_limit(h_current_dlg_, IDC_EDIT_FOA, limit);
//}
//
//
///*
//*   VA Radio Button event
//*/
//void CalculateDlg::on_click_VA() {
//    SetWindowText(GetDlgItem(h_current_dlg_, IDC_EDIT_VA), nullptr);
//    SetWindowText(GetDlgItem(h_current_dlg_, IDC_EDIT_RVA), nullptr);
//    SetWindowText(GetDlgItem(h_current_dlg_, IDC_EDIT_FOA), nullptr);
//    SetWindowText(GetDlgItem(h_current_dlg_, IDC_EDIT_LOCATE), nullptr);
//
//    SendDlgItemMessage(h_current_dlg_, IDC_EDIT_VA, EM_SETREADONLY, 0, 0);
//    SendDlgItemMessage(h_current_dlg_, IDC_EDIT_RVA, EM_SETREADONLY, 1, 0);
//    SendDlgItemMessage(h_current_dlg_, IDC_EDIT_FOA, EM_SETREADONLY, 1, 0);
//}
//
///*
//*   RVA Radio Button event
//*/
//void CalculateDlg::on_click_RVA() {
//    SetWindowText(GetDlgItem(h_current_dlg_, IDC_EDIT_VA), nullptr);
//    SetWindowText(GetDlgItem(h_current_dlg_, IDC_EDIT_RVA), nullptr);
//    SetWindowText(GetDlgItem(h_current_dlg_, IDC_EDIT_FOA), nullptr);
//    SetWindowText(GetDlgItem(h_current_dlg_, IDC_EDIT_LOCATE), nullptr);
//
//    SendDlgItemMessage(h_current_dlg_, IDC_EDIT_VA, EM_SETREADONLY, 1, 0);
//    SendDlgItemMessage(h_current_dlg_, IDC_EDIT_RVA, EM_SETREADONLY, 0, 0);
//    SendDlgItemMessage(h_current_dlg_, IDC_EDIT_FOA, EM_SETREADONLY, 1, 0);
//}
//
///*
//*   FOA Radio Button event
//*/
//void CalculateDlg::OnClickFOA() {
//    SetWindowText(GetDlgItem(h_current_dlg_, IDC_EDIT_VA), nullptr);
//    SetWindowText(GetDlgItem(h_current_dlg_, IDC_EDIT_RVA), nullptr);
//    SetWindowText(GetDlgItem(h_current_dlg_, IDC_EDIT_FOA), nullptr);
//    SetWindowText(GetDlgItem(h_current_dlg_, IDC_EDIT_LOCATE), nullptr);
//
//    SendDlgItemMessage(h_current_dlg_, IDC_EDIT_VA, EM_SETREADONLY, 1, 0);
//    SendDlgItemMessage(h_current_dlg_, IDC_EDIT_RVA, EM_SETREADONLY, 1, 0);
//    SendDlgItemMessage(h_current_dlg_, IDC_EDIT_FOA, EM_SETREADONLY, 0, 0);
//}
//
///*
//*   计算按钮 event
//*/
//void CalculateDlg::OnClickCalc() {
//    TCHAR editBuffer[9] = { 0 };
//    if (IsDlgButtonChecked(h_current_dlg_, IDC_RADIO_VA)) {
//        SendMessage(GetDlgItem(h_current_dlg_, IDC_EDIT_VA), WM_GETTEXT, 9, (LPARAM)editBuffer);
//        DWORD VA = -1;
//        TcharToDword(editBuffer, &VA, 16);
//        from_VA_calc(VA);
//
//    }
//    else if (IsDlgButtonChecked(h_current_dlg_, IDC_RADIO_RVA)) {
//        SendMessage(GetDlgItem(h_current_dlg_, IDC_EDIT_RVA), WM_GETTEXT, 9, (LPARAM)editBuffer);
//        DWORD RVA = -1;
//        TcharToDword(editBuffer, &RVA, 16);
//        from_RVA_calc(RVA);
//    }
//    else {
//        SendMessage(GetDlgItem(h_current_dlg_, IDC_EDIT_FOA), WM_GETTEXT, 9, (LPARAM)editBuffer);
//        DWORD FOA = -1;
//        TcharToDword(editBuffer, &FOA, 16);
//        from_FOA_calc(FOA);
//    }
//}
//
//void CalculateDlg::from_VA_calc(DWORD VA) {
//    DWORD RVA = VA - (DWORD)AnalysePE::GetAnalyse().GetHeaders().optionalHeader->ImageBase;
//    PositionInPE pos = AnalysePE::GetAnalyse().PositonInfoRVA(RVA);
//
//    switch (pos) {
//    case inHead:
//    {
//        TCHAR* locateName = const_cast<TCHAR*>(TEXT("Header"));
//        SetWindowText(GetDlgItem(h_current_dlg_, IDC_EDIT_LOCATE), locateName);
//
//        TCHAR RVABuffer[9] = { 0 };
//        wsprintfW(RVABuffer, L"%08X", RVA);
//        SetWindowText(GetDlgItem(h_current_dlg_, IDC_EDIT_RVA), RVABuffer);
//
//        TCHAR FOABuffer[9] = { 0 };
//        wsprintfW(FOABuffer, L"%08X", RVA);
//        SetWindowText(GetDlgItem(h_current_dlg_, IDC_EDIT_FOA), FOABuffer);
//
//        TCHAR VABuffer[9] = { 0 };
//        wsprintfW(VABuffer, L"%08X", VA);
//        SetWindowText(GetDlgItem(h_current_dlg_, IDC_EDIT_VA), VABuffer);
//        return;
//    }
//    case inSection:
//    {
//        TCHAR* locateName = nullptr;
//        DWORD secIndex = AnalysePE::GetAnalyse().InWhichSectionRVA(RVA);
//        AnalysePE::GetAnalyse().GetTcharSectionName(secIndex, &locateName);
//        SetWindowText(GetDlgItem(h_current_dlg_, IDC_EDIT_LOCATE), locateName);
//
//        TCHAR RVABuffer[9] = { 0 };
//        wsprintfW(RVABuffer, L"%08X", RVA);
//        SetWindowText(GetDlgItem(h_current_dlg_, IDC_EDIT_RVA), RVABuffer);
//
//        DWORD FOA = -1;
//        FOA = AnalysePE::GetAnalyse().RVAToFOA(RVA);
//        TCHAR FOABuffer[9] = { 0 };
//        wsprintfW(FOABuffer, L"%08X", FOA);
//        SetWindowText(GetDlgItem(h_current_dlg_, IDC_EDIT_FOA), FOABuffer);
//
//        TCHAR VABuffer[9] = { 0 };
//        wsprintfW(VABuffer, L"%08X", VA);
//        SetWindowText(GetDlgItem(h_current_dlg_, IDC_EDIT_VA), VABuffer);
//        return;
//    }
//    case outFile:
//    {
//        MessageBox(0, TEXT("Out Of File!"), TEXT("Error"), MB_OK);
//        return;
//    }
//    default:
//        return;
//    }
//}
//
//void CalculateDlg::from_RVA_calc(DWORD RVA) {
//    PositionInPE pos = AnalysePE::GetAnalyse().PositonInfoRVA(RVA);
//    switch (pos) {
//    case inHead:
//    {
//        TCHAR* locateName = const_cast<TCHAR*>(TEXT("Header"));
//        SetWindowText(GetDlgItem(h_current_dlg_, IDC_EDIT_LOCATE), locateName);
//
//        TCHAR RVABuffer[9] = { 0 };
//        wsprintfW(RVABuffer, L"%08X", RVA);
//        SetWindowText(GetDlgItem(h_current_dlg_, IDC_EDIT_RVA), RVABuffer);
//
//        TCHAR FOABuffer[9] = { 0 };
//        wsprintfW(FOABuffer, L"%08X", RVA);
//        SetWindowText(GetDlgItem(h_current_dlg_, IDC_EDIT_FOA), FOABuffer);
//
//        TCHAR VABuffer[9] = { 0 };
//        wsprintfW(VABuffer, L"%08X", RVA + (DWORD)AnalysePE::GetAnalyse().GetHeaders().optionalHeader->ImageBase);
//        SetWindowText(GetDlgItem(h_current_dlg_, IDC_EDIT_VA), VABuffer);
//        return;
//    }
//    case inSection:
//    {
//        DWORD secIndex = AnalysePE::GetAnalyse().InWhichSectionRVA(RVA);
//        TCHAR* locateName = nullptr;
//        AnalysePE::GetAnalyse().GetTcharSectionName(secIndex, &locateName);
//        SetWindowText(GetDlgItem(h_current_dlg_, IDC_EDIT_LOCATE), locateName);
//
//        TCHAR RVABuffer[9] = { 0 };
//        wsprintfW(RVABuffer, L"%08X", RVA);
//        SetWindowText(GetDlgItem(h_current_dlg_, IDC_EDIT_RVA), RVABuffer);
//
//        DWORD FOA = -1;
//        FOA = AnalysePE::GetAnalyse().RVAToFOA(RVA);
//        TCHAR FOABuffer[9] = { 0 };
//        wsprintfW(FOABuffer, L"%08X", FOA);
//        SetWindowText(GetDlgItem(h_current_dlg_, IDC_EDIT_FOA), FOABuffer);
//
//        TCHAR VABuffer[9] = { 0 };
//        wsprintfW(VABuffer, L"%08X", RVA + (DWORD)AnalysePE::GetAnalyse().GetHeaders().optionalHeader->ImageBase);
//        SetWindowText(GetDlgItem(h_current_dlg_, IDC_EDIT_VA), VABuffer);
//        return;
//    }
//    case outFile:
//    {
//        MessageBox(0, TEXT("Out Of File!"), TEXT("Error"), MB_OK);
//        return;
//    }
//    default:
//        return;
//    }
//}
//
//void CalculateDlg::from_FOA_calc(DWORD FOA) {
//    PositionInPE pos = AnalysePE::GetAnalyse().PositonInfoFOA(FOA);
//    switch (pos) {
//    case inHead:
//    {
//        TCHAR* locateName = const_cast<TCHAR*>(TEXT("Header"));
//        SetWindowText(GetDlgItem(h_current_dlg_, IDC_EDIT_LOCATE), locateName);
//
//        TCHAR RVABuffer[9] = { 0 };
//        wsprintfW(RVABuffer, L"%08X", FOA);
//        SetWindowText(GetDlgItem(h_current_dlg_, IDC_EDIT_RVA), RVABuffer);
//
//        TCHAR FOABuffer[9] = { 0 };
//        wsprintfW(FOABuffer, L"%08X", FOA);
//        SetWindowText(GetDlgItem(h_current_dlg_, IDC_EDIT_FOA), FOABuffer);
//
//        TCHAR VABuffer[9] = { 0 };
//        wsprintfW(VABuffer, L"%08X", FOA + (DWORD)AnalysePE::GetAnalyse().GetHeaders().optionalHeader->ImageBase);
//        SetWindowText(GetDlgItem(h_current_dlg_, IDC_EDIT_VA), VABuffer);
//        return;
//    }
//    case inSection:
//    {
//        DWORD secIndex = AnalysePE::GetAnalyse().InWhichSectionFOA(FOA);
//        TCHAR* locateName = nullptr;
//        AnalysePE::GetAnalyse().GetTcharSectionName(secIndex, &locateName);
//        SetWindowText(GetDlgItem(h_current_dlg_, IDC_EDIT_LOCATE), locateName);
//
//        DWORD RVA = -1;
//        RVA = AnalysePE::GetAnalyse().FOAToRVA(FOA);
//        TCHAR RVABuffer[9] = { 0 };
//        wsprintfW(RVABuffer, L"%08X", RVA);
//        SetWindowText(GetDlgItem(h_current_dlg_, IDC_EDIT_RVA), RVABuffer);
//
//        TCHAR FOABuffer[9] = { 0 };
//        wsprintfW(FOABuffer, L"%08X", FOA);
//        SetWindowText(GetDlgItem(h_current_dlg_, IDC_EDIT_FOA), FOABuffer);
//
//        TCHAR VABuffer[9] = { 0 };
//        wsprintfW(VABuffer, L"%08X", RVA + (DWORD)AnalysePE::GetAnalyse().GetHeaders().optionalHeader->ImageBase);
//        SetWindowText(GetDlgItem(h_current_dlg_, IDC_EDIT_VA), VABuffer);
//        return;
//    }
//    case outFile:
//    {
//        MessageBox(0, TEXT("Out Of File!"), TEXT("Error"), MB_OK);
//        return;
//    }
//    default:
//        return;
//    }
//}
//
//LRESULT CALLBACK CalculateDlg::calc_proc(HWND h_calc, UINT message, WPARAM w_param, LPARAM l_param) {
//    switch (message)
//    {
//    case WM_INITDIALOG:
//        // calculate窗口子类化
//        RvaOldProc = (WNDPROC)SetWindowLong(GetDlgItem(h_calc, IDC_EDIT_RVA), GWL_WNDPROC, (LONG)RVA_new_proc);
//        VaOldProc = (WNDPROC)SetWindowLong(GetDlgItem(h_calc, IDC_EDIT_VA), GWL_WNDPROC, (LONG)VA_new_proc);
//        FoaOldProc = (WNDPROC)SetWindowLong(GetDlgItem(h_calc, IDC_EDIT_FOA), GWL_WNDPROC, (LONG)FOA_new_proc);
//
//        this_dlg_->set_current_dlg_HWND(h_calc);
//        this_dlg_->init_radio_button();
//        break;
//    case WM_COMMAND:
//    {
//        int wmId = LOWORD(w_param);
//        switch (wmId)
//        {
//        case IDC_RADIO_VA:
//            this_dlg_->on_click_VA();
//            break;
//        case IDC_RADIO_RVA:
//            this_dlg_->on_click_RVA();
//            break;
//        case IDC_RADIO_FOA:
//            this_dlg_->OnClickFOA();
//            break;
//        case IDC_BUTTON_CALC:
//            this_dlg_->OnClickCalc();
//            break;
//        case IDCANCEL:
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
//
///*
//*   实现edit控件的输入限制，hex only
//*/
//LRESULT CALLBACK CalculateDlg::RVA_new_proc(HWND hwnd, UINT message, WPARAM w_param, LPARAM l_param) {
//    TCHAR tCh;
//    switch (message)
//    {
//    case WM_CHAR:
//        tCh = (TCHAR)w_param;
//        if (!is_tchar_hex(tCh))
//            return 0;
//        break;
//    }
//    return CallWindowProc(RvaOldProc, hwnd, message, w_param, l_param);
//}
//
//LRESULT CALLBACK CalculateDlg::VA_new_proc(HWND hwnd, UINT message, WPARAM w_param, LPARAM l_param) {
//    TCHAR tCh;
//    switch (message)
//    {
//    case WM_CHAR:
//        tCh = (TCHAR)w_param;
//        if (!is_tchar_hex(tCh))
//            return 0;
//        break;
//    }
//    return CallWindowProc(VaOldProc, hwnd, message, w_param, l_param);
//}
//
//LRESULT CALLBACK CalculateDlg::FOA_new_proc(HWND hwnd, UINT message, WPARAM w_param, LPARAM l_param) {
//    TCHAR tCh;
//    switch (message)
//    {
//    case WM_CHAR:
//        tCh = (TCHAR)w_param;
//        if (!is_tchar_hex(tCh))
//            return 0;
//        break;
//    }
//    return CallWindowProc(FoaOldProc, hwnd, message, w_param, l_param);
//}
//
