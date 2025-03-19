module;

#include <windows.h>
#include "..\GUI\resource.h"

export module ImportDlg;

import STL;
import DialogEX;
//import ListControl;
import WinHandle;

namespace petools {

	export class ImportDlg final : public DialogEX {
	public:
		explicit ImportDlg(HWND parent) noexcept : DialogEX(IDD_DIALOG_IMPORT, parent) {}
		~ImportDlg() noexcept override = default;

		//void init_dialog() override {}


		//void InitMainList();
		//void plantMainColumn();
		//void plantMainItem();

		//void InitFuncList();
		//void plantFuncColumn();
		//void plantFuncItem();

		//DWORD GetFirstThunkFromMainList(DWORD rowID);
	private:
		//std::unique_ptr<ListCtrl> mainList_ = nullptr;
		//std::unique_ptr<ListCtrl> funcList_ = nullptr;


		LRESULT handle_message(const WindowHandle&, UINT, WPARAM, LPARAM) override;
	};

} //namespace petools