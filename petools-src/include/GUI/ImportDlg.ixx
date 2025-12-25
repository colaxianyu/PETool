module;

#include <windows.h>
#include "resources/resource.h"

export module ImportDlg;

import STL;
import DialogEX;
import ListControl;
import WinHandle;

namespace petools::gui {

	export class ImportDlg final : public DialogEX {
	public:
		explicit ImportDlg(HWND parent) noexcept : DialogEX(IDD_DIALOG_IMPORT, parent) {}
		~ImportDlg() noexcept override = default;

		void InitDialog() noexcept override;
		void ShowDialog() noexcept override;

		void init_main_list() noexcept;
		void plant_main_column() noexcept;
		void plant_main_item() noexcept;

		void init_function_list() noexcept;
		void plant_function_column() noexcept;
		void plant_func_item() noexcept;

	private:
		std::unique_ptr<ListCtrl> main_list_ = nullptr;
		std::unique_ptr<ListCtrl> func_list_ = nullptr;

		DWORD get_first_thunk_from_main_list(DWORD row_id) noexcept;
		LRESULT HandleMessage(const WindowHandle&, UINT, WPARAM, LPARAM) override;
	};

} //namespace petools