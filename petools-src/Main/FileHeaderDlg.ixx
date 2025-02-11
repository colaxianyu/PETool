//module;
//
//#include <windows.h>
//
//export module FileHeaderDlg;
//
//import STL;
//import Dialog;
//import TimeStampDlg;
//
//export class FileHeaderDlg : public DialogEX
//{
//public:
//	FileHeaderDlg(HWND h_parent = nullptr);
//	virtual ~FileHeaderDlg() override;
//	virtual void init_dlg() override;
//	virtual void plant() override;
//	virtual void close_dlg() override;
//
//	void SetFileHeaderInfo();
//
//	void CreateTimeDlg();
//private:
//	std::unique_ptr<TimeStampDlg> timeStampDlg_ = nullptr;
//
//	static FileHeaderDlg* this_dlg_;
//	void set_this_dlg() { this_dlg_ = this; }
//
//	static LRESULT CALLBACK FileHeaderProc(HWND hFile, UINT message, WPARAM w_param, LPARAM l_param);
//};