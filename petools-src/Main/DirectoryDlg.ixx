module;

#include <windows.h>
#include "resource.h"

export module DirectoryDlg;

import DialogEX;
import STL;
import WinHandle;
//import ExportDlg;
//import ImportDlg;
//import RelocationDlg;
//import AnalysePE;

namespace petools {

	export class DirectoryDlg final : public DialogEX {
	public:
		explicit DirectoryDlg(HWND h_parent) noexcept : DialogEX(IDD_DIALOG_DIRECTORY, h_parent) {}
		~DirectoryDlg() noexcept override = default;

		void init_dialog() override {};

		void SetDirHeaderInfo();
	private:

		LRESULT handle_message(const WindowHandle&, UINT, WPARAM, LPARAM) override;
	};

} //namespace petools
