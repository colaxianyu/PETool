//module;
//
//#include "resource.h"
//#include <windows.h>
//#include <commctrl.h>
//
//#define WM_DELETE_DLG (WM_APP + 1)
//
//module ImportDlg;
//
//import STL;
//import AnalysePE;
//
//using petools::show::ItemWidthAndName;
//using petools::CharToTchar;
//using petools::TcharToDword;
//using std::array;
//using std::wstring;
//
//extern HINSTANCE app_inst;
//
//ImportDlg* ImportDlg::this_dlg_ = nullptr;
//
//ImportDlg::ImportDlg(HWND h_parent)
//    : DialogEX(IDD_DIALOG_IMPORT, h_parent)
//{
//
//}
//
//ImportDlg::~ImportDlg() {
//
//}
//
//void ImportDlg::init_dlg() {
//    set_this_dlg();
//}
//
//void ImportDlg::plant() {
//    DialogBox(app_inst, MAKEINTRESOURCE(id_template_), h_parent_dlg_, (DLGPROC)ImportProc);
//}
//
//void ImportDlg::close_dlg() {
//	funcList_.reset();
//	mainList_.reset();
//	EndDialog(h_current_dlg_, 0);
//}
//
//void ImportDlg::InitMainList() {
//    mainList_ = std::unique_ptr<ListCtrl>(new ListCtrl(GetDlgItem(h_current_dlg_, IDC_LIST_IMP_MAIN)
//        , [&]() {plantMainColumn(); }, [&]() {plantMainItem(); }));
//    mainList_->init(LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM | LVCF_FMT, LVIF_TEXT);
//    mainList_->plant_column();
//    mainList_->plant_item();
//}
//
//void ImportDlg::plantMainColumn() {
//    array<ItemWidthAndName<DWORD, wstring>, 6> items = { {
//        { 120, L"Dll Name" },
//        { 160, L"Original First Thunk" },
//        { 120, L"Time Date Stamp" },
//        { 120, L"Forwarder Chain" },
//        { 80, L"Name" },
//        { 100, L"First Thunk" }
//    } };
//
//    for (size_t i = 0; i < items.size(); i++) {
//        mainList_->SetColumn(items[i], i);
//        SendMessage(mainList_->get_list_handle(), LVM_INSERTCOLUMN, i, mainList_->get_column_addr());
//    }
//}
//
//void ImportDlg::plantMainItem() {
//    IMAGE_IMPORT_DESCRIPTOR* tempImport = AnalysePE::GetAnalyse().GetImport();;
//
//    LV_ITEM item;
//    memset(&item, 0, sizeof(LV_ITEM));
//    
//
//    for (int i = 0; tempImport->OriginalFirstThunk != 0; i++, tempImport++) {
//        item.mask = LVIF_TEXT;
//        item.iItem = i;
//        item.iSubItem = 0;     
//        char* dllName = (char*)((DWORD)AnalysePE::GetAnalyse().GetHeaders().dosHeader
//            + AnalysePE::GetAnalyse().RVAToFOA(tempImport->Name));
//        TCHAR* tDllName = nullptr;
//        CharToTchar(dllName, &tDllName);
//        item.pszText = tDllName;
//        SendMessage(mainList_->get_list_handle(), LVM_INSERTITEM, 0, (DWORD)&item);
//
//        item.mask = LVIF_TEXT;
//        item.iItem = i;
//        item.iSubItem = 1;
//        TCHAR tOriFirstThunk[9] = { 0 };
//        wsprintf(tOriFirstThunk, L"%08X", tempImport->OriginalFirstThunk);
//        item.pszText = tOriFirstThunk;
//        ListView_SetItem(mainList_->get_list_handle(), (DWORD)&item);
//
//        item.iItem = i;
//        item.iSubItem = 2;
//        TCHAR tTimeStamp[9] = { 0 };
//        wsprintf(tTimeStamp, L"%08X", tempImport->TimeDateStamp);
//        item.pszText = tTimeStamp;
//        ListView_SetItem(mainList_->get_list_handle(), (DWORD)&item);
//
//        item.iItem = i;
//        item.iSubItem = 3;
//        TCHAR tChain[9] = { 0 };
//        wsprintf(tChain, L"%08X", tempImport->ForwarderChain);
//        item.pszText = tChain;
//        ListView_SetItem(mainList_->get_list_handle(), (DWORD)&item);
//
//        item.iItem = i;
//        item.iSubItem = 4;
//        TCHAR tNameOffset[9] = { 0 };
//        wsprintf(tNameOffset, L"%08X", tempImport->Name);
//        item.pszText = tNameOffset;
//        ListView_SetItem(mainList_->get_list_handle(), (DWORD)&item);
//
//        item.iItem = i;
//        item.iSubItem = 5;
//        TCHAR tFirstThunk[9] = { 0 };
//        wsprintf(tFirstThunk, L"%08X", tempImport->FirstThunk);
//        item.pszText = tFirstThunk;
//        ListView_SetItem(mainList_->get_list_handle(), (DWORD)&item);
//    }
//}
//
//void ImportDlg::InitFuncList() {
//    funcList_ = std::unique_ptr<ListCtrl>(new ListCtrl(GetDlgItem(h_current_dlg_, IDC_LIST_IMP_FUNC)
//        , [&]() {plantFuncColumn(); }, [&]() {plantFuncItem(); }));
//    funcList_->init(LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM | LVCF_FMT, LVIF_TEXT);
//    funcList_->plant_column();
//}
//
//void ImportDlg::plantFuncColumn() {
//    array<ItemWidthAndName<DWORD, wstring>, 5> items = { {
//		{ 320, L"API Name" },
//		{ 80, L"Thunk RVA" },
//		{ 100, L"Thunk Offset" },
//		{ 100, L"Thunk Value" },
//		{ 80, L"Hint" }
//	} };
//
//    for (size_t i = 0; i < items.size(); i++) {
//        funcList_->SetColumn(items[i], i);
//        SendMessage(funcList_->get_list_handle(), LVM_INSERTCOLUMN, i, funcList_->get_column_addr());
//    }
//}
//
//void ImportDlg::plantFuncItem() {
//    DWORD mainListRow = SendMessage(mainList_->get_list_handle(), LVM_GETNEXTITEM, -1, LVNI_SELECTED);
//    if (mainListRow == -1) {
//        return;
//    }
//    SendMessage(funcList_->get_list_handle(), LVM_DELETEALLITEMS, 0, 0);
//
//    LV_ITEM item;
//    memset(&item, 0, sizeof(LV_ITEM));
// 
//    DWORD firstThunk = GetFirstThunkFromMainList(mainListRow);
//
//    // INT与IAT都是双字大小的Union，直接定义为DWORD*方便操作
//    DWORD* tempIAT = (DWORD*)((DWORD)AnalysePE::GetAnalyse().GetHeaders().dosHeader
//        + AnalysePE::GetAnalyse().RVAToFOA(firstThunk));
//
//    for (int j = 0; (*tempIAT) != 0; j++, tempIAT++) {
//        IMAGE_IMPORT_BY_NAME* tempByName = (IMAGE_IMPORT_BY_NAME*)((DWORD)AnalysePE::GetAnalyse().GetHeaders().dosHeader
//            + AnalysePE::GetAnalyse().RVAToFOA(*tempIAT));
//
//        item.mask = LVIF_TEXT;
//        item.iItem = j;
//        item.iSubItem = 0;
//        char* APIName = tempByName->Name;
//        TCHAR* tAPIName = nullptr;
//        CharToTchar(APIName, &tAPIName);
//        item.pszText = tAPIName;
//        SendMessage(funcList_->get_list_handle(), LVM_INSERTITEM, 0, (DWORD)&item);
//
//        item.iItem = j;
//        item.iSubItem = 1;
//        TCHAR tThunkRVA[9] = { 0 };
//        wsprintf(tThunkRVA, L"%08X", firstThunk + j * 4);
//        item.pszText = tThunkRVA;
//        ListView_SetItem(funcList_->get_list_handle(), (DWORD)&item);
//
//        item.iItem = j;
//        item.iSubItem = 2;
//        TCHAR tThunkOffset[9] = { 0 };
//        wsprintf(tThunkOffset, L"%08X", AnalysePE::GetAnalyse().RVAToFOA(firstThunk + j * 4));
//        item.pszText = tThunkOffset;
//        ListView_SetItem(funcList_->get_list_handle(), (DWORD)&item);
//
//        item.iItem = j;
//        item.iSubItem = 3;
//        TCHAR tThunkValue[9] = { 0 };
//        wsprintf(tThunkValue, L"%08X", *tempIAT);
//        item.pszText = tThunkValue;
//        ListView_SetItem(funcList_->get_list_handle(), (DWORD)&item);
//
//        item.iItem = j;
//        item.iSubItem = 4;
//        TCHAR tHint[5] = { 0 };
//        wsprintf(tHint, L"%04X", tempByName->Hint);
//        item.pszText = tHint;
//        ListView_SetItem(funcList_->get_list_handle(), (DWORD)&item);
//    }
//}
//
//DWORD ImportDlg::GetFirstThunkFromMainList(DWORD rowID) {
//    TCHAR firstThunkBuffer[9] = { 0 };
//    LV_ITEM item;
//    memset(&item, 0, sizeof(item));
//
//    item.iSubItem = 1;
//    item.pszText = firstThunkBuffer;
//    item.cchTextMax = 9;
//    SendMessage(mainList_->get_list_handle(), LVM_GETITEMTEXT, rowID, (DWORD)&item);
//
//
//    DWORD firstThunk = 0;
//    TcharToDword(firstThunkBuffer, &firstThunk, 16);
//    return firstThunk;
//}
//
//LRESULT CALLBACK ImportDlg::ImportProc(HWND hImport, UINT message, WPARAM w_param, LPARAM l_param) {
//    NMHDR* hdr = (NMHDR*)l_param;
//    switch (message)
//    {
//    case WM_INITDIALOG:
//        this_dlg_->set_current_dlg_HWND(hImport);
//        this_dlg_->InitMainList();
//        this_dlg_->InitFuncList();
//        break;
//    case WM_COMMAND:
//    {
//        int wmId = LOWORD(w_param);
//
//        switch (wmId) {
//        case IDOK:
//            this_dlg_->close_dlg();
//            break;
//        default:
//            break;
//        }
//        break;
//    }
//    case WM_CLOSE:
//        this_dlg_->close_dlg();
//        break;
//    case WM_NOTIFY:
//        if (w_param == IDC_LIST_IMP_MAIN && hdr->code == NM_CLICK) {
//            this_dlg_->plantFuncItem();
//        }
//        break;
//    default:
//        return FALSE;
//    }
//    return FALSE;
//}
