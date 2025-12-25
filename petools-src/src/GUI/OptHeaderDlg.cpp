module;

#include <windows.h>
#include "resources/resource.h"

module OptHeaderDlg;

import DialogManager;
import AnalysePE;
import Utility;


//void OptHeaderDlg::SetOptHeaderInfo() {
//    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_OPTMAGIC, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->Magic, 4);
//    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_OPTMAJORLINK, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->MajorLinkerVersion, 2);
//    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_OPTMINORLINK, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->MinorLinkerVersion, 2);
//    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_OPTCODESIZE, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->SizeOfCode, 8);
//    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_OPTINITDATA, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->SizeOfInitializedData, 8);
//    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_OPTUNINITDATA, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->SizeOfUninitializedData, 8);
//    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_OPTENTRY, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->AddressOfEntryPoint, 8);
//    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_OPTCODEBASE, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->BaseOfCode, 8);
//    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_OPTDATABASE, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->BaseOfData, 8);
//    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_OPTIMAGEBASE, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->ImageBase, 8);
//    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_OPTSECALIGN, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->SectionAlignment, 8);
//    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_OPTFILEALIGN, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->FileAlignment, 8);
//    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_OPTMAJOROS, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->MajorOperatingSystemVersion, 4);
//    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_OPTMINOROS, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->MinorOperatingSystemVersion, 4);
//    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_OPTMAJORIMAGE, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->MajorImageVersion, 4);
//    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_OPTMINORIMAGE, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->MinorImageVersion, 4);
//    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_OPTMAJORSUBSYS, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->MajorSubsystemVersion, 4);
//    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_OPTMINORSUBSYS, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->MinorSubsystemVersion, 4);
//    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_OPTWINVERSION, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->Win32VersionValue, 8);
//    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_OPTIMAGESIZE, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->SizeOfImage, 8);
//    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_OPTHEADERSSIZE, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->SizeOfHeaders, 8);
//    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_OPTCHECK, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->CheckSum, 8);
//    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_OPTSUBSYS, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->Subsystem, 4);
//    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_OPTDLLCHA, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->DllCharacteristics, 4);
//    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_OPTSTACKRES, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->SizeOfStackReserve, 8);
//    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_OPTSTACKCOM, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->SizeOfStackCommit, 8);
//    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_OPTHEAPRES, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->SizeOfHeapReserve, 8);
//    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_OPTHEAPCOM, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->SizeOfHeapCommit, 8);
//    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_OPTFLAGS, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->LoaderFlags, 8);
//    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_OPTRVAANDSIZE, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->NumberOfRvaAndSizes, 8);
//}

namespace petools::gui {
    void OptHeaderDlg::InitDialog() noexcept {
        set_optional_header_info();
    }

    void OptHeaderDlg::set_optional_header_info() noexcept {
        const auto& header = PeAnalyse().GetHeaders();

        struct pe_field_mapping {
            DWORD template_id;
            DWORD value;
            DWORD show_length = 8;
        };

        const pe_field_mapping pe_fields[] = {
            { IDC_EDIT_OPTMAGIC, header.optionalHeader->Magic, 4 },
            { IDC_EDIT_OPTMAJORLINK, header.optionalHeader->MajorLinkerVersion, 2 },
            { IDC_EDIT_OPTMINORLINK, header.optionalHeader->MinorLinkerVersion, 2 },
            { IDC_EDIT_OPTCODESIZE, header.optionalHeader->SizeOfCode },
            { IDC_EDIT_OPTINITDATA, header.optionalHeader->SizeOfInitializedData },
            { IDC_EDIT_OPTUNINITDATA, header.optionalHeader->SizeOfUninitializedData },
            { IDC_EDIT_OPTENTRY, header.optionalHeader->AddressOfEntryPoint },
            { IDC_EDIT_OPTCODEBASE, header.optionalHeader->BaseOfCode },
            { IDC_EDIT_OPTDATABASE, header.optionalHeader->BaseOfData },
            { IDC_EDIT_OPTIMAGEBASE, header.optionalHeader->ImageBase },
            { IDC_EDIT_OPTSECALIGN, header.optionalHeader->SectionAlignment },
            { IDC_EDIT_OPTFILEALIGN, header.optionalHeader->FileAlignment },
            { IDC_EDIT_OPTMAJOROS, header.optionalHeader->MajorOperatingSystemVersion, 4 },
            { IDC_EDIT_OPTMINOROS, header.optionalHeader->MinorOperatingSystemVersion, 4 },
            { IDC_EDIT_OPTMAJORIMAGE, header.optionalHeader->MajorImageVersion, 4 },
            { IDC_EDIT_OPTMINORIMAGE, header.optionalHeader->MinorImageVersion, 4 },
            { IDC_EDIT_OPTMAJORSUBSYS, header.optionalHeader->MajorSubsystemVersion, 4 },
            { IDC_EDIT_OPTMINORSUBSYS, header.optionalHeader->MinorSubsystemVersion, 4 },
            { IDC_EDIT_OPTWINVERSION, header.optionalHeader->Win32VersionValue },
            { IDC_EDIT_OPTIMAGESIZE, header.optionalHeader->SizeOfImage },
            { IDC_EDIT_OPTHEADERSSIZE, header.optionalHeader->SizeOfHeaders },
            { IDC_EDIT_OPTCHECK, header.optionalHeader->CheckSum },
            { IDC_EDIT_OPTSUBSYS, header.optionalHeader->Subsystem, 4 },
            { IDC_EDIT_OPTDLLCHA, header.optionalHeader->DllCharacteristics, 4 },
            { IDC_EDIT_OPTSTACKRES, header.optionalHeader->SizeOfStackReserve },
            { IDC_EDIT_OPTSTACKCOM, header.optionalHeader->SizeOfStackCommit },
            { IDC_EDIT_OPTHEAPRES, header.optionalHeader->SizeOfHeapReserve },
            { IDC_EDIT_OPTHEAPCOM, header.optionalHeader->SizeOfHeapCommit },
            { IDC_EDIT_OPTFLAGS, header.optionalHeader->LoaderFlags },
            { IDC_EDIT_OPTRVAANDSIZE, header.optionalHeader->NumberOfRvaAndSizes }
        };

        for (const auto& field : pe_fields) {
            petools::utility::set_dlg_item_text(current_hwnd_, field.template_id, field.value, field.show_length);
        }
    }

    LRESULT OptHeaderDlg::HandleMessage(const WindowHandle& h_dlg, UINT message, WPARAM w_param, LPARAM l_param) {
        switch (message)
        {
        case WM_COMMAND:
        {
            int wmId = LOWORD(w_param);
            switch (wmId)
            {
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