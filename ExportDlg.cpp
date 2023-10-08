#include "ExportDlg.h"
#include "resource.h"

extern HINSTANCE appInst;

ExportDlg* ExportDlg::thisDlg_ = nullptr;

ExportDlg::ExportDlg(HWND hParent)
    : DialogEX(IDD_DIALOG_EXPORT, hParent),
    export_(nullptr)
{

}

ExportDlg::~ExportDlg() {

}

void ExportDlg::InitDlg() {
    SetThisDlg();
    SetExportPtr();
}

void ExportDlg::Plant() {
    DialogBox(appInst, MAKEINTRESOURCE(idTemplate_), hParentDlg_, (DLGPROC)ExportProc);
}

void ExportDlg::SetExportInfo() {
    TCHAR characteristic[33] = { 0 };
    wsprintfW(characteristic, L"%08X", export_->Characteristics);
    SetDlgItemText(hCurrentDlg_, IDC_EDIT_EXP_CHA, characteristic);

    TCHAR timeDateStamp[9] = {0};
    wsprintfW(timeDateStamp, L"%08X", export_->TimeDateStamp);
    SetDlgItemText(hCurrentDlg_, IDC_EDIT_EXP_TIME, timeDateStamp);

    TCHAR base[9] = { 0 };
    wsprintfW(base, L"%08X", export_->Base);
    SetDlgItemText(hCurrentDlg_, IDC_EDIT_EXP_BASE, base);

    TCHAR nameOffset[9] = { 0 };
    wsprintfW(nameOffset, L"%08X", export_->Name);
    SetDlgItemText(hCurrentDlg_, IDC_EDIT_EXP_NAMEOFFSET, nameOffset);

    TCHAR* name = nullptr;
    char* cName = (char*)((DWORD)AnalysePE::GetAnalyse().GetHeaders().dosHeader
        + AnalysePE::GetAnalyse().RVAToFOA(export_->Name));
    AnalysePE::GetAnalyse().CharToTchar(cName, &name);
    SetDlgItemText(hCurrentDlg_, IDC_EDIT_EXP_NAME, name);
    cName = nullptr;
    name = nullptr;

    TCHAR numFunc[9] = { 0 };
    wsprintfW(numFunc, L"%08X", export_->NumberOfFunctions);
    SetDlgItemText(hCurrentDlg_, IDC_EDIT_EXP_NUMFUNC, numFunc);

    TCHAR numFuncName[9] = { 0 };
    wsprintfW(numFuncName, L"%08X", export_->NumberOfNames);
    SetDlgItemText(hCurrentDlg_, IDC_EDIT_EXP_NUMNAME, numFuncName);

    TCHAR addrFunc[9] = { 0 };
    wsprintfW(addrFunc, L"%08X", export_->AddressOfFunctions);
    SetDlgItemText(hCurrentDlg_, IDC_EDIT_EXP_ADDRFUNC, addrFunc);

    TCHAR addrFuncName[9] = { 0 };
    wsprintfW(addrFuncName, L"%08X", export_->AddressOfNames);
    SetDlgItemText(hCurrentDlg_, IDC_EDIT_EXP_ADDRNAME, addrFuncName);

    TCHAR addrOrdinal[9] = { 0 };
    wsprintfW(addrOrdinal, L"%08X", export_->AddressOfNameOrdinals);
    SetDlgItemText(hCurrentDlg_, IDC_EDIT_EXP_ADDRORD, addrOrdinal);
}

void ExportDlg::InitFuncList() {
    funcList_ = std::unique_ptr<ListCrl>(new ListCrl(GetDlgItem(hCurrentDlg_, IDC_LIST_EXP_FUNC)
        , [&]() {PlantFuncColumn(); }, [&]() {PlantFuncItem(); }));
    funcList_->Init(LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM | LVCF_FMT, LVIF_TEXT);
    funcList_->PlantColumn();
    funcList_->PlantItem();
}

void ExportDlg::PlantFuncColumn() {
    std::vector<widthAndName> items;
    items.push_back(widthAndName(90, TEXT("Ordinal")));
    items.push_back(widthAndName(70, TEXT("RVA")));
    items.push_back(widthAndName(90, TEXT("Offset")));
    items.push_back(widthAndName(390, TEXT("Function Name")));

    for (int i = 0; i < items.size(); i++) {
        funcList_->SetColumn(items[i], i);
        SendMessage(funcList_->GetList(), LVM_INSERTCOLUMN, i, funcList_->GetColumnAddr());
    }
}

void ExportDlg::PlantFuncItem() {
    DWORD* funcAddr = (DWORD*)((DWORD)AnalysePE::GetAnalyse().GetHeaders().dosHeader
        + AnalysePE::GetAnalyse().RVAToFOA(export_->AddressOfFunctions));
        
    LV_ITEM item;
    memset(&item, 0, sizeof(LV_ITEM));
    item.mask = LVIF_TEXT;

    for (int i = 0; i < export_->NumberOfFunctions; i++) {
        if (*(funcAddr + i) != 0) {
            item.iItem = i;
            item.iSubItem = 0;
            TCHAR ordinal[5] = { 0 };
            wsprintf(ordinal, L"%04X", i + export_->Base);
            item.pszText = ordinal;
            SendMessage(funcList_->GetList(), LVM_INSERTITEM, 0, (DWORD)&item);

            item.iItem = i;
            item.iSubItem = 1;
            TCHAR funcRVA[9] = { 0 };
            wsprintf(funcRVA, L"%08X", *funcAddr);
            item.pszText = funcRVA;
            ListView_SetItem(funcList_->GetList(), (DWORD)&item);

            item.iItem = i;
            item.iSubItem = 2;
            TCHAR funcOffset[9] = { 0 };
            wsprintf(funcOffset, L"%08X", AnalysePE::GetAnalyse().RVAToFOA(*funcAddr));
            item.pszText = funcOffset;
            ListView_SetItem(funcList_->GetList(), (DWORD)&item);

            WORD ordinalTableIndex = AnalysePE::GetAnalyse().GetOrdinalTableIndex(i);
            if (ordinalTableIndex != -1) {
                item.iItem = i;
                item.iSubItem = 3;
                TCHAR* tFuncName = nullptr;
                char* name = AnalysePE::GetAnalyse().GetExportFuncName(ordinalTableIndex);
                AnalysePE::GetAnalyse().CharToTchar(name, &tFuncName);
                item.pszText = tFuncName;
                ListView_SetItem(funcList_->GetList(), (DWORD)&item);
                name = nullptr;
                tFuncName = nullptr;
            }
            else {
                item.iItem = i;
                item.iSubItem = 3;
                TCHAR tFuncName[2] = L"-";
                item.pszText = tFuncName;
                ListView_SetItem(funcList_->GetList(), (DWORD)&item);
            }
        }
    }
}

LRESULT CALLBACK ExportDlg::ExportProc(HWND hExport, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message)
    {
    case WM_INITDIALOG:
        thisDlg_->SetCurrentDlgHWND(hExport);
        thisDlg_->SetExportInfo();
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

    default:
        return FALSE;
    }
    return FALSE;
}
