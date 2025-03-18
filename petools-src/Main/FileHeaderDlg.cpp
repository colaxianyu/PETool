module;

#include <windows.h>
#include "resource.h"

module FileHeaderDlg;

import DialogManager;
//import TimeStampDlg;

//import AnalysePE;

//using petools::show::SetDlgItemText_t;

//void FileHeaderDlg::SetFileHeaderInfo() {
//    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_MACHINE, AnalysePE::GetAnalyse().GetHeaders().fileHeader->Machine, 4);
//    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_SECTIONS, AnalysePE::GetAnalyse().GetHeaders().fileHeader->NumberOfSections, 4);
//    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_TIMEDATA, AnalysePE::GetAnalyse().GetHeaders().fileHeader->TimeDateStamp, 8);
//    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_POINTER, AnalysePE::GetAnalyse().GetHeaders().fileHeader->PointerToSymbolTable, 4);
//    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_COFFTABLE, AnalysePE::GetAnalyse().GetHeaders().fileHeader->NumberOfSymbols, 4);
//    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_OPTSIZE, AnalysePE::GetAnalyse().GetHeaders().fileHeader->SizeOfOptionalHeader, 4);
//    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_CHA, AnalysePE::GetAnalyse().GetHeaders().fileHeader->Characteristics, 4);
//}

namespace petools {

    LRESULT FileHeaderDlg::handle_message(const WindowHandle& h_dlg, UINT message, WPARAM w_param, LPARAM l_param) {
        switch (message)
        {
        case WM_COMMAND:
        {
            int wmId = LOWORD(w_param);
            switch (wmId)
            {
            case IDC_BUTTON_DETAILDATA:
				//dialog_mgr.open_dialog<TimeStampDlg>(current_hwnd_.borrow());
                break;
            case IDOK:
                dialog_mgr.close_dialog();
                break;
            }
            break;
        }
        case WM_CLOSE:
            dialog_mgr.close_dialog();
            break;
        default:
            return FALSE;
        }
        return FALSE;
    }

} //namespace petools