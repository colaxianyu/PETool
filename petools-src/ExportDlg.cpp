module;

#include "resource.h"
#include <windows.h>
#include <commctrl.h>

module ExportDlg;

import STL;
import AnalysePE;

using petools::show::ItemWidthAndName;
using petools::show::SetDlgItemText_t;
using petools::CharToTchar;
using std::array;
using std::wstring;

extern HINSTANCE appInst;

ExportDlg* ExportDlg::thisDlg_ = nullptr;

ExportDlg::ExportDlg(HWND hParent)
    : DialogEX(IDD_DIALOG_EXPORT, hParent)
{

}

ExportDlg::~ExportDlg() {

}

void ExportDlg::InitDlg() {
    SetThisDlg();
}

void ExportDlg::Plant() {
    DialogBox(appInst, MAKEINTRESOURCE(idTemplate_), hParentDlg_, (DLGPROC)ExportProc);
}

void ExportDlg::CloseDlg() {
    funcList_.reset();
	EndDialog(hCurrentDlg_, 0);
}

void ExportDlg::SetExportInfo() {
    IMAGE_EXPORT_DIRECTORY* temp_export = AnalysePE::GetAnalyse().GetExport();

    SetDlgItemText_t(hCurrentDlg_, IDC_EDIT_EXP_CHA, temp_export->Characteristics, 8);
    SetDlgItemText_t(hCurrentDlg_, IDC_EDIT_EXP_TIME, temp_export->TimeDateStamp, 8);
    SetDlgItemText_t(hCurrentDlg_, IDC_EDIT_EXP_BASE, temp_export->Base, 8);
    SetDlgItemText_t(hCurrentDlg_, IDC_EDIT_EXP_NAMEOFFSET, temp_export->Name, 8);
    SetDlgItemText_t(hCurrentDlg_, IDC_EDIT_EXP_NUMFUNC, temp_export->NumberOfFunctions, 8);
    SetDlgItemText_t(hCurrentDlg_, IDC_EDIT_EXP_NUMNAME, temp_export->NumberOfNames, 8);
    SetDlgItemText_t(hCurrentDlg_, IDC_EDIT_EXP_ADDRFUNC, temp_export->AddressOfFunctions, 8);
    SetDlgItemText_t(hCurrentDlg_, IDC_EDIT_EXP_ADDRNAME, temp_export->AddressOfNames, 8);
    SetDlgItemText_t(hCurrentDlg_, IDC_EDIT_EXP_ADDRORD, temp_export->AddressOfNameOrdinals, 8);

    TCHAR* name = nullptr;
    char* cName = (char*)((DWORD)AnalysePE::GetAnalyse().GetHeaders().dosHeader
        + AnalysePE::GetAnalyse().RVAToFOA(temp_export->Name));
    CharToTchar(cName, &name);
    SetDlgItemText(hCurrentDlg_, IDC_EDIT_EXP_NAME, name);
    cName = nullptr;
    name = nullptr;
}

void ExportDlg::InitFuncList() {
    funcList_ = std::unique_ptr<ListCrl>(new ListCrl(GetDlgItem(hCurrentDlg_, IDC_LIST_EXP_FUNC)
        , [&]() {PlantFuncColumn(); }, [&]() {PlantFuncItem(); }));
    funcList_->Init(LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM | LVCF_FMT, LVIF_TEXT);
    funcList_->PlantColumn();
    funcList_->PlantItem();
}

void ExportDlg::PlantFuncColumn() {

    array<ItemWidthAndName<DWORD, std::wstring>, 4> items = { {
        { 90, L"Ordinal" },
        { 70, L"RVA" },
        { 90, L"Offset" },
        { 390, L"Function Name" }
    } };

    for (size_t i = 0; i < items.size(); i++) {
        funcList_->SetColumn(items[i], i);
        SendMessage(funcList_->GetList(), LVM_INSERTCOLUMN, i, funcList_->GetColumnAddr());
    }
}

void ExportDlg::PlantFuncItem() {
    IMAGE_EXPORT_DIRECTORY* temp_export = AnalysePE::GetAnalyse().GetExport();
    DWORD* funcAddr = (DWORD*)((DWORD)AnalysePE::GetAnalyse().GetHeaders().dosHeader
        + AnalysePE::GetAnalyse().RVAToFOA(temp_export->AddressOfFunctions));
        
    LV_ITEM item;
    memset(&item, 0, sizeof(LV_ITEM));
    item.mask = LVIF_TEXT;

    for (DWORD i = 0; i < temp_export->NumberOfFunctions; i++) {
        if (*(funcAddr + i) != 0) {
            item.iItem = i;
            item.iSubItem = 0;
            TCHAR ordinal[5] = { 0 };
            wsprintf(ordinal, L"%04X", i + temp_export->Base);
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
                CharToTchar(name, &tFuncName);
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
