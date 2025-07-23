module;

#include <windows.h>
#include <commctrl.h>
#include <cstdio>
#include "..\GUI\resource.h"

module ImportDlg;

import STL;
import Concept;
import Tool;
import DialogManager;
import AnalysePE;

using std::array;
using std::wstring;
using tools::CharToTchar;
using tools::TcharToDword;

namespace petools {

    void ImportDlg::init_dialog() noexcept {
        init_main_list();
		init_function_list();
    }

    void ImportDlg::show_dialog() noexcept {
		main_list_->plant_column();
		main_list_->plant_item();
		func_list_->plant_column();

		ShowWindow(current_hwnd_, get_cmd_show());
		UpdateWindow(current_hwnd_);
	}

    void ImportDlg::init_main_list() noexcept {
        main_list_ = std::unique_ptr<ListCtrl>(new ListCtrl(GetDlgItem(current_hwnd_, IDC_LIST_IMP_MAIN)
            , [&]() {plant_main_column(); }, [&]() {plant_main_item(); }));
        main_list_->init(LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM | LVCF_FMT, LVIF_TEXT);
    }

    void ImportDlg::plant_main_column() noexcept {
		array<column_definition, 6> items = { {
			{ 120, L"Dll Name" },
			{ 160, L"Original First Thunk" },
			{ 120, L"Time Date Stamp" },
			{ 120, L"Forwarder Chain" },
			{ 80, L"Name" },
			{ 100, L"First Thunk" }
		} };

        main_list_->set_column(items);
    }

    void ImportDlg::plant_main_item() noexcept {
        IMAGE_IMPORT_DESCRIPTOR* import_descriptor = pe_analyse.GetImport();

        LV_ITEM item;
        memset(&item, 0, sizeof(LV_ITEM));

        const auto& header = pe_analyse.GetHeaders();

        for (auto cur : tool::make_ranges(import_descriptor));

        for (int i = 0; import_descriptor->OriginalFirstThunk != 0; i++, import_descriptor++) {
            item.mask = LVIF_TEXT;
            item.iItem = i;
            item.iSubItem = 0;
            char* dllName = (char*)((DWORD)header.dosHeader + pe_analyse.RVAToFOA(import_descriptor->Name));
            TCHAR* tDllName = nullptr;
            CharToTchar(dllName, &tDllName);
            item.pszText = tDllName;
            SendMessage(main_list_->get_list_handle(), LVM_INSERTITEM, 0, (DWORD)&item);

            item.mask = LVIF_TEXT;
            item.iItem = i;
            item.iSubItem = 1;
            TCHAR tOriFirstThunk[9] = { 0 };
            wsprintf(tOriFirstThunk, L"%08X", import_descriptor->OriginalFirstThunk);
            item.pszText = tOriFirstThunk;
            ListView_SetItem(main_list_->get_list_handle(), (DWORD)&item);

            item.iItem = i;
            item.iSubItem = 2;
            TCHAR tTimeStamp[9] = { 0 };
            wsprintf(tTimeStamp, L"%08X", import_descriptor->TimeDateStamp);
            item.pszText = tTimeStamp;
            ListView_SetItem(main_list_->get_list_handle(), (DWORD)&item);

            item.iItem = i;
            item.iSubItem = 3;
            TCHAR tChain[9] = { 0 };
            wsprintf(tChain, L"%08X", import_descriptor->ForwarderChain);
            item.pszText = tChain;
            ListView_SetItem(main_list_->get_list_handle(), (DWORD)&item);

            item.iItem = i;
            item.iSubItem = 4;
            TCHAR tNameOffset[9] = { 0 };
            wsprintf(tNameOffset, L"%08X", import_descriptor->Name);
            item.pszText = tNameOffset;
            ListView_SetItem(main_list_->get_list_handle(), (DWORD)&item);

            item.iItem = i;
            item.iSubItem = 5;
            TCHAR tFirstThunk[9] = { 0 };
            wsprintf(tFirstThunk, L"%08X", import_descriptor->FirstThunk);
            item.pszText = tFirstThunk;
            ListView_SetItem(main_list_->get_list_handle(), (DWORD)&item);
        }
    }

    void ImportDlg::init_function_list() noexcept {
        func_list_ = std::unique_ptr<ListCtrl>(new ListCtrl(GetDlgItem(current_hwnd_, IDC_LIST_IMP_FUNC)
            , [&]() {plant_function_column(); }, [&]() {plant_func_item(); }));
        func_list_->init(LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM | LVCF_FMT, LVIF_TEXT);
    }
    
    void ImportDlg::plant_function_column() noexcept {
		array<column_definition, 5> items = { {
			{ 320, L"API Name" },
			{ 80, L"Thunk RVA" },
			{ 100, L"Thunk Offset" },
			{ 100, L"Thunk Value" },
			{ 80, L"Hint" }
		} };
        
		func_list_->set_column(items);
    }
    
    void ImportDlg::plant_func_item() noexcept {
        DWORD main_list_row = SendMessage(main_list_->get_list_handle(), LVM_GETNEXTITEM, -1, LVNI_SELECTED);
        if (main_list_row == -1) {
            return;
        }
        SendMessage(func_list_->get_list_handle(), LVM_DELETEALLITEMS, 0, 0);
    
        LV_ITEM item;
        memset(&item, 0, sizeof(LV_ITEM));
     
        DWORD first_thunk = get_first_thunk_from_main_list(main_list_row);
    
        // INT与IAT都是双字大小的Union，直接定义为DWORD*方便操作
        DWORD* IAT = (DWORD*)((DWORD)pe_analyse.GetHeaders().dosHeader
            + pe_analyse.RVAToFOA(first_thunk));
    
        for (int j = 0; (*IAT) != 0; j++, IAT++) {
            IMAGE_IMPORT_BY_NAME* import_by_name = (IMAGE_IMPORT_BY_NAME*)((DWORD)pe_analyse.GetHeaders().dosHeader
                + pe_analyse.RVAToFOA(*IAT));
    
            item.mask = LVIF_TEXT;
            item.iItem = j;
            item.iSubItem = 0;
            char* api_name = import_by_name->Name;
            TCHAR* t_api_name = nullptr;
            CharToTchar(api_name, &t_api_name);
            item.pszText = t_api_name;
            SendMessage(func_list_->get_list_handle(), LVM_INSERTITEM, 0, (DWORD)&item);
    
            item.iItem = j;
            item.iSubItem = 1;
            TCHAR t_thunk_rva[9] = { 0 };
            wsprintf(t_thunk_rva, L"%08X", first_thunk + j * 4);
            item.pszText = t_thunk_rva;
            ListView_SetItem(func_list_->get_list_handle(), (DWORD)&item);
    
            item.iItem = j;
            item.iSubItem = 2;
            TCHAR t_thunk_offset[9] = { 0 };
            wsprintf(t_thunk_offset, L"%08X", pe_analyse.RVAToFOA(first_thunk + j * 4));
            item.pszText = t_thunk_offset;
            ListView_SetItem(func_list_->get_list_handle(), (DWORD)&item);
    
            item.iItem = j;
            item.iSubItem = 3;
            TCHAR t_thunk_value[9] = { 0 };
            wsprintf(t_thunk_value, L"%08X", *IAT);
            item.pszText = t_thunk_value;
            ListView_SetItem(func_list_->get_list_handle(), (DWORD)&item);
    
            item.iItem = j;
            item.iSubItem = 4;
            TCHAR t_hint[5] = { 0 };
            wsprintf(t_hint, L"%04X", import_by_name->Hint);
            item.pszText = t_hint;
            ListView_SetItem(func_list_->get_list_handle(), (DWORD)&item);
        }
    }
    
    DWORD ImportDlg::get_first_thunk_from_main_list(DWORD row_id) noexcept {
        TCHAR first_thunk_buffer[9] = { 0 };
        LV_ITEM item;
        memset(&item, 0, sizeof(item));
    
        item.iSubItem = 1;
        item.pszText = first_thunk_buffer;
        item.cchTextMax = 9;
        SendMessage(main_list_->get_list_handle(), LVM_GETITEMTEXT, row_id, (DWORD)&item);
    
    
        DWORD first_thunk = 0;
        TcharToDword(first_thunk_buffer, &first_thunk, 16);
        return first_thunk;
    }

    LRESULT ImportDlg::handle_message(const WindowHandle& h_dlg, UINT message, WPARAM w_param, LPARAM l_param) {
        NMHDR* hdr = (NMHDR*)l_param;
        switch (message)
        {
        case WM_COMMAND:
        {
            int wmId = LOWORD(w_param);

            switch (wmId) {
            case IDOK:
                dialog_mgr.close_dialog();
                break;
            default:
                break;
            }
            break;
        }
        case WM_NOTIFY:
            if (w_param == IDC_LIST_IMP_MAIN && hdr->code == NM_CLICK) {
                plant_func_item();
            }
            break;
        case WM_CLOSE:
            dialog_mgr.close_dialog();
            break;
        default:
            return FALSE;
        }
        return FALSE;
    }

} //namespace petools