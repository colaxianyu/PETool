#include "ImportDlg.h"
#include "resource.h"
#include <string>
import Utils;

extern HINSTANCE appInst;

ImportDlg* ImportDlg::thisDlg_ = nullptr;

ImportDlg::ImportDlg(HWND hParent)
    : DialogEX(IDD_DIALOG_IMPORT, hParent),
    import_(nullptr)
{

}

ImportDlg::~ImportDlg() {
    mainList_.reset();
    funcList_.reset();
    import_ = nullptr;
}

void ImportDlg::InitDlg() {
    SetThisDlg();
    SetImportPtr();
}

void ImportDlg::SetImportPtr() {
    import_ = AnalysePE::GetAnalyse().GetImport();
}

void ImportDlg::Plant() {
    DialogBox(appInst, MAKEINTRESOURCE(idTemplate_), hParentDlg_, (DLGPROC)ImportProc);
}

void ImportDlg::InitMainList() {
    mainList_ = std::unique_ptr<ListCrl>(new ListCrl(GetDlgItem(hCurrentDlg_, IDC_LIST_IMP_MAIN)
        , [&]() {PlantMainColumn(); }, [&]() {PlantMainItem(); }));
    mainList_->Init(LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM | LVCF_FMT, LVIF_TEXT);
    mainList_->PlantColumn();
    mainList_->PlantItem();
}

void ImportDlg::PlantMainColumn() {
    std::vector<widthAndName> items;
    items.push_back(widthAndName(120, TEXT("Dll Name")));
    items.push_back(widthAndName(160, TEXT("Original First Thunk")));
    items.push_back(widthAndName(120, TEXT("Time Date Stamp")));
    items.push_back(widthAndName(120, TEXT("Forwarder Chain")));
    items.push_back(widthAndName(80, TEXT("Name")));
    items.push_back(widthAndName(100, TEXT("First Thunk")));

    for (int i = 0; i < items.size(); i++) {
        mainList_->SetColumn(items[i], i);
        SendMessage(mainList_->GetList(), LVM_INSERTCOLUMN, i, mainList_->GetColumnAddr());
    }
}

void ImportDlg::PlantMainItem() {
    IMAGE_IMPORT_DESCRIPTOR* tempImport = import_;

    LV_ITEM item;
    memset(&item, 0, sizeof(LV_ITEM));
    

    for (int i = 0; tempImport->OriginalFirstThunk != 0 && tempImport->FirstThunk != 0; i++, tempImport++) {
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
    std::vector<widthAndName> items;
    items.push_back(widthAndName(320, TEXT("API Name")));
    items.push_back(widthAndName(80, TEXT("Thunk RVA")));
    items.push_back(widthAndName(100, TEXT("Thunk Offset")));
    items.push_back(widthAndName(100, TEXT("Thunk Value")));
    items.push_back(widthAndName(80, TEXT("Hint")));

    for (int i = 0; i < items.size(); i++) {
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
