module;

#include <windows.h>
#include <CommCtrl.h>
#include "resources/resource.h"

module SecHeaderDlg;

import DialogManager;
import AnalysePE;

using std::array;
using std::wstring;

namespace petools::gui {

	void SecHeaderDlg::InitDialog() noexcept {
		init_section_list();
	}

    void SecHeaderDlg::ShowDialog() noexcept {
        plant_section_column();
        plant_section_item();
        ShowWindow(current_hwnd_, GetCmdShow());
        UpdateWindow(current_hwnd_);
    }

    void SecHeaderDlg::init_section_list() noexcept {
        section_list_ = std::unique_ptr<ListCtrl>(new ListCtrl(GetDlgItem(current_hwnd_, IDC_LIST_SECTION)));
        section_list_->Init(LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM | LVCF_FMT, LVIF_TEXT);
    }
    
    void SecHeaderDlg::plant_section_column() noexcept {
        array<ColumnDefinition, 7> items = { {
    		{ 30, L"#" },
    		{ 80, L"Name" },
    		{ 110, L"Virtual Size"},
    		{ 120, L"Virtual Offset"},
    		{ 80, L"Raw Size"},
            { 90, L"Raw Offset" },
    		{ 120, L"Characteristics"}
    	} };
    
        section_list_->SetColumn(items);
    }
    
    void SecHeaderDlg::plant_section_item() noexcept {
        LV_ITEM item;
        memset(&item, 0, sizeof(LV_ITEM));
        item.mask = LVIF_TEXT;
    
        IMAGE_SECTION_HEADER* sec_header = PeAnalyse().GetHeaders().sectionHeader;

        DWORD index = 0;
        auto SetSectionHexText = [&](HWND hDlg, DWORD row, DWORD value = 0) {
            TCHAR text[32] = { 0 };
            item.iItem = row;
            item.iSubItem = index;
    
            if (index == 0) {
                wsprintf(text, L"%d", row + 1);
                item.pszText = text;
                SendMessage(hDlg, LVM_INSERTITEM, 0, (DWORD)&item);
            }
            else if (index == 1) {
                TCHAR* nameBuffer = nullptr;
                PeAnalyse().GetTcharSectionName(row, &nameBuffer);
                item.pszText = nameBuffer;
                ListView_SetItem(hDlg, &item);
            }
            else {
                wsprintfW(text, L"%08X", value); 
                item.pszText = text;
                ListView_SetItem(hDlg, &item);
            }
            
            index++;
    	};
    
        for (int row = 0; sec_header->Name != 0 && sec_header->Characteristics != 0; row++, sec_header++) {
            SetSectionHexText(section_list_->GetListHandle(), row);
            SetSectionHexText(section_list_->GetListHandle(), row);
            SetSectionHexText(section_list_->GetListHandle(), row, sec_header->Misc.VirtualSize);
            SetSectionHexText(section_list_->GetListHandle(), row, sec_header->VirtualAddress);
            SetSectionHexText(section_list_->GetListHandle(), row, sec_header->SizeOfRawData);
            SetSectionHexText(section_list_->GetListHandle(), row, sec_header->PointerToRawData);
            SetSectionHexText(section_list_->GetListHandle(), row, sec_header->Characteristics);
            index = 0;
        }
    }

    LRESULT SecHeaderDlg::HandleMessage(const WindowHandle& h_dlg, UINT message, WPARAM w_param, LPARAM l_param) {
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