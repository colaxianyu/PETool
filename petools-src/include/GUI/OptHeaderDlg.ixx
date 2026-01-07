module;

#include <windows.h>
#include "resources/resource.h"

export module OptHeaderDlg;

import DialogEX;
import WinHandle;

namespace petools::gui {

	export class OptHeaderDlg final : public DialogEX {
	public:
		explicit OptHeaderDlg(HWND parent) noexcept : DialogEX(IDD_DIALOG_OPTHEADER, parent) {}
		~OptHeaderDlg() noexcept override = default;

		void InitDialog() noexcept override;
		void set_optional_header_info() noexcept;

		//void SetOptHeaderInfo();
	private:
		LRESULT HandleMessage(const WindowHandle&, UINT, WPARAM, LPARAM) override;
	};

} //namespace petools