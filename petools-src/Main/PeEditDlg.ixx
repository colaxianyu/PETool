//module;
//
//#include <windows.h>
//#include <CommCtrl.h>
//#pragma comment(lib, "comctl32.lib")
//
//export module PeEditDlg;
//
//import STL;
//
//import DialogEX;
//import FileManager;
////import AnalysePE;
////import FileHeaderDlg;
////import OptHeaderDlg;
////import SecHeaderDlg;
////import DirectoryDlg;
////import CalculateDlg;
////import InjectImportDlg;
//
//using std::unique_ptr;
//using std::vector;
//using tools::config::filename_max;
//
//export class PeEditDlg : public DialogEX
//{
//public:
//	explicit PeEditDlg(INT template_id, HWND h_parent = nullptr) noexcept;
//	virtual ~PeEditDlg() noexcept override;
//
//	void init_dialog() override;
//	void show_dialog() override;
//	void close_dialog() override;
//
//	//void SetFileName(TCHAR* fileName);
//	//void OpenFile(const TCHAR* filePath, const char* mdoe = "rb");
//	//void SetEditTitle();
//	//void SetPEMainInfo();
//
//	//void CreateFileDlg();
//	//void CreateOptDlg();
//	//void CreateSecDlg();
//	//void CreateDirDlg();
//	//void CreateCalcDlg();
//	//void CreateInjectImportDlg();
//
//	//void SaveAsFile();
//	//FileManage* GetFileManage() { return fileManage_.get(); }
//	std::unique_ptr<FileManager>& GetFileManager() noexcept { return file_manager_; }
//private:
//	//TCHAR fileName_[filename_max];
//	unique_ptr<FileManager> file_manager_;
//	//vector<unique_ptr<DialogEX>> dlg_list_;
//
//	//static PeEditDlg* this_dlg_;
//	//void set_this_dlg() { this_dlg_ = this; }
//
//	LRESULT dialog_proc(HWND h_edit, UINT message, WPARAM w_param, LPARAM l_param) override;
//};
