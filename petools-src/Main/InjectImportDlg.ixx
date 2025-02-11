//module;
//
//#include <windows.h>
//
//export module InjectImportDlg;
//
//import Dialog;
//
//export class InjectImportDlg : public DialogEX
//{
//public:
//	InjectImportDlg(HWND h_parent = nullptr);
//	virtual ~InjectImportDlg() override;
//	virtual void init_dlg() override;
//	virtual void plant() override;
//
//	void SelectDll();
//
//private:
//	static InjectImportDlg* this_dlg_;
//	void set_this_dlg() { this_dlg_ = this; }
//
//	static LRESULT CALLBACK InjectImportProc(HWND hInject, UINT message, WPARAM w_param, LPARAM l_param);
//};