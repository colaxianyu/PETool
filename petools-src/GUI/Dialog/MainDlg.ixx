module;

#include <windows.h>
#include "..\GUI\resource.h"

export module MainDlg;

import STL;
import DialogEX;
import WinHandle;
import ListControl;

//import ProtectorDlg;
//import PeEditDlg;

using std::unique_ptr;
using std::optional;

namespace petools {

	export class MainDlg final : public DialogEX {
	public:
		explicit MainDlg() noexcept : DialogEX(IDD_DIALOG_MAIN) {}

		~MainDlg() noexcept override = default;

		void init_dialog() noexcept override;
		void show_dialog() noexcept override;
		void close_dialog() noexcept override;

		void init_process_list() noexcept;
		void init_module_list() noexcept;

		void plant_process_item() noexcept;
		void plant_process_column() noexcept;
		void plant_module_column() noexcept;
		void plant_module_item() noexcept;

		optional<DWORD> get_pid(INT row_index) noexcept;

	private:
		IconHandle icon_;
		unique_ptr<ListCtrl> process_list_ = nullptr;
		unique_ptr<ListCtrl> module_list_ = nullptr;

		LRESULT handle_message(const WindowHandle&, UINT, WPARAM, LPARAM) override;
	};

} //namespace petools