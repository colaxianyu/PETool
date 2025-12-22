module;

#include <windows.h>
#include <Tlhelp32.h>
//#include <WtsApi32.h>
#include <CommCtrl.h>
#include "resources/resource.h"

#pragma comment(lib, "WtsApi32.lib")

module MainDlg;

//// import AnalysePE;
//import FileManager;
////import AboutDlg;
////import ProtectorDlg;

//import ListControl;
import PeEditDlg;
import AboutDlg;
import DialogManager;
import Concept;
import Utility;

using std::vector;
using std::array;
using std::wstring;
using std::size_t;
using std::optional;
using std::make_unique;

using petools::utility::wstring_to_string;
using petools::utility::to_wstring_hex;

using tools::config::filename_max;
using tools::config::format_filter;

struct process_showing_config {
    static constexpr size_t process_column_length = 180;
    static constexpr size_t pid_column_length = 80;
    static constexpr size_t image_base_column_length = 90;
    static constexpr size_t image_size_column_length = 90;

    static constexpr size_t pid_hex_width = 6;
    static constexpr size_t image_base_hex_width = 8;
    static constexpr size_t image_size_hex_width = 8;
};

struct module_showing_config {
    static constexpr size_t module_name_column_length = 360;
    static constexpr size_t module_size_column_length = 100;

    static constexpr size_t module_size_hex_width = 8;
};

namespace petools::gui {

    void MainDlg::InitDialog() {
        auto handle = LoadIcon(GetInstance(), MAKEINTRESOURCE(IDI_ICON_MAINICO));
        icon_ = make_handle<HICON, decltype(&DestroyIcon)>(handle);
        SendMessage(current_hwnd_.get(), WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(icon_.get()));
        SendMessage(current_hwnd_.get(), WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(icon_.get()));
        InitProcessList();
        InitModuleList();
    }

    void MainDlg::ShowDialog() {
        process_list_->plant_column();
        process_list_->plant_item();
        module_list_->plant_column();

        ShowWindow(current_hwnd_.get(), GetCmdShow());
        UpdateWindow(current_hwnd_.get());
    }

    void MainDlg::InitProcessList() noexcept {
        process_list_ = make_unique<ListCtrl>(
            GetDlgItem(current_hwnd_.get(), IDC_LIST_PROCESS),
            [&]() {PlantProcessColumn(); },
            [&]() {PlantProcessItem(); }
        );
        process_list_->init(LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM | LVCF_FMT, LVIF_TEXT);
    }

    void MainDlg::InitModuleList() noexcept {
        module_list_ = make_unique<ListCtrl>(
            GetDlgItem(current_hwnd_.get(), IDC_LIST_MODULE),
            [&]() {PlantModuleColumn(); },
            [&]() {PlantModuleItem(); }
        );
        module_list_->init(LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM, LVIF_TEXT);
    }

    void MainDlg::PlantProcessColumn() noexcept {
        array<column_definition, 4> items = { {
            { process_showing_config::process_column_length, L"Process" },
            { process_showing_config::pid_column_length, L"PID" },
            { process_showing_config::image_base_column_length, L"Image Base" },
            { process_showing_config::image_size_column_length, L"Image Size" }
        } };

        process_list_->set_column(items);
    }

    void MainDlg::PlantProcessItem() noexcept {
        PROCESSENTRY32 process;
        memset(&process, 0, sizeof(process));
        process.dwSize = sizeof(process);
        HANDLE process_snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

        if (process_snap == INVALID_HANDLE_VALUE) {
            return;
        }

        MODULEENTRY32 module;
        memset(&module, 0, sizeof(module));
        module.dwSize = sizeof(module);

        std::size_t row = 0;

        if (Process32First(process_snap, &process)) {
            do {
                HANDLE module_snap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, process.th32ProcessID);
                if (module_snap == INVALID_HANDLE_VALUE) {
                    continue;
                }

                if (Module32First(module_snap, &module)) {
                    std::vector<item_definition> items;
                    items.push_back(item_definition{ 0, process.szExeFile });
                    items.push_back(item_definition{ 1, to_wstring_hex(process.th32ProcessID, process_showing_config::pid_hex_width) });
                    items.push_back(item_definition{ 2, to_wstring_hex(reinterpret_cast<DWORD>(module.modBaseAddr), process_showing_config::image_base_hex_width) });
                    items.push_back(item_definition{ 3, to_wstring_hex(module.modBaseSize, process_showing_config::image_size_hex_width) });

                    process_list_->set_item(items, row++);
                }

                CloseHandle(module_snap);
            } while (Process32Next(process_snap, &process));
        }

        CloseHandle(process_snap);
    }

    void MainDlg::PlantModuleColumn() noexcept {
        array<column_definition, 2> items = { {
            { module_showing_config::module_name_column_length, L"Module Name" },
            { module_showing_config::module_size_column_length, L"Module Size" }
        } };

        module_list_->set_column(items);
    }

    void MainDlg::PlantModuleItem() noexcept {
        DWORD process_list_row = SendMessage(process_list_->get_list_handle(), LVM_GETNEXTITEM, -1, LVNI_SELECTED);
        if (process_list_row == -1) {
            return;
        }

        SendMessage(module_list_->get_list_handle(), LVM_DELETEALLITEMS, 0, 0);

        std::optional<DWORD> pid_result = GetPid(process_list_row);
        if (!pid_result.has_value()) {
            return;
        }
        HANDLE h_module = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid_result.value());
        if (h_module == INVALID_HANDLE_VALUE) {
            return;
        }

        MODULEENTRY32 module;
        memset(&module, 0, sizeof(module));
        module.dwSize = sizeof(module);

        int row = 0;

        if (Module32First(h_module, &module)) {
            do {
                std::vector<item_definition> items;
                items.push_back(item_definition{ 0, module.szExePath });
                items.push_back(item_definition{ 1, to_wstring_hex(module.modBaseSize, module_showing_config::module_size_hex_width) });

                module_list_->set_item(items, row++);
            } while (Module32Next(h_module, &module));
        }

        CloseHandle(h_module);
    }

    optional<DWORD> MainDlg::GetPid(INT row_index) noexcept {
        std::wstring pid_str(process_showing_config::pid_hex_width + 1, L'\0');
        LV_ITEM item;
        memset(&item, 0, sizeof(item));

        item.iSubItem = 1;
        item.pszText = &pid_str[0];
        item.cchTextMax = process_showing_config::pid_hex_width + 1;
        SendMessage(process_list_->get_list_handle(), LVM_GETITEMTEXT, row_index, reinterpret_cast<LPARAM>(&item));

        return petools::utility::wstring_to_dword(pid_str, numeric_base::hex{});
    }

    bool MainDlg::OnCommand(WORD id, WORD code, HWND ctrl) {
        switch (id)
        {
        case IDC_BUTTON_PEVIEW:
        {
            optional<std::string> file_name_result = petools::utility::choose_file(current_hwnd_.get());
            if (!file_name_result.has_value()) {
                return true;
            }
            // DialogMgr().OpenDialog<PeEditDlg>(GetCurrentHWND(), file_name_result.value());
            return true;
        }
        case IDC_BUTTON_ABOUT:
        {
            DialogMgr().OpenDialog<AboutDlg>(GetCurrentHWND());
            return true;
        }
        case IDC_BUTTON_PROTECT:
        {
            //create_protector_dlg();
            return true;
        }
        case IDC_BUTTON_QUIT:
        {
            DialogMgr().CloseDialog();
            return true;
        }
        default:
            break;
        }
        return false;
    }

    LRESULT MainDlg::OnOtherMessage(UINT msg, WPARAM w_param, LPARAM l_param) {
        switch (msg) {
        case WM_NOTIFY: 
        {
			NMHDR* hdr = (NMHDR*)l_param;
            if (w_param == IDC_LIST_PROCESS && hdr->code == NM_CLICK) {
                PlantModuleItem();
                return true;
            }
        }
        default:
			break;
        }
        return false;
    }

    //LRESULT MainDlg::HandleMessage(const WindowHandle& h_dlg, UINT message, WPARAM w_param, LPARAM l_param) {
    //    NMHDR* hdr = (NMHDR*)l_param;
    //    switch (message)
    //    {
    //    case WM_COMMAND:
    //    {
    //        int wmId = LOWORD(w_param);
    //        std::optional<std::string> file_name_result = std::make_optional<std::string>();
    //        switch (wmId)
    //        {
    //        case IDC_BUTTON_PEVIEW:
    //            file_name_result = tool::choose_file(current_hwnd_.get());
    //            if (!file_name_result.has_value()) {
    //                break;
    //            }
				////dialog_mgr.open_dialog<PeEditDlg>(current_hwnd_.borrow(), file_name_result.value());
    //            break;
    //        case IDC_BUTTON_ABOUT:
    //            DialogMgr().OpenDialog<AboutDlg>(current_hwnd_.borrow());
    //            break;
    //        case IDC_BUTTON_PROTECT:
    //            //create_protector_dlg();
    //            break;
    //        case IDC_BUTTON_QUIT:
    //            //dialog_mgr.close_dialog();
    //            break;
    //        default:
    //            break;
    //        }
    //        break;
    //    }
    //    case WM_CLOSE:
    //        //dialog_mgr.close_dialog();
    //        break;
    //    case WM_NOTIFY:
    //        if (w_param == IDC_LIST_PROCESS && hdr->code == NM_CLICK) {
    //            PlantModuleItem();
    //        }
    //        break;
    //    default:
    //        return FALSE;
    //    }
    //    //return FALSE;
    //}

} //namespace petools