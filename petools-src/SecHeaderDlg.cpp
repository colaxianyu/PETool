module;

#include "resource.h"
#include <windows.h>
#include <CommCtrl.h>

module SecHeaderDlg;

import AnalysePE;

using std::array;
using std::wstring;
using petools::show::ItemWidthAndName;

extern HINSTANCE appInst;
extern int cmdShow;

SecHeaderDlg* SecHeaderDlg::thisDlg_ = nullptr;

SecHeaderDlg::SecHeaderDlg(HWND hParent)
	: DialogEX(IDD_DIALOG_SECTIONS, hParent)
{

}

SecHeaderDlg::~SecHeaderDlg() {

}

void SecHeaderDlg::InitDlg() {
	SetThisDlg();
}

void SecHeaderDlg::CloseDlg() {
	sectionList_.reset();
	EndDialog(hCurrentDlg_, 0);
}

void SecHeaderDlg::Plant() {
	DialogBox(appInst, MAKEINTRESOURCE(idTemplate_), hParentDlg_, (DLGPROC)SectionProc);
}

void SecHeaderDlg::InitSectionList() {
    sectionList_ = std::unique_ptr<ListCrl>(new ListCrl(GetDlgItem(hCurrentDlg_, IDC_LIST_SECTION),
        [&]() {thisDlg_->PlantSectionColumn(); }, [&]() {thisDlg_->PlantSectionItem(); }));
    sectionList_->Init(LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM | LVCF_FMT, LVIF_TEXT);
    sectionList_->PlantColumn();
    sectionList_->PlantItem();
}

void SecHeaderDlg::PlantSectionColumn() {
    array<ItemWidthAndName<DWORD, wstring>, 7> items = { {
		{ 40, L"#" },
		{ 70, L"Name" },
		{ 110, L"Virtual Size"},
		{ 120, L"Virtual Offset"},
		{ 80, L"Raw Size"},
        { 90, L"Raw Offset" },
		{ 120, L"Characteristics"}
	} };

    for (int i = 0; i < items.size(); i++) {
        sectionList_->SetColumn(items[i], i);
        SendMessage(sectionList_->GetList(), LVM_INSERTCOLUMN, i, sectionList_->GetColumnAddr());
    }
}

void SecHeaderDlg::PlantSectionItem() {
    LV_ITEM item;
    memset(&item, 0, sizeof(LV_ITEM));
    item.mask = LVIF_TEXT;

    IMAGE_SECTION_HEADER* tempSecHeader = AnalysePE::GetAnalyse().GetHeaders().sectionHeader;

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
            AnalysePE::GetAnalyse().GetTcharSectionName(row, &nameBuffer);
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

    for (int row = 0; tempSecHeader->Name != 0 && tempSecHeader->Characteristics != 0; row++, tempSecHeader++) {
        SetSectionHexText(sectionList_->GetList(), row);
        SetSectionHexText(sectionList_->GetList(), row);
        SetSectionHexText(sectionList_->GetList(), row, tempSecHeader->Misc.VirtualSize);
        SetSectionHexText(sectionList_->GetList(), row, tempSecHeader->VirtualAddress);
        SetSectionHexText(sectionList_->GetList(), row, tempSecHeader->SizeOfRawData);
        SetSectionHexText(sectionList_->GetList(), row, tempSecHeader->PointerToRawData);
        SetSectionHexText(sectionList_->GetList(), row, tempSecHeader->Characteristics);
        index = 0;
    }
}

LRESULT CALLBACK SecHeaderDlg::SectionProc(HWND hSec, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message)
    {
    case WM_INITDIALOG:
        thisDlg_->SetCurrentDlgHWND(hSec);
        thisDlg_->InitSectionList();
        break;
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        switch (wmId)
        {
        case IDOK:
            thisDlg_->CloseDlg();
            break;
        }
        break;
    }
    case WM_CLOSE:
        thisDlg_->CloseDlg();
        break;
    default:
        return FALSE;
    }
    return FALSE;
}