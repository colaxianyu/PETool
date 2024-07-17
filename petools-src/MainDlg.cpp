module;

#include "resource.h"
#include <windows.h>
#include <Tlhelp32.h>
#include <WtsApi32.h>
#include <CommCtrl.h>
#pragma comment(lib, "WtsApi32.lib")


module MainDlg;

import AnalysePE;
import STL;

constexpr DWORD pidLen = 8;
constexpr DWORD addressLen = 10;

extern HINSTANCE appInst;
extern int cmdShow;

using std::vector;
using std::array;
using std::wstring;
using petools::config::filename_max;
using petools::config::formatFilter;
using petools::show::ItemWidthAndName;
using petools::TcharToDword;

MainDlg* MainDlg::thisDlg_ = nullptr;

MainDlg::MainDlg(HWND hParent)
    : DialogEX(IDD_DIALOG_MAIN, hParent)
{
    hIcon_ = LoadIcon(appInst, MAKEINTRESOURCE(IDI_ICON_MAINICO));
}

MainDlg::~MainDlg() {

}

void MainDlg::InitDlg() {
    SetThisDlg();
    
    hCurrentDlg_ = CreateDialog(appInst, MAKEINTRESOURCE(idTemplate_), 0, (DLGPROC)MianProc);
    if (!hCurrentDlg_)
    {
        return;
    }
    
    InitProcessList();
    InitModuleList(); 
}

void MainDlg::Plant() {
    SendMessage(hCurrentDlg_, WM_SETICON, ICON_SMALL, (long)hIcon_);
    SendMessage(hCurrentDlg_, WM_SETICON, ICON_BIG, (long)hIcon_);

    processList_->PlantColumn();
    processList_->PlantItem();
    moduleList_->PlantColumn();

    ShowWindow(hCurrentDlg_, cmdShow);
    UpdateWindow(hCurrentDlg_);
}

void MainDlg::CloseDlg() {
    for (auto& dlg : dlg_list_) {
        auto name = typeid(*dlg).name();
        dlg.reset();
    }
    processList_.reset();
    moduleList_.reset();
    DestroyWindow(hCurrentDlg_);
    ExitProcess(0);
}

void MainDlg::InitProcessList() {
    processList_ = std::unique_ptr<ListCrl>(new ListCrl(GetDlgItem(hCurrentDlg_, IDC_LIST_PROCESS), 
        [&]() {PlantProcessColumn(); }, [&]() {PlantProcessItem(); }));
    processList_->Init(LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM | LVCF_FMT, LVIF_TEXT);
}

void MainDlg::InitModuleList() {
    moduleList_ = std::unique_ptr<ListCrl>(new ListCrl(GetDlgItem(hCurrentDlg_, IDC_LIST_MODULE),
        [&]() {PlantModuleColumn(); }, [&]() {PlantModuleItem(); }));
    moduleList_->Init(LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM, LVIF_TEXT);
}

void MainDlg::PlantProcessColumn() {
    array<ItemWidthAndName<DWORD, wstring>, 4> items = { {
		{ 190, L"Process" },
		{ 70, L"PID" },
		{ 90, L"Image Base" },
		{ 90, L"Image Size" }
	} };

    for (size_t i = 0; i < items.size(); i++) {
        processList_->SetColumn(items[i], i);
        SendMessage(processList_->GetList(), LVM_INSERTCOLUMN, i, processList_->GetColumnAddr());
    }
}

void MainDlg::PlantProcessItem() {
    LV_ITEM item;
    memset(&item, 0, sizeof(LV_ITEM));
    item.mask = LVIF_TEXT;

    PROCESSENTRY32 process;
    memset(&process, 0, sizeof(process));
    process.dwSize = sizeof(process);
    HANDLE processSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (processSnap != INVALID_HANDLE_VALUE) {
        MODULEENTRY32 module;
        memset(&module, 0, sizeof(module));
        module.dwSize = sizeof(module);

        bool isProc = Process32First(processSnap, &process);
        int row = 0;
        do {
            HANDLE moduleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, process.th32ProcessID);
            if (Module32First(moduleSnap, &module) == FALSE) {
                continue;
            }
            else {
                item.pszText = process.szExeFile;
                item.iItem = row;
                item.iSubItem = 0;
                SendMessage(processList_->GetList(), LVM_INSERTITEM, 0, (DWORD)&item);

                TCHAR pid[pidLen] = { 0 };
                wsprintfW(pid, L"%06d", process.th32ProcessID);
                item.pszText = pid;
                item.iItem = row;
                item.iSubItem = 1;
                ListView_SetItem(processList_->GetList(), &item);

                item.iItem = row;
                item.iSubItem = 2;
                TCHAR imageBase[addressLen];
                wsprintfW(imageBase, L"%08X", (DWORD)module.modBaseAddr);
                item.pszText = imageBase;
                ListView_SetItem(processList_->GetList(), &item);

                item.iItem = row;
                item.iSubItem = 3;
                TCHAR imageSize[addressLen];
                wsprintfW(imageSize, L"%08X", module.modBaseSize);
                item.pszText = imageSize;
                ListView_SetItem(processList_->GetList(), &item);

                CloseHandle(moduleSnap);
                row++;
            }
        } while (isProc = Process32Next(processSnap, &process));
    }
    CloseHandle(processSnap);
}

void MainDlg::PlantModuleColumn() {

    array<ItemWidthAndName<DWORD, wstring>, 2> items = { {
		{ 370, L"Module Name" },
		{ 90, L"Module Size" }
	} };

    for (size_t i = 0; i < items.size(); i++) {
        moduleList_->SetColumn(items[i], i);
        SendMessage(moduleList_->GetList(), LVM_INSERTCOLUMN, i, moduleList_->GetColumnAddr());
    }
}

void MainDlg::PlantModuleItem() {
    DWORD procListRow = SendMessage(processList_->GetList(), LVM_GETNEXTITEM, -1, LVNI_SELECTED);
    if (procListRow == -1) {
        return;
    }
    SendMessage(moduleList_->GetList(), LVM_DELETEALLITEMS, 0, 0);
    
    DWORD pid = GetPID(procListRow);
    HANDLE hModu = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);

    LV_ITEM item;
    memset(&item, 0, sizeof(item));
    item.mask = LVIF_TEXT;

    MODULEENTRY32 module;
    memset(&module, 0, sizeof(module));
    module.dwSize = sizeof(module);

    BOOL idModu = Module32First(hModu, &module);
    int row= 0;
    do {
        item.iItem = row;
        item.iSubItem = 0;
        item.pszText = module.szExePath;
        SendMessage(moduleList_->GetList(), LVM_INSERTITEM, 0, (DWORD)&item);

        item.iItem = row;
        item.iSubItem = 1;
        TCHAR imageSize[addressLen];
        wsprintfW(imageSize, L"%08X", module.modBaseSize);
        item.pszText = imageSize;
        ListView_SetItem(moduleList_->GetList(), &item);

        row++;
    } while (idModu = Module32Next(hModu, &module));

    CloseHandle(hModu);
}

void MainDlg::CreateAboutDlg() {
    CreateDlg<AboutDlg>(hCurrentDlg_, dlg_list_);
}

void MainDlg::CreatePeEditDlg() {
    TCHAR fileName[filename_max];
    memset(fileName, 0, filename_max);
    
    std::unique_ptr<PeEditDlg> peEditDlg;
    if (GetOpenFileNameEx(fileName)) {
        peEditDlg = std::make_unique<PeEditDlg>(hCurrentDlg_);
        peEditDlg->OpenFile(fileName);
        if (!peEditDlg->GetFileManage()->IsOpenFile()) {
            peEditDlg->CloseDlg();
            peEditDlg.reset();
            return;
        }

        AnalysePE::GetAnalyse().Init(peEditDlg->GetFileManage()->GetFile(), peEditDlg->GetFileManage()->GetFileSize());
        peEditDlg->InitDlg();
        peEditDlg->SetFileName(fileName);
        peEditDlg->Plant();
        dlg_list_.push_back(std::move(peEditDlg));
    }
    return;
}

void MainDlg::CreateProtectorDlg() {
    CreateDlg<ProtectorDlg>(hCurrentDlg_, dlg_list_);
}


DWORD MainDlg::GetPID(int rowIndex) {
    TCHAR pidBuffer[pidLen] = { 0 };
    LV_ITEM item;
    memset(&item, 0, sizeof(item));

    item.iSubItem = 1;
    item.pszText = pidBuffer;
    item.cchTextMax = pidLen;
    SendMessage(processList_->GetList(), LVM_GETITEMTEXT, rowIndex, (DWORD)&item);

    DWORD pid;
    TcharToDword(pidBuffer, &pid, 10);
    return pid;
    return 0;
}

BOOL MainDlg::GetOpenFileNameEx(TCHAR* fileName) {
    OPENFILENAME openFile;
    memset(&openFile, 0, sizeof(openFile));

    openFile.lStructSize = sizeof(OPENFILENAME);
    openFile.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
    openFile.hwndOwner = hParentDlg_;
    openFile.lpstrFilter = formatFilter;
    openFile.lpstrFile = fileName;
    openFile.nMaxFile = MAX_PATH;
    
    if (GetOpenFileName(&openFile) == FALSE) {
        return FALSE;
    }
    return TRUE;
}

LRESULT CALLBACK MainDlg::MianProc(HWND hMain, UINT message, WPARAM wParam, LPARAM lParam) {
    NMHDR* hdr = (NMHDR*)lParam;
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        switch (wmId)
        {
        case IDC_BUTTON_PEVIEW:
            thisDlg_->CreatePeEditDlg();
            break;
        case IDC_BUTTON_ABOUT:
            thisDlg_->CreateAboutDlg();
            break;
        case IDC_BUTTON_PROTECT:
            thisDlg_->CreateProtectorDlg();
            break;
        case IDC_BUTTON_QUIT:
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
        if (wParam == IDC_LIST_PROCESS && hdr->code == NM_CLICK) {
            thisDlg_->PlantModuleItem();
        }
        break;
    default:
        return FALSE;
    }
    return FALSE;
}