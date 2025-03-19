module;

#include <windows.h>
#include "..\GUI\resource.h"

export module DirectoryDlg;

import DialogEX;
import STL;
import WinHandle;

namespace petools {

	export class DirectoryDlg final : public DialogEX {
	public:
		explicit DirectoryDlg(HWND parent) noexcept : DialogEX(IDD_DIALOG_DIRECTORY, parent) {}
		~DirectoryDlg() noexcept override = default;

		//void init_dialog() override {};

		//void SetDirHeaderInfo();
	private:

		LRESULT handle_message(const WindowHandle&, UINT, WPARAM, LPARAM) override;
	};

} //namespace petools
