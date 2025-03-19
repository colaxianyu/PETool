module;

#include <windows.h>
#include "..\GUI\resource.h"

export module FileHeaderDlg;

import DialogEX;
import WinHandle;

namespace petools {

	export class FileHeaderDlg final : public DialogEX {
	public:
		explicit FileHeaderDlg(HWND parent) noexcept : DialogEX(IDD_DIALOG_FILEHEAD, parent) {}
		~FileHeaderDlg() noexcept override = default;

		virtual void init_dialog() override {};

		//void SetFileHeaderInfo();
	private:

		virtual LRESULT handle_message(const WindowHandle&, UINT, WPARAM, LPARAM) override;
	};

} //namespace petools