#include "SecHeaderDlg.h"
#include "resource.h"
#include "AnalysePE.h"
#include <windows.h>

extern HINSTANCE appInst;
extern int cmdShow;

SecHeaderDlg* SecHeaderDlg::thisDlg_ = nullptr;

SecHeaderDlg::SecHeaderDlg(HWND hParent)
	: DialogEX(IDD_DIALOG_SECTIONS, hParent)
{

}

SecHeaderDlg::~SecHeaderDlg() {
    sectionList_.reset();
}

void SecHeaderDlg::InitDlg() {
	SetThisDlg();
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
    std::vector<widthAndName> items;
    items.push_back(widthAndName(40, TEXT("#")));
    items.push_back(widthAndName(70, TEXT("Name")));
    items.push_back(widthAndName(110, TEXT("Virtual Size")));
    items.push_back(widthAndName(120, TEXT("Virtual Offset")));
    items.push_back(widthAndName(80, TEXT("Raw Size")));
    items.push_back(widthAndName(90, TEXT("Raw Offset")));
    items.push_back(widthAndName(120, TEXT("Characteristics")));

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

    int row = 0;
    do {
        item.iItem = row;
        item.iSubItem = 0;
        TCHAR indexBuffer[100] = { 0 };
        wsprintf(indexBuffer, L"%d", row + 1);
        item.pszText = indexBuffer;
        SendMessage(sectionList_->GetList(), LVM_INSERTITEM, 0, (DWORD)&item);

        item.iItem = row;
        item.iSubItem = 1;
        TCHAR* nameBuffer = nullptr;
        AnalysePE::GetAnalyse().GetTcharSectionName(row, &nameBuffer);
        item.pszText = nameBuffer;
        ListView_SetItem(sectionList_->GetList(), (DWORD)&item);

        item.iItem = row;
        item.iSubItem = 2;
        TCHAR virtualSizeBuffer[9] = { 0 };
        wsprintf(virtualSizeBuffer, L"%08X", tempSecHeader->Misc.VirtualSize);
        item.pszText = virtualSizeBuffer;
        ListView_SetItem(sectionList_->GetList(), (DWORD)&item);

        item.iItem = row;
        item.iSubItem = 3;
        TCHAR rvaBuffer[9] = { 0 };
        wsprintf(rvaBuffer, L"%08X", tempSecHeader->VirtualAddress);
        item.pszText = rvaBuffer;
        ListView_SetItem(sectionList_->GetList(), (DWORD)&item);

        item.iItem = row;
        item.iSubItem = 4;
        TCHAR rawSizeBuffer[9] = { 0 };
        wsprintf(rawSizeBuffer, L"%08X", tempSecHeader->SizeOfRawData);
        item.pszText = rawSizeBuffer;
        ListView_SetItem(sectionList_->GetList(), (DWORD)&item);

        item.iItem = row;
        item.iSubItem = 5;
        TCHAR foaBuffer[9] = { 0 };
        wsprintf(foaBuffer, L"%08X", tempSecHeader->PointerToRawData);
        item.pszText = foaBuffer;
        ListView_SetItem(sectionList_->GetList(), (DWORD)&item);

        item.iItem = row;
        item.iSubItem = 6;
        TCHAR chaBuffer[9] = { 0 };
        wsprintf(chaBuffer, L"%08X", tempSecHeader->Characteristics);
        item.pszText = chaBuffer;
        ListView_SetItem(sectionList_->GetList(), (DWORD)&item);

        row++;
        tempSecHeader++;
    } while (tempSecHeader->Name != 0 && tempSecHeader->Characteristics != 0);
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