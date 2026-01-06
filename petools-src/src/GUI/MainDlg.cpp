module;

#include <windows.h>
#include <CommCtrl.h>
#include "resources/resource.h"

module MainDlg;

//// import AnalysePE;
//import FileManager;
////import ProtectorDlg;

import PeEditDlg;
import AboutDlg;
import DialogManager;
import Concept;

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
        icon_.reset(handle);
        SendMessage(current_hwnd_.get(), WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(icon_.get()));
        SendMessage(current_hwnd_.get(), WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(icon_.get()));
        InitProcessList();
        InitModuleList();
    }

    void MainDlg::ShowDialog() {
        PlantProcessColumn();
        PlantProcessItem();
        PlantModuleColumn();

        ShowWindow(current_hwnd_.get(), GetCmdShow());
        UpdateWindow(current_hwnd_.get());
    }

    void MainDlg::InitProcessList() noexcept {
        process_list_ = std::make_unique<ListCtrl>(GetDlgItem(current_hwnd_.get(), IDC_LIST_PROCESS));
        auto process_result =  process_list_->Init(LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM | LVCF_FMT, LVIF_TEXT);
        if (!process_result.has_value()) {
			// TODO: log error
        }
    }

    void MainDlg::InitModuleList() noexcept {
        module_list_ = std::make_unique<ListCtrl>(GetDlgItem(current_hwnd_.get(), IDC_LIST_MODULE));
        auto module_result = module_list_->Init(LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM, LVIF_TEXT);
        if (!module_result.has_value()) {
			// TODO: log error
        }
    }

    void MainDlg::PlantProcessColumn() noexcept {
        std::array<ColumnDefinition, 4> items = { {
            { process_showing_config::process_column_length, L"Process" },
            { process_showing_config::pid_column_length, L"PID" },
            { process_showing_config::image_base_column_length, L"Image Base" },
            { process_showing_config::image_size_column_length, L"Image Size" }
        } };

        auto set_column_result = process_list_->SetColumn(items);
        if (!set_column_result.has_value()) {
			// TODO: log error
        }
    }

    void MainDlg::PlantProcessItem() noexcept {

        auto processes = petools::utility::EnumProcesses();

        std::size_t row = 0;
        for (const auto& p : processes) {
            std::vector<ItemDefinition> items;
            items.push_back({ 0, p.exe_name });
            items.push_back({ 1, to_wstring_hex(p.pid,        process_showing_config::pid_hex_width) });
            items.push_back({ 2, to_wstring_hex(p.image_base, process_showing_config::image_base_hex_width) });
            items.push_back({ 3, to_wstring_hex(p.image_size, process_showing_config::image_size_hex_width) });

            auto set_item_result = process_list_->SetItem(items, row++);
            if (!set_item_result.has_value()) {
				// TODO: log error
            }
        }
    }

    void MainDlg::PlantModuleColumn() noexcept {
        std::array<ColumnDefinition, 2> items = { {
            { module_showing_config::module_name_column_length, L"Module Name" },
            { module_showing_config::module_size_column_length, L"Module Size" }
        } };

        auto set_solumn_result = module_list_->SetColumn(items);
        if (!set_solumn_result.has_value()) {
			// TODO: log error
        }
    }

    void MainDlg::PlantModuleItem() noexcept {

        auto process_list_row = process_list_->SelectedIndex();
        if (!process_list_row.has_value()) {
			// TODO: log error
            return;
        }

        auto clear_result = module_list_->ClearItems();
        if (!clear_result.has_value()) {
			// TODO: log error
            return; 
        }

        auto pid_result = GetPid(process_list_row.value());
        if (!pid_result.has_value()) {
            return;
        }

        auto modules = petools::utility::EnumModules(pid_result.value());

        int row = 0;
        for (const auto& m : modules) {
            std::vector<ItemDefinition> items;
            items.push_back({ 0, m.path });
            items.push_back({ 1, to_wstring_hex(m.size, module_showing_config::module_size_hex_width) });

            auto set_item_result = module_list_->SetItem(items, row++);
            if (!set_item_result.has_value()) {
				// TODO: log error
            }
        }
    }

	std::expected<DWORD, petools::utility::PidParseError> MainDlg::GetPid(INT row_index) noexcept {
		std::wstring pid_str(process_showing_config::pid_hex_width + 1, L'\0');

		LV_ITEM item;
		memset(&item, 0, sizeof(item));

		item.iSubItem = 1;
		item.pszText = pid_str.data();
		item.cchTextMax = static_cast<int>(pid_str.size());
		SendMessage(process_list_->GetListHandle(), LVM_GETITEMTEXT, row_index, reinterpret_cast<LPARAM>(&item));
		pid_str.resize(std::wcslen(pid_str.c_str()));

		return petools::utility::ParsePidHex(pid_str);
	}

    bool MainDlg::OnCommand(WORD id, WORD code, HWND ctrl) {
        switch (id)
        {
        case IDC_BUTTON_PEVIEW:
        {
            std::optional<std::string> file_name_result = petools::utility::choose_file(current_hwnd_.get());
            if (!file_name_result.has_value()) {
                return true;
            }

            PeEditDlg* hwnd = DialogMgr().OpenDialog<PeEditDlg>(GetCurrentHWND(), file_name_result.value());
            if (hwnd == nullptr) {
                // TODO: log error
            }
            return true;
        }
        case IDC_BUTTON_ABOUT:
        {
            AboutDlg* hwnd = DialogMgr().OpenDialog<AboutDlg>(GetCurrentHWND());
            if (hwnd == nullptr) {
				// TODO: log error
            }
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

} //namespace petools