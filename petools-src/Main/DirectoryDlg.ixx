//module;
//
//#include <windows.h>
//
//export module DirectoryDlg;
//
//import Dialog;
//import STL;
//import ExportDlg;
//import ImportDlg;
//import RelocationDlg;
//import AnalysePE;
//
//using std::unique_ptr;
//using std::wstring;
//using std::same_as;
//using std::function;
//using std::vector;
//using std::make_unique;
//
//export class DirectoryDlg : public DialogEX
//{
//public:
//	DirectoryDlg(HWND h_parent = nullptr);
//	virtual ~DirectoryDlg() override;
//	virtual void init_dlg() override;
//	virtual void plant() override;
//	virtual void close_dlg() override;
//
//	void CreateExpertDlg();
//	void CreateImportDlg();
//	void CreateRelocationDlg();
//
//	void SetDirHeaderInfo();                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
//private:
//	vector<unique_ptr<DialogEX>> dlg_list_;
//	std::unique_ptr<ExportDlg> exportDlg_ = nullptr;
//	std::unique_ptr<ImportDlg> importDlg_ = nullptr;
//	std::unique_ptr<RelocationDlg> relocationDlg_ = nullptr;
//
//	static DirectoryDlg* this_dlg_;
//	void set_this_dlg() { this_dlg_ = this; }
//
//	static LRESULT CALLBACK DirProc(HWND hDir, UINT message, WPARAM w_param, LPARAM l_param);
//};
