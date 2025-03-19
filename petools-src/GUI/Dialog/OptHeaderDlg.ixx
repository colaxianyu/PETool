module;

#include <windows.h>
#include "..\GUI\resource.h"

export module OptHeaderDlg;

import DialogEX;
import WinHandle;

namespace petools {

	export class OptHeaderDlg final : public DialogEX {
	public:
		explicit OptHeaderDlg(HWND parent) noexcept : DialogEX(IDD_DIALOG_OPTHEADER, parent) {}
		~OptHeaderDlg() noexcept override = default;

		void init_dialog() override;

		//void SetOptHeaderInfo();
	private:
		LRESULT handle_message(const WindowHandle&, UINT, WPARAM, LPARAM) override;
	};

} //namespace petools