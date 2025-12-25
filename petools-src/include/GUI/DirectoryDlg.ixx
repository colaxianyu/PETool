module;

#include <windows.h>
#include "resources/resource.h"

export module DirectoryDlg;

import DialogEX;
import STL;
import WinHandle;

namespace petools::gui {

	export class DirectoryDlg final : public DialogEX {
	public:
		explicit DirectoryDlg(HWND parent) noexcept : DialogEX(IDD_DIALOG_DIRECTORY, parent) {}
		~DirectoryDlg() noexcept override = default;

		void InitDialog() noexcept override;
		void set_directory_info() noexcept;

	private:

		LRESULT HandleMessage(const WindowHandle&, UINT, WPARAM, LPARAM) override;
	};

} //namespace petools
