//module;
//
//#include "resource.h"
//#include <windows.h>
//#include <CommCtrl.h>
//
//module SecHeaderDlg;
//
//import AnalysePE;
//
//using std::array;
//using std::wstring;
//using petools::show::ItemWidthAndName;
//
//extern HINSTANCE app_inst;
//extern int cmd_show;
//
//SecHeaderDlg* SecHeaderDlg::this_dlg_ = nullptr;
//
//SecHeaderDlg::SecHeaderDlg(HWND h_parent) noexcept(true)
//	: DialogEX(IDD_DIALOG_SECTIONS, h_parent)
//{
//
//}
//
//SecHeaderDlg::~SecHeaderDlg() {
//
//}
//
//void SecHeaderDlg::init_dlg() {
//	set_this_dlg();
//}
//
//void SecHeaderDlg::close_dlg() {
//	section_list_.reset();
//	EndDialog(h_current_dlg_, 0);
//}
//
//void SecHeaderDlg::plant() {
//	DialogBox(app_inst, MAKEINTRESOURCE(id_template_), h_parent_dlg_, (DLGPROC)section_proc);
//}
//
//void SecHeaderDlg::init_section_list() {
//    section_list_ = std::unique_ptr<ListCtrl>(new ListCtrl(GetDlgItem(h_current_dlg_, IDC_LIST_SECTION),
//        [&]() {this_dlg_->plant_section_column(); }, [&]() {this_dlg_->plant_section_item(); }));
//    section_list_->init(LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM | LVCF_FMT, LVIF_TEXT);
//    section_list_->plant_column();
//    section_list_->plant_item();
//}
//
//void SecHeaderDlg::plant_section_column() {
//    array<ItemWidthAndName<DWORD, wstring>, 7> items = { {
//		{ 40, L"#" },
//		{ 70, L"Name" },
//		{ 110, L"Virtual Size"},
//		{ 120, L"Virtual Offset"},
//		{ 80, L"Raw Size"},
//        { 90, L"Raw Offset" },
//		{ 120, L"Characteristics"}
//	} };
//
//    for (int i = 0; i < items.size(); i++) {
//        section_list_->SetColumn(items[i], i);
//        SendMessage(section_list_->get_list_handle(), LVM_INSERTCOLUMN, i, section_list_->get_column_addr());
//    }
//}
//
//void SecHeaderDlg::plant_section_item() {
//    LV_ITEM item;
//    memset(&item, 0, sizeof(LV_ITEM));
//    item.mask = LVIF_TEXT;
//
//    IMAGE_SECTION_HEADER* tempSecHeader = AnalysePE::GetAnalyse().GetHeaders().sectionHeader;
//
//    DWORD index = 0;
//    auto SetSectionHexText = [&](HWND hDlg, DWORD row, DWORD value = 0) {
//        TCHAR text[32] = { 0 };
//        item.iItem = row;
//        item.iSubItem = index;
//
//        if (index == 0) {
//            wsprintf(text, L"%d", row + 1);
//            item.pszText = text;
//            SendMessage(hDlg, LVM_INSERTITEM, 0, (DWORD)&item);
//        }
//        else if (index == 1) {
//            TCHAR* nameBuffer = nullptr;
//            AnalysePE::GetAnalyse().GetTcharSectionName(row, &nameBuffer);
//            item.pszText = nameBuffer;
//            ListView_SetItem(hDlg, &item);
//        }
//        else {
//            wsprintfW(text, L"%08X", value); 
//            item.pszText = text;
//            ListView_SetItem(hDlg, &item);
//        }
//        
//        index++;
//	};
//
//    for (int row = 0; tempSecHeader->Name != 0 && tempSecHeader->Characteristics != 0; row++, tempSecHeader++) {
//        SetSectionHexText(section_list_->get_list_handle(), row);
//        SetSectionHexText(section_list_->get_list_handle(), row);
//        SetSectionHexText(section_list_->get_list_handle(), row, tempSecHeader->Misc.VirtualSize);
//        SetSectionHexText(section_list_->get_list_handle(), row, tempSecHeader->VirtualAddress);
//        SetSectionHexText(section_list_->get_list_handle(), row, tempSecHeader->SizeOfRawData);
//        SetSectionHexText(section_list_->get_list_handle(), row, tempSecHeader->PointerToRawData);
//        SetSectionHexText(section_list_->get_list_handle(), row, tempSecHeader->Characteristics);
//        index = 0;
//    }
//}
//
//LRESULT CALLBACK SecHeaderDlg::section_proc(HWND h_sec, UINT message, WPARAM w_param, LPARAM l_param) {
//    switch (message)
//    {
//    case WM_INITDIALOG:
//        this_dlg_->set_current_dlg_HWND(h_sec);
//        this_dlg_->init_section_list();
//        break;
//    case WM_COMMAND:
//    {
//        int wmId = LOWORD(w_param);
//        switch (wmId)
//        {
//        case IDOK:
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