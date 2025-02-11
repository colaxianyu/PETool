//module;
//
//#include "resource.h"
//#include <windows.h>
//#include <typeinfo>
//
//module DirectoryDlg;
//
//import AnalysePE;
//import STL;
//
//using std::wstring;
//using std::array;
//
//extern HINSTANCE app_inst;
//
//DirectoryDlg* DirectoryDlg::this_dlg_ = nullptr;
//
//DirectoryDlg::DirectoryDlg(HWND h_parent)
//    : DialogEX(IDD_DIALOG_DIRECTORY, h_parent)
//{
//
//}
//
//DirectoryDlg::~DirectoryDlg() {
//
//}
//
//void DirectoryDlg::init_dlg() {
//    set_this_dlg();
//}
//
//void DirectoryDlg::plant() {
//    DialogBox(app_inst, MAKEINTRESOURCE(id_template_), h_parent_dlg_, (DLGPROC)DirProc);
//}
//
//void DirectoryDlg::close_dlg() {
//	for (auto& dlg : dlg_list_) {
//        auto name = typeid(*dlg).name();
//        dlg.reset();
//	}
//    EndDialog(h_current_dlg_, 0);
//}
//
//void DirectoryDlg::CreateExpertDlg() {
//    create_dlg<ExportDlg>(h_current_dlg_, dlg_list_, L"No Export Descriptor",  []() -> bool {
//        return AnalysePE::GetAnalyse().IsHaveExport();
//        });
//}
//
//void DirectoryDlg::CreateImportDlg() {
//    create_dlg<ImportDlg>(h_current_dlg_, dlg_list_, L"No Import Descriptor",  []() -> bool {
//        return AnalysePE::GetAnalyse().IsHaveImport();
//        });
//}
//
//void DirectoryDlg::CreateRelocationDlg() {
//    create_dlg<RelocationDlg>(h_current_dlg_, dlg_list_, L"No Relocation Descriptor",  []() -> bool {
//        return AnalysePE::GetAnalyse().IsHaveRelocation();
//        });
//}
//
//void DirectoryDlg::SetDirHeaderInfo() {
//    struct DirectoryInfo {
//        DWORD rva_id_;
//        DWORD size_id_;
//        DWORD directory_index_;
//
//    };
//
//    array<DirectoryInfo, 16> dir_info = { {
//        {IDC_EDIT_EXPORTRAV, IDC_EDIT_EXPORTSIZE, 0},
//        {IDC_EDIT_IMPORTRAV, IDC_EDIT_IMPORTSIZE, 1},
//        {IDC_EDIT_RESOURCERVA, IDC_EDIT_RESOURCESIZE, 2},
//        {IDC_EDIT_EXCEPTIONRVA, IDC_EDIT_EXCEPTIONSIZE, 3},
//        {IDC_EDIT_SECURITYRVA, IDC_EDIT_SECURITYSIZE, 4},
//        {IDC_EDIT_RELOCATIONRVA, IDC_EDIT_RELOCATIONSIZE, 5},
//        {IDC_EDIT_DEBUGRVA, IDC_EDIT_DEBUGSIZE, 6},
//        {IDC_EDIT_ASDRVA, IDC_EDIT_ASDSIZE, 7},
//        {IDC_EDIT_RVAGPRVA, IDC_EDIT_RVAGPSIZE, 8},
//        {IDC_EDIT_TLSRVA, IDC_EDIT_TLSSIZE, 9},
//        {IDC_EDIT_LOADRVA, IDC_EDIT_LOADSIZE, 10},
//        {IDC_EDIT_BOUNDRVA, IDC_EDIT_BOUNDSIZE, 11},
//        {IDC_EDIT_IATRVA, IDC_EDIT_IATSIZE, 12},
//        {IDC_EDIT_DELAYRVA, IDC_EDIT_DELAYSIZE, 13},
//        {IDC_EDIT_COMRVA, IDC_EDIT_COMSIZE, 14},
//        {IDC_EDIT_RESERVEDRVA, IDC_EDIT_RESERVEDSIZE, 15}
//    } };
//
//    for (const auto& info : dir_info) {
//        TCHAR rva[9] = { 0 };
//        TCHAR size[9] = { 0 };
//        wsprintfW(rva, L"%08X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->DataDirectory[info.directory_index_].VirtualAddress);
//        wsprintfW(size, L"%08X", AnalysePE::GetAnalyse().GetHeaders().optionalHeader->DataDirectory[info.directory_index_].Size);
//        SetDlgItemText(h_current_dlg_, info.rva_id_, rva);
//        SetDlgItemText(h_current_dlg_, info.size_id_, size);
//    }
//}
//
//LRESULT CALLBACK DirectoryDlg::DirProc(HWND hDir, UINT message, WPARAM w_param, LPARAM l_param) {
//    switch (message)
//    {
//    case WM_INITDIALOG:
//        this_dlg_->set_current_dlg_HWND(hDir);
//        this_dlg_->SetDirHeaderInfo();
//        break;
//    case WM_COMMAND:
//    {
//        int wmId = LOWORD(w_param);
//        switch (wmId)
//        {
//        case IDC_BUTTON_EXPORT:
//            this_dlg_->CreateExpertDlg();
//            break;
//        case IDC_BUTTON_IMPORT:
//            this_dlg_->CreateImportDlg();
//            break;
//        case IDC_BUTTON_RELOCATION:
//            this_dlg_->CreateRelocationDlg();
//            break;
//        case IDOK:
//            this_dlg_->close_dlg();
//            break;
//        }
//        break;
//    }
//    case WM_CLOSE:
//        this_dlg_->close_dlg();
//        break;
//    default:
//        return FALSE;
//    }
//    return FALSE;
//}
