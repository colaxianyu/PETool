//module;
//
//#include <windows.h>
//
//export module ImportDlg;
//
//import STL;
//import Dialog;
//import ListControl;
//
//export class ImportDlg : public DialogEX
//{
//public:
//	ImportDlg(HWND h_parent = nullptr);
//	virtual ~ImportDlg() override;
//	virtual void init_dlg() override;
//	virtual void plant() override;
//	virtual void close_dlg() override;
//
//	void InitMainList();
//	void plantMainColumn();
//	void plantMainItem();
//
//	void InitFuncList();
//	void plantFuncColumn();
//	void plantFuncItem();
//
//	DWORD GetFirstThunkFromMainList(DWORD rowID);
//private:
//	static ImportDlg* this_dlg_;
//	std::unique_ptr<ListCtrl> mainList_ = nullptr;
//	std::unique_ptr<ListCtrl> funcList_ = nullptr;
//
//	void set_this_dlg() { this_dlg_ = this; }
//
//	static LRESULT CALLBACK ImportProc(HWND hImport, UINT message, WPARAM w_param, LPARAM l_param);
//};