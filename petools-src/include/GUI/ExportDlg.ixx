module;

#include <windows.h>
#include "resources/resource.h"

export module ExportDlg;

import STL;
import DialogEX;
import WinHandle;
//import ListControl;

namespace petools {

	export class ExportDlg final : public DialogEX
	{
	public:
		explicit ExportDlg(HWND parent) : DialogEX(IDD_DIALOG_EXPORT, parent) {}
		~ExportDlg() noexcept override = default;

		//void SetExportInfo();
		//void InitFuncList();
		//void plantFuncColumn();
		//void plantFuncItem();

	private:
		LRESULT handle_message(const WindowHandle&, UINT, WPARAM, LPARAM) override;
	};

} //namespace petools