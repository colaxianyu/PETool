module;

#include <windows.h>
#include "resources/resource.h"

export module FileHeaderDlg;

import DialogEX;
import WinHandle;

namespace petools::gui {

	export class FileHeaderDlg final : public DialogEX {
	public:
		explicit FileHeaderDlg(HWND parent) noexcept : DialogEX(IDD_DIALOG_FILEHEAD, parent) {}
		~FileHeaderDlg() noexcept override = default;

		void InitDialog() noexcept override;
		void set_file_header_info() noexcept;

	private:

		virtual LRESULT HandleMessage(const WindowHandle&, UINT, WPARAM, LPARAM) override;
	};

} //namespace petools