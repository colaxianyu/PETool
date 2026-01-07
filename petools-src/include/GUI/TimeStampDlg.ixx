module;

#include <windows.h>
#include "resources/resource.h"

export module TimeStampDlg;

import DialogEX;
import WinHandle;

namespace petools::gui {

	export class TimeStampDlg final : public DialogEX {
	public:
		explicit TimeStampDlg(HWND parent) noexcept : DialogEX(IDD_DIALOG_TIMESTAMP, parent) {}
		~TimeStampDlg() noexcept override = default;
		//virtual void init_dlg() override;
		//virtual void plant() override;

		//void set_TSdlg_info();

	private:

		LRESULT HandleMessage(const WindowHandle&, UINT, WPARAM, LPARAM) override;
	};

} //namespace petools