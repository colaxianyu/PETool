module;

#include <windows.h>
#include "resources/resource.h"

module FileHeaderDlg;

import DialogManager;
import TimeStampDlg;
import AnalysePE;
import Utility;

//import AnalysePE;

namespace petools::gui {

    void FileHeaderDlg::InitDialog() noexcept {
        set_file_header_info();
    }

    void FileHeaderDlg::set_file_header_info() noexcept {
		const auto& headers = PeAnalyse().GetHeaders();

        struct pe_field_mapping {
            DWORD template_id;
            DWORD value;
            DWORD show_length = 8;
        };

        const pe_field_mapping pe_fields[] = {
            { IDC_EDIT_MACHINE, headers.fileHeader->Machine, 4 },
            { IDC_EDIT_SECTIONS, headers.fileHeader->NumberOfSections, 4 },
            { IDC_EDIT_TIMEDATA, headers.fileHeader->TimeDateStamp },
            { IDC_EDIT_POINTER, headers.fileHeader->PointerToSymbolTable, 4 },
            { IDC_EDIT_COFFTABLE, headers.fileHeader->NumberOfSymbols, 4 },
            { IDC_EDIT_OPTSIZE, headers.fileHeader->SizeOfOptionalHeader, 4 },
            { IDC_EDIT_CHA, headers.fileHeader->Characteristics, 4 }
        };

        for (const auto& field : pe_fields) {
            petools::utility::set_dlg_item_text(current_hwnd_, field.template_id, field.value, field.show_length);
        }
    }

    LRESULT FileHeaderDlg::HandleMessage(const WindowHandle& h_dlg, UINT message, WPARAM w_param, LPARAM l_param) {
        switch (message)
        {
        case WM_COMMAND:
        {
            int wmId = LOWORD(w_param);
            switch (wmId)
            {
            case IDC_BUTTON_DETAILDATA:
                DialogMgr().OpenDialog<TimeStampDlg>(current_hwnd_.borrow());
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