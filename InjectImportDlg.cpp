#include "InjectImportDlg.h"
#include "resource.h"
#include "AnalysePE.h"
#include <string>

extern HINSTANCE appInst;

InjectImportDlg* InjectImportDlg::thisDlg_ = nullptr;

InjectImportDlg::InjectImportDlg(HWND hParent)
    : DialogEX(IDD_DIALOG_INJECTIMPORT, hParent)
{

}

InjectImportDlg::~InjectImportDlg() {

}

void InjectImportDlg::InitDlg() {
    SetThisDlg();
}

void InjectImportDlg::Plant() {
    DialogBox(appInst, MAKEINTRESOURCE(idTemplate_), hParentDlg_, (DLGPROC)InjectImportProc);
}

void InjectImportDlg::SelectDll() {
    TCHAR tDllName[260] = { 0 };
    SendMessage(GetDlgItem(hCurrentDlg_, IDC_EDIT_INJECTDLL), WM_GETTEXT, 260, (LPARAM)tDllName);
    TCHAR tFuncName[260] = { 0 };
    SendMessage(GetDlgItem(hCurrentDlg_, IDC_EDIT_IMJECTFUNC), WM_GETTEXT, 260, (LPARAM)tFuncName);

    AnalysePE::GetAnalyse().DllInject(tDllName, tFuncName);



    /*TCHAR tCurrentDirectory[260] = { 0 };
    GetCurrentDirectory(260, tCurrentDirectory);

    std::string path;
    char* currentDirectory = nullptr;
    AnalysePE::GetAnalyse().TcharToChar(tCurrentDirectory, &currentDirectory);
    path += currentDirectory;
    path += '\\';
    char* DllName = nullptr;
    AnalysePE::GetAnalyse().TcharToChar(tDllName, &DllName);
    path += DllName;

    const char sectionHeaderData[40] = {
0x2E, 0x6D, 0x79, 0x64, 0x61, 0x74, 0x61, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x50, 0x00, 0x00,
0x00, 0x10, 0x00, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x40
    };

    AnalysePE::GetAnalyse().AddSection(sectionHeaderData);
    PeHeaders newFileHeaders = AnalysePE::GetAnalyse().GetHeaders();
    void* newSectionStart = (void*)((DWORD)newFileHeaders.dosHeader
        + (newFileHeaders.sectionHeader + newFileHeaders.fileHeader->NumberOfSections - 1)->PointerToRawData);
    IMAGE_IMPORT_DESCRIPTOR* fileImport = (IMAGE_IMPORT_DESCRIPTOR*)((DWORD)newFileHeaders.dosHeader +
        AnalysePE::GetAnalyse().RVAToFOA(newFileHeaders.optionalHeader->DataDirectory[1].VirtualAddress));

    void* sectionIndex = newSectionStart;
    memset(sectionIndex, 0, 0x1000);
    for (int i = 0; fileImport->OriginalFirstThunk != 0 && fileImport->FirstThunk != 0; i++, fileImport++) {
        memcpy(sectionIndex, fileImport, sizeof(IMAGE_IMPORT_DESCRIPTOR));
        IMAGE_IMPORT_DESCRIPTOR* s = (IMAGE_IMPORT_DESCRIPTOR*)sectionIndex;
        sectionIndex = (BYTE*)sectionIndex + 20;
    }

    IMAGE_IMPORT_DESCRIPTOR* newImport = (IMAGE_IMPORT_DESCRIPTOR*)sectionIndex;
    DWORD* INT = (DWORD*)((DWORD)newImport + 40);
    DWORD* IAT = (DWORD*)((DWORD)INT + 8);
    IMAGE_IMPORT_BY_NAME* func = (IMAGE_IMPORT_BY_NAME*)((DWORD)IAT + 11);
    char* dllName = (char*)((DWORD)func + 11);

    char dllNameTemp[12] = "DLLTest.dll";
    memcpy(dllName, &dllNameTemp, 12);
    char funcNameTemp[8] = "_Plus@8";
    memcpy(func->Name, &funcNameTemp, 8);

    char* a = func->Name;

    *INT = AnalysePE::GetAnalyse().FOAToRVA((DWORD)func - (DWORD)newFileHeaders.dosHeader);
    *IAT = AnalysePE::GetAnalyse().FOAToRVA((DWORD)func - (DWORD)newFileHeaders.dosHeader);

    newImport->OriginalFirstThunk = AnalysePE::GetAnalyse().FOAToRVA((DWORD)INT - (DWORD)newFileHeaders.dosHeader);
    newImport->FirstThunk = AnalysePE::GetAnalyse().FOAToRVA((DWORD)IAT - (DWORD)newFileHeaders.dosHeader);
    newImport->Name = AnalysePE::GetAnalyse().FOAToRVA((DWORD)dllName - (DWORD)newFileHeaders.dosHeader);

    newFileHeaders.optionalHeader->DataDirectory[1].VirtualAddress = AnalysePE::GetAnalyse().FOAToRVA((DWORD)newSectionStart
        - (DWORD)newFileHeaders.dosHeader);*/
    //newFileHeaders.optionalHeader->DataDirectory[1].Size += 20;
}

LRESULT CALLBACK InjectImportDlg::InjectImportProc(HWND hInject, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message)
    {
    case WM_INITDIALOG:
        thisDlg_->SetCurrentDlgHWND(hInject);
        break;
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        switch (wmId)
        {
        case IDC_BUTTON_DLLSELECT:
            thisDlg_->SelectDll();
            break;
        default:
            break;
        }
        break;
    }
    case WM_CLOSE:
        thisDlg_->CloseDlg();
        break;
    case IDOK:
        thisDlg_->CloseDlg();
        break;
    default:
        return FALSE;
    }
    return FALSE;
}
