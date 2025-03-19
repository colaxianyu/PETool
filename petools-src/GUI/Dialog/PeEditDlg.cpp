module;

#include <windows.h>
#include <string>
#include "..\GUI\resource.h"

module PeEditDlg;

import FileHeaderDlg;
import OptHeaderDlg;
import SecHeaderDlg;
import DirectoryDlg;
import DialogManager;

//import FileManage;
//import AnalysePE;

using std::wstring;
using tools::show::SetDlgItemText_t;
using tools::config::filename_max;

namespace petools {

    void PeEditDlg::init_dialog() {
        set_dialog_title();
    }

    void PeEditDlg::set_dialog_title() {
        wstring file_path = string_to_wstring(file_manager_.get_file_path());
		wstring title = title_prefix_ + file_path;
        SetWindowText(current_hwnd_, title.c_str());
    }

    //void PeEditDlg::SetPEMainInfo() {
    //    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_ENTRY, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->AddressOfEntryPoint, 8);
    //    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_IMAGEBASE, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->ImageBase, 8);
    //    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_IMAGESIZE, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->SizeOfImage, 8);
    //    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_CODEBASE, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->BaseOfCode, 8);
    //    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_DATABASE, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->BaseOfData, 8);
    //    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_SECTIONALIGN, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->SectionAlignment, 8);
    //    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_FILEALIGN, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->FileAlignment, 8);
    //    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_HEADERSIZE, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->SizeOfHeaders, 8);
    //    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_OPTHEADERSIZE, AnalysePE::GetAnalyse().GetHeaders().fileHeader->SizeOfOptionalHeader, 8);
    //    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_SECTIONSIZE, AnalysePE::GetAnalyse().GetHeaders().fileHeader->NumberOfSections, 8);
    //    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_CHARACT, AnalysePE::GetAnalyse().GetHeaders().fileHeader->Characteristics, 8);
    //    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_TIMESTAMP, AnalysePE::GetAnalyse().GetHeaders().fileHeader->TimeDateStamp, 8);
    //    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_SUBSYS, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->Subsystem, 8);
    //    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_DIRSIZE, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->NumberOfRvaAndSizes, 8);
    //}

    //void PeEditDlg::SaveAsFile() {
    //    OPENFILENAME openFile = {0};
    //
    //    TCHAR* original_path = nullptr;
    //    CharToTchar(fileManage_->GetFilePath(), &original_path);
    //
    //    TCHAR new_path[MAX_PATH] = { 0 };
    //    if (original_path != nullptr) {
    //        TCHAR* dot_pos = wcsrchr(original_path, '.');
    //        if (dot_pos != nullptr) {
    //            size_t insert_pos = dot_pos - original_path;
    //            wcsncpy_s(new_path, original_path, insert_pos);
    //            wcscat_s(new_path, MAX_PATH, TEXT("(2)"));
    //            wcscat_s(new_path, MAX_PATH, dot_pos);
    //        }
    //        else {
    //            wcsncpy_s(new_path, original_path, MAX_PATH - 4);
    //            wcscat_s(new_path, MAX_PATH, TEXT("(2)"));
    //        }
    //    }
    //
    //    openFile.lStructSize = sizeof(OPENFILENAME);
    //    openFile.Flags = OFN_HIDEREADONLY | OFN_PATHMUSTEXIST;
    //    openFile.FlagsEx = OFN_EX_NOPLACESBAR;
    //    openFile.hwndOwner = h_parent_dlg_;
    //    openFile.lpstrFilter = TEXT("*.exe\0\0*.dll\0");
    //    openFile.lpstrFile = new_path;
    //    openFile.nMaxFile = MAX_PATH;
    //    GetSaveFileName(&openFile);
    //
    //    char* path = nullptr;
    //    TcharToChar(new_path, &path);
    //    fileManage_->SaveAsFile(AnalysePE::GetAnalyse().GetFileBuffer(), AnalysePE::GetAnalyse().GetFileBufferSzie(), path);
    //
    //}

    LRESULT PeEditDlg::handle_message(const WindowHandle& h_dlg, UINT message, WPARAM w_param, LPARAM l_param) {
        NMHDR* phdr = (NMHDR*)l_param;
        switch (message)
        {
        case WM_COMMAND:
        {
            int wmId = LOWORD(w_param);

            switch (wmId)
            {
            case IDC_BUTTON_FILEHEADER:
				dialog_mgr.open_dialog<FileHeaderDlg>(current_hwnd_.borrow());
                break;
            case IDC_BUTTON_OPTHEADER:
                dialog_mgr.open_dialog<OptHeaderDlg>(current_hwnd_.borrow());
                break;
            case IDC_BUTTON_SECTIONS:
                dialog_mgr.open_dialog<SecHeaderDlg>(current_hwnd_.borrow());
                break;
            case IDC_BUTTON_DIRECTORIES:
				dialog_mgr.open_dialog<DirectoryDlg>(current_hwnd_.borrow());
                break;
            case IDC_BUTTON_CALCFOA:
                //dialog_mgr.open_dialog<TimeStampDlg>(current_hwnd_.borrow());
                break;
            case IDC_BUTTON_INJECT:
                //this_dlg_->CreateInjectImportDlg();
                break;
            case IDC_BUTTON_SAVE:
                //this_dlg_->SaveAsFile();
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