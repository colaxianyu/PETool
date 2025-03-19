module;

#include "..\GUI\resource.h"

export module AboutDlg;

import DialogEX;
import WinHandle;

namespace petools {

	export class AboutDlg final : public DialogEX {
	public:
		explicit AboutDlg(HWND parent) noexcept : DialogEX(IDD_DIALOG_ABOUT, parent) {}
		~AboutDlg() noexcept override = default;

	private:
		LRESULT handle_message(const WindowHandle&, UINT, WPARAM, LPARAM) override;
	};

} //namespace petools