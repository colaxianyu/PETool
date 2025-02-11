//module;
//
//#include "resource.h"
//#include <string>
//#include <windows.h>
//#include <iostream>
//
//module PeEditDlg;
//
//import DialogEX;
//import DialogManager;
//import STL;
////import FileManage;
////import AnalysePE;
//
//using tools::show::SetDlgItemText_t;
//using tools::config::filename_max;
//using tools::CharToTchar;
//using tools::TcharToChar;
//
//extern HINSTANCE app_inst;
//
//constexpr TCHAR editDlgTitle[10] = TEXT("PE±à¼­Æ÷ - ");
//
//PeEditDlg::PeEditDlg(INT template_id, HWND h_parent) noexcept
//    : DialogEX(template_id, h_parent){
//
//}
//
//PeEditDlg::~PeEditDlg() noexcept = default;
//
//void PeEditDlg::init_dialog() {
//	file_manager_ = std::make_unique<FileManager>();
//}
//
//void PeEditDlg::show_dialog() {
//    ShowWindow(h_current_dlg_, cmd_show_);
//    UpdateWindow(h_current_dlg_);
//}
//
//void PeEditDlg::close_dialog() {
//    if (DialogManager::instance().find_dialog(get_template_id()).get()) {
//        DialogManager::instance().unregister_dialog(get_template_id());
//    }
//}
//
////void PeEditDlg::SetFileName(TCHAR* fileName) {
////    wsprintfW(fileName_, L"%s", fileName);
////}
//
////void PeEditDlg::OpenFile(const TCHAR* filePath, const char* mode) {
////    fileManage_ = std::make_unique<FileManage>(filePath, mode);
////}
//
////void PeEditDlg::SetEditTitle() {
////    TCHAR tempTitle[280] = { 0 };
////    lstrcat(tempTitle, editDlgTitle);
////    lstrcat(tempTitle, fileName_);
////    SetWindowText(h_current_dlg_, tempTitle);
////}
//
////void PeEditDlg::SetPEMainInfo() {
////    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_ENTRY, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->AddressOfEntryPoint, 8);
////    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_IMAGEBASE, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->ImageBase, 8);
////    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_IMAGESIZE, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->SizeOfImage, 8);
////    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_CODEBASE, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->BaseOfCode, 8);
////    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_DATABASE, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->BaseOfData, 8);
////    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_SECTIONALIGN, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->SectionAlignment, 8);
////    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_FILEALIGN, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->FileAlignment, 8);
////    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_HEADERSIZE, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->SizeOfHeaders, 8);
////    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_OPTHEADERSIZE, AnalysePE::GetAnalyse().GetHeaders().fileHeader->SizeOfOptionalHeader, 8);
////    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_SECTIONSIZE, AnalysePE::GetAnalyse().GetHeaders().fileHeader->NumberOfSections, 8);
////    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_CHARACT, AnalysePE::GetAnalyse().GetHeaders().fileHeader->Characteristics, 8);
////    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_TIMESTAMP, AnalysePE::GetAnalyse().GetHeaders().fileHeader->TimeDateStamp, 8);
////    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_SUBSYS, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->Subsystem, 8);
////    SetDlgItemText_t(h_current_dlg_, IDC_EDIT_DIRSIZE, AnalysePE::GetAnalyse().GetHeaders().optionalHeader->NumberOfRvaAndSizes, 8);
////}
//
////void PeEditDlg::CreateFileDlg() {
////    create_dlg<FileHeaderDlg>(h_current_dlg_, dlg_list_);
////};
////
////void PeEditDlg::CreateOptDlg() {
////    create_dlg<OptHeaderDlg>(h_current_dlg_, dlg_list_);
////}
////
////void PeEditDlg::CreateSecDlg() {
////    create_dlg<SecHeaderDlg>(h_current_dlg_, dlg_list_);
////}
////
////void PeEditDlg::CreateDirDlg() {
////    create_dlg<DirectoryDlg>(h_current_dlg_, dlg_list_);
////}
////
////void PeEditDlg::CreateCalcDlg() {
////    create_dlg<CalculateDlg>(h_current_dlg_, dlg_list_);
////}
////
////void PeEditDlg::CreateInjectImportDlg() {
////    create_dlg<InjectImportDlg>(h_current_dlg_, dlg_list_);
////}
//
////void PeEditDlg::SaveAsFile() {
////    OPENFILENAME openFile = {0};
////
////    TCHAR* original_path = nullptr;
////    CharToTchar(fileManage_->GetFilePath(), &original_path);
////
////    TCHAR new_path[MAX_PATH] = { 0 };
////    if (original_path != nullptr) {
////        TCHAR* dot_pos = wcsrchr(original_path, '.');
////        if (dot_pos != nullptr) {
////            size_t insert_pos = dot_pos - original_path;
////            wcsncpy_s(new_path, original_path, insert_pos);
////            wcscat_s(new_path, MAX_PATH, TEXT("(2)"));
////            wcscat_s(new_path, MAX_PATH, dot_pos);
////        }
////        else {
////            wcsncpy_s(new_path, original_path, MAX_PATH - 4);
////            wcscat_s(new_path, MAX_PATH, TEXT("(2)"));
////        }
////    }
////
////    openFile.lStructSize = sizeof(OPENFILENAME);
////    openFile.Flags = OFN_HIDEREADONLY | OFN_PATHMUSTEXIST;
////    openFile.FlagsEx = OFN_EX_NOPLACESBAR;
////    openFile.hwndOwner = h_parent_dlg_;
////    openFile.lpstrFilter = TEXT("*.exe\0\0*.dll\0");
////    openFile.lpstrFile = new_path;
////    openFile.nMaxFile = MAX_PATH;
////    GetSaveFileName(&openFile);
////
////    char* path = nullptr;
////    TcharToChar(new_path, &path);
////    fileManage_->SaveAsFile(AnalysePE::GetAnalyse().GetFileBuffer(), AnalysePE::GetAnalyse().GetFileBufferSzie(), path);
////
////}
//
//LRESULT PeEditDlg::dialog_proc(HWND h_edit, UINT message, WPARAM w_param, LPARAM l_param) {
//    NMHDR* phdr = (NMHDR*)l_param;
//    switch (message)
//    {
//    case WM_INITDIALOG:
//        //this_dlg_->set_current_dlg_HWND(hEdit);
//        //this_dlg_->SetEditTitle();
//        //this_dlg_->SetPEMainInfo();
//        break;
//    case WM_COMMAND:
//    {
//        int wmId = LOWORD(w_param);
//
//        switch (wmId)
//        {
//        case IDC_BUTTON_FILEHEADER:
//            //this_dlg_->CreateFileDlg();
//            break;
//        case IDC_BUTTON_OPTHEADER:
//            //this_dlg_->CreateOptDlg();
//            break;
//        case IDC_BUTTON_SECTIONS:
//            //this_dlg_->CreateSecDlg();
//            break;
//        case IDC_BUTTON_DIRECTORIES:
//            //this_dlg_->CreateDirDlg();
//            break;
//        case IDC_BUTTON_CALCFOA:
//            //this_dlg_->CreateCalcDlg();
//            break;
//        case IDC_BUTTON_INJECT:
//            //this_dlg_->CreateInjectImportDlg();
//            break;
//        case IDC_BUTTON_SAVE:
//            //this_dlg_->SaveAsFile();
//            break;
//        case IDOK:
//            //AnalysePE::GetAnalyse().UnloadPeData();
//
//            dialog_mgr.change_state(template_id_, DialogEvent::Close);
//            break;
//        }
//        break;
//    }
//    case WM_CLOSE: 
//        //AnalysePE::GetAnalyse().UnloadPeData();
//        dialog_mgr.change_state(template_id_, DialogEvent::Close);
//        break;
//    default:
//        return FALSE;
//    }
//    return FALSE;
//}