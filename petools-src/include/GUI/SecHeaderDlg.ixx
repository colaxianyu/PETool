module;

#include <windows.h>
#include "resources/resource.h"

export module SecHeaderDlg;

import STL;
import DialogEX;
import WinHandle;
import ListControl;

//using std::unique_ptr;

namespace petools::gui {

	export class SecHeaderDlg final : public DialogEX {
	public:
		explicit SecHeaderDlg(HWND h_parent) noexcept : DialogEX(IDD_DIALOG_SECTIONS, h_parent) {}
		~SecHeaderDlg() noexcept override = default;

		void InitDialog() noexcept override;
		void ShowDialog() noexcept override;

		void init_section_list() noexcept;

		void plant_section_column() noexcept;
		void plant_section_item() noexcept;

	private:
		std::unique_ptr<ListCtrl> section_list_ = nullptr;

		LRESULT HandleMessage(const WindowHandle&, UINT, WPARAM, LPARAM) override;
	};

} //namespace petools