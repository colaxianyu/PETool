module;

#include <windows.h>
#include <commctrl.h>
#include <cstdio>
#include "resources/resource.h"

module RelocationDlg;

import STL;
import DialogManager;
import AnalysePE;
import Utility;

using std::array;
using tools::CharToTchar;

//void RelocationDlg::init_dlg() {
//    set_this_dlg();
//}


//void RelocationDlg::InitBlockList() {
//    blockList_ = std::unique_ptr<ListCtrl>(new ListCtrl(GetDlgItem(h_current_dlg_, IDC_LIST_RELO_BLOCK)
//        , [&]() {plantBlockColumn(); }, [&]() {plantBlockItem(); }));
//    blockList_->init(LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM | LVCF_FMT, LVIF_TEXT);
//    blockList_->plant_column();
//    blockList_->plant_item();
//}
//
//void RelocationDlg::plantBlockColumn() {
//    array<ItemWidthAndName<DWORD, wstring>, 5> items = { {
//		{ 50, L"Index" },
//		{ 100, L"Section" },
//		{ 80, L"RVA" },
//		{ 110, L"Size Of Block" },
//		{ 120, L"Items(HEX/DEC)" }
//	} };
//
//    for (size_t i = 0; i < items.size(); i++) {
//        blockList_->SetColumn(items[i], i);
//        SendMessage(blockList_->get_list_handle(), LVM_INSERTCOLUMN, i, blockList_->get_column_addr());
//    }
//}
//
//void RelocationDlg::plantBlockItem() {
//    IMAGE_BASE_RELOCATION* tempRelocation = AnalysePE::GetAnalyse().GetRelocation();
//
//    LV_ITEM item;
//    memset(&item, 0, sizeof(LV_ITEM));
//    item.mask = LVIF_TEXT;
//
//    for (int i = 1; tempRelocation->SizeOfBlock != 0 && tempRelocation->VirtualAddress != 0; i++) {
//        item.iItem = i - 1;
//        item.iSubItem = 0;
//        TCHAR tIndex[5] = { 0 };
//        wsprintf(tIndex, L"%4d", i);
//        item.pszText = tIndex;
//        SendMessage(blockList_->get_list_handle(), LVM_INSERTITEM, 0, (DWORD)&item);
//
//        item.mask = LVIF_TEXT;
//        item.iItem = i - 1;
//        item.iSubItem = 1;
//        char* sectionName = nullptr;
//        DWORD sectionNum = AnalysePE::GetAnalyse().InWhichSectionRVA(tempRelocation->VirtualAddress);
//        AnalysePE::GetAnalyse().GetCharSectionName(sectionNum, &sectionName);
//        std::string sName;
//        sName += "(\"";
//        sName += sectionName;
//        sName += "\")";
//        TCHAR* tSectionName = nullptr;
//        CharToTchar(sName.c_str(), &tSectionName);
//        item.pszText = tSectionName;
//        ListView_SetItem(blockList_->get_list_handle(), (DWORD)&item);
//
//        item.iItem = i - 1;
//        item.iSubItem = 2;
//        TCHAR tRVA[9] = { 0 };
//        wsprintf(tRVA, L"%08X", tempRelocation->VirtualAddress);
//        item.pszText = tRVA;
//        ListView_SetItem(blockList_->get_list_handle(), (DWORD)&item);
//
//        item.iItem = i - 1;
//        item.iSubItem = 3;
//        TCHAR tBlockSize[9] = { 0 };
//        wsprintf(tBlockSize, L"%08X", tempRelocation->SizeOfBlock);
//        item.pszText = tBlockSize;
//        ListView_SetItem(blockList_->get_list_handle(), (DWORD)&item);
//
//        item.iItem = i - 1;
//        item.iSubItem = 4;
//        DWORD itemNum = (tempRelocation->SizeOfBlock - 8) / 2;
//        TCHAR* tItem = nullptr;
//        std::string sItem;
//        char buffer[10] = { 0 };
//        sprintf_s(buffer, "%X", itemNum);
//        sItem += buffer;
//        sItem += "h / ";
//        memset(buffer, 0, sizeof(buffer));
//        sprintf_s(buffer, "%d", itemNum);
//        sItem += buffer;
//        sItem += 'd';
//        CharToTchar(sItem.c_str(), &tItem);
//        item.pszText = tItem;
//        ListView_SetItem(blockList_->get_list_handle(), (DWORD)&item);
//
//        tempRelocation = (IMAGE_BASE_RELOCATION*)((DWORD)(tempRelocation) + tempRelocation->SizeOfBlock);
//    }
//}
//
//void RelocationDlg::InitBlockItemList() {
//    blockItemList_ = std::unique_ptr<ListCtrl>(new ListCtrl(GetDlgItem(h_current_dlg_, IDC_LIST_RELO_BLOCKITEM)
//        , [&]() {plantBlockItemColumn(); }, [&]() {plantBlockItemItem(); }));
//    blockItemList_->init(LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM | LVCF_FMT, LVIF_TEXT);
//    blockItemList_->plant_column();
//}
//
//void RelocationDlg::plantBlockItemColumn() {
//    array<ItemWidthAndName<DWORD, wstring>, 5> items = { {
//            { 50, L"Index" },
//            { 80, L"RVA" },
//            { 80, L"Offset" },
//            { 100, L"Type" },
//            { 120, L"Relocation Data"}
//    } };
//
//    for (size_t i = 0; i < items.size(); i++) {
//        blockItemList_->SetColumn(items[i], i);
//        SendMessage(blockItemList_->get_list_handle(), LVM_INSERTCOLUMN, i, blockItemList_->get_column_addr());
//    }
//}
//
//void RelocationDlg::plantBlockItemItem() {
//    DWORD BlockItemcListRow = SendMessage(blockList_->get_list_handle(), LVM_GETNEXTITEM, -1, LVNI_SELECTED);
//    if (BlockItemcListRow == -1) {
//        return;
//    }
//    SendMessage(blockItemList_->get_list_handle(), LVM_DELETEALLITEMS, 0, 0);
//
//    DWORD relocationIndex = GetRelocationTableIndex(BlockItemcListRow);
//    IMAGE_BASE_RELOCATION* tempRelocation = AnalysePE::GetAnalyse().GetRelocation(relocationIndex);
//
//    LV_ITEM item;
//    memset(&item, 0, sizeof(LV_ITEM));
//    item.mask = LVIF_TEXT;
//
//    DWORD itemNum = (tempRelocation->SizeOfBlock - 8) / 2;
//    WORD* blockItem = (WORD*)((DWORD)tempRelocation + 8);
//    for (DWORD j = 1; j <= itemNum; j++, blockItem++) {
//        item.iItem = j - 1;
//        item.iSubItem = 0;
//        TCHAR tIndex[5] = { 0 };
//        wsprintf(tIndex, L"%4d", j);
//        item.pszText = tIndex;
//        SendMessage(blockItemList_->get_list_handle(), LVM_INSERTITEM, 0, (DWORD)&item);
//
//        // 高4位表示是否需要修改，若为3，则修改4字节，若为0，则不修改
//        if ((*blockItem & 0xF000) >> 12 == 3) {
//            item.iItem = j - 1;
//            item.iSubItem = 1;
//            DWORD itemRVA = tempRelocation->VirtualAddress + (*blockItem & 0x0FFF);
//            TCHAR tItemRVA[9] = { 0 };
//            wsprintf(tItemRVA, L"%08X", itemRVA);
//            item.pszText = tItemRVA;
//            ListView_SetItem(blockItemList_->get_list_handle(), (DWORD)&item);
//
//            item.iItem = j - 1;
//            item.iSubItem = 2;
//            TCHAR tItemOffset[9] = { 0 };
//            wsprintf(tItemOffset, L"%08X", AnalysePE::GetAnalyse().RVAToFOA(itemRVA));
//            item.pszText = tItemOffset;
//            ListView_SetItem(blockItemList_->get_list_handle(), (DWORD)&item);
//
//            item.iItem = j - 1;
//            item.iSubItem = 3;
//            TCHAR tTypeInfo[12] = { L"HIGHTLOW(3)" };
//            item.pszText = tTypeInfo;
//            ListView_SetItem(blockItemList_->get_list_handle(), (DWORD)&item);
//
//            item.iItem = j - 1;
//            item.iSubItem = 4;
//            DWORD* relocationData = (DWORD*)((DWORD)AnalysePE::GetAnalyse().GetHeaders().dosHeader
//                + AnalysePE::GetAnalyse().RVAToFOA(itemRVA));
//            TCHAR tRelocationData[9] = { 0 };
//            wsprintf(tRelocationData, L"%4X", *relocationData);
//            item.pszText = tRelocationData;
//            ListView_SetItem(blockItemList_->get_list_handle(), (DWORD)&item);
//        }
//        else {
//            item.iItem = j - 1;
//            item.iSubItem = 1;
//            DWORD itemRVA = tempRelocation->VirtualAddress + (*blockItem & 0x0FFF);
//            TCHAR tItemRVA[9] = { L"-" };
//            item.pszText = tItemRVA;
//            ListView_SetItem(blockItemList_->get_list_handle(), (DWORD)&item);
//
//            item.iItem = j - 1;
//            item.iSubItem = 2;
//            TCHAR tItemOffset[9] = { L"-" };
//            item.pszText = tItemOffset;
//            ListView_SetItem(blockItemList_->get_list_handle(), (DWORD)&item);
//
//            item.iItem = j - 1;
//            item.iSubItem = 3;
//            std::string typeInfo = "HIGHTLOW(0)";
//            TCHAR* tType = nullptr;
//            CharToTchar(typeInfo.c_str(), &tType);
//            item.pszText = tType;
//            ListView_SetItem(blockItemList_->get_list_handle(), (DWORD)&item);
//
//            item.iItem = j - 1;
//            item.iSubItem = 4;
//            TCHAR tRelocationData[9] = { L"-" };
//            item.pszText = tRelocationData;
//            ListView_SetItem(blockItemList_->get_list_handle(), (DWORD)&item);
//        }
//    }
//}
//
//DWORD RelocationDlg::GetRelocationTableIndex(DWORD rowID) {
//    TCHAR indexBuffer[9] = { 0 };
//    LV_ITEM item;
//    memset(&item, 0, sizeof(item));
//
//    item.iSubItem = 0;
//    item.pszText = indexBuffer;
//    item.cchTextMax = 9;
//    SendMessage(blockList_->get_list_handle(), LVM_GETITEMTEXT, rowID, (DWORD)&item);
//
//
//    DWORD index = 0;
//    TcharToDword(indexBuffer, &index, 10);
//    return index - 1;
//}

namespace petools::gui {

	void RelocationDlg::InitDialog() noexcept {
        init_block_list();
		//InitBlockItemList();
	}

    void RelocationDlg::ShowDialog() noexcept {
        plant_block_column();
        plant_block_item();
		ShowWindow(current_hwnd_, GetCmdShow());
		UpdateWindow(current_hwnd_);
    }

    void RelocationDlg::init_block_list() noexcept {
        block_list_ = std::unique_ptr<ListCtrl>(new ListCtrl(GetDlgItem(current_hwnd_, IDC_LIST_RELO_BLOCK)));
        block_list_->Init(LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM | LVCF_FMT, LVIF_TEXT);
    }

	void RelocationDlg::plant_block_column() noexcept {
		array<ColumnDefinition, 5> items = { {
			{ 50, L"Index" },
			{ 100, L"Section" },
			{ 80, L"RVA" },
			{ 110, L"Size Of Block" },
			{ 120, L"Items(HEX/DEC)" }
		} };

		block_list_->SetColumn(items);
	}

    void RelocationDlg::plant_block_item() noexcept {
        IMAGE_BASE_RELOCATION* relocation = PeAnalyse().GetRelocation();

        LV_ITEM item;
        memset(&item, 0, sizeof(LV_ITEM));
        item.mask = LVIF_TEXT;

        for (int i = 1; relocation->SizeOfBlock != 0 && relocation->VirtualAddress != 0; i++) {
            item.iItem = i - 1;
            item.iSubItem = 0;
            TCHAR t_index[5] = { 0 };
            wsprintf(t_index, L"%4d", i);
            item.pszText = t_index;
            SendMessage(block_list_->GetListHandle(), LVM_INSERTITEM, 0, (DWORD)&item);

            item.mask = LVIF_TEXT;
            item.iItem = i - 1;
            item.iSubItem = 1;
            char* section_name = nullptr;
            DWORD section_num = PeAnalyse().InWhichSectionRVA(relocation ->VirtualAddress);
            PeAnalyse().GetCharSectionName(section_num, &section_name);
            std::string sName;
            sName += "(\"";
            sName += section_name;
            sName += "\")";
            TCHAR* t_section_name = nullptr;
            CharToTchar(sName.c_str(), &t_section_name);
            item.pszText = t_section_name;
            ListView_SetItem(block_list_->GetListHandle(), (DWORD)&item);

            item.iItem = i - 1;
            item.iSubItem = 2;
            TCHAR t_rva[9] = { 0 };
            wsprintf(t_rva, L"%08X", relocation->VirtualAddress);
            item.pszText = t_rva;
            ListView_SetItem(block_list_->GetListHandle(), (DWORD)&item);

            item.iItem = i - 1;
            item.iSubItem = 3;
            TCHAR t_block_size[9] = { 0 };
            wsprintf(t_block_size, L"%08X", relocation->SizeOfBlock);
            item.pszText = t_block_size;
            ListView_SetItem(block_list_->GetListHandle(), (DWORD)&item);

            item.iItem = i - 1;
            item.iSubItem = 4;
            DWORD item_num = (relocation->SizeOfBlock - 8) / 2;
            TCHAR* t_item = nullptr;
            std::string s_item;
            char buffer[10] = { 0 };
            sprintf_s(buffer, "%X", item_num);
            s_item += buffer;
            s_item += "h / ";
            memset(buffer, 0, sizeof(buffer));
            sprintf_s(buffer, "%d", item_num);
            s_item += buffer;
            s_item += 'd';
            CharToTchar(s_item.c_str(), &t_item);
            item.pszText = t_item;
            ListView_SetItem(block_list_->GetListHandle(), (DWORD)&item);

            relocation = (IMAGE_BASE_RELOCATION*)((DWORD)(relocation) + relocation->SizeOfBlock);
        }
    }

    LRESULT RelocationDlg::HandleMessage(const WindowHandle& h_dlg, UINT message, WPARAM w_param, LPARAM l_param) {
        NMHDR* hdr = (NMHDR*)l_param;
        switch (message)
        {
        case WM_COMMAND:
        {
            int wmId = LOWORD(w_param);
            switch (wmId) {
            case IDOK:
                DialogMgr().CloseDialog();
                break;
            default:
                break;
            }
            break;
        }
        case WM_CLOSE:
			DialogMgr().CloseDialog();
            break;
        case WM_NOTIFY:
            if (w_param == IDC_LIST_RELO_BLOCK && hdr->code == NM_CLICK) {
                //this_dlg_->plantBlockItemItem();
            }
            break;
        default:
            return FALSE;
        }
        return FALSE;
    }

} //namespace petools
