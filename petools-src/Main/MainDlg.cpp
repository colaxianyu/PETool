module;

#include <windows.h>
#include <Tlhelp32.h>
#include <WtsApi32.h>
#include <CommCtrl.h>
#include "resource.h"
#include "spdlog/spdlog.h"

#pragma comment(lib, "WtsApi32.lib")

module MainDlg;

//// import AnalysePE;
import STL;
//import DialogManager;
//import FileManager;
////import AboutDlg;
////import PeEditDlg;
////import ProtectorDlg;

static constexpr size_t process_column_length = 180;
static constexpr size_t pid_column_length = 80;
static constexpr size_t image_base_column_length = 90;
static constexpr size_t image_size_column_length = 90;

static constexpr size_t module_name_column_length = 360;
static constexpr size_t module_size_column_length = 100;

static constexpr size_t pid_hex_width = 6;
static constexpr size_t image_base_hex_width = 8;
static constexpr size_t image_size_hex_width = 8;

static constexpr size_t module_size_hex_width = 8;

using std::vector;
using std::array;
using std::wstring;
using tools::config::filename_max;
using tools::config::format_filter;
using tools::wstring_to_dword;
using tools::to_wstring_hex;

void MainDlg::init_dialog_impl() {
    init_process_list();
    init_module_list();
}

void MainDlg::show_dialog_impl() {
    SendMessage(h_current_dlg_, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(h_icon_.get()));
    SendMessage(h_current_dlg_, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(h_icon_.get()));

    process_list_->plant_column();
    process_list_->plant_item();
    module_list_->plant_column();

    ShowWindow(h_current_dlg_, cmd_show_);
    UpdateWindow(h_current_dlg_);
}

void MainDlg::close_dialog_impl() {
    process_list_.reset();
    module_list_.reset();

    PostQuitMessage(0);
}

void MainDlg::init_process_list() {
    process_list_ = std::make_unique<ListCtrl>(
        GetDlgItem(h_current_dlg_, IDC_LIST_PROCESS),
        [&]() {plant_process_column(); },
        [&]() {plant_process_item(); }
    );
    process_list_->init(LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM | LVCF_FMT, LVIF_TEXT);
}

void MainDlg::init_module_list() {
    module_list_ = std::make_unique<ListCtrl>(
        GetDlgItem(h_current_dlg_, IDC_LIST_MODULE),
        [&]() {plant_module_column(); },
        [&]() {plant_module_item(); }
    );
    module_list_->init(LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM, LVIF_TEXT);
}

void MainDlg::plant_process_column() {
    array<ColumnDefinition, 4> items = { {
		{ process_column_length, L"Process" },
		{ pid_column_length, L"PID" },
		{ image_base_column_length, L"Image Base" },
		{ image_size_column_length, L"Image Size" }
	} };

    process_list_->set_column(items);
}

void MainDlg::plant_process_item() {
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

    while (Process32Next(process_snap, &process)) {
        HANDLE module_snap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, process.th32ProcessID);

        if (module_snap != INVALID_HANDLE_VALUE && Module32First(module_snap, &module)) {
            std::vector<ItemDefinition> items;
            items.push_back(ItemDefinition{ 0, process.szExeFile });
            items.push_back(ItemDefinition{ 1, to_wstring_hex(process.th32ProcessID, pid_hex_width) });
            items.push_back(ItemDefinition{ 2, to_wstring_hex(reinterpret_cast<DWORD>(module.modBaseAddr), image_base_hex_width) });
            items.push_back(ItemDefinition{ 3, to_wstring_hex(module.modBaseSize, image_size_hex_width) });

            process_list_->set_item(items, row);
            CloseHandle(module_snap);
            row++;
        }
    }

    CloseHandle(process_snap);
}

void MainDlg::plant_module_column() {
    array<ColumnDefinition, 2> items = { {
		{ module_name_column_length, L"Module Name" },
		{ module_size_column_length, L"Module Size" }
	} };

    module_list_->set_column(items);
}

void MainDlg::plant_module_item() {
    DWORD process_list_row = SendMessage(process_list_->get_list_handle(), LVM_GETNEXTITEM, -1, LVNI_SELECTED);
    if (process_list_row == -1) {
        return;
    }

    SendMessage(module_list_->get_list_handle(), LVM_DELETEALLITEMS, 0, 0);
    
	std::optional<DWORD> pid_result = get_pid(process_list_row);
	if (!pid_result.has_value()) {
		return;
	}
    HANDLE h_module = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid_result.value());

    MODULEENTRY32 module;
    memset(&module, 0, sizeof(module));
    module.dwSize = sizeof(module);
    
    int row= 0;

    while (Module32Next(h_module, &module)) {
        std::vector<ItemDefinition> items;
        items.push_back(ItemDefinition{ 0, module.szExePath });
        items.push_back(ItemDefinition{ 1, to_wstring_hex(module.modBaseSize, module_size_hex_width) });

        module_list_->set_item(items, row);
        row++;
    }

    CloseHandle(h_module);
}

std::optional<DWORD> MainDlg::get_pid(INT row_index) noexcept {
	std::wstring pid_str(pid_hex_width + 1, L'\0');
    LV_ITEM item;
    memset(&item, 0, sizeof(item));

    item.iSubItem = 1;
    item.pszText = &pid_str[0];
    item.cchTextMax = pid_hex_width + 1;
    SendMessage(process_list_->get_list_handle(), LVM_GETITEMTEXT, row_index, reinterpret_cast<LPARAM>(&item));

    return wstring_to_dword(pid_str, numeric_base::hex{});
}

LRESULT MainDlg::dialog_proc_impl(HWND h_about, UINT message, WPARAM w_param, LPARAM l_param) {
    NMHDR* hdr = (NMHDR*)l_param;

    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(w_param);
        switch (wmId)
        {
        case IDC_BUTTON_PEVIEW:
            //create_pe_edit_dlg();
            break;
        case IDC_BUTTON_ABOUT:
            //create_about_dlg();
            break;
        case IDC_BUTTON_PROTECT:
            //create_protector_dlg();
            break;
        case IDC_BUTTON_QUIT:
            //dialog_mgr.change_state(template_id_, DialogEvent::Close);
            break;
        default:
            break;
        }
        break;
    }
    case WM_CLOSE:
		//dialog_mgr.change_state(template_id_, DialogEvent::Close);
        break;
    case WM_NOTIFY:
        if (w_param == IDC_LIST_PROCESS && hdr->code == NM_CLICK) {
            plant_module_item();
        }
        break;
    default:
        return FALSE;
    }
    return FALSE;
}