module;

#include "resource.h"
#include <windows.h>
#include <commctrl.h>

#define WM_DELETE_DLG (WM_APP + 1)

module ImportDlg;

import STL;
import AnalysePE;

using petools::show::ItemWidthAndName;
using petools::CharToTchar;
using petools::TcharToDword;
using std::array;
using std::wstring;

extern HINSTANCE appInst;

ImportDlg* ImportDlg::thisDlg_ = nullptr;

ImportDlg::ImportDlg(HWND hParent)
    : DialogEX(IDD_DIALOG_IMPORT, hParent)
{

}

ImportDlg::~ImportDlg() {

}

void ImportDlg::InitDlg() {
    SetThisDlg();
}

void ImportDlg::Plant() {
    DialogBox(appInst, MAKEINTRESOURCE(idTemplate_), hParentDlg_, (DLGPROC)ImportProc);
}

void ImportDlg::CloseDlg() {
	funcList_.reset();
	mainList_.reset();
	EndDialog(hCurrentDlg_, 0);
}

void ImportDlg::InitMainList() {
    mainList_ = std::unique_ptr<ListCrl>(new ListCrl(GetDlgItem(hCurrentDlg_, IDC_LIST_IMP_MAIN)
        , [&]() {PlantMainColumn(); }, [&]() {PlantMainItem(); }));
    mainList_->Init(LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM | LVCF_FMT, LVIF_TEXT);
    mainList_->PlantColumn();
    mainList_->PlantItem();
}

void ImportDlg::PlantMainColumn() {
    array<ItemWidthAndName<DWORD, wstring>, 6> items = { {
        { 120, L"Dll Name" },
        { 160, L"Original First Thunk" },
        { 120, L"Time Date Stamp" },
        { 120, L"Forwarder Chain" },
        { 80, L"Name" },
        { 100, L"First Thunk" }
    } };

    for (size_t i = 0; i < items.size(); i++) {
        mainList_->SetColumn(items[i], i);
        SendMessage(mainList_->GetList(), LVM_INSERTCOLUMN, i, mainList_->GetColumnAddr());
    }
}

void ImportDlg::PlantMainItem() {
    IMAGE_IMPORT_DESCRIPTOR* tempImport = AnalysePE::GetAnalyse().GetImport();;

    LV_ITEM item;
    memset(&item, 0, sizeof(LV_ITEM));
    

    for (int i = 0; tempImport->OriginalFirstThunk != 0; i++, tempImport++) {
        item.mask = LVIF_TEXT;
        item.iItem = i;
        item.iSubItem = 0;     
        char* dllName = (char*)((DWORD)AnalysePE::GetAnalyse().GetHeaders().dosHeader
            + AnalysePE::GetAnalyse().RVAToFOA(tempImport->Name));
        TCHAR* tDllName = nullptr;
        CharToTchar(dllName, &tDllName);
        item.pszText = tDllName;
        SendMessage(mainList_->GetList(), LVM_INSERTITEM, 0, (DWORD)&item);

        item.mask = LVIF_TEXT;
        item.iItem = i;
        item.iSubItem = 1;
        TCHAR tOriFirstThunk[9] = { 0 };
        wsprintf(tOriFirstThunk, L"%08X", tempImport->OriginalFirstThunk);
        item.pszText = tOriFirstThunk;
        ListView_SetItem(mainList_->GetList(), (DWORD)&item);

        item.iItem = i;
        item.iSubItem = 2;
        TCHAR tTimeStamp[9] = { 0 };
        wsprintf(tTimeStamp, L"%08X", tempImport->TimeDateStamp);
        item.pszText = tTimeStamp;
        ListView_SetItem(mainList_->GetList(), (DWORD)&item);

        item.iItem = i;
        item.iSubItem = 3;
        TCHAR tChain[9] = { 0 };
        wsprintf(tChain, L"%08X", tempImport->ForwarderChain);
        item.pszText = tChain;
        ListView_SetItem(mainList_->GetList(), (DWORD)&item);

        item.iItem = i;
        item.iSubItem = 4;
        TCHAR tNameOffset[9] = { 0 };
        wsprintf(tNameOffset, L"%08X", tempImport->Name);
        item.pszText = tNameOffset;
        ListView_SetItem(mainList_->GetList(), (DWORD)&item);

        item.iItem = i;
        item.iSubItem = 5;
        TCHAR tFirstThunk[9] = { 0 };
        wsprintf(tFirstThunk, L"%08X", tempImport->FirstThunk);
        item.pszText = tFirstThunk;
        ListView_SetItem(mainList_->GetList(), (DWORD)&item);
    }
}

void ImportDlg::InitFuncList() {
    funcList_ = std::unique_ptr<ListCrl>(new ListCrl(GetDlgItem(hCurrentDlg_, IDC_LIST_IMP_FUNC)
        , [&]() {PlantFuncColumn(); }, [&]() {PlantFuncItem(); }));
    funcList_->Init(LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM | LVCF_FMT, LVIF_TEXT);
    funcList_->PlantColumn();
}

void ImportDlg::PlantFuncColumn() {
    array<ItemWidthAndName<DWORD, wstring>, 5> items = { {
		{ 320, L"API Name" },
		{ 80, L"Thunk RVA" },
		{ 100, L"Thunk Offset" },
		{ 100, L"Thunk Value" },
		{ 80, L"Hint" }
	} };

    for (size_t i = 0; i < items.size(); i++) {
        funcList_->SetColumn(items[i], i);
        SendMessage(funcList_->GetList(), LVM_INSERTCOLUMN, i, funcList_->GetColumnAddr());
    }
}

void ImportDlg::PlantFuncItem() {
    DWORD mainListRow = SendMessage(mainList_->GetList(), LVM_GETNEXTITEM, -1, LVNI_SELECTED);
    if (mainListRow == -1) {
        return;
    }
    SendMessage(funcList_->GetList(), LVM_DELETEALLITEMS, 0, 0);

    LV_ITEM item;
    memset(&item, 0, sizeof(LV_ITEM));
 
    DWORD firstThunk = GetFirstThunkFromMainList(mainListRow);

    // INT与IAT都是双字大小的Union，直接定义为DWORD*方便操作
    DWORD* tempIAT = (DWORD*)((DWORD)AnalysePE::GetAnalyse().GetHeaders().dosHeader
        + AnalysePE::GetAnalyse().RVAToFOA(firstThunk));

    for (int j = 0; (*tempIAT) != 0; j++, tempIAT++) {
        IMAGE_IMPORT_BY_NAME* tempByName = (IMAGE_IMPORT_BY_NAME*)((DWORD)AnalysePE::GetAnalyse().GetHeaders().dosHeader
            + AnalysePE::GetAnalyse().RVAToFOA(*tempIAT));

        item.mask = LVIF_TEXT;
        item.iItem = j;
        item.iSubItem = 0;
        char* APIName = tempByName->Name;
        TCHAR* tAPIName = nullptr;
        CharToTchar(APIName, &tAPIName);
        item.pszText = tAPIName;
        SendMessage(funcList_->GetList(), LVM_INSERTITEM, 0, (DWORD)&item);

        item.iItem = j;
        item.iSubItem = 1;
        TCHAR tThunkRVA[9] = { 0 };
        wsprintf(tThunkRVA, L"%08X", firstThunk + j * 4);
        item.pszText = tThunkRVA;
        ListView_SetItem(funcList_->GetList(), (DWORD)&item);

        item.iItem = j;
        item.iSubItem = 2;
        TCHAR tThunkOffset[9] = { 0 };
        wsprintf(tThunkOffset, L"%08X", AnalysePE::GetAnalyse().RVAToFOA(firstThunk + j * 4));
        item.pszText = tThunkOffset;
        ListView_SetItem(funcList_->GetList(), (DWORD)&item);

        item.iItem = j;
        item.iSubItem = 3;
        TCHAR tThunkValue[9] = { 0 };
        wsprintf(tThunkValue, L"%08X", *tempIAT);
        item.pszText = tThunkValue;
        ListView_SetItem(funcList_->GetList(), (DWORD)&item);

        item.iItem = j;
        item.iSubItem = 4;
        TCHAR tHint[5] = { 0 };
        wsprintf(tHint, L"%04X", tempByName->Hint);
        item.pszText = tHint;
        ListView_SetItem(funcList_->GetList(), (DWORD)&item);
    }
}

DWORD ImportDlg::GetFirstThunkFromMainList(DWORD rowID) {
    TCHAR firstThunkBuffer[9] = { 0 };
    LV_ITEM item;
    memset(&item, 0, sizeof(item));

    item.iSubItem = 1;
    item.pszText = firstThunkBuffer;
    item.cchTextMax = 9;
    SendMessage(mainList_->GetList(), LVM_GETITEMTEXT, rowID, (DWORD)&item);


    DWORD firstThunk = 0;
    TcharToDword(firstThunkBuffer, &firstThunk, 16);
    return firstThunk;
}

LRESULT CALLBACK ImportDlg::ImportProc(HWND hImport, UINT message, WPARAM wParam, LPARAM lParam) {
    NMHDR* hdr = (NMHDR*)lParam;
    switch (message)
    {
    case WM_INITDIALOG:
        thisDlg_->SetCurrentDlgHWND(hImport);
        thisDlg_->InitMainList();
        thisDlg_->InitFuncList();
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
        if (wParam == IDC_LIST_IMP_MAIN && hdr->code == NM_CLICK) {
            thisDlg_->PlantFuncItem();
        }
        break;
    default:
        return FALSE;
    }
    return FALSE;
}
