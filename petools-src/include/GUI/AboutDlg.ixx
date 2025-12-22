module;

#include "resources/resource.h"

export module AboutDlg;

import DialogEX;
import WinHandle;

namespace petools::gui {

	export class AboutDlg final : public DialogEX {
	public:
		explicit AboutDlg(HWND parent) noexcept : DialogEX(IDD_DIALOG_ABOUT, parent) {}
		~AboutDlg() noexcept override = default;
	};

} //namespace petools::gui