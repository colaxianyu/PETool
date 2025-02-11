//module;
//
//#include <windows.h>
//
//export module ExportDlg;
//
//import STL;
//import Dialog;
//import ListControl;
//
//export class ExportDlg : public DialogEX
//{
//public:
//	ExportDlg(HWND h_parent = nullptr);
//	virtual ~ExportDlg() override;
//	virtual void init_dlg() override;
//	virtual void plant() override;
//	virtual void close_dlg() override;
//
//	void SetExportInfo();
//	void InitFuncList();
//	void plantFuncColumn();
//	void plantFuncItem();
//
//
//private:
//	inline static ExportDlg* this_dlg_ = nullptr;
//	std::unique_ptr<ListCtrl> funcList_ = nullptr;
//
//	void set_this_dlg() { this_dlg_ = this; }
//
//	static LRESULT CALLBACK ExportProc(HWND hExport, UINT message, WPARAM w_param, LPARAM l_param);
//};