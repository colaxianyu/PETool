module;

#include <windows.h>
#include "..\GUI\resource.h"

export module RelocationDlg;

import STL;
import DialogEX;
import WinHandle;
import ListControl;

namespace petools {

	export class RelocationDlg final : public DialogEX {
	public:
		explicit RelocationDlg(HWND parent) noexcept : DialogEX(IDD_DIALOG_RELOCATION, parent) {}
		~RelocationDlg() noexcept override = default;

		//void InitBlockList();
		//void plantBlockColumn();
		//void plantBlockItem();

		//void InitBlockItemList();
		//void plantBlockItemColumn();
		//void plantBlockItemItem();

		//DWORD GetRelocationTableIndex(DWORD rowID);

	private:
		//std::unique_ptr<ListCtrl> blockList_ = nullptr;
		//std::unique_ptr<ListCtrl> blockItemList_ = nullptr;

		LRESULT handle_message(const WindowHandle&, UINT, WPARAM, LPARAM) override;
	};

} //namespace petools