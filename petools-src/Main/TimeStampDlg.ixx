//module;
//
//#include <windows.h>
//
//export module TimeStampDlg;
//
//import Dialog;
//
//export class TimeStampDlg : public DialogEX {
//public:
//	TimeStampDlg(HWND h_parent = nullptr);
//	virtual ~TimeStampDlg() override;
//	virtual void init_dlg() override;
//	virtual void plant() override;
//
//	void set_TSdlg_info();
//
//private:
//	static TimeStampDlg* this_dlg_;
//	void set_this_dlg() { this_dlg_ = this; }
//
//	static LRESULT CALLBACK TsProc(HWND hAbout, UINT message, WPARAM w_param, LPARAM l_param);
//};