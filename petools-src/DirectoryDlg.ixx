module;

#include <windows.h>

export module DirectoryDlg;

import Dialog;
import STL;
import ExportDlg;
import ImportDlg;
import RelocationDlg;
import AnalysePE;

using std::unique_ptr;
using std::wstring;
using std::same_as;
using std::function;
using std::vector;
using std::make_unique;

export class DirectoryDlg : public DialogEX
{
public:
	DirectoryDlg(HWND hParent = nullptr);
	virtual ~DirectoryDlg() override;
	virtual void InitDlg() override;
	virtual void Plant() override;
	virtual void CloseDlg() override;

	void CreateExpertDlg();
	void CreateImportDlg();
	void CreateRelocationDlg();

	void SetDirHeaderInfo();                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
private:
	vector<unique_ptr<DialogEX>> dlg_list_;
	std::unique_ptr<ExportDlg> exportDlg_ = nullptr;
	std::unique_ptr<ImportDlg> importDlg_ = nullptr;
	std::unique_ptr<RelocationDlg> relocationDlg_ = nullptr;

	static DirectoryDlg* thisDlg_;
	void SetThisDlg() { thisDlg_ = this; }

	static LRESULT CALLBACK DirProc(HWND hDir, UINT message, WPARAM wParam, LPARAM lParam);
};
