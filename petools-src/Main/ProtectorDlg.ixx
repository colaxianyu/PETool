//module;
//
//#include <windows.h>
//
//export module ProtectorDlg;
//
//import DialogEX;
//import STL;
//
//using std::unique_ptr;
//
//export class ProtectorDlg : public DialogEX
//{
//public:
//	explicit ProtectorDlg(INT template_id, HWND h_parent = nullptr) noexcept;
//	virtual ~ProtectorDlg() noexcept override;
//
//	virtual void show_dialog() override;
//	virtual void close_dialog() override;
//
//	//void SelectProtector();
//	//void SelectRawProcess();
//
//	//void ReadProtectorToBuffer();
//	//void ReadRawProcessToBuffer();
//
//	//char* EncodeRawProcess();
//	//void ProtectProcess();
//
//	//void SaveFile();
//private:
//	//unique_ptr<FileManage> protector_ = nullptr;
//	//unique_ptr<FileManage> rawProcess_ = nullptr;
//	//unique_ptr<char[]> protectorBuffer_ = nullptr;
//	//unique_ptr<char[]> rawProcessBuffer_ = nullptr;
//
//	LRESULT dialog_proc(HWND h_protector, UINT message, WPARAM w_param, LPARAM l_param) override;
//};