module;

#include <windows.h>
#include "resources/resource.h"

export module MainDlg;

import std.compat;
import STL;
import DialogEX;
import WinHandle;
import ListControl;

//import ProtectorDlg;
//import PeEditDlg;

namespace petools::gui {

	export class MainDlg final : public DialogEX {
	public:
		explicit MainDlg() noexcept : DialogEX(IDD_DIALOG_MAIN) {}

		~MainDlg() noexcept override = default;

		void InitDialog() override;
		void ShowDialog() override;

		void InitProcessList() noexcept;
		void InitModuleList() noexcept;

		void PlantProcessItem() noexcept;
		void PlantProcessColumn() noexcept;
		void PlantModuleColumn() noexcept;
		void PlantModuleItem() noexcept;

		std::optional<DWORD> GetPid(INT row_index) noexcept;

	private:
		IconHandle icon_;
		std::unique_ptr<ListCtrl> process_list_ = nullptr;
		std::unique_ptr<ListCtrl> module_list_ = nullptr;

		bool OnCommand(WORD, WORD, HWND) override;
		virtual LRESULT OnOtherMessage(UINT, WPARAM, LPARAM) override;

	};

} //namespace petools::gui