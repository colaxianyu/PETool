//module;
//
//#include <windows.h>
//#include "resource.h"
//
//export module SecHeaderDlg;
//
//import STL;
//import DialogEX;
//import WinHandle;
//import ListControl;
//
////using std::unique_ptr;
//
//namespace petools {
//
//	export class SecHeaderDlg final : public DialogEX {
//	public:
//		explicit SecHeaderDlg(HWND h_parent) noexcept : DialogEX(IDD_DIALOG_SECTIONS, h_parent) {}
//		~SecHeaderDlg() noexcept override = default;
//
//		void init_dialog() override {}
//
//		//void init_section_list();
//
//		//void plant_section_column();
//		//void plant_section_item();
//
//	private:
//		std::unique_ptr<ListCtrl> section_list_ = nullptr;
//
//		LRESULT handle_message(const WindowHandle&, UINT, WPARAM, LPARAM) override;
//	};
//
//} //namespace petools