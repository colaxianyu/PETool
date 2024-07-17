module;

#include <windows.h>
#include <CommCtrl.h>
#pragma comment(lib, "comctl32.lib")

export module PeEditDlg;

import STL;
import Dialog;
import FileManage;
import AnalysePE;
import FileHeaderDlg;
import OptHeaderDlg;
import SecHeaderDlg;
import DirectoryDlg;
import CalculateDlg;
import InjectImportDlg;

using std::unique_ptr;
using std::vector;
using petools::config::filename_max;

export class PeEditDlg : public DialogEX
{
public:
	PeEditDlg(HWND hParent = nullptr);
	virtual ~PeEditDlg() override;
	virtual void InitDlg() override;
	virtual void Plant() override;
	virtual void CloseDlg() override;

	void SetFileName(TCHAR* fileName);
	void OpenFile(const TCHAR* filePath, const char* mdoe = "rb");
	void SetEditTitle();
	void SetPEMainInfo();

	void CreateFileDlg();
	void CreateOptDlg();
	void CreateSecDlg();
	void CreateDirDlg();
	void CreateCalcDlg();
	void CreateInjectImportDlg();

	void SaveAsFile();
	FileManage* GetFileManage() { return fileManage_.get(); }
private:
	TCHAR fileName_[filename_max];
	unique_ptr<FileManage> fileManage_ = nullptr;
	vector<unique_ptr<DialogEX>> dlg_list_;

	static PeEditDlg* thisDlg_;
	void SetThisDlg() { thisDlg_ = this; }

	static LRESULT CALLBACK EditProc(HWND hEdit, UINT message, WPARAM wParam, LPARAM lParam);
};
