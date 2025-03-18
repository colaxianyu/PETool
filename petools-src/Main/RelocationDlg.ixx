//module;
//
//#include <windows.h>
//
//export module RelocationDlg;
//
//import STL;
//import Dialog;
//import ListControl;
//
//export class RelocationDlg : public DialogEX
//{
//public:
//	RelocationDlg(HWND h_parent = nullptr);
//	virtual ~RelocationDlg() override;
//	virtual void init_dlg() override;
//	virtual void plant() override;
//	virtual void close_dlg() override;
//
//	void InitBlockList();
//	void plantBlockColumn();
//	void plantBlockItem();
//
//	void InitBlockItemList();
//	void plantBlockItemColumn();
//	void plantBlockItemItem();
//
//	DWORD GetRelocationTableIndex(DWORD rowID);
//private:
//	static RelocationDlg* this_dlg_;
//	std::unique_ptr<ListCtrl> blockList_ = nullptr;
//	std::unique_ptr<ListCtrl> blockItemList_ = nullptr;
//
//	void set_this_dlg() { this_dlg_ = this; }
//
//	static LRESULT CALLBACK RelocationProc(HWND hRelocation, UINT message, WPARAM w_param, LPARAM l_param);
//};