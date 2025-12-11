module;

#include <windows.h>
#include <string>
#include "resources/resource.h"

module PeEditDlg;

import FileHeaderDlg;
import OptHeaderDlg;
import SecHeaderDlg;
import DirectoryDlg;
import DialogManager;
import AnalysePE;
import Tool;

using std::wstring;

using petools::tool::string_to_wstring;
using tools::config::filename_max;

namespace petools {

    void PeEditDlg::init_dialog() noexcept {
        set_dialog_title();
        set_main_info();
    }

    void PeEditDlg::set_dialog_title() noexcept {
  //      wstring file_path = string_to_wstring(file_manager_.get_file_path());
		//wstring title = title_prefix_ + file_path;
  //      SetWindowText(current_hwnd_, title.c_str());

        wstring file_path = string_to_wstring(file_manager_.get_file_path());

        // 🔁 改写这里：
        wstring title = title_prefix_;  // 拷贝前缀
        title += file_path;             // 再拼接文件名

        SetWindowText(current_hwnd_, title.c_str());
    }

    void PeEditDlg::set_main_info() noexcept {
  //      const auto& headers = pe_analyse.GetHeaders();

  //      struct pe_field_mapping {
  //          DWORD template_id;
  //          DWORD value;
		//	DWORD show_length = 8;
  //      };

		//const pe_field_mapping pe_fields[] = {
		//	{ IDC_EDIT_ENTRY, headers.optionalHeader->AddressOfEntryPoint },
		//	{ IDC_EDIT_IMAGEBASE, headers.optionalHeader->ImageBase },
		//	{ IDC_EDIT_IMAGESIZE, headers.optionalHeader->SizeOfImage },
		//	{ IDC_EDIT_CODEBASE, headers.optionalHeader->BaseOfCode },
		//	{ IDC_EDIT_DATABASE, headers.optionalHeader->BaseOfData },
		//	{ IDC_EDIT_SECTIONALIGN, headers.optionalHeader->SectionAlignment },
		//	{ IDC_EDIT_FILEALIGN, headers.optionalHeader->FileAlignment },
		//	{ IDC_EDIT_HEADERSIZE, headers.optionalHeader->SizeOfHeaders },
		//	{ IDC_EDIT_OPTHEADERSIZE, headers.fileHeader->SizeOfOptionalHeader },
		//	{ IDC_EDIT_SECTIONSIZE, headers.fileHeader->NumberOfSections },
		//	{ IDC_EDIT_CHARACT, headers.fileHeader->Characteristics },
		//	{ IDC_EDIT_TIMESTAMP, headers.fileHeader->TimeDateStamp },
		//	{ IDC_EDIT_SUBSYS, headers.optionalHeader->Subsystem },
		//	{ IDC_EDIT_DIRSIZE, headers.optionalHeader->NumberOfRvaAndSizes }
		//};

		//for (const auto& field : pe_fields) {
		//	tool::set_dlg_item_text(current_hwnd_, field.template_id, field.value, field.show_length);
		//}
    }

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
    //    NMHDR* phdr = (NMHDR*)l_param;
    //    switch (message)
    //    {
    //    case WM_COMMAND:
    //    {
    //        int wmId = LOWORD(w_param);

    //        switch (wmId)
    //        {
    //        case IDC_BUTTON_FILEHEADER:
				//dialog_mgr.open_dialog<FileHeaderDlg>(current_hwnd_.borrow());
    //            break;
    //        case IDC_BUTTON_OPTHEADER:
    //            dialog_mgr.open_dialog<OptHeaderDlg>(current_hwnd_.borrow());
    //            break;
    //        case IDC_BUTTON_SECTIONS:
    //            dialog_mgr.open_dialog<SecHeaderDlg>(current_hwnd_.borrow());
    //            break;
    //        case IDC_BUTTON_DIRECTORIES:
				//dialog_mgr.open_dialog<DirectoryDlg>(current_hwnd_.borrow());
    //            break;
    //        case IDC_BUTTON_CALCFOA:
    //            //dialog_mgr.open_dialog<TimeStampDlg>(current_hwnd_.borrow());
    //            break;
    //        case IDC_BUTTON_INJECT:
    //            //this_dlg_->CreateInjectImportDlg();
    //            break;
    //        case IDC_BUTTON_SAVE:
    //            //this_dlg_->SaveAsFile();
    //            break;
    //        case IDOK:
    //            dialog_mgr.close_dialog();
    //            break;
    //        }
    //        break;
    //    }
    //    case WM_CLOSE:
    //        dialog_mgr.close_dialog();
    //        break;
    //    default:
    //        return FALSE;
    //    }
    //    return FALSE;
        return FALSE;
    }

} //namespace petools