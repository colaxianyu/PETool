//module;
//
//#include <windows.h>
//#include "resource.h"
//
//module SecHeaderDlg;
//
//import DialogManager;
//
////import AnalysePE;
//
////using std::array;
////using std::wstring;
////using petools::show::ItemWidthAndName;
//
//namespace petools {
//
//    //void SecHeaderDlg::init_section_list() {
//    //    section_list_ = std::unique_ptr<ListCtrl>(new ListCtrl(GetDlgItem(h_current_dlg_, IDC_LIST_SECTION),
//    //        [&]() {this_dlg_->plant_section_column(); }, [&]() {this_dlg_->plant_section_item(); }));
//    //    section_list_->init(LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM | LVCF_FMT, LVIF_TEXT);
//    //    section_list_->plant_column();
//    //    section_list_->plant_item();
//    //}
//    //
//    //void SecHeaderDlg::plant_section_column() {
//    //    array<ItemWidthAndName<DWORD, wstring>, 7> items = { {
//    //		{ 40, L"#" },
//    //		{ 70, L"Name" },
//    //		{ 110, L"Virtual Size"},
//    //		{ 120, L"Virtual Offset"},
//    //		{ 80, L"Raw Size"},
//    //        { 90, L"Raw Offset" },
//    //		{ 120, L"Characteristics"}
//    //	} };
//    //
//    //    for (int i = 0; i < items.size(); i++) {
//    //        section_list_->SetColumn(items[i], i);
//    //        SendMessage(section_list_->get_list_handle(), LVM_INSERTCOLUMN, i, section_list_->get_column_addr());
//    //    }
//    //}
//    //
//    //void SecHeaderDlg::plant_section_item() {
//    //    LV_ITEM item;
//    //    memset(&item, 0, sizeof(LV_ITEM));
//    //    item.mask = LVIF_TEXT;
//    //
//    //    IMAGE_SECTION_HEADER* tempSecHeader = AnalysePE::GetAnalyse().GetHeaders().sectionHeader;
//    //
//    //    DWORD index = 0;
//    //    auto SetSectionHexText = [&](HWND hDlg, DWORD row, DWORD value = 0) {
//    //        TCHAR text[32] = { 0 };
//    //        item.iItem = row;
//    //        item.iSubItem = index;
//    //
//    //        if (index == 0) {
//    //            wsprintf(text, L"%d", row + 1);
//    //            item.pszText = text;
//    //            SendMessage(hDlg, LVM_INSERTITEM, 0, (DWORD)&item);
//    //        }
//    //        else if (index == 1) {
//    //            TCHAR* nameBuffer = nullptr;
//    //            AnalysePE::GetAnalyse().GetTcharSectionName(row, &nameBuffer);
//    //            item.pszText = nameBuffer;
//    //            ListView_SetItem(hDlg, &item);
//    //        }
//    //        else {
//    //            wsprintfW(text, L"%08X", value); 
//    //            item.pszText = text;
//    //            ListView_SetItem(hDlg, &item);
//    //        }
//    //        
//    //        index++;
//    //	};
//    //
//    //    for (int row = 0; tempSecHeader->Name != 0 && tempSecHeader->Characteristics != 0; row++, tempSecHeader++) {
//    //        SetSectionHexText(section_list_->get_list_handle(), row);
//    //        SetSectionHexText(section_list_->get_list_handle(), row);
//    //        SetSectionHexText(section_list_->get_list_handle(), row, tempSecHeader->Misc.VirtualSize);
//    //        SetSectionHexText(section_list_->get_list_handle(), row, tempSecHeader->VirtualAddress);
//    //        SetSectionHexText(section_list_->get_list_handle(), row, tempSecHeader->SizeOfRawData);
//    //        SetSectionHexText(section_list_->get_list_handle(), row, tempSecHeader->PointerToRawData);
//    //        SetSectionHexText(section_list_->get_list_handle(), row, tempSecHeader->Characteristics);
//    //        index = 0;
//    //    }
//    //}
//
//    LRESULT SecHeaderDlg::handle_message(const WindowHandle& h_dlg, UINT message, WPARAM w_param, LPARAM l_param) {
//        switch (message)
//        {
//        case WM_COMMAND:
//        {
//            int wmId = LOWORD(w_param);
//            switch (wmId)
//            {
//            case IDOK:
//                dialog_mgr.close_dialog();
//                break;
//            }
//            break;
//        }
//        case WM_CLOSE:
//            dialog_mgr.close_dialog();
//            break;
//        default:
//            return FALSE;
//        }
//        return FALSE;
//    }
//
//} //namespace petools