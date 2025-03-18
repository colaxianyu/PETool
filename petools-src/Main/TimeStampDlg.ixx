//module;
//
//#include <windows.h>
//#include "resource.h"
//
//export module TimeStampDlg;
//
//import STL;
//import DialogEX;
//import WinHandle;
//
//namespace petools {
//
//	export class TimeStampDlg final : public DialogEX {
//	public:
//		explicit TimeStampDlg(HWND parent) noexcept : DialogEX(IDD_DIALOG_TIMESTAMP, parent) {}
//		~TimeStampDlg() noexcept override = default;
//		//virtual void init_dlg() override;
//		//virtual void plant() override;
//
//		//void set_TSdlg_info();
//
//	private:
//
//		LRESULT handle_message(const WindowHandle&, UINT, WPARAM, LPARAM) override;
//	};
//
//} //namespace petools