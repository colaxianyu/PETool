module;

#include "resource.h"
#include <windows.h>
#include <commctrl.h>
#include <cstdio>

module RelocationDlg;

import STL;
import AnalysePE;

using petools::show::ItemWidthAndName;
using petools::CharToTchar;
using petools::TcharToDword;
using std::array;
using std::wstring;

extern HINSTANCE appInst;

RelocationDlg* RelocationDlg::thisDlg_ = nullptr;

RelocationDlg::RelocationDlg(HWND hParent)
    : DialogEX(IDD_DIALOG_RELOCATION, hParent)
{

}

RelocationDlg::~RelocationDlg() {

}

void RelocationDlg::InitDlg() {
    SetThisDlg();
}

void RelocationDlg::Plant() {
    DialogBox(appInst, MAKEINTRESOURCE(idTemplate_), hParentDlg_, (DLGPROC)RelocationProc);
}

void RelocationDlg::CloseDlg() {
	blockList_.reset();
	blockItemList_.reset();
	EndDialog(hCurrentDlg_, 0);
}

void RelocationDlg::InitBlockList() {
    blockList_ = std::unique_ptr<ListCrl>(new ListCrl(GetDlgItem(hCurrentDlg_, IDC_LIST_RELO_BLOCK)
        , [&]() {PlantBlockColumn(); }, [&]() {PlantBlockItem(); }));
    blockList_->Init(LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM | LVCF_FMT, LVIF_TEXT);
    blockList_->PlantColumn();
    blockList_->PlantItem();
}

void RelocationDlg::PlantBlockColumn() {
    array<ItemWidthAndName<DWORD, wstring>, 5> items = { {
		{ 50, L"Index" },
		{ 100, L"Section" },
		{ 80, L"RVA" },
		{ 110, L"Size Of Block" },
		{ 120, L"Items(HEX/DEC)" }
	} };

    for (size_t i = 0; i < items.size(); i++) {
        blockList_->SetColumn(items[i], i);
        SendMessage(blockList_->GetList(), LVM_INSERTCOLUMN, i, blockList_->GetColumnAddr());
    }
}

void RelocationDlg::PlantBlockItem() {
    IMAGE_BASE_RELOCATION* tempRelocation = AnalysePE::GetAnalyse().GetRelocation();

    LV_ITEM item;
    memset(&item, 0, sizeof(LV_ITEM));
    item.mask = LVIF_TEXT;

    for (int i = 1; tempRelocation->SizeOfBlock != 0 && tempRelocation->VirtualAddress != 0; i++) {
        item.iItem = i - 1;
        item.iSubItem = 0;
        TCHAR tIndex[5] = { 0 };
        wsprintf(tIndex, L"%4d", i);
        item.pszText = tIndex;
        SendMessage(blockList_->GetList(), LVM_INSERTITEM, 0, (DWORD)&item);

        item.mask = LVIF_TEXT;
        item.iItem = i - 1;
        item.iSubItem = 1;
        char* sectionName = nullptr;
        DWORD sectionNum = AnalysePE::GetAnalyse().InWhichSectionRVA(tempRelocation->VirtualAddress);
        AnalysePE::GetAnalyse().GetCharSectionName(sectionNum, &sectionName);
        std::string sName;
        sName += "(\"";
        sName += sectionName;
        sName += "\")";
        TCHAR* tSectionName = nullptr;
        CharToTchar(sName.c_str(), &tSectionName);
        item.pszText = tSectionName;
        ListView_SetItem(blockList_->GetList(), (DWORD)&item);

        item.iItem = i - 1;
        item.iSubItem = 2;
        TCHAR tRVA[9] = { 0 };
        wsprintf(tRVA, L"%08X", tempRelocation->VirtualAddress);
        item.pszText = tRVA;
        ListView_SetItem(blockList_->GetList(), (DWORD)&item);

        item.iItem = i - 1;
        item.iSubItem = 3;
        TCHAR tBlockSize[9] = { 0 };
        wsprintf(tBlockSize, L"%08X", tempRelocation->SizeOfBlock);
        item.pszText = tBlockSize;
        ListView_SetItem(blockList_->GetList(), (DWORD)&item);

        item.iItem = i - 1;
        item.iSubItem = 4;
        DWORD itemNum = (tempRelocation->SizeOfBlock - 8) / 2;
        TCHAR* tItem = nullptr;
        std::string sItem;
        char buffer[10] = { 0 };
        sprintf_s(buffer, "%X", itemNum);
        sItem += buffer;
        sItem += "h / ";
        memset(buffer, 0, sizeof(buffer));
        sprintf_s(buffer, "%d", itemNum);
        sItem += buffer;
        sItem += 'd';
        CharToTchar(sItem.c_str(), &tItem);
        item.pszText = tItem;
        ListView_SetItem(blockList_->GetList(), (DWORD)&item);

        tempRelocation = (IMAGE_BASE_RELOCATION*)((DWORD)(tempRelocation) + tempRelocation->SizeOfBlock);
    }
}

void RelocationDlg::InitBlockItemList() {
    blockItemList_ = std::unique_ptr<ListCrl>(new ListCrl(GetDlgItem(hCurrentDlg_, IDC_LIST_RELO_BLOCKITEM)
        , [&]() {PlantBlockItemColumn(); }, [&]() {PlantBlockItemItem(); }));
    blockItemList_->Init(LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM | LVCF_FMT, LVIF_TEXT);
    blockItemList_->PlantColumn();
}

void RelocationDlg::PlantBlockItemColumn() {
    array<ItemWidthAndName<DWORD, wstring>, 5> items = { {
            { 50, L"Index" },
            { 80, L"RVA" },
            { 80, L"Offset" },
            { 100, L"Type" },
            { 120, L"Relocation Data"}
    } };

    for (size_t i = 0; i < items.size(); i++) {
        blockItemList_->SetColumn(items[i], i);
        SendMessage(blockItemList_->GetList(), LVM_INSERTCOLUMN, i, blockItemList_->GetColumnAddr());
    }
}

void RelocationDlg::PlantBlockItemItem() {
    DWORD BlockItemcListRow = SendMessage(blockList_->GetList(), LVM_GETNEXTITEM, -1, LVNI_SELECTED);
    if (BlockItemcListRow == -1) {
        return;
    }
    SendMessage(blockItemList_->GetList(), LVM_DELETEALLITEMS, 0, 0);

    DWORD relocationIndex = GetRelocationTableIndex(BlockItemcListRow);
    IMAGE_BASE_RELOCATION* tempRelocation = AnalysePE::GetAnalyse().GetRelocation(relocationIndex);

    LV_ITEM item;
    memset(&item, 0, sizeof(LV_ITEM));
    item.mask = LVIF_TEXT;

    DWORD itemNum = (tempRelocation->SizeOfBlock - 8) / 2;
    WORD* blockItem = (WORD*)((DWORD)tempRelocation + 8);
    for (DWORD j = 1; j <= itemNum; j++, blockItem++) {
        item.iItem = j - 1;
        item.iSubItem = 0;
        TCHAR tIndex[5] = { 0 };
        wsprintf(tIndex, L"%4d", j);
        item.pszText = tIndex;
        SendMessage(blockItemList_->GetList(), LVM_INSERTITEM, 0, (DWORD)&item);

        // 高4位表示是否需要修改，若为3，则修改4字节，若为0，则不修改
        if ((*blockItem & 0xF000) >> 12 == 3) {
            item.iItem = j - 1;
            item.iSubItem = 1;
            DWORD itemRVA = tempRelocation->VirtualAddress + (*blockItem & 0x0FFF);
            TCHAR tItemRVA[9] = { 0 };
            wsprintf(tItemRVA, L"%08X", itemRVA);
            item.pszText = tItemRVA;
            ListView_SetItem(blockItemList_->GetList(), (DWORD)&item);

            item.iItem = j - 1;
            item.iSubItem = 2;
            TCHAR tItemOffset[9] = { 0 };
            wsprintf(tItemOffset, L"%08X", AnalysePE::GetAnalyse().RVAToFOA(itemRVA));
            item.pszText = tItemOffset;
            ListView_SetItem(blockItemList_->GetList(), (DWORD)&item);

            item.iItem = j - 1;
            item.iSubItem = 3;
            TCHAR tTypeInfo[12] = { L"HIGHTLOW(3)" };
            item.pszText = tTypeInfo;
            ListView_SetItem(blockItemList_->GetList(), (DWORD)&item);

            item.iItem = j - 1;
            item.iSubItem = 4;
            DWORD* relocationData = (DWORD*)((DWORD)AnalysePE::GetAnalyse().GetHeaders().dosHeader
                + AnalysePE::GetAnalyse().RVAToFOA(itemRVA));
            TCHAR tRelocationData[9] = { 0 };
            wsprintf(tRelocationData, L"%4X", *relocationData);
            item.pszText = tRelocationData;
            ListView_SetItem(blockItemList_->GetList(), (DWORD)&item);
        }
        else {
            item.iItem = j - 1;
            item.iSubItem = 1;
            DWORD itemRVA = tempRelocation->VirtualAddress + (*blockItem & 0x0FFF);
            TCHAR tItemRVA[9] = { L"-" };
            item.pszText = tItemRVA;
            ListView_SetItem(blockItemList_->GetList(), (DWORD)&item);

            item.iItem = j - 1;
            item.iSubItem = 2;
            TCHAR tItemOffset[9] = { L"-" };
            item.pszText = tItemOffset;
            ListView_SetItem(blockItemList_->GetList(), (DWORD)&item);

            item.iItem = j - 1;
            item.iSubItem = 3;
            std::string typeInfo = "HIGHTLOW(0)";
            TCHAR* tType = nullptr;
            CharToTchar(typeInfo.c_str(), &tType);
            item.pszText = tType;
            ListView_SetItem(blockItemList_->GetList(), (DWORD)&item);

            item.iItem = j - 1;
            item.iSubItem = 4;
            TCHAR tRelocationData[9] = { L"-" };
            item.pszText = tRelocationData;
            ListView_SetItem(blockItemList_->GetList(), (DWORD)&item);
        }
    }
}

DWORD RelocationDlg::GetRelocationTableIndex(DWORD rowID) {
    TCHAR indexBuffer[9] = { 0 };
    LV_ITEM item;
    memset(&item, 0, sizeof(item));

    item.iSubItem = 0;
    item.pszText = indexBuffer;
    item.cchTextMax = 9;
    SendMessage(blockList_->GetList(), LVM_GETITEMTEXT, rowID, (DWORD)&item);


    DWORD index = 0;
    TcharToDword(indexBuffer, &index, 10);
    return index - 1;
}

LRESULT CALLBACK RelocationDlg::RelocationProc(HWND hRelocation, UINT message, WPARAM wParam, LPARAM lParam) {
    NMHDR* hdr = (NMHDR*)lParam;
    switch (message)
    {
    case WM_INITDIALOG:
        thisDlg_->SetCurrentDlgHWND(hRelocation);
        thisDlg_->InitBlockList();
        thisDlg_->InitBlockItemList();
        break;
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);

        switch (wmId) {
        case IDOK:
            thisDlg_->CloseDlg();
            break;
        default:
            break;
        }
        break;
    }
    case WM_CLOSE:
        thisDlg_->CloseDlg();
        break;
    case WM_NOTIFY:
        if (wParam == IDC_LIST_RELO_BLOCK && hdr->code == NM_CLICK) {
            thisDlg_->PlantBlockItemItem();
        }
        break;
    default:
        return FALSE;
    }
    return FALSE;
}
