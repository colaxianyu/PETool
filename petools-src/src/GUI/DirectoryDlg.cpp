module;

#include <windows.h>
#include "resources/resource.h"

module DirectoryDlg;

import STL;
import DialogManager;
import ExportDlg;
import ImportDlg;
import RelocationDlg;
import AnalysePE;

//using std::wstring;
using std::array;

namespace petools::gui {

    void DirectoryDlg::InitDialog() noexcept {
        set_directory_info();
    }

    void DirectoryDlg::set_directory_info() noexcept {
		IMAGE_DATA_DIRECTORY* data_directory = PeAnalyse().GetHeaders().optionalHeader->DataDirectory;

        struct DirectoryInfo {
            DWORD rva_id_;
            DWORD size_id_;
            DWORD directory_index_;
        };
    
        array<DirectoryInfo, 16> dir_info = { {
            {IDC_EDIT_EXPORTRAV, IDC_EDIT_EXPORTSIZE, 0},
            {IDC_EDIT_IMPORTRAV, IDC_EDIT_IMPORTSIZE, 1},
            {IDC_EDIT_RESOURCERVA, IDC_EDIT_RESOURCESIZE, 2},
            {IDC_EDIT_EXCEPTIONRVA, IDC_EDIT_EXCEPTIONSIZE, 3},
            {IDC_EDIT_SECURITYRVA, IDC_EDIT_SECURITYSIZE, 4},
            {IDC_EDIT_RELOCATIONRVA, IDC_EDIT_RELOCATIONSIZE, 5},
            {IDC_EDIT_DEBUGRVA, IDC_EDIT_DEBUGSIZE, 6},
            {IDC_EDIT_ASDRVA, IDC_EDIT_ASDSIZE, 7},
            {IDC_EDIT_RVAGPRVA, IDC_EDIT_RVAGPSIZE, 8},
            {IDC_EDIT_TLSRVA, IDC_EDIT_TLSSIZE, 9},
            {IDC_EDIT_LOADRVA, IDC_EDIT_LOADSIZE, 10},
            {IDC_EDIT_BOUNDRVA, IDC_EDIT_BOUNDSIZE, 11},
            {IDC_EDIT_IATRVA, IDC_EDIT_IATSIZE, 12},
            {IDC_EDIT_DELAYRVA, IDC_EDIT_DELAYSIZE, 13},
            {IDC_EDIT_COMRVA, IDC_EDIT_COMSIZE, 14},
            {IDC_EDIT_RESERVEDRVA, IDC_EDIT_RESERVEDSIZE, 15}
        } };
    
        for (const auto& info : dir_info) {
            TCHAR rva[9] = { 0 };
            TCHAR size[9] = { 0 };
            wsprintfW(rva, L"%08X", data_directory[info.directory_index_].VirtualAddress);
            wsprintfW(size, L"%08X", data_directory[info.directory_index_].Size);
            SetDlgItemText(current_hwnd_, info.rva_id_, rva);
            SetDlgItemText(current_hwnd_, info.size_id_, size);
        }
    }

    LRESULT DirectoryDlg::HandleMessage(const WindowHandle& h_dlg, UINT message, WPARAM w_param, LPARAM l_param) {
        switch (message)
        {
        case WM_COMMAND:
        {
            int wmId = LOWORD(w_param);
            switch (wmId)
            {
            case IDC_BUTTON_EXPORT:
				DialogMgr().OpenDialog<ExportDlg>(current_hwnd_.borrow());
                break;
            case IDC_BUTTON_IMPORT:
                DialogMgr().OpenDialog<ImportDlg>(current_hwnd_.borrow());
                break;
            case IDC_BUTTON_RELOCATION:
                DialogMgr().OpenDialog<RelocationDlg>(current_hwnd_.borrow());
                break;
            case IDOK:
                DialogMgr().CloseDialog();
                break;
            }
            break;
        }
        case WM_CLOSE:
            DialogMgr().CloseDialog();
            break;
        default:
            return FALSE;
        }
        return FALSE;
    }

} //namespace petools