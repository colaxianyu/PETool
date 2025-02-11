//module;
//
//#include <windows.h>
//
//export module SecHeaderDlg;
//
//import Dialog;
//import STL;
//import ListControl;
//
//
//export class SecHeaderDlg : public DialogEX
//{
//public:
//	SecHeaderDlg(HWND h_parent = nullptr) noexcept(true);
//	virtual ~SecHeaderDlg() noexcept(true) override;
//	virtual void init_dlg() override;
//	virtual void plant() override;
//	virtual void close_dlg() override;
//
//	void init_section_list();
//
//	void plant_section_column();
//	void plant_section_item();
//private:
//	std::unique_ptr<ListCtrl> section_list_ = nullptr;
//
//	static SecHeaderDlg* this_dlg_;
//	void set_this_dlg() { this_dlg_ = this; }
//
//	static LRESULT CALLBACK section_proc(HWND h_sec, UINT message, WPARAM w_param, LPARAM l_param);
//};
//
