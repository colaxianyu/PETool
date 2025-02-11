//module;
//
//#include <windows.h>
//
//export module OptHeaderDlg;
//
//import Dialog;
//
//export class OptHeaderDlg : public DialogEX
//{
//public:
//	OptHeaderDlg(HWND h_parent = nullptr);
//	virtual ~OptHeaderDlg() override;
//	virtual void init_dlg() override;
//	virtual void plant() override;
//
//	void SetOptHeaderInfo();
//private:
//	static OptHeaderDlg* this_dlg_;
//	void set_this_dlg() { this_dlg_ = this; }
//
//	static LRESULT CALLBACK OptHeaderProc(HWND hOpt, UINT message, WPARAM w_param, LPARAM l_param);
//};
//
